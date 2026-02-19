

# To Benchmark Blazium PR
```sh
# Checkout repo
git clone https://github.com/MisterPuma80/blazium_benchmarker
cd blazium_benchmarker
git checkout benchmark_to_upper_to_lower

# Download engines
./benchmarker.sh download

# Download and patch engine with PR code
./benchmarker.sh reset patch:597 engine benchmarks run show

# Build engines and benchmark game
./benchmarker.sh engine benchmarks run show

# Run benchmarks and show graph
./benchmarker.sh run show

# Or run patch benchmark with clang and mold
./benchmarker.sh reset linker:mold use_llvm:yes patch:597 engine benchmarks run show
```

# Requirements

- git
- python3
- python csvlook
- scons
- curl
- C++ compiler and linker
