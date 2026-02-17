# blazium_benchmarker

# Benchmark Blazium PR
```sh
# Checkout repo
git clone https://github.com/MisterPuma80/blazium_benchmarker
cd blazium_benchmarker

# Download engines
./benchmarker.sh download


# Run patch benchmark
git checkout benchmark_to_upper_to_lower
./benchmarker.sh reset patch:597 engine benchmarks run show

# Run patch benchmark with clang and mold
git checkout benchmark_to_upper_to_lower
./benchmarker.sh reset linker:mold use_llvm:yes patch:597 engine benchmarks run show
```

# Requirements

. git

. curl

. python3

. python csvlook

. scons