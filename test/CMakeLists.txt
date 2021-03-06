#-------------------------------------------------------------------------------
# Copyright (c) 2017, Arm Limited. All rights reserved.
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
project(tfm_tests LANGUAGES ASM C)
embedded_project_fixup()

get_filename_component(TFM_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

#Check incoming configuration options
if (NOT DEFINED SERVICES_TEST_ENABLED)
	message(FATAL_ERROR "Incomplete build configuration: SERVICES_TEST_ENABLED is undefined. ")
endif()

if (NOT DEFINED CORE_TEST)
	message(FATAL_ERROR "Incomplete build configuration: CORE_TEST is undefined. ")
endif()

if(CORE_TEST)
	set (TFM_LVL 3)
else()
	set (TFM_LVL 1)
endif()

#Configure our options as needed.
if (CORE_TEST_INTERACTIVE OR CORE_TEST_POSITIVE)
	set(ENABLE_CORE_TESTS True)
	set(ENABLE_CORE_TESTS_2 True)
else()
	set(ENABLE_CORE_TESTS False)
	set(ENABLE_CORE_TESTS_2 False)
endif()

if (NOT DEFINED CMSIS_5_DIR)
	set(CMSIS_5_DIR ${TFM_ROOT_DIR}/../CMSIS_5)
endif()
set(ENABLE_SECURE_STORAGE_SERVICE_TESTS TRUE)
set(ENABLE_INVERT_SERVICE_TESTS TRUE)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeLists.inc)

#Build the secure library
add_library(tfm_secure_tests STATIC ${ALL_SRC_C} ${ALL_SRC_C_S})
embedded_set_target_compile_defines(TARGET tfm_secure_tests LANGUAGE C DEFINES __thumb2__ __ARM_FEATURE_CMSE=3 TFM_LVL=${TFM_LVL} APPEND)


#Build the non-secure library
set(CMAKE_STATIC_LIBRARY_PREFIX_C "lib")
add_library(tfm_non_secure_tests STATIC ${ALL_SRC_C} ${ALL_SRC_C_NS})
embedded_set_target_compile_defines(TARGET tfm_non_secure_tests LANGUAGE C DEFINES __thumb2__ __ARM_FEATURE_CMSE=3 __DOMAIN_NS=1 APPEND)
#__DOMAIN_NS=1

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Default install location for tfm_storage." FORCE)
endif()

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/framework/integ_test.h
				${CMAKE_CURRENT_SOURCE_DIR}/suites/core/non_secure/svc_core_test_ns.h
				${CMAKE_CURRENT_SOURCE_DIR}/suites/sst/non_secure/os_wrapper.h
		DESTINATION tfm/test/inc)

install(TARGETS tfm_non_secure_tests
		DESTINATION tfm/test/lib
		PUBLIC_HEADER DESTINATION tfm/test/inc)


embedded_project_end(tfm_non_secure_tests)
embedded_project_end(tfm_secure_tests)

