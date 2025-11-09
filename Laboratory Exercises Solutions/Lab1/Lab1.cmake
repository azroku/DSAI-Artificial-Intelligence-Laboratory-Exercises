# Set the name of the project and the final executable
set(Lab1_NAME Lab1)

# Find all the necessary source and header files for the project
file(GLOB Lab1_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB Lab1_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(Lab1_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB Lab1_INC_TD  ${MY_INC}/td/*.h)
file(GLOB Lab1_INC_GUI ${MY_INC}/gui/*.h)

# Set up the application icon for different platforms (macOS, Windows, etc.)
set(Lab1_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(Lab1_APP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(Lab1_APP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# Create the executable program from all the gathered source files
add_executable(${Lab1_NAME} ${Lab1_INCS} ${Lab1_SOURCES} ${Lab1_INC_TD}  
				${Lab1_INC_GUI} ${Lab1_APP_ICON})

# Organize the files into virtual folders in the IDE for better navigation
source_group("inc"            FILES ${Lab1_INCS})
source_group("inc\\td"        FILES ${Lab1_INC_TD})
source_group("inc\\gui"        FILES ${Lab1_INC_GUI})
source_group("src"            FILES ${Lab1_SOURCES})

# Link the executable with the required natId framework libraries
target_link_libraries(${Lab1_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

# Use natId's custom functions to finalize the GUI application setup
setTargetPropertiesForGUIApp(${Lab1_NAME} ${Lab1_PLIST})

setAppIcon(${Lab1_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${Lab1_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${Lab1_NAME})