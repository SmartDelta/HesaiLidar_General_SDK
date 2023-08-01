cmake_minimum_required(VERSION 2.8)
project(PandarGeneralSDK)

find_package( Boost REQUIRED  COMPONENTS thread)
find_package( PCL REQUIRED COMPONENTS common )
find_package( PkgConfig REQUIRED )
find_package( yaml-cpp REQUIRED )

pkg_check_modules( PCAP REQUIRED libpcap )

# set (CMAKE_CXX_FLAGS "-fPIC --std=c++11")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fPIC --std=c++14")

add_subdirectory(src/PandarGeneralRaw)

add_library( ${PROJECT_NAME} SHARED
    src/pandarGeneral_sdk.cc
    src/tcp_command_client.c
    src/util.c
    src/wrapper.cc
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
    PandarGeneral
    ${Boost_LIBRARIES}
    ${PCL_IO_LIBRARIES}
    Boost::thread
    ${PCAP_LIBRARIES}
    yaml-cpp
)

target_link_directories(${PROJECT_NAME}
    PUBLIC
    ${Boost_LIBRARY_DIRS}
    ${PCL_IO_LIBRARY_DIRS}
    ${PCAP_LIBRARY_DIRS}
    yaml-cpp
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	.
    include
    src/PandarGeneralRaw/include
    ${Boost_INCLUDE_DIRS}
    ${PCL_INCLUDE_DIRS}
    ${PCAP_INCLUDE_DIRS}
)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    find_package(PCL REQUIRED)
    add_executable(PandarGeneralTest
        test/test.cc
    )

    target_link_libraries(PandarGeneralTest
        ${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${PCL_IO_LIBRARIES}
        Boost::thread
    )
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

