# blazium_benchmarker

# Simple Example
```sh
# Checkout repo
git clone https://github.com/MisterPuma80/blazium_benchmarker
cd blazium_benchmarker
git checkout master

# Download engines
./benchmarker.sh download

# Modify the engine code at example_blazium_4.5_modified/blazium ...
# Add your benchmarks to example_blazium_4.5_modified/src/gdexample.cpp ...

# Build everything and run benchmarks
./benchmarker.sh engine benchmarks run show

```

# Benchmark Blazium PRs
```sh
# Checkout repo
git clone https://github.com/MisterPuma80/blazium_benchmarker
cd blazium_benchmarker

# Download engines
./benchmarker.sh download


# Run patch benchmark
git checkout blazium_patch_597
./benchmarker.sh reset patch:597 engine benchmarks run show

# Run different patch benchmark
git checkout blazium_patch_602
./benchmarker.sh reset patch:602 cores:10 linker:mold use_llvm:yes engine benchmarks run show
```

# Requirements

. git

. curl

. python3

. python csvlook

. scons