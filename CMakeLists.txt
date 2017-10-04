cmake_minimum_required(VERSION 3.0)
project(PiEscape2 LANGUAGES C CXX)

#
#
# Compiler setup
#
#

#according to https://gcc.gnu.org/projects/cxx-status.html we need at least 4.8.1 for C++11 and 4.9 for C++14
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
    message(FATAL_ERROR "You need at least gcc 4.9  (found gcc version ${CMAKE_CXX_COMPILER_VERSION})")
endif()

set(SYSTEM_TYPE x86)
set(WINDOWS_BINARIES_FOLDER Windows_binaries)
set(SDL_FOLDER_LOCATION libraries/SDL-1.2.15)
set(SDL_IMAGE_FOLDER_LOCATION libraries/SDL_image-1.2.12)
set(GLEW_FOLDER_LOCATION libraries/glew-2.0.0)
set(GLMC_FOLDER_LOCATION libraries/glmc)

set(WINDOWS_BINARIES ${CMAKE_CURRENT_SOURCE_DIR}/${WINDOWS_BINARIES_FOLDER}/${SYSTEM_TYPE})

if (WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W2")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -W2")
    #add_definitions(/W2)
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=gnu11 -Wall -Wno-comment -fms-extensions -fdiagnostics-color ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++14 -fms-extensions -Wall -Wno-comment -fdiagnostics-color")

    #consider -Werror instead of -Wall to make all warnings fatal:
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Werror -Wno-comment -Wno-unused-variable -fms-extensions -fdiagnostics-color ")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++14 -fms-extensions -Werror -Wno-comment -Wno-unused-variable -fdiagnostics-color")
endif()

# Detect if compiling on raspberry PI.
if ( EXISTS /opt/vc/include/bcm_host.h )
    message(STATUS "Note: Compiling on Raspberry Pi")
    add_definitions( -DRPI -DIS_RPI )
    set(RPI True)
else()
    message(STATUS "Note: Compiling on NON Raspberry Pi")
endif()


#
# Build lists with certain groups of source files.
# We will use these lists later.
#

file(GLOB es_SRC
        "pi_escape/es/*.h"
        "pi_escape/es/*.c"
        )

file(GLOB es_system_SRC
        "pi_escape/es/system/*.h"
        "pi_escape/es/system/*.c"
        )

file(GLOB graphics_SRC
        "pi_escape/graphics/*.h"
        "pi_escape/graphics/*.c"
        )

file(GLOB ANIM_SRC
        "pi_escape/anim/*.h"
        "pi_escape/anim/*.cpp"
        )

file(GLOB level_SRC
        "pi_escape/level/*.h"
        "pi_escape/level/*.c"
        )

file(GLOB sensor_SRC
        "pi_escape/sensor/*.h"
        "pi_escape/sensor/*.c"
        )

file(GLOB imu_SRC
        "pi_escape/imu/*.h"
        "pi_escape/imu/*.cpp"
        )

file(GLOB led_SRC
        "pi_escape/led/*.h"
        "pi_escape/led/*.c"
        "pi_escape/led/*.cpp"
        )

file(GLOB util_C_SRC
        "util/util.h"
        "util/util.c"
        "util/sleep.h"
        "util/sleep.c"
        "util/rgb_triple.h"
        )

file(GLOB test_C_SRC
        "util/min_unit.h"
        "util/min_unit.c"
        )

file(GLOB RTIMULib_SRC
        "libraries/RTIMULib/*.h"
        "libraries/RTIMULib/*.cpp"
        )

file(GLOB RTIMULib_drivers_SRC
        "libraries/RTIMULib/IMUDrivers/*.h"
        "libraries/RTIMULib/IMUDrivers/*.cpp"
        )

file(GLOB glmc_lib_SRC
        "libraries/glmc/include/*.h"
        "libraries/glmc/src/*.c"
        )

if (RPI)
    #nothing to do, it is added by wildcard already
else()
    #remove systems that should not be added

    #this doesn't work: #list(REMOVE_ITEM es_system_SRC "game/es/system/orientation_system.c" "game/es/system/orientation_system.h")

    string (REPLACE "pi_escape/es/system/orientation_system.c" "" es_system_SRC "${es_system_SRC}")
    string (REPLACE "pi_escape/es/system/orientation_system.h" "" es_system_SRC "${es_system_SRC}")
    string (REPLACE "pi_escape/es/system/sensor_system.c" "" es_system_SRC "${es_system_SRC}")
    string (REPLACE "pi_escape/es/system/sensor_system.h" "" es_system_SRC "${es_system_SRC}")
    string (REPLACE "pi_escape/es/system/led_system.c" "" es_system_SRC "${es_system_SRC}")
    string (REPLACE "pi_escape/es/system/led_system.h" "" es_system_SRC "${es_system_SRC}")
endif()


#
# Define which files are used to create which executable
#


set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${level_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${es_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${es_system_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${util_C_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${graphics_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${glmc_lib_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${led_SRC})
set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${sensor_SRC})


if (RPI)
    set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${imu_SRC})
    set(GAME_PART1_SRC_FILES ${GAME_PART1_SRC_FILES} ${RTIMULib_SRC} ${RTIMULib_drivers_SRC})
endif()


add_executable(pi_escape2_part1 pi_escape2_part1_main.c ${GAME_PART1_SRC_FILES})


add_executable(pi_escape2_part2 pi_escape2_part2_main.cpp ${ANIM_SRC} ${GAME_PART1_SRC_FILES})

add_executable(pi_escape2_benchmark benchmarking_main.c ${GAME_PART1_SRC_FILES})
add_executable(pi_escape2_part1_benchmark pi_escape2_part1_main.c ${GAME_PART1_SRC_FILES})


set(TEST_SOURCE_FILES pi_escape2_test_main.c ${test_C_SRC} ${GAME_PART1_SRC_FILES})
add_executable(pi_escape2_test ${TEST_SOURCE_FILES})


set(GLMC_DEMO_SOURCE_FILES glmc_demo_main.c)
add_executable(glmc_demo ${GLMC_DEMO_SOURCE_FILES} ${glmc_lib_SRC})




#
# Setup the source directory structure in the Visual Studio IDE
# (source_group is a visual tweak for Visual Studio, it does not change how things are compiled)
#

source_group("game\\led" FILES ${led_SRC})
source_group("game\\sensor" FILES ${sensor_SRC})
source_group("game\\imu" FILES ${imu_SRC})
source_group("game\\level" FILES ${level_SRC})
source_group("game\\es" FILES ${es_SRC})
source_group("game\\es\\system" FILES ${es_system_SRC})
source_group("util" FILES ${util_CPP_SRC} ${util_C_SRC} ${test_C_SRC})
source_group("game\\graphics" FILES ${graphics_SRC})
source_group("game\\anim" FILES ${ANIM_SRC})




#
# Now find the required dependencies (libraries) and use them when linking the executables
#


find_package(OpenGL REQUIRED)
if (WIN32)
    set(SDL_LIBRARY ${SDL_FOLDER_LOCATION}/lib/${SYSTEM_TYPE}/SDL)
    set(SDL_INCLUDE_DIR ${SDL_FOLDER_LOCATION}/include)
    set(SDL_IMAGE_LIBRARY ${SDL_IMAGE_FOLDER_LOCATION}/lib/${SYSTEM_TYPE}/SDL_image)
    set(SDL_IMAGE_INCLUDE_DIR ${SDL_IMAGE_FOLDER_LOCATION}/include)
endif()

find_Package(SDL REQUIRED)
find_Package(SDL_image REQUIRED)

if(NOT SDL_FOUND)
    #on debian based systems, if this fails, run: apt-get install libsdl1.2-dev
    message( FATAL ERROR "SDL not found!")
endif(NOT SDL_FOUND)

if(NOT SDLIMAGE_FOUND)
    #on debian based systems, if this fails, run: apt-get install libsdl-image1.2-dev
    message( FATAL ERROR "SDL image not found!")
endif(NOT SDLIMAGE_FOUND)

if ( RPI )
    include_directories( /opt/vc/include/ /opt/vc/include/interface/vcos/pthreads /opt/vc/include/interface/vmcs_host/linux /opt/vc/src/hello_pi/libs/ilclient /opt/vc/src/hello_pi/libs/vgfont )
    find_library (BCM_HOST_LIBRARY bcm_host HINTS /opt/vc/lib/ /opt/vc/src/hello_pi/libs/vgfont )
    find_library (VCOS_LIBRARY vcos HINTS /opt/vc/lib/ /opt/vc/src/hello_pi/libs/vgfont )
    find_library (VCHIQ_ARM_LIBRARY vchiq_arm HINTS /opt/vc/lib/ /opt/vc/src/hello_pi/libs/vgfont )
    find_library (GLESv2_LIBRARY GLESv2 HINTS /opt/vc/lib/ /opt/vc/src/hello_pi/libs/vgfont )
    find_library (EGL_LIBRARY EGL HINTS /opt/vc/lib/ /opt/vc/src/hello_pi/libs/vgfont )

    target_link_libraries (pi_escape2_part1 ${GLESv2_LIBRARY} ${BCM_HOST_LIBRARY})
    target_link_libraries (pi_escape2_part1 ${EGL_LIBRARY} ${VCOS_LIBRARY} ${VCHIQ_ARM_LIBRARY} )
    target_link_libraries (pi_escape2_part1 pthread rt dl )

    target_link_libraries (pi_escape2_part2 ${GLESv2_LIBRARY} ${BCM_HOST_LIBRARY})
    target_link_libraries (pi_escape2_part2 ${EGL_LIBRARY} ${VCOS_LIBRARY} ${VCHIQ_ARM_LIBRARY} )
    target_link_libraries (pi_escape2_part2 pthread rt dl )

    target_link_libraries (pi_escape2_test ${GLESv2_LIBRARY} ${BCM_HOST_LIBRARY})
    target_link_libraries (pi_escape2_test ${EGL_LIBRARY} ${VCOS_LIBRARY} ${VCHIQ_ARM_LIBRARY} )
    target_link_libraries (pi_escape2_test pthread rt dl )

    target_link_libraries (pi_escape2_part1_benchmark ${GLESv2_LIBRARY} ${BCM_HOST_LIBRARY})
    target_link_libraries (pi_escape2_part1_benchmark ${EGL_LIBRARY} ${VCOS_LIBRARY} ${VCHIQ_ARM_LIBRARY} )
    target_link_libraries (pi_escape2_part1_benchmark pthread rt dl )

    target_link_libraries (pi_escape2_benchmark ${GLESv2_LIBRARY} ${BCM_HOST_LIBRARY})
    target_link_libraries (pi_escape2_benchmark ${EGL_LIBRARY} ${VCOS_LIBRARY} ${VCHIQ_ARM_LIBRARY} )
    target_link_libraries (pi_escape2_benchmark pthread rt dl )
else()
    if(WIN32)
        set(GLEW_LIBRARY ${GLEW_FOLDER_LOCATION}/lib/${SYSTEM_TYPE}/glew32)
        set(GLEW_INCLUDE_DIR ${GLEW_FOLDER_LOCATION}/include)
    endif()

    #on debian based systems, if this fails, run: apt-get install libglew-dev
    Find_Package(GLEW REQUIRED)
    include_directories(${GLEW_INCLUDE_DIR} )
    target_link_libraries (pi_escape2_part1 ${GLEW_LIBRARY} )
    target_link_libraries (pi_escape2_part2 ${GLEW_LIBRARY} )
    target_link_libraries (pi_escape2_test ${GLEW_LIBRARY} )

    target_link_libraries (pi_escape2_benchmark ${GLEW_LIBRARY} )
    target_link_libraries (pi_escape2_part1_benchmark ${GLEW_LIBRARY} )
endif()

set(GLMC_INCLUDE_DIR ${GLMC_FOLDER_LOCATION}/include)
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${GLMC_INCLUDE_DIR}/glmc.h )
    message( FATAL_ERROR "GLMC header not found at ${GLMC_INCLUDE_DIR}/glmc.h!")
endif()

set(GLMC_SOURCE_DIR ${GLMC_FOLDER_LOCATION}/src)
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${GLMC_SOURCE_DIR}/glmc_model.c )
    message( FATAL_ERROR "GLMC source not found at ${GLMC_SOURCE_DIR}/glmc_model.c!")
endif()

include_directories(${SDL_INCLUDE_DIR} ${SDL_IMAGE_INCLUDE_DIR} ${GLMC_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS} )
target_link_libraries (pi_escape2_part1 ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${GLMC_LIBRARY} ${OPENGL_LIBRARY})
target_link_libraries (pi_escape2_part2 ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${GLMC_LIBRARY} ${OPENGL_LIBRARY})
target_link_libraries (pi_escape2_test ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${GLMC_LIBRARY} ${OPENGL_LIBRARY})

target_link_libraries (pi_escape2_benchmark ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${GLMC_LIBRARY} ${OPENGL_LIBRARY})
target_link_libraries (pi_escape2_part1_benchmark ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${GLMC_LIBRARY} ${OPENGL_LIBRARY})
target_link_libraries (glmc_demo ${GLMC_LIBRARY})
if(NOT WIN32)
    target_link_libraries (pi_escape2_part1 m)
    target_link_libraries (pi_escape2_part2 m)
    target_link_libraries (pi_escape2_test m)

    target_link_libraries (pi_escape2_benchmark m)
    target_link_libraries(pi_escape2_part1_benchmark m)
    target_link_libraries(glmc_demo m)
else()
    file(GLOB MY_LIB_DLLS
            "${WINDOWS_BINARIES}/*.dll"
            )
    file(COPY ${MY_LIB_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()

target_compile_definitions (pi_escape2_benchmark PRIVATE BENCHMARK=1)
target_compile_definitions (pi_escape2_part1_benchmark PRIVATE BENCHMARK=1)
