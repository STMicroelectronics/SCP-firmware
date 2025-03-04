#
# Arm SCP/MCP Software
# Copyright (c) 2022-2023, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

set(TEST_SRC mod_scmi_perf)
set(TEST_FILE mod_scmi_perf_ph)

if(TEST_ON_TARGET)
    set(TEST_MODULE scmi_perf)
    set(MODULE_ROOT ${CMAKE_SOURCE_DIR}/module)
else()
    set(UNIT_TEST_TARGET ${TEST_FILE}_unit_test)
endif()

set(MODULE_SRC ${MODULE_ROOT}/${TEST_MODULE}/src)
set(MODULE_INC ${MODULE_ROOT}/${TEST_MODULE}/include)
list(APPEND OTHER_MODULE_INC ${MODULE_ROOT}/scmi_perf/include)
list(APPEND OTHER_MODULE_INC ${MODULE_ROOT}/scmi/include)
list(APPEND OTHER_MODULE_INC ${MODULE_ROOT}/dvfs/include)
list(APPEND OTHER_MODULE_INC ${MODULE_ROOT}/timer/include)
set(MODULE_UT_SRC ${CMAKE_CURRENT_LIST_DIR})
set(MODULE_UT_INC ${CMAKE_CURRENT_LIST_DIR})
set(MODULE_UT_MOCK_SRC ${CMAKE_CURRENT_LIST_DIR}/../mocks)

list(APPEND MOCK_REPLACEMENTS fwk_module)
list(APPEND MOCK_REPLACEMENTS fwk_id)
list(APPEND MOCK_REPLACEMENTS fwk_core)
list(APPEND MOCK_REPLACEMENTS fwk_mm)

include(${SCP_ROOT}/unit_test/module_common.cmake)

target_compile_definitions(${UNIT_TEST_TARGET} PUBLIC
    "BUILD_HAS_SCMI_PERF_FAST_CHANNELS")
target_compile_definitions(${UNIT_TEST_TARGET} PUBLIC
    "BUILD_HAS_SCMI_PERF_PLUGIN_HANDLER")
target_compile_definitions(${UNIT_TEST_TARGET} PUBLIC
    "BUILD_HAS_SCMI_PERF_PROTOCOL_OPS")

target_compile_definitions(${UNIT_TEST_TARGET} PUBLIC
    "SCP_ENABLE_PLUGIN_HANDLER")
