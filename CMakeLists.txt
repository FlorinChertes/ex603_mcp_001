cmake_minimum_required (VERSION 2.6)
project (ex603_mcp_001_prj)

IF (MSVC) 
	# require C++17
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")

	# Activate C++ exception handling 
	IF (NOT CMAKE_CXX_FLAGS MATCHES "/EHsc") 
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
	ENDIF ()
ELSE ()
	SET(GCC_STD_CPP_17_COMPILE_FLAGS "-std=c++17")
	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_STD_CPP_17_COMPILE_FLAGS}")
ENDIF () 

set(INCLUDE_DIRS "test/inc" "lib/inc")

include_directories(${INCLUDE_DIRS})

# add the executable
add_executable(ex603_mcp_type_func

	test/inc/test_type_func.h
	test/src/test_type_func.cpp

	lib/inc/type_func.h
	lib/src/type_func.cpp

	main/main.cpp
)

