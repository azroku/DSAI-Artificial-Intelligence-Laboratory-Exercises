set(MAZESALIH_NAME mazeSalih)				#Naziv prvog projekta u solution-u

file(GLOB MAZESALIH_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB MAZESALIH_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(MAZESALIH_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB MAZESALIH_INC_TD  ${MY_INC}/td/*.h)
file(GLOB MAZESALIH_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${MAZESALIH_NAME} ${MAZESALIH_INCS} ${MAZESALIH_SOURCES} ${MAZESALIH_INC_TD}  ${MAZESALIH_INC_GUI})

source_group("inc"            FILES ${MAZESALIH_INCS})
source_group("inc\\td"        FILES ${MAZESALIH_INC_TD})
source_group("inc\\gui"        FILES ${MAZESALIH_INC_GUI})
source_group("src"            FILES ${MAZESALIH_SOURCES})

target_link_libraries(${MAZESALIH_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${MAZESALIH_NAME} ${MAZESALIH_PLIST})

setIDEPropertiesForGUIExecutable(${MAZESALIH_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${MAZESALIH_NAME})
