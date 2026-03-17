

# Simple Example
```sh
# Checkout repo
git clone https://github.com/MisterPuma80/blazium_benchmarker
cd blazium_benchmarker
git checkout all_benchmarks

# Download engines
./benchmarker.sh download

# Optionally install patch from branch into modified blazium
./benchmarker.sh reset patch=https://github.com/blazium-games/blazium/compare/blazium-4.5...MisterPuma80:blazium:faster_string_to_lower5.diff

# Or Install patch from blazium pr into modified blazium
#./benchmarker.sh reset patch=https://github.com/blazium-games/blazium/pull/696969.patch

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
