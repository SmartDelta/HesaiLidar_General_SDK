cmake_minimum_required(VERSION 2.8)
project(PandarGeneral)


find_package( Boost REQUIRED COMPONENTS )
find_package( Eigen3 REQUIRED )
find_package(PCL REQUIRED COMPONENTS common)

include_directories(${PCL_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

add_library( ${PROJECT_NAME} STATIC
	src/input.cc
	src/pandarGeneral_internal.cc
	src/pandarGeneral.cc
    src/pcap_reader.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(${PROJECT_NAME} PUBLIC ${PCL_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	include
	include/pandarGeneral
)

target_link_libraries(${PROJECT_NAME}
		PUBLIC
		${Boost_LIBRARIES}
		${PCL_IO_LIBRARIES}
)

if(TARGET Eigen3::Eigen)
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