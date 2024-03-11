# SHM Operation Helper

This lib has built-in thread to sync the data periodically, but usually I used another thread and sync the data either with `write` or `read`

## Build
```bash
mkdir build
cmake -S . -B build
cmake --build build --clean-first
```