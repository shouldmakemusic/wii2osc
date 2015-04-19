CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(Wii2Osc)

# Preliminary stuff
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	SET(LINUX 1)
	ADD_DEFINITIONS(-DLINUX)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")
SET(CMAKE_CXX_FLAGS "-g -Wall")

# Third-party libraries
FIND_PACKAGE(Bluetooth REQUIRED)

FIND_PACKAGE(OpenCV QUIET)
IF(OpenCV_FOUND)
	MESSAGE(STATUS "Found OpenCV:")
	MESSAGE(STATUS "   - Include Dir: ${OpenCV_INCLUDE_DIRS}")
	MESSAGE(STATUS "   - Libs: ${OpenCV_LIBS}")
	MESSAGE(STATUS "   - Version: ${OpenCV_VERSION}")
	IF("${OpenCV_VERSION}" STRLESS "2.2.0")
		ADD_DEFINITIONS(-DCV_OLD)
	ENDIF("${OpenCV_VERSION}" STRLESS "2.2.0")
ELSE(OpenCV_FOUND)
	MESSAGE(STATUS "OpenCV not found but not required. WiiC-ML will not be compiled.")
ENDIF(OpenCV_FOUND)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# separate versions of NetworkingUtils.cpp and UdpSocket.cpp are provided for Win32 and POSIX
# the IpSystemTypePath selects the correct ones based on the current platform

IF(WIN32)
 set(IpSystemTypePath ip/win32)
 set(LIBS ${LIBS} Ws2_32 winmm)
ELSE(WIN32)
 set(IpSystemTypePath ip/posix)
ENDIF(WIN32)

ADD_LIBRARY(oscpack 

ip/IpEndpointName.h
ip/IpEndpointName.cpp

ip/NetworkingUtils.h
${IpSystemTypePath}/NetworkingUtils.cpp

ip/UdpSocket.h
${IpSystemTypePath}/UdpSocket.cpp

ip/PacketListener.h
ip/TimerListener.h

osc/OscTypes.h
osc/OscTypes.cpp 
osc/OscHostEndianness.h
osc/OscException.h
osc/OscPacketListener.h
osc/MessageMappingOscPacketListener.h
osc/OscReceivedElements.h
osc/OscReceivedElements.cpp
osc/OscPrintReceivedElements.h
osc/OscPrintReceivedElements.cpp
osc/OscOutboundPacketStream.h
osc/OscOutboundPacketStream.cpp

)

# Source code
ADD_SUBDIRECTORY(wiic)
ADD_SUBDIRECTORY(wiicpp)
ADD_SUBDIRECTORY(ml)
ADD_SUBDIRECTORY(util)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/wiic/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/wiicpp/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/log/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ml/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/util/)

ADD_LIBRARY(util
util/RangeUtil.h
util/RangeUtil.cpp
util/OSCSender.h
util/OSCSender.cpp
util/WiiReceiver.h
util/WiiReceiver.cpp
) 

ADD_EXECUTABLE(wii2osc wii2osc.cpp)
TARGET_LINK_LIBRARIES(wii2osc oscpack ${LIBS})
TARGET_LINK_LIBRARIES(wii2osc wiicpp)
TARGET_LINK_LIBRARIES(wii2osc wiic)
TARGET_LINK_LIBRARIES(wii2osc util)
INSTALL(TARGETS wii2osc DESTINATION /usr/local/bin)

# Config dir
MAKE_DIRECTORY($ENV{HOME}/.wiic)

# Documentation
#SET(DOXYFILE_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/../doc")
#include(UseDoxygen)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
endif()

enable_testing()
