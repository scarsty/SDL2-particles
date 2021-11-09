cmake_minimum_required(VERSION 3.20)
project(particles-lib)
set(CMAKE_CXX_STANDARD 17)

#find_package(SDL2)

set(particles-lib_src ParticleSystem.h ParticleSystem.cpp)
#set(particles-test_src ParticleExample.h ParticleExample.cpp main.cpp)

add_library(particles-lib STATIC ${particles-lib_src})
#add_executable(particles-test ${particles-test_src})

#target_link_libraries(particles-test
#        PRIVATE ${SDL2_LIBRARIES}
#        PRIVATE particles-lib
#        )

target_include_directories(particles-lib PUBLIC ./)
#target_include_directories(particles-test PRIVATE ${SDL2_INCLUDE_DIRS})