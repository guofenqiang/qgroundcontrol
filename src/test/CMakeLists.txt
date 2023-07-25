
add_library(MyObject
	myobject.cc
)

target_link_libraries(MyObject
	PUBLIC
		qgc
)

target_include_directories(MyObject
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
	)

