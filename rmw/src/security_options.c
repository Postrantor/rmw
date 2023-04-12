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

#include "rmw/security_options.h"

#include <stddef.h>

#include "rcutils/strdup.h"
#include "rmw/error_handling.h"

/**
 * @brief 获取一个零初始化的安全选项结构体 (Get a zero-initialized security options structure)
 *
 * @return 返回一个零初始化的安全选项结构体 (Return a zero-initialized security options structure)
 */
rmw_security_options_t rmw_get_zero_initialized_security_options() {
  // 初始化一个零值的安全选项结构体变量 (Initialize a zero-valued security options structure
  // variable)
  rmw_security_options_t zero_initialized_options = {0, NULL};
  // 返回这个零值的安全选项结构体变量 (Return this zero-valued security options structure variable)
  return zero_initialized_options;
}

/**
 * @brief 获取默认的安全选项结构体 (Get the default security options structure)
 *
 * @return 返回默认的安全选项结构体 (Return the default security options structure)
 */
rmw_security_options_t rmw_get_default_security_options() {
  // 初始化一个默认值的安全选项结构体变量 (Initialize a default-valued security options structure
  // variable)
  rmw_security_options_t default_options = {
      RMW_SECURITY_ENFORCEMENT_PERMISSIVE,
      NULL,
  };
  // 返回这个默认值的安全选项结构体变量 (Return this default-valued security options structure
  // variable)
  return default_options;
}

/**
 * @brief 复制安全选项结构体 (Copy the security options structure)
 *
 * @param[in] src 源安全选项结构体指针 (Pointer to the source security options structure)
 * @param[in] allocator 分配器指针 (Pointer to the allocator)
 * @param[out] dst 目标安全选项结构体指针 (Pointer to the destination security options structure)
 * @return 返回操作结果状态 (Return the operation result status)
 */
rmw_ret_t rmw_security_options_copy(const rmw_security_options_t* src,
                                    const rcutils_allocator_t* allocator,
                                    rmw_security_options_t* dst) {
  // 检查输入参数是否为空 (Check if input arguments are NULL)
  RMW_CHECK_ARGUMENT_FOR_NULL(src, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(dst, RMW_RET_INVALID_ARGUMENT);

  // 复制安全根路径字符串 (Copy the security root path string)
  char* new_root_path = rcutils_strdup(src->security_root_path, *allocator);
  // 如果源安全根路径不为空，但新的根路径为空，则分配失败 (If the source security root path is not
  // NULL but the new root path is NULL, the allocation fails)
  if (src->security_root_path && !new_root_path) {
    RMW_SET_ERROR_MSG("failed to copy security root path");
    return RMW_RET_BAD_ALLOC;
  }
  // 释放目标安全选项结构体中原有的安全根路径内存 (Release the memory of the original security root
  // path in the destination security options structure)
  allocator->deallocate(dst->security_root_path, allocator->state);
  // 将新的安全根路径赋值给目标安全选项结构体 (Assign the new security root path to the destination
  // security options structure)
  dst->security_root_path = new_root_path;
  // 复制安全策略执行标志 (Copy the security enforcement flag)
  dst->enforce_security = src->enforce_security;
  // 返回操作成功状态 (Return the operation success status)
  return RMW_RET_OK;
}

/**
 * @brief 设置安全选项结构体的根路径 (Set the root path of the security options structure)
 *
 * @param[in] security_root_path 安全根路径字符串 (Security root path string)
 * @param[in] allocator 分配器指针 (Pointer to the allocator)
 * @param[out] security_options 安全选项结构体指针 (Pointer to the security options structure)
 * @return 返回操作结果状态 (Return the operation result status)
 */
rmw_ret_t rmw_security_options_set_root_path(const char* security_root_path,
                                             const rcutils_allocator_t* allocator,
                                             rmw_security_options_t* security_options) {
  // 检查输入参数是否为空 (Check if input arguments are NULL)
  RMW_CHECK_ARGUMENT_FOR_NULL(security_root_path, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(security_options, RMW_RET_INVALID_ARGUMENT);

  // 复制新的安全根路径字符串 (Copy the new security root path string)
  char* new_root_path = rcutils_strdup(security_root_path, *allocator);
  // 如果新的根路径为空，则分配失败 (If the new root path is NULL, the allocation fails)
  if (!new_root_path) {
    RMW_SET_ERROR_MSG("failed to copy security root path");
    return RMW_RET_BAD_ALLOC;
  }
  // 释放安全选项结构体中原有的安全根路径内存 (Release the memory of the original security root path
  // in the security options structure)
  allocator->deallocate(security_options->security_root_path, allocator->state);
  // 将新的安全根路径赋值给安全选项结构体 (Assign the new security root path to the security options
  // structure)
  security_options->security_root_path = new_root_path;
  // 返回操作成功状态 (Return the operation success status)
  return RMW_RET_OK;
}

/**
 * @brief 终止安全选项结构体 (Finalize the security options structure)
 *
 * @param[out] security_options 安全选项结构体指针 (Pointer to the security options structure)
 * @param[in] allocator 分配器指针 (Pointer to the allocator)
 * @return 返回操作结果状态 (Return the operation result status)
 */
rmw_ret_t rmw_security_options_fini(rmw_security_options_t* security_options,
                                    const rcutils_allocator_t* allocator) {
  // 检查输入参数是否为空 (Check if input arguments are NULL)
  RMW_CHECK_ARGUMENT_FOR_NULL(security_options, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);
  // 释放安全选项结构体中的安全根路径内存 (Release the memory of the security root path in the
  // security options structure)
  allocator->deallocate(security_options->security_root_path, allocator->state);
  // 将零初始化的安全选项结构体赋值给当前安全选项结构体 (Assign the zero-initialized security
  // options structure to the current security options structure)
  *security_options = rmw_get_zero_initialized_security_options();
  // 返回操作成功状态 (Return the operation success status)
  return RMW_RET_OK;
}
