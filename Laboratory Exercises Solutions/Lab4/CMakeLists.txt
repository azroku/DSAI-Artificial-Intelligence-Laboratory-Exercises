cmake_minimum_required(VERSION 3.18)

# Set the name for the Visual Studio solution file
set(SOLUTION_NAME lab4_8puzzle)

# ======= Boilerplate for all NatID Projects =======
project(${SOLUTION_NAME})
set(SOURCE_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(HOME_ROOT $ENV{HOME})
if (WIN32)
    string(REPLACE "\\" "/" HOME_ROOT "${HOME_ROOT}")
endif()
set(WORK_ROOT ${HOME_ROOT}/Work)
include(${WORK_ROOT}/DevEnv/Common.cmake)
# ===============================================

# Include the framework scripts we need
include(${WORK_ROOT}/DevEnv/natGUI.cmake)

# Include the project-specific cmake file
include(lab4_puzzle.cmake)