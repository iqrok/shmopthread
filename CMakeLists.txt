cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME "ShmopThread")
set(PROJECT_VERSION "0.0.1")

project("${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O2")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_compile_options (-fcolor-diagnostics)
endif ()

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories("${INCLUDE_DIR}")

set(OUTPUT_LIB_SHMOPTHREAD "${PROJECT_NAME}")
set(OBJLIB "$<TARGET_OBJECTS:${OUTPUT_LIB_SHMOPTHREAD}>")
set(SOURCE_LIB_SHMOPTHREAD "${INCLUDE_DIR}/ShmopThread.h" "${SOURCE_DIR}/ShmopThread.cpp")

# this is the "object library" target: compiles the sources only once
add_library("${OUTPUT_LIB_SHMOPTHREAD}" OBJECT "${SOURCE_LIB_SHMOPTHREAD}")
# shared libraries need PIC
set_property(TARGET "${OUTPUT_LIB_SHMOPTHREAD}" PROPERTY POSITION_INDEPENDENT_CODE 1)

# static lib
add_library("${OUTPUT_LIB_SHMOPTHREAD}_STATIC" STATIC "${OBJLIB}")
set_target_properties("${OUTPUT_LIB_SHMOPTHREAD}_STATIC" PROPERTIES OUTPUT_NAME "${OUTPUT_LIB_SHMOPTHREAD}")

# shared lib
add_library("${OUTPUT_LIB_SHMOPTHREAD}_SHARED" SHARED "${OBJLIB}")
set_target_properties("${OUTPUT_LIB_SHMOPTHREAD}_SHARED" PROPERTIES OUTPUT_NAME "${OUTPUT_LIB_SHMOPTHREAD}")

# example write
SET(OUTPUT_BINARY_TEST_WRITE "test-write")
SET(SOURCE_FILES_TEST_WRITE "${CMAKE_CURRENT_SOURCE_DIR}/example/write.cpp")
add_executable("${OUTPUT_BINARY_TEST_WRITE}" "${SOURCE_FILES_TEST_WRITE}" "${OBJLIB}")

# example write
SET(OUTPUT_BINARY_TEST_READ "test-read")
SET(SOURCE_FILES_TEST_READ "${CMAKE_CURRENT_SOURCE_DIR}/example/read.cpp")
add_executable("${OUTPUT_BINARY_TEST_READ}" "${SOURCE_FILES_TEST_READ}" "${OBJLIB}")
