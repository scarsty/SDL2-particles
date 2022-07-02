cmake_minimum_required(VERSION 3.16.3)
project(sdl2-example)

add_executable(sdl2-example main.cpp DemoApplication.cpp DemoApplication.h ParticleObject.cpp ParticleObject.h)

if( WIN32 OR WIN64 )
    find_package(SDL2 CONFIG REQUIRED)
    find_package(sdl2-image CONFIG REQUIRED)
    target_link_libraries(sdl2-example PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2_image CppParticleSystem)
else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(sdl2-example ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} CppParticleSystem)
endif()

#target_link_directories(sdl2-example PRIVATE CppParticleSystem/)
#target_link_libraries(sdl2-example CppParticleSystem)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)