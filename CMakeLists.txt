cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME "ShmopThread")
set(PROJECT_VERSION "0.1.0")

project("${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_compile_options (-fcolor-diagnostics)
endif ()

add_compile_options(
	"-Wall"
	"-Wfatal-errors"
	"-fexceptions"
	"-Wpedantic"
	"-O2")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

# link lib pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

# link lib rt (realtime), for shm_op and shm_unlink
link_libraries(rt)

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

if(BUILD_TEST)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/example")
endif ()
