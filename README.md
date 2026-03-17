

# Simple Example
```sh
# Checkout repo
git clone https://github.com/MisterPuma80/blazium_benchmarker
cd blazium_benchmarker
git checkout benchmark_get_children

# Download engines
./benchmarker.sh download

# Modify the engine code at example_blazium_4.5_modified/blazium ...
# Add your benchmarks to example_blazium_4.5_modified/src/gdexample.cpp ...

# Run benchmark
./benchmarker.sh engines benchmarks run show

# Run again using different compiler and linker settings
#./benchmarker.sh cores=10 linker=mold use_llvm=yes engines benchmarks run show

```

# Requirements

- git
- python3
- python csvlook
- scons
- curl
- C++ compiler and linker
