

# Benchmark Blazium PRs
```sh
# Checkout repo
git clone https://github.com/MisterPuma80/blazium_benchmarker
cd blazium_benchmarker
git checkout benchmark_to_upper_to_lower_v6

# Download engines
./benchmarker.sh download

# Install patch from blazium PR into modified blazium
./benchmarker.sh reset patch=https://github.com/blazium-games/blazium/pull/652.patch

# Build engines benchmarks
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
