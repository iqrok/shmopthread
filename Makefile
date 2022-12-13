all: build

create-dir:
	rm -rf build
	mkdir -p build

build: create-dir
	cmake -S . -B build
	cmake --build build --clean-first

build-combined: create-dir
	cmake -DCOMBINED=1 -S . -B build
	cmake --build build --clean-first

debug: create-dir
	cmake -DDEBUG=1 -S . -B build/
	cmake --build build --clean-first

sample:
	rm -rf example/build
	mkdir -p example/build
	cmake -S example -B example/build/
	cmake --build example/build --clean-first
	example/build/test
