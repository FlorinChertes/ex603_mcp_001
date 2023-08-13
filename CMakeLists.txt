# set minimum cmake version
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

# project name and language
project (ex603_mcp_001_prj  LANGUAGES CXX)


IF (MSVC)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
ELSE()
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
ENDIF()

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
        GIT_SHALLOW    1
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)


IF (MSVC)
	# Avoid error LNK2038
	set(gtest_force_shared_crt on)

	# Activate C++ exception handling 
	IF (NOT CMAKE_CXX_FLAGS MATCHES "/EHsc") 
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
	ENDIF ()
ELSE ()
	if(CMAKE_BUILD_TYPE STREQUAL "")
		# Build type is not set e.g., command was "cmake .."
		message(STATUS " Build type unspecified, set to Release")
		set(CMAKE_BUILD_TYPE Release)
	else()
		message(STATUS " Build type specified as '${CMAKE_BUILD_TYPE}'")
	endif()

	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -Wnull-dereference")

	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		SET(GCC_STD_CPP_CONCEPTS_COMPILE_FLAGS "-fconcepts")
		SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_STD_CPP_CONCEPTS_COMPILE_FLAGS}")
	endif()

	SET(GCC_STD_CPP_PERMISIV_COMPILE_FLAGS "-fpermissive")
	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_STD_CPP_PERMISIV_COMPILE_FLAGS}")
ENDIF () 

set(INCLUDE_DIRS "test/inc" "lib/inc")

include_directories(${INCLUDE_DIRS})

#------------------------------------------------------------------------------
# add the executable 001
#------------------------------------------------------------------------------
set(SRC_001

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

	test/src/test_csv_reader.cpp
	test/inc/test_csv_reader.h
	test/inc/scope_timer.h

	test/src/test_polymorphism.cpp
	test/inc/test_polymorphism.h

	test/inc/test_strong_type.h
	test/src/test_strong_type.cpp

	test/inc/test_book_camp.h
	test/src/test_book_camp.cpp

	lib/inc/playing_cards.h
	lib/src/playing_cards.cpp

	test/inc/test_concurrency.h
	test/src/test_concurrency.cpp

	test/inc/test_refref_01.h
	test/src/test_refref_01.cpp

	test/src/test_pmr.cpp
	test/inc/test_pmr.h

)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	add_executable(ex603_mcp_001_ex
		main/main_01_clang.cpp	${SRC_001}

		#est/inc/coro/tracingawaiter.hpp
		#test/inc/coro/tracingcoro.hpp

		#test/src/test_coro.cpp
		#test/inc/test_coro.h
	)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# using GCC
	add_executable(ex603_mcp_001_ex
		main/main_01_gcc.cpp	${SRC_001}

		lib/inc/ranges_func.h
		test/inc/test_ranges.h
		test/src/test_ranges.cpp

		test/src/test_polymorphism_variant.cpp
		test/inc/test_polymorphism_variant.h

		#test/src/test_visitor.cpp
		#test/inc/test_visitor.h

		test/inc/coro/tracingawaiter.hpp
		test/inc/coro/tracingcoro.hpp

		test/inc/coro/coro_iter.hpp

		test/inc/coro/coropriosched.hpp
		test/inc/coro/coroprio.hpp

		test/inc/coro/coropool.hpp
		test/inc/coro/coroasync.hpp

		test/src/test_coro.cpp
		test/inc/test_coro.h
	)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# using Visual Studio C++
	add_executable(ex603_mcp_001_ex
		main/main_01_msvc.cpp	${SRC_001}

		lib/inc/ranges_func.h
		test/inc/test_ranges.h
		test/src/test_ranges.cpp

		test/src/test_polymorphism_variant.cpp
		test/inc/test_polymorphism_variant.h

		test/src/test_visitor.cpp
		test/inc/test_visitor.h

		test/inc/coro/tracingawaiter.hpp
		test/inc/coro/tracingcoro.hpp

		test/inc/coro/corogenback.hpp
		test/inc/coro/backawaiter.hpp

		test/inc/coro/coro_iter.hpp

		test/inc/coro/corochat.hpp

		test/inc/coro/coro_return_type.hpp

		test/inc/coro/coropriosched.hpp
		test/inc/coro/coroprio.hpp

		test/inc/coro/coropool.hpp
		test/inc/coro/coroasync.hpp

		test/src/test_coro.cpp
		test/inc/test_coro.h
	)
endif()

#------------------------------------------------------------------------------
# add the executable 002
#------------------------------------------------------------------------------
add_executable(ex603_mcp_002_ex

	test/inc/scope_timer.h

	test/inc/test_read_file.h
	test/src/test_read_file.cpp

	main/main_02.cpp
)

#------------------------------------------------------------------------------
# add the executable 003
#------------------------------------------------------------------------------
set(TEST_NAME ex603_mcp_003_gtest
	)

IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(TEST_SOURCES
		main/main_03_gtest.cpp
		test/src/gtest_ranges_01.cpp
		test/src/gtest_ranges_02.cpp
	)
ELSE()
	set(TEST_SOURCES
		main/main_03_gtest.cpp
		test/src/gtest_ranges_01.cpp
	)
ENDIF()

set(SOURCES
	lib/inc/ranges_func.h
	)

enable_testing()

# add the executable
add_executable(	${TEST_NAME})
target_sources(${TEST_NAME} PRIVATE ${TEST_SOURCES} ${SOURCES})

target_link_libraries(${TEST_NAME} gtest gmock)
gtest_discover_tests(${TEST_NAME})

#------------------------------------------------------------------------------
# add the executable 004
#------------------------------------------------------------------------------
add_executable(ex603_mcp_004_ex

	lib/inc/person.h
	lib/src/person.cpp

	test/inc/test_person.h
	test/src/test_person.cpp

	lib/inc/geometry.h
	lib/inc/ext_polymorph.h
	lib/inc/ext_polymorph_strategy.h
	lib/src/ext_polymorph_strategy.cpp

	test/inc/test_ext_polymorph.h
	test/src/test_ext_polymorph.cpp

	lib/inc/type_erasure.h

	test/inc/test_type_erasure.h
	test/src/test_type_erasure.cpp

	main/main_04.cpp
)