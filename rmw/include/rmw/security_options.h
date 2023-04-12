// Copyright 2020 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RMW__SECURITY_OPTIONS_H_
#define RMW__SECURITY_OPTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "rcutils/allocator.h"
#include "rmw/ret_types.h"
#include "rmw/visibility_control.h"

/**
 * @brief 安全性执行策略枚举 (Security enforcement policy enumeration)
 */
typedef enum RMW_PUBLIC_TYPE rmw_security_enforcement_policy_e {
  RMW_SECURITY_ENFORCEMENT_PERMISSIVE,  ///< 宽松的安全策略 (Permissive security policy)
  RMW_SECURITY_ENFORCEMENT_ENFORCE,     ///< 强制执行的安全策略 (Enforced security policy)
} rmw_security_enforcement_policy_t;

/**
 * @brief 安全选项结构体 (Security options structure)
 */
typedef struct RMW_PUBLIC_TYPE rmw_security_options_s {
  rmw_security_enforcement_policy_t enforce_security;  ///< 执行安全策略 (Enforce security policy)
  char* security_root_path;                            ///< 安全根路径 (Security root path)
} rmw_security_options_t;

/// 获取零初始化的安全选项 (Get zero initialized security options)
/**
 * @returns 零初始化的安全选项结构体实例 (A zero initialized security options structure instance)
 */
RMW_PUBLIC
rmw_security_options_t rmw_get_zero_initialized_security_options();

/// 获取默认初始化的安全选项 (Get default initialized security options)
/**
 * @returns 默认初始化的安全选项结构体实例 (A default initialized security options structure
 * instance)
 */
RMW_PUBLIC
rmw_security_options_t rmw_get_default_security_options();

/// 复制给定的安全选项 (Copy the given security options)
/**
 * @param[in] src 要复制的安全选项 (Security options to be copied)
 * @param[in] allocator 在复制数据到新安全选项时使用的分配器 (Allocator used when copying data to
 * the new security options)
 * @param[out] dst 要设置的安全选项 (Security options to be set)
 * @returns RMW_RET_BAD_ALLOC, 或 (or)
 * @returns RMW_RET_OK
 */
RMW_PUBLIC
rmw_ret_t rmw_security_options_copy(const rmw_security_options_t* src,
                                    const rcutils_allocator_t* allocator,
                                    rmw_security_options_t* dst);

/// 设置给定安全选项的安全根路径。 (Set the security root path for the given security options.)
/**
 * 提供的 `security_root_path` 将被复制到分配的内存中。
 * (The provided `security_root_path` will be copied into allocated memory.)
 *
 * \param[in] security_root_path 要设置的路径。 (path to be set.)
 * \param[in] allocator 用于分配新路径的分配器。 (allocator used to allocate the new path.)
 * \param[inout] security_options 要设置的安全选项。 (security options to be set.)
 * \returns RMW_RET_BAD_ALLOC, 或者 (or)
 * \returns RMW_RET_OK
 */
RMW_PUBLIC
rmw_ret_t rmw_security_options_set_root_path(const char* security_root_path,
                                             const rcutils_allocator_t* allocator,
                                             rmw_security_options_t* security_options);

/// 终止给定的 security_options。 (Finalize the given security_options.)
/**
 * \param[in] security_options 要终止的安全选项。 (security options to be finalized.)
 * \param[in] allocator 用于释放根路径的分配器。 (allocator used to deallocate the root path.)
 * \returns RMW_RET_ERROR, 或者 (or)
 * \returns RMW_RET_OK
 */
RMW_PUBLIC
rmw_ret_t rmw_security_options_fini(rmw_security_options_t* security_options,
                                    const rcutils_allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif  // RMW__SECURITY_OPTIONS_H_
