set(Lab3_NAME Lab3)

file(GLOB Lab3_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB Lab3_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)

file(GLOB Lab3_INC_TD  ${MY_INC}/td/*.h)
file(GLOB Lab3_INC_GUI ${MY_INC}/gui/*.h)

set(Lab3_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(Lab3_APP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(Lab3_APP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

add_executable(${Lab3_NAME} ${Lab3_INCS} ${Lab3_SOURCES} ${Lab3_INC_TD}  
				${Lab3_INC_GUI} ${Lab3_APP_ICON})

source_group("inc"            FILES ${Lab3_INCS})
source_group("inc\\td"        FILES ${Lab3_INC_TD})
source_group("inc\\gui"        FILES ${Lab3_INC_GUI})
source_group("src"            FILES ${Lab3_SOURCES})

target_link_libraries(${Lab3_NAME} 
    debug ${MU_LIB_DEBUG} 
    debug ${NATGUI_LIB_DEBUG} 
    optimized ${MU_LIB_RELEASE} 
    optimized ${NATGUI_LIB_RELEASE}
)

setTargetPropertiesForGUIApp(${Lab3_NAME} ${Lab3_PLIST})

setAppIcon(${Lab3_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${Lab3_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${Lab3_NAME})