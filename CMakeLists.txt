cmake_minimum_required(VERSION 2.8)


if(DEFINED SLEEPING_TIME)
else()
  set(SLEEPING_TIME "0")
endif()


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "  sleep ${SLEEPING_TIME};  CCACHE_DIR=/tmp/ccachedir ccache")
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


if(EM)
  set(EXTENSION ".html")
  set(CMAKE_CXX_COMPILER  em++)
  set(CMAKE_CXX_FLAGS "-std=c++14 -Werror -Wno-switch -O2")
  set(CMAKE_CXX_FLAGS_DEBUG "")
  set(CMAKE_CXX_LINK_FLAGS  "${CMAKE_CXX_LINK_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2 --preload-file expandmetal.png --preload-file lena_std.png --use-preload-plugins")
endif()




set(SOURCES
  isom_LineContext.cpp
  isom_FaceRenderingContext.cpp
  isom_TextureRenderingContext.cpp
  isom_point.cpp
  isom_transformer.cpp
  isom_dot.cpp
  isom_DotSet.cpp
  isom_line.cpp
  isom_polygon.cpp
  isom_TexturedPolygon.cpp
  isom_plane.cpp
  isom_box.cpp
  isom_object.cpp
  isom_renderer.cpp
  isom_font.cpp
  isom_image.cpp
  isom_screen.cpp
)


include_directories(.)
include_directories(/usr/include/SDL2)
add_executable("testBox${EXTENSION}" testBox.cpp ${SOURCES})
add_executable("testLineContext${EXTENSION}" testLineContext.cpp ${SOURCES})
add_executable("testTextureRenderingContext${EXTENSION}" testTextureRenderingContext.cpp ${SOURCES})
add_executable("testBuilding${EXTENSION}" testBuilding.cpp ${SOURCES})


if(EM)
  set_target_properties("testTextureRenderingContext${EXTENSION}" PROPERTIES LINK_FLAGS "--shell-file ../shell__tTRC.html")
  set_target_properties("testBox${EXTENSION}" PROPERTIES LINK_FLAGS "--shell-file ../shell__tB.html")
  set_target_properties("testLineContext${EXTENSION}" PROPERTIES LINK_FLAGS "--shell-file ../shell__tLC.html")
endif()




