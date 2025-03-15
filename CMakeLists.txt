# CMakeList.txt : CMake project for GraphEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("GraphEngine")

# Add source to this project's executable.
add_executable (GraphEngine "GraphEngine.cpp"  "Input.h" "Graph.h" "Graph.cpp" "Settings.h"  "NodeAndEdge.h"   "Simulation.h" "Simulation.cpp" "Input.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GraphEngine PROPERTY CXX_STANDARD 20)
endif()

# Find SFML (Make sure SFML is installed correctly!)
set(SFML_DIR "C:/Users/user/SFML-3.0.0/lib/cmake/SFML") # Adjust the path if needed
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System)

# Include SFML headers
target_include_directories(GraphEngine PRIVATE "C:/Users/user/SFML-3.0.0/include")

# Link SFML libraries
target_link_libraries(GraphEngine SFML::Graphics SFML::Window SFML::System)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi /DEBUG")
