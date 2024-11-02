cmake_minimum_required(VERSION 3.21)
project(SimuMori LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin")

FetchContent_Declare(json
	GIT_REPOSITORY https://github.com/nlohmann/json.git
	GIT_TAG 6af826d0bdb55e4b69e3ad817576745335f243ca)
FetchContent_MakeAvailable(json)

add_executable(SimuMori)
add_subdirectory(Source)

target_include_directories(SimuMori
  PRIVATE Include
)
target_link_libraries(SimuMori
  PRIVATE nlohmann_json
)

add_custom_target(Run
  COMMAND SimuMori
  DEPENDS SimuMori
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
