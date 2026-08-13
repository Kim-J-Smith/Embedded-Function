# HOW TO BENCHMARK

1. Make sure you have basic C++ compiler and building tools installed. (g++/clang++ and make/ninja) If not, you can use following command to install them on Ubuntu/Debian:

```bash
sudo apt update && sudo apt install build-essential -y
```

2. Make sure you have CMake installed. If not, download and install it from [here](https://cmake.org/download/). Or you can use following command to install CMake on Ubuntu/Debian:

```bash
sudo apt update && sudo apt install cmake -y
```

3. Use following command to run benchmark:

```bash
cd ./benchmark # Go to benchmark directory: <repo_root>/benchmark

cmake -B build -S . -DCMAKE_CXX_STANDARD=20 # Generate build files (proxy need C++20)

cmake --build build --config Release --target benchmark --parallel
```
