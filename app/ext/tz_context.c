/*
 * Copyright (c) 2015-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ----------------------------------------------------------------------------
 *
 * $Date:        15. October 2016
 * $Revision:    1.1.0
 *
 * Project:      TrustZone for ARMv8-M
 * Title:        Context Management for ARMv8-M TrustZone - Stub implementation,
 *               TFM will take charge of the stack memory management in S mode
 *
 *---------------------------------------------------------------------------*/

#include "tz_context.h"


/// Initialize secure context memory system
/// \return execution status (1: success, 0: error)
uint32_t TZ_InitContextSystem_S (void) {
  return 1U;    // Success
}


/// Allocate context memory for calling secure software modules in TrustZone
/// \param[in]  module   identifies software modules called from non-secure mode
/// \return value != 0 id TrustZone memory slot identifier
/// \return value 0    no memory available or internal error
TZ_MemoryId_t TZ_AllocModuleContext_S (TZ_ModuleId_t module) {
  uint32_t slot = 0;
  return (slot + 1U);
}


/// Free context memory that was previously allocated with \ref TZ_AllocModuleContext_S
/// \param[in]  id  TrustZone memory slot identifier
/// \return execution status (1: success, 0: error)
uint32_t TZ_FreeModuleContext_S (TZ_MemoryId_t id) {
  return 1U;    // Success
}


/// Load secure context (called on RTOS thread context switch)
/// \param[in]  id  TrustZone memory slot identifier
/// \return execution status (1: success, 0: error)
uint32_t TZ_LoadContext_S (TZ_MemoryId_t id) {
  return 1U;    // Success
}


/// Store secure context (called on RTOS thread context switch)
/// \param[in]  id  TrustZone memory slot identifier
/// \return execution status (1: success, 0: error)
uint32_t TZ_StoreContext_S (TZ_MemoryId_t id) {
  return 1U;    // Success
}
