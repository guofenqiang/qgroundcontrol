
add_library(tools
	telecontrol.cpp
	ecm.cpp
)

target_link_libraries(tools
	PUBLIC
		qgc
)

target_include_directories(tools
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
	)

