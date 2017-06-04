cmake_minimum_required(VERSION 2.8)


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "  sleep 1;  CCACHE_DIR=/tmp/ccachedir ccache")
endif(CCACHE_FOUND)


set(CMAKE_VERBOSE_MAKEFILE ON)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


set(CMAKE_C_COMPILER  gcc)
set(CMAKE_C_FLAGS   "-std=c14 -Werror")
set(CMAKE_CXX_COMPILER  "g++")
set(CMAKE_CXX_FLAGS "-std=c++14 -Werror")
set(CMAKE_CXX_LINK_FLAGS  "-lstdc++ -lz -lpng -lSDL2 -lSDL2_image -lSDL2main")


set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS}   -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -march=core2 -fomit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS}   -Og -g")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -Og -g")


if(EMSCRIPTEN)
  set(EXECUTABLE_EXTENSION ".html")
  set(CMAKE_CXX_COMPILER  em++)
  set(CMAKE_CXX_FLAGS "-std=c++14 -Werror -Wno-switch -O2")
  set(CMAKE_CXX_FLAGS_DEBUG "")
  set(CMAKE_CXX_LINK_FLAGS  "${CMAKE_CXX_LINK_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2 --preload-file test.png --preload-file motion.png --use-preload-plugins")
endif()




set(SOURCES
  isom_LineContext.cpp
  isom_FaceRenderingContext.cpp
  isom_TextureRenderingContext.cpp
  isom_point.cpp
  isom_plane.cpp
  isom_renderer.cpp
  isom_font.cpp
  isom_image.cpp
  isom_screen.cpp
)


include_directories(.)
include_directories(/usr/include/SDL2)
add_executable("testMain${EXECUTABLE_EXTENSION}" test_main.cpp ${SOURCES})
add_executable("testLineContext${EXECUTABLE_EXTENSION}" test__LineContext.cpp ${SOURCES})


if(EMSCRIPTEN)
# set_target_properties("Dolmo${EXECUTABLE_EXTENSION}" PROPERTIES LINK_FLAGS "--shell-file shell.html")
endif()




