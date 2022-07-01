cmake_minimum_required(VERSION 3.16.3)
project(CppParticleSystem)

set(CppParticleSystem
        ParticleSystem.h
        ParticleSystem.cpp
        )

add_library(CppParticleSystem STATIC ${CppParticleSystem})
target_include_directories(CppParticleSystem PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")