cmake_minimum_required(VERSION 2.8)
project(PandarGeneral)


find_package( Boost REQUIRED COMPONENTS exception )
find_package( Eigen3 REQUIRED )
find_package(PCL REQUIRED COMPONENTS common)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set (CMAKE_CXX_FLAGS "-fPIC --std=c++11")
endif()

add_library( ${PROJECT_NAME} STATIC
	src/input.cc
	src/pandarGeneral_internal.cc
	src/pandarGeneral.cc
    src/pcap_reader.cpp
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	.
	include
	include/pandarGeneral
	${Boost_INCLUDE_DIRS}
	${PCL_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
		PUBLIC
		${Boost_LIBRARIES}
		${PCL_IO_LIBRARIES}
)

if(TARGET Eigen3::Eigen)
	target_include_directories(${PROJECT_NAME} PUBLIC Eigen3::Eigen)
	target_link_libraries(${PROJECT_NAME} PUBLIC Eigen3::Eigen)
else()
	target_include_directories(${PROJECT_NAME} PUBLIC ${EIGEN3_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} PUBLIC ${EIGEN3_LIBRARIES})
	target_link_directories(${PROJECT_NAME} PUBLIC ${EIGEN3_LIBRARY_DIRS})
endif()

target_link_directories(${PROJECT_NAME}
                PUBLIC
                ${Boost_LIBRARY_DIRS}
                ${PCL_IO_LIBRARY_DIRS}
)

install(TARGETS ${PROJECT_NAME})