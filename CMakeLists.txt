cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

project(cv-snake)

add_executable(cv-snake src/game.cpp)
add_executable(cv-snake-ai src/ai.cpp)

add_subdirectory(libs/raylib)
find_package(Torch REQUIRED HINTS libs/libtorch)
find_package(OpenCV REQUIRED)

message(STATUS "TORCH_INCLUDE_DIRS: ${TORCH_INCLUDE_DIRS}")
message(STATUS "TORCH_LIBRARIES: ${TORCH_LIBRARIES}")
message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")

include_directories(include libs/raygui/src ${TORCH_INCLUDE_DIRS})

target_include_directories(cv-snake-ai PRIVATE ${OpenCV_INCLUDE_DIRS})

target_link_libraries(cv-snake-ai ${TORCH_LIBRARIES} opencv_core opencv_highgui opencv_imgproc)
target_link_libraries(cv-snake raylib)