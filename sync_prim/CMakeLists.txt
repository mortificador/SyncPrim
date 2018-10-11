cmake_minimum_required (VERSION 3.8)

set(SYNC_PRIM_SRC 
	${CMAKE_CURRENT_SOURCE_DIR}/src/base_reset_event.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/manual_reset_event.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/auto_reset_event.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/count_down_event.cpp
)

add_library(sync_prim ${SYNC_PRIM_SRC})
