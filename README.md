

# To Benchmark Blazium PR
```sh
# Checkout repo
git clone https://github.com/MisterPuma80/blazium_benchmarker
cd blazium_benchmarker
git checkout benchmark_to_upper_to_lower

# Download engines
./benchmarker.sh download

# Modify the engine code at example_blazium_4.5_modified/blazium ...
# Add your benchmarks to example_blazium_4.5_modified/src/gdexample.cpp ...

# Build everything and run benchmarks
./benchmarker.sh engines benchmarks run show

# Run again using different compiler and linker settings
./benchmarker.sh cores=10 linker=mold use_llvm=yes engines benchmarks run show

```

# Benchmark Blazium PRs
```sh
# Checkout repo
git clone https://github.com/MisterPuma80/blazium_benchmarker
cd blazium_benchmarker

# Download engines
./benchmarker.sh download

# Install patch into modified blazium
./benchmarker.sh reset patch=https://github.com/blazium-games/blazium/pull/597.patch

# Run benchmark
./benchmarker.sh engines benchmarks run show

```

# Requirements

- git
- python3
- python csvlook
- scons
- curl
- C++ compiler and linker
