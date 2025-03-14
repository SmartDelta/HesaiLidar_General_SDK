cmake_minimum_required(VERSION 2.8)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(PandarGeneralSDK)

find_package( Boost REQUIRED  COMPONENTS thread)
find_package( PCL REQUIRED COMPONENTS common )
find_package( PkgConfig REQUIRED )
find_package( yaml-cpp REQUIRED )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package( Threads REQUIRED )

if(WIN32)
    find_package(PThreads4W REQUIRED)
endif()

pkg_check_modules( PCAP REQUIRED libpcap )

add_subdirectory(src/PandarGeneralRaw)

add_library( ${PROJECT_NAME} SHARED
    src/pandarGeneral_sdk.cc
    src/tcp_command_client.c
    "src/utils.cpp"
    src/wrapper.cc
)

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
    PandarGeneral
    ${Boost_LIBRARIES}
    ${PCL_IO_LIBRARIES}
    Boost::thread
    ${PCAP_LIBRARIES}
    yaml-cpp
    Threads::Threads
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC PThreads4W::PThreads4W)
endif()

target_link_directories(${PROJECT_NAME}
    PUBLIC
    ${Boost_LIBRARY_DIRS}
    ${PCL_IO_LIBRARY_DIRS}
    ${PCAP_LIBRARY_DIRS}
    yaml-cpp
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
    include
    src/PandarGeneralRaw/include
    ${Boost_INCLUDE_DIRS}
    ${PCL_INCLUDE_DIRS}
    ${PCAP_INCLUDE_DIRS}
)

install(TARGETS ${PROJECT_NAME})