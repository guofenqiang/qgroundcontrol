
add_library(ProtocolConversion
	udpclient.cpp
	udpserver.cpp
	protocolconversion.cpp
)

target_link_libraries(ProtocolConversion
	PUBLIC
		qgc
)

target_include_directories(ProtocolConversion
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
	)

