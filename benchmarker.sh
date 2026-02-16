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

ITERATIONS=100000000
COMPILER=""
LINKER=""
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

download_blazium_4_5() {
	# Download Blazium
	git clone https://github.com/blazium-engine/blazium
	cd blazium
	git checkout blazium-4.5
	git submodule update --init --remote --recursive
	cd ..

	# Download Blazium CPP API Bindings
	git clone https://github.com/blazium-engine/blazium-cpp
	cd blazium-cpp
	git checkout blazium-dev
	cd ..
}

build_blazium() {
	# Build Blazium editor and export templates
	cd blazium
	rm -f bin/*.x86_64
	rm -f bin/*.llvm
	rm -f bin/*.exe
	scons platform=linuxbsd target=editor dev_build=no dev_mode=no $COMPILER_AND_LINKER tests=yes execinfo=yes scu_build=yes -j $BUILD_CORES
	scons platform=linuxbsd target=template_release dev_build=no dev_mode=no $COMPILER_AND_LINKER scu_build=yes -j $BUILD_CORES
	remove_llvm_ext bin/blazium.linuxbsd.editor.x86_64.llvm
	remove_llvm_ext bin/blazium.linuxbsd.template_release.x86_64.llvm
	cd ..

	# Dump fresh API json file and header from Blazium Editor
	cd blazium-cpp/gdextension
	rm -f extension_api.json
	rm -f gdextension_interface.h
	../../blazium/bin/blazium.linuxbsd.editor.x86_64 --headless --dump-extension-api --dump-gdextension-interface
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

engine() {
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
	scons platform=linux target=template_release dev_build=no $COMPILER -j $BUILD_CORES iterations=$ITERATIONS
	scons platform=linux target=template_debug dev_build=yes $COMPILER -j $BUILD_CORES iterations=$ITERATIONS
	rm -f -rf demo/export
	mkdir -p demo/export
	sleep 3
	./blazium/bin/blazium.linuxbsd.editor.x86_64 --export-release "Benchmarks Headless Linux" ./demo/project.godot
	echo "!!!! Done building release blazium"
	cd ..

	echo "Building Blazium 4.5 Modified ..."
	cd example_blazium_4.5_modified
	scons platform=linux target=template_release dev_build=no $COMPILER -j $BUILD_CORES iterations=$ITERATIONS
	scons platform=linux target=template_debug dev_build=yes $COMPILER -j $BUILD_CORES iterations=$ITERATIONS
	rm -f -rf demo/export
	mkdir -p demo/export
	sleep 3
	./blazium/bin/blazium.linuxbsd.editor.x86_64 --export-release "Benchmarks Headless Linux" ./demo/project.godot
	echo "!!!! Done building release blazium"
	cd ..

	set +x
}

run() {
	set -x

	# Constantly refresh sudo in a sub process that is killed at exit
	sudo -v
	( while true; do sudo -nv; sleep 60; done ) &>/dev/null &
	SUDO_LOOP_PID=$!
	trap 'kill $SUDO_LOOP_PID' EXIT

	# Turn off cpu freq scaling, address space randomization, and hyper threading
	#sudo cpupower frequency-set --governor performance
	#echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
	#echo off | sudo tee /sys/devices/system/cpu/smt/control

	# Wait for kernel to adjust to settings
	#sleep 10

	cwd="$PWD"
	sudo nice -n -20 sudo -u $USER taskset -c 5 ./example_blazium_4.5/demo/export/demo.x86_64 --no-header --headless --single-threaded-scene --fixed-fps 60 --delta-smoothing disable --disable-vsync --disable-render-loop -- name "benchmark_blazium_4_5.json" cwd $cwd
	sudo nice -n -20 sudo -u $USER taskset -c 5 ./example_blazium_4.5_modified/demo/export/demo.x86_64 --no-header --headless --single-threaded-scene --fixed-fps 60 --delta-smoothing disable --disable-vsync --disable-render-loop -- name "benchmark_blazium_4_5_modified.json" cwd $cwd

	# Turn things back on
	#echo on | sudo tee /sys/devices/system/cpu/smt/control
	#echo 2 | sudo tee /proc/sys/kernel/randomize_va_space
	#sudo cpupower frequency-set --governor schedutil

	set +x
}

show() {
	set -x

	python3 make_chart.py --iterations $ITERATIONS

	set +x
}

clean() {
	set -x

	cd example_blazium_4.5_modified
	rm -f .sconsign.dblite
	rm -f src/gdexample.os
	rm -f src/register_types.os
	rm -f demo/bin/*.so
	rm -rf -f demo/export

	cd blazium
	git clean -fxd
	cd ..

	cd blazium-cpp
	git clean -fxd
	cd ..

	cd ..



	cd example_blazium_4.5
	rm -f .sconsign.dblite
	rm -f src/gdexample.os
	rm -f src/register_types.os
	rm -f demo/bin/*.so
	rm -rf -f demo/export

	cd blazium
	git clean -fxd
	cd ..

	cd blazium-cpp
	git clean -fxd
	cd ..

	cd ..

	set +x
}

patch() {
	set -x

	IFS=":" read -r prefix pr_number <<< "$1"

	mkdir -p patches
	curl -L -o patches/pr_$pr_number.patch https://github.com/blazium-games/blazium/pull/$pr_number.patch

	cd example_blazium_4.5_modified/blazium
	git reset --hard HEAD
	git clean -fd
	git apply ../../patches/pr_$pr_number.patch
	cd ../..

	set +x
}

linker() {
	set -x

	IFS=":" read -r prefix linker_name <<< "$1"

	LINKER="linker=$linker_name"
	COMPILER_AND_LINKER="$COMPILER $LINKER"
	#echo $COMPILER_AND_LINKER

	set +x
}

use_llvm() {
	set -x

	IFS=":" read -r prefix use_llvm <<< "$1"

	COMPILER="use_llvm=$use_llvm"
	COMPILER_AND_LINKER="$COMPILER $LINKER"
	#echo $COMPILER_AND_LINKER

	set +x
}

cores() {
	set -x

	IFS=":" read -r prefix cores <<< "$1"

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
	echo "./benchmarker.sh download - Downloads engine, export templates, and cpp api bindings"
	echo "./benchmarker.sh patch:github_pr_# - Downloads and applies a github PR patch"
	echo "./benchmarker.sh linker:name - The linker to use. Default system default."
	echo "./benchmarker.sh use_llvm:true or false - To use LLVM or not. Default false."
	echo "./benchmarker.sh cores:number - The number of cpu cores to use for -j."
	echo "./benchmarker.sh reset - Resets any changes to engine code, but ignores unknown files"
	echo "./benchmarker.sh engine - Builds engine, export templates, and dumps api json file"
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
			sleep 1
			;;
		patch:+([0-9]))
			patch "$param"
			sleep 1
			;;
		linker:+(*))
			linker "$param"
			;;
		use_llvm:+(*))
			use_llvm "$param"
			;;
		cores:+([0-9]))
			cores "$param"
			;;
		reset)
			reset
			sleep 1
			;;
		engine)
			engine
			sleep 5
			;;
		benchmarks)
			benchmarks
			sleep 5
			;;
		run)
			run
			sleep 1
			;;
		show)
			show
			;;
		clean)
			clean
			sleep 1
			;;
		*)
			help
			exit 1
			;;
	esac
done
