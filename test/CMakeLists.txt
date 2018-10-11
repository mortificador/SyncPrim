cmake_minimum_required(VERSION 3.8)
project(sync_prim_tests)

set(SYNC_PRIM_PROJ_DIR 
	${CMAKE_CURRENT_DIR}../sync_prim/
)
set (EXTERNAL_PROJECTS_DIR
	${CMAKE_CURRENT_DIR}../external
)
set(GOOGLETEST_PROJECT_DIR
	${EXTERNAL_PROJECTS_DIR}/googletest/googletest
)
set(GOOGLEMOCK_PROJECT_DIR
	${EXTERNAL_PROJECTS_DIR}/googletest/googlemock
)
set(TEST_SRC_FILES 
	${CMAKE_CURRENT_DIR}main.cpp
	${CMAKE_CURRENT_DIR}manual_reset_event_tests.cpp
	${CMAKE_CURRENT_DIR}count_down_event_tests.cpp
)


include_directories ("${SYNC_PRIM_PROJ_DIR}/inc")
include_directories ("${GOOGLETEST_PROJECT_DIR}/include")
include_directories ("${GOOGLEMOCK_PROJECT_DIR}/include")

add_executable(sync_prim_tests ${TEST_SRC_FILES})

add_subdirectory("${SYNC_PRIM_PROJ_DIR}" sync_prim)

#gtest brings in gmock by default
add_subdirectory("${GOOGLETEST_PROJECT_DIR}" gtest) 

target_link_libraries(sync_prim_tests sync_prim gtest)
