

# Benchmark Blazium PRs
```sh
# Checkout repo
git clone https://github.com/MisterPuma80/blazium_benchmarker
cd blazium_benchmarker
git checkout benchmark_to_upper_to_lower_v6

# Download engines
./benchmarker.sh download

# Install patch from branch into modified blazium
./benchmarker.sh reset patch=https://github.com/blazium-games/blazium/compare/blazium-dev...MisterPuma80:blazium:blazium_faster_string_to_lower6.diff

# Or Install patch from blazium pr into modified blazium
# ./benchmarker.sh reset patch=https://github.com/blazium-games/blazium/pull/696969.patch

# Build benchmark
./benchmarker.sh engines
./benchmarker.sh benchmarks

# Run benchmark
./benchmarker.sh run show


```

# Requirements

- git
- python3
- python csvlook
- scons
- curl
- C++ compiler and linker
