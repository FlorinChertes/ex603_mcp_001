# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project (ex603_mcp_001_prj  LANGUAGES CXX)

# require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF (MSVC)
	# Activate C++ exception handling 
	IF (NOT CMAKE_CXX_FLAGS MATCHES "/EHsc") 
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
	ENDIF ()
ELSE ()
	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wnull-dereference")

	SET(GCC_STD_CPP_CONCEPTS_COMPILE_FLAGS "-fconcepts")
	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_STD_CPP_CONCEPTS_COMPILE_FLAGS}")

	SET(GCC_STD_CPP_PERMISIV_COMPILE_FLAGS "-fpermissive")
	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_STD_CPP_PERMISIV_COMPILE_FLAGS}")
ENDIF () 

set(INCLUDE_DIRS "test/inc" "lib/inc")

include_directories(${INCLUDE_DIRS})

# add the executable
add_executable(ex603_mcp_type_func

	test/inc/test_type_func.h
	test/src/test_type_func.cpp

	test/inc/test_chrono_func.h
	test/src/test_chrono_func.cpp

	test/inc/test_func_arg.h
	test/src/test_func_arg.cpp

	test/inc/test_emc_sm_item26.h
	test/src/test_emc_sm_item26.cpp

	test/inc/test_unique_ptr.h
	test/src/test_unique_ptr.cpp

	test/inc/test_lambda.h
	test/src/test_lambda.cpp

	test/inc/test_overloading_tf.h
	test/src/test_overloading_tf.cpp

	test/inc/test_shape.h
	test/src/test_shape.cpp

	test/inc/test_utils.h
	test/src/test_utils.cpp

	lib/inc/type_func.h
	lib/src/type_func.cpp

	lib/inc/arg_func.h

	lib/inc/emc_sm_item_26_01.h
	lib/src/emc_sm_item_26_01.cpp

	main/main.cpp
)
