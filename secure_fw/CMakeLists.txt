#-------------------------------------------------------------------------------
# Copyright (c) 2017-2018, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.7)

#Tell cmake where our modules can be found
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../cmake)

#Include common stuff to control cmake.
include("Common/BuildSys")

#Start an embedded project.
embedded_project_start(CONFIG "${CMAKE_CURRENT_LIST_DIR}/../ConfigDefault.cmake")
project(tfm_s LANGUAGES ASM C)
embedded_project_fixup()

set (SECURE_FW_DIR "${CMAKE_CURRENT_LIST_DIR}")
set (TFM_ROOT_DIR "${SECURE_FW_DIR}/..")
set (TEST_DIR "${TFM_ROOT_DIR}/test")
set (INTERFACE_DIR "${TFM_ROOT_DIR}/interface")

if(CORE_TEST)
	set (TFM_LVL 3)
else()
	set (TFM_LVL 1)
endif()

include(${SECURE_FW_DIR}/spm/CMakeLists.inc)
include(${SECURE_FW_DIR}/core/CMakeLists.inc)
include(${SECURE_FW_DIR}/ns_callable/CMakeLists.inc)

set(BUILD_CMSIS_CORE On)
set(BUILD_RETARGET On)
set(BUILD_NATIVE_DRIVERS On)
set(BUILD_STARTUP On)
set(BUILD_TARGET_CFG On)
set(BUILD_TARGET_HARDWARE_KEYS On)
set(BUILD_CMSIS_DRIVERS On)
set(BUILD_TIME Off)
set(BUILD_UART_STDOUT On)
set(BUILD_FLASH Off)
if(NOT DEFINED PLATFORM_CMAKE_FILE)
	message (FATAL_ERROR "Platform specific CMake is not defined. Please set PLATFORM_CMAKE_FILE.")
elseif(NOT EXISTS ${PLATFORM_CMAKE_FILE})
	message (FATAL_ERROR "Platform specific CMake \"${PLATFORM_CMAKE_FILE}\" file does not exist. Please fix value of PLATFORM_CMAKE_FILE.")
else()
	include(${PLATFORM_CMAKE_FILE})
endif()

if(NOT DEFINED S_SCATTER_FILE_NAME)
	message(FATAL_ERROR "ERROR: Incomplete Configuration: S_SCATTER_FILE_NAME not defined, Include this file from a Config*.cmake")
endif()
embedded_set_target_linker_file(TARGET tfm_s PATH "${S_SCATTER_FILE_NAME}")

embedded_target_include_directories(TARGET ${PROJECT_NAME} PATH ${TFM_ROOT_DIR} ABSOLUTE APPEND)

#Specify what we build
add_executable(${PROJECT_NAME} ${ALL_SRC_C} ${ALL_SRC_C_S} ${ALL_SRC_ASM_S})

#Adds the test directory
add_subdirectory(${TFM_ROOT_DIR}/test ${CMAKE_BINARY_DIR}/test)

#Add the secure storage library target
add_subdirectory(${SECURE_FW_DIR}/services/secure_storage)

add_dependencies(${PROJECT_NAME} tfm_storage)
add_dependencies(${PROJECT_NAME} tfm_secure_tests)

#Set macro definitions for the project.
embedded_set_target_compile_defines(TARGET ${PROJECT_NAME} LANGUAGE C DEFINES __thumb2__ __ARM_FEATURE_CMSE=3 TFM_LVL=${TFM_LVL} DAUTH_CHIP_DEFAULT APPEND)

if (REGRESSION OR CORE_TEST)
	#The test service veneers may not be referenced in the secure binary so the
	#veneer objects are explicitly loaded from the secure tests library.
	target_link_libraries(${PROJECT_NAME} tfm_storage $<TARGET_LINKER_FILE:tfm_secure_tests>\(*veneers.o\) tfm_secure_tests)
else()
	target_link_libraries(${PROJECT_NAME} tfm_storage)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS " --predefine=\"-DTFM_LVL=${TFM_LVL}\"")

if (NOT DEFINED CORE_TEST_SERVICES)
	message(FATAL_ERROR "Incomplete build configuration: CORE_TEST_SERVICES is undefined. ")
elseif (CORE_TEST_SERVICES)
	set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " --predefine=\"-DCORE_TEST_SERVICES\"")
endif()

if (NOT DEFINED SST_TEST_SERVICES)
	message(FATAL_ERROR "Incomplete build configuration: SST_TEST_SERVICES is undefined. ")
elseif (SST_TEST_SERVICES)
	set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " --predefine=\"-DSST_TEST_SERVICES\"")
endif()

if (NOT DEFINED BL2)
	message(FATAL_ERROR "Incomplete build configuration: BL2 is undefined. ")
elseif (BL2)
	set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " --predefine=\"-DBL2\"")
endif()

if(CORE_TEST)
	embedded_set_target_compile_defines(TARGET ${PROJECT_NAME} LANGUAGE C DEFINES TFM_CORE_DEBUG CORE_TEST_SERVICES APPEND)
	set(SECURE_AXF_DIR_PREFIX "${CMAKE_BINARY_DIR}/unit_test/")
	set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SECURE_AXF_DIR_PREFIX})
	set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " --predefine=\"-DCORE_TEST_SERVICES\"")
endif()

#Generate binary file from axf
compiler_generate_binary_output(${PROJECT_NAME})

#Configure where we put the CMSE veneers generated by the compiler.
if (NOT DEFINED S_VENEER_FILE)
	set(S_VENEER_FILE "${CMAKE_CURRENT_BINARY_DIR}/s_veneers.o")
endif()
compiler_set_cmse_output(${PROJECT_NAME} "${S_VENEER_FILE}")

#Configure what file shall be installed.
#Set install location. Keep original value to avoid overriding command line
#settings.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Default install location for secure_fw." FORCE)
endif()

install(DIRECTORY ${TFM_ROOT_DIR}/interface/include/
		DESTINATION tfm/inc)

install(DIRECTORY ${TFM_ROOT_DIR}/interface/src/
		DESTINATION tfm/src)

install(FILES ${S_VENEER_FILE}
		DESTINATION tfm/veneers)

#Finally let cmake system apply changes after the whole project is defined.
embedded_project_end(${PROJECT_NAME})
