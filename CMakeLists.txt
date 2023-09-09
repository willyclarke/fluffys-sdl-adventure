cmake_minimum_required(VERSION 3.26)

# Specify the project name and the required C++ standard
project(fluffydalek VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        main
)

FetchContent_Declare(
  SDL3_ttf
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
  GIT_TAG        main
)

################################################################################
## NOTE: Build applications with static libraries.
################################################################################
set(BUILD_SHARED_LIBS OFF)

################################################################################
## Fetch Catch2 and SDL
################################################################################
FetchContent_MakeAvailable(Catch2 SDL3 SDL3_ttf)

FetchContent_GetProperties(SDL3)
FetchContent_GetProperties(SDL3_ttf)

if(NOT sdl3_POPULATED)
  FetchContent_Populate(SDL3)
  add_subdirectory(${sdl3_SOURCE_DIR} ${sdl3_BINARY_DIR})
endif()

if(NOT sdl3_ttf_POPULATED)
  FetchContent_Populate(SDL3_ttf)
  add_subdirectory(${sdl3_ttf_SOURCE_DIR} ${sdl3_ttf_BINARY_DIR})
endif()

message(STATUS "sdl3_SOURCE_DIR: ${sdl3_SOURCE_DIR}")
message(STATUS "sdl3_ttf_SOURCE_DIR: ${sdl3_ttf_SOURCE_DIR}")

##############################################################################
# Add library target with source files
##############################################################################
add_library(drawprimitives
  src/lib/drawprimitives.cpp
  src/lib/triangle2d.cpp
  src/lib/fluffymath.cpp
  src/lib/splines.cpp
)

##############################################################################
# Link SDL libraries to the executable
# Note that we are using public for the library so that it propagates the
# include folders to anyone linking to the library.
##############################################################################
target_link_libraries(drawprimitives PUBLIC SDL3::SDL3-static SDL3_ttf::SDL3_ttf-static)

add_executable(triangle apps/triangle.cpp )
target_link_libraries(triangle PRIVATE drawprimitives)

add_executable(wireframe apps/wireframe.cpp)
target_link_libraries(wireframe PRIVATE drawprimitives)

if(APPLE)
  set(APPNAME FluffyDalek)
  set(MACOSX_BUNDLE ON)
  add_executable(${APPNAME} MACOSX_BUNDLE apps/wireframe.cpp)
  target_link_libraries(${APPNAME} PRIVATE drawprimitives)

  set_target_properties(${APPNAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME "Fluffy Dalek"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_LONG_VERSION_STRING "1.0.0"
    MACOSX_BUNDLE_ICON_FILE "AppIcon.icns"
    MACOSX_BUNDLE_GUI_IDENTIFIER "no.clarke.fluffydalek"
    )

  # Additional macOS-specific configurations...
  install(TARGETS ${APPNAME}
    BUNDLE DESTINATION ./
    RUNTIME DESTINATION bin
    )
endif()

# Enable testing
enable_testing()

# Define test files (replace with your test files)
set(TEST_FILES tests/test1.cpp)

# Add test executable
add_executable(tests ${TEST_FILES}
  src/lib/triangle2d.cpp
  src/lib/fluffymath.cpp
  src/lib/splines.cpp
)

# Link test executable with Catch2 and your project libraries
target_link_libraries(tests Catch2::Catch2WithMain)

# Add test to CTest
include(CTest)
include(Catch)
catch_discover_tests(tests)

###
# Installation.
###
# Font files
if(APPLE)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources/TTF
    DESTINATION ${APPNAME}.app/Contents/resources
    )
else()
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources/TTF
    DESTINATION resources)
endif()

include(CPack)
set(CPACK_PACKAGE_NAME "FluffyDalek")
set(CPACK_PACKAGE_VERSION "1.0.0")
