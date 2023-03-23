cmake_minimum_required(VERSION 3.14)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)

# Find Boost libraries
set(Boost_INCLUDE_DIR /home/hilda/Hanif/Mapping/1.79.0/include)
set(Boost_LIBRARY_DIR /home/hilda/Hanif/Mapping/1.79.0/lib)
set(BOOST_ROOT /home/hilda/Hanif/Mapping/1.79.0)
find_package(Boost COMPONENTS system filesystem REQUIRED)

# Add your source files here
add_executable(MyExecutable main.cpp)

# Link with Boost libraries
target_link_libraries(MyExecutable Boost::system Boost::filesystem pthread)
