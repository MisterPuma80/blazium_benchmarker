#!/bin/bash

############################################################################
#  benchmarker.sh                                                          #
############################################################################
#                           This file is part of:                          #
#                   MisterPuma80's Blazium Benchmarker                     #
#            https://github.com/MisterPuma80/blazium_benchmarker           #
############################################################################
# Copyright (c) 2024-2026 MisterPuma80 <MisterPuma80@gmail.com>            #
#                                                                          #
# Permission is hereby granted, free of charge, to any person obtaining    #
# a copy of this software and associated documentation files (the          #
# "Software"), to deal in the Software without restriction, including      #
# without limitation the rights to use, copy, modify, merge, publish,      #
# distribute, sublicense, and/or sell copies of the Software, and to       #
# permit persons to whom the Software is furnished to do so, subject to    #
# the following conditions:                                                #
#                                                                          #
# The above copyright notice and this permission notice shall be           #
# included in all copies or substantial portions of the Software.          #
#                                                                          #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          #
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       #
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   #
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     #
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     #
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        #
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   #
############################################################################

set -e
set +x
shopt -s extglob




: << 'COMMENT'

This is a script to benchmark Blazium.

COMMENT

ITERATIONS=10000000
COMPILER="use_llvm=no"
LINKER="linker=default"
COMPILER_AND_LINKER="$COMPILER $LINKER"


# Ensure the script is NOT run as root
if [[ "$EUID" -eq 0 ]]; then
	echo "This script must NOT be run as root."
	exit 1
fi

# Change directory to the script's directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
#echo "$SCRIPT_DIR"
cd "$SCRIPT_DIR"

# Get the architecture
ARCH=`uname -m`
ARCH=${ARCH,,}
if [[ "$ARCH" == "x86_64" ]] || [[ "$ARCH" == "amd64" ]]; then
	ARCH='x86_64'
else
	echo "Unsupported architecture: $ARCH"
	exit 1
fi

# Get the OS
if [[ "$OSTYPE" == "linux"* ]]; then
	OS="linux"
elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
	OS="windows"
else
	echo "Unknown OS: $OSTYPE"
	exit 1
fi

# Get the parameters for this OS
PLATFORM="Unknown"
PLATFORM_TEMPLATES="Unknown"
EXT="Unknown"
EXPORT_NAME="Unknown"
PYTHON="Unknown"
if [[ "$OS" == "linux" ]]; then
	PLATFORM="linuxbsd"
	PLATFORM_TEMPLATES="linux"
	EXT="x86_64"
	EXPORT_NAME="Linux"
	PYTHON="python3"
elif [[ "$OS" == "windows" ]]; then
	PLATFORM="windows"
	PLATFORM_TEMPLATES="windows"
	EXT="x86_64.exe"
	EXPORT_NAME="Windows Desktop"
	PYTHON="python"
else
	echo "Unknown PLATFORM"
	exit 1
fi

# Get cpu core count-1 clamped between 1 and 1000
BUILD_CORES=`nproc`
if [[ ! "$BUILD_CORES" =~ ^[0-9]+$ ]]; then
	BUILD_CORES=1
fi
BUILD_CORES=$((BUILD_CORES - 1))
if (( BUILD_CORES < 1 )); then
	BUILD_CORES=1
elif (( BUILD_CORES > 1000 )); then
	BUILD_CORES=1000
fi

remove_llvm_ext() {
	if [ -f "$1" ]; then
		mv "$1" "${1%.llvm}"
	fi
}

url_to_filename() {
	$PYTHON -c "
import urllib.parse, sys
# URL encode the raw url
raw_url = sys.argv[1]
encoded = urllib.parse.quote(raw_url, safe='')

# Trim dot and space
clean = encoded.strip('. ')

# Prefix _ if using a reserved Windows name
reserved = {
	'CON', 'PRN', 'AUX', 'NUL', 'COM1', 'COM2', 'COM3', 'COM4',
	'COM5', 'COM6', 'COM7', 'COM8', 'COM9', 'LPT1', 'LPT2',
	'LPT3', 'LPT4', 'LPT5', 'LPT6', 'LPT7', 'LPT8', 'LPT9',
}
if clean.upper() in reserved:
	clean = '_' + clean

print(clean)
" "$1"
}

download_blazium_4_5() {
	# Download Blazium
	if [ ! -d "blazium" ]; then
		git clone https://github.com/blazium-engine/blazium
		cd blazium
		git checkout blazium-4.5
		git submodule update --init --remote --recursive
		cd ..
	else
		echo "blazium directory already exists. Skiping download."
	fi

	# Download Blazium CPP API Bindings
	if [ ! -d "blazium-cpp" ]; then
		git clone https://github.com/blazium-engine/blazium-cpp
		cd blazium-cpp
		git checkout blazium-dev
		cd ..
	else
		echo "blazium-cpp directory already exists. Skiping download."
	fi
}

build_blazium() {
	# Build Blazium editor and export templates
	cd blazium
	rm -f bin/*.x86_64
	rm -f bin/*.llvm
	rm -f bin/*.exe
	scons platform=$PLATFORM target=editor dev_build=no dev_mode=no $COMPILER_AND_LINKER tests=no execinfo=yes scu_build=yes -j $BUILD_CORES
	scons platform=$PLATFORM target=template_release dev_build=no dev_mode=no $COMPILER_AND_LINKER scu_build=yes -j $BUILD_CORES
	remove_llvm_ext bin/blazium.$PLATFORM.editor.x86_64.llvm
	remove_llvm_ext bin/blazium.$PLATFORM.template_release.x86_64.llvm
	cd ..

	# Dump fresh API json file and header from Blazium Editor
	cd blazium-cpp/gdextension
	rm -f extension_api.json
	rm -f gdextension_interface.h
	../../blazium/bin/blazium.$PLATFORM.editor.$EXT --headless --dump-extension-api --dump-gdextension-interface
	cd ../..
}

download() {
	set -x

	cd example_blazium_4.5
	download_blazium_4_5
	cd ..

	cd example_blazium_4.5_modified
	download_blazium_4_5
	cd ..

	set +x
}

engines() {
	set -x

	cd example_blazium_4.5
	build_blazium
	cd ..

	cd example_blazium_4.5_modified
	build_blazium
	cd ..

	set +x
}

benchmarks() {
	set -x

	echo "Building Blazium 4.5 ..."
	cd example_blazium_4.5
	scons platform=$PLATFORM_TEMPLATES target=template_release dev_build=no $COMPILER -j $BUILD_CORES iterations=$ITERATIONS
	scons platform=$PLATFORM_TEMPLATES target=template_debug dev_build=yes $COMPILER -j $BUILD_CORES iterations=$ITERATIONS
	rm -f -rf demo/export
	mkdir -p demo/export
	./blazium/bin/blazium.$PLATFORM.editor.$EXT --export-release "$EXPORT_NAME" ./demo/project.godot
	echo "!!!! Done building release blazium"
	cd ..

	echo "Building Blazium 4.5 Modified ..."
	cd example_blazium_4.5_modified
	scons platform=$PLATFORM_TEMPLATES target=template_release dev_build=no $COMPILER -j $BUILD_CORES iterations=$ITERATIONS
	scons platform=$PLATFORM_TEMPLATES target=template_debug dev_build=yes $COMPILER -j $BUILD_CORES iterations=$ITERATIONS
	rm -f -rf demo/export
	mkdir -p demo/export
	./blazium/bin/blazium.$PLATFORM.editor.$EXT --export-release "$EXPORT_NAME" ./demo/project.godot
	echo "!!!! Done building release blazium"
	cd ..

	set +x
}

run() {
	set -x

	# If on Linux, constantly refresh sudo in a sub process that is killed at exit
	if [[ "$OS" == "linux" ]]; then
		sudo -v
		( while true; do sudo -nv; sleep 60; done ) &>/dev/null &
		SUDO_LOOP_PID=$!
		trap 'kill $SUDO_LOOP_PID' EXIT
	fi

	# Turn off cpu freq scaling, address space randomization, and hyper threading
	#sudo cpupower frequency-set --governor performance
	#echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
	#echo off | sudo tee /sys/devices/system/cpu/smt/control

	# Wait for kernel to adjust to settings
	#sleep 10

	cwd="$PWD"
	if [[ "$OS" == "linux" ]]; then
		sudo nice -n -20 sudo -u $USER taskset -c 5 ./example_blazium_4.5/demo/export/demo.x86_64 --no-header --headless --single-threaded-scene --fixed-fps 60 --delta-smoothing disable --disable-vsync --disable-render-loop -- name "benchmark_blazium_4_5.json" cwd $cwd
		sudo nice -n -20 sudo -u $USER taskset -c 5 ./example_blazium_4.5_modified/demo/export/demo.x86_64 --no-header --headless --single-threaded-scene --fixed-fps 60 --delta-smoothing disable --disable-vsync --disable-render-loop -- name "benchmark_blazium_4_5_modified.json" cwd $cwd
	else
		./example_blazium_4.5/demo/export/demo.x86_64 --no-header --headless --single-threaded-scene --fixed-fps 60 --delta-smoothing disable --disable-vsync --disable-render-loop -- name "benchmark_blazium_4_5.json" cwd $cwd
		./example_blazium_4.5_modified/demo/export/demo.x86_64 --no-header --headless --single-threaded-scene --fixed-fps 60 --delta-smoothing disable --disable-vsync --disable-render-loop -- name "benchmark_blazium_4_5_modified.json" cwd $cwd
	fi

	# Turn things back on
	#echo on | sudo tee /sys/devices/system/cpu/smt/control
	#echo 2 | sudo tee /proc/sys/kernel/randomize_va_space
	#sudo cpupower frequency-set --governor schedutil

	set +x
}

show() {
	set -x

	$PYTHON make_chart.py --iterations $ITERATIONS

	set +x
}

clean_blazium() {
	rm -f .sconsign.dblite
	rm -f src/gdexample.os
	rm -f src/register_types.os
	rm -f demo/bin/*.so
	rm -rf -f demo/export

	if [ -d "blazium" ]; then
		cd blazium
		scons platform=$PLATFORM target=editor dev_build=no dev_mode=no $COMPILER_AND_LINKER tests=no execinfo=yes scu_build=yes -c
		scons platform=$PLATFORM target=template_release dev_build=no dev_mode=no $COMPILER_AND_LINKER scu_build=yes -c
		find . -name "*.gen.cpp" -delete
		find . -type d -name "__pycache__" -exec rm -rf {} +
		find . -type d -name "scu" -exec rm -rf {} +
		rm -f -rf bin
		rm -f -rf platform/linuxbsd/wayland/protocol
		rm -f .scons_env.json
		rm -rf .scons_node_count
		rm -rf .sconsign5.dblite
		rm -rf config.log
		rm -f -rf .sconf_temp
		rm -f build_env
		rm -f generate_build_env.py
		cd ..
	fi

	if [ -d "blazium-cpp" ]; then
		cd blazium-cpp
		scons platform=$PLATFORM_TEMPLATES target=editor -c
		scons platform=$PLATFORM_TEMPLATES target=template_release -c
		git restore gdextension/gdextension_interface.h
		git restore gdextension/extension_api.json
		find . -type d -name "__pycache__" -exec rm -rf {} +
		rm -f .sconsign.dblite
		cd ..
	fi
}

clean() {
	set -x

	cd example_blazium_4.5
	clean_blazium
	cd ..

	cd example_blazium_4.5_modified
	clean_blazium
	cd ..

	set +x
}

patch() {
	set -x

	local prefix raw_url encoded_url
	IFS="=" read -r prefix raw_url <<< "$1"

	mkdir -p patches
	encoded_url=$(url_to_filename "$raw_url")
	curl -L -o patches/$encoded_url $raw_url

	cd example_blazium_4.5_modified/blazium
	git reset --hard HEAD
	git clean -fd
	git apply ../../patches/$encoded_url
	cd ../..

	set +x
}

linker() {
	set -x

	local prefix linker_name
	IFS="=" read -r prefix linker_name <<< "$1"

	LINKER="linker=$linker_name"
	COMPILER_AND_LINKER="$COMPILER $LINKER"
	#echo $COMPILER_AND_LINKER

	set +x
}

use_llvm() {
	set -x

	local prefix use_llvm
	IFS="=" read -r prefix use_llvm <<< "$1"

	COMPILER="use_llvm=$use_llvm"
	COMPILER_AND_LINKER="$COMPILER $LINKER"
	#echo $COMPILER_AND_LINKER

	set +x
}

cores() {
	set -x

	local prefix cores
	IFS="=" read -r prefix cores <<< "$1"

	BUILD_CORES=$cores
	#echo $BUILD_CORES

	set +x
}

reset() {
	set -x

	cd example_blazium_4.5_modified
	cd blazium
	git reset --hard HEAD
	cd ..
	cd blazium-cpp
	git reset --hard HEAD
	cd ..
	cd ..

	cd example_blazium_4.5
	cd blazium
	git reset --hard HEAD
	cd ..
	cd blazium-cpp
	git reset --hard HEAD
	cd ..
	cd ..

	set +x
}

help() {
	echo "./benchmarker.sh help - Prints help."
	echo "./benchmarker.sh download - Downloads engines, export templates, and cpp api bindings"
	echo "./benchmarker.sh patch=URL.patch - Downloads and applies a git patch"
	echo "./benchmarker.sh linker=name - The linker to use. Default system default."
	echo "./benchmarker.sh use_llvm=yes or no - To use LLVM or not. Defaults to no."
	echo "./benchmarker.sh cores=number - The number of cpu cores to use for -j."
	echo "./benchmarker.sh reset - Resets any changes to engine code, but ignores unknown files"
	echo "./benchmarker.sh engines - Builds engines, export templates, and dumps api json file"
	echo "./benchmarker.sh benchmarks - Builds benchmarks as a game"
	echo "./benchmarker.sh run - Runs benchmarks"
	echo "./benchmarker.sh show - Shows benchmarks"
	echo "./benchmarker.sh clean - Deletes generated files"
}

if [ $# -eq 0 ]; then
	help
fi

for param in "$@"; do
	case "$param" in
		help)
			help
			;;
		download)
			download
			;;
		patch=+(*))
			patch "$param"
			;;
		linker=+(*))
			linker "$param"
			;;
		use_llvm=+(*))
			use_llvm "$param"
			;;
		cores=+([0-9]))
			cores "$param"
			;;
		reset)
			reset
			;;
		engines)
			engines
			;;
		benchmarks)
			benchmarks
			;;
		run)
			run
			;;
		show)
			show
			;;
		clean)
			clean
			;;
		*)
			help
			exit 1
			;;
	esac
done
