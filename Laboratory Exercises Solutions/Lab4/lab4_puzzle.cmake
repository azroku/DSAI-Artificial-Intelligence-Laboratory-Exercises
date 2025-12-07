# Set the name of our executable
set(PUZZLE_APP_NAME lab4_8puzzle)

# Automatically find all source (.cpp) and header (.h) files in the src directory
file(GLOB PUZZLE_SOURCES  "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
file(GLOB PUZZLE_HEADERS  "${CMAKE_CURRENT_LIST_DIR}/src/*.h")

# Define the executable target
add_executable(${PUZZLE_APP_NAME} ${PUZZLE_SOURCES} ${PUZZLE_HEADERS})

# Link the required NatID libraries (mainUtils and natGUI)
target_link_libraries(${PUZZLE_APP_NAME}
    debug       ${MU_LIB_DEBUG}
    debug       ${NATGUI_LIB_DEBUG}
    optimized   ${MU_LIB_RELEASE}
    optimized   ${NATGUI_LIB_RELEASE}
)

# Set properties needed for a NatID GUI application
set(PUZZLE_PLIST ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
setTargetPropertiesForGUIApp(${PUZZLE_APP_NAME} ${PUZZLE_PLIST})
setIDEPropertiesForGUIExecutable(${PUZZLE_APP_NAME} ${CMAKE_CURRENT_LIST_DIR})
setPlatformDLLPath(${PUZZLE_APP_NAME})