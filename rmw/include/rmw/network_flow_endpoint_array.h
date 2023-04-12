// Copyright 2020 Ericsson AB
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

#ifndef RMW__NETWORK_FLOW_ENDPOINT_ARRAY_H_
#define RMW__NETWORK_FLOW_ENDPOINT_ARRAY_H_

#if __cplusplus
extern "C" {
#endif

#include "rcutils/allocator.h"
#include "rmw/network_flow_endpoint.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// 用于存储 network_flow_endpoint_t 数组的结构体 (Structure to hold an arrary of
/// network_flow_endpoint_t)
typedef struct RMW_PUBLIC_TYPE rmw_network_flow_endpoint_array_s {
  /// 数组大小 (Size of the array)
  size_t size;
  /// rmw_network_flow_endpoint_t 类型的数组 (Array of rmw_network_flow_endpoint_t)
  rmw_network_flow_endpoint_t* network_flow_endpoint;
  /// 内存分配器 (Allocator)
  rcutils_allocator_t* allocator;
} rmw_network_flow_endpoint_array_t;

/// 返回一个成员值为零初始化的 rmw_network_flow_endpoint_array_t 实例 (Return a
/// rmw_network_flow_endpoint_array_t instance with zero-initialized members)
RMW_PUBLIC
rmw_network_flow_endpoint_array_t rmw_get_zero_initialized_network_flow_endpoint_array(void);

/// 检查 network_flow_endpoint_array 实例是否为零初始化 (Check if network_flow_endpoint_array
/// instance is zero-initialized)
/**
 * \param[in] network_flow_endpoint_array 待检查的数组 (arrary to be checked)
 * \returns 若数组为零初始化，则返回 `RMW_RET_OK`，否则 (If array is zero-initialized, returns
 * `RMW_RET_OK`, or) \returns 若 `network_flow_endpoint_array` 为 NULL，则返回
 * `RMW_RET_INVALID_ARGUMENT`，否则 (If `network_flow_endpoint_array` is NULL, returns
 * `RMW_RET_INVALID_ARGUMENT`, or) \returns 若 `network_flow_endpoint_array` 未被零初始化，则返回
 * `RMW_RET_ERROR`。 (If `network_flow_endpoint_array` is not zero-initialized, returns
 * `RMW_RET_ERROR`.) \remark 失败时设置 RMW 错误状态 (RMW error state is set on failure)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_network_flow_endpoint_array_check_zero(
    const rmw_network_flow_endpoint_array_t* network_flow_endpoint_array);

/// 为 rmw_network_flow_endpoint_array_t 实例分配内存 (Allocate a rmw_network_flow_endpoint_array_t
/// instance)
/**
 * \param[inout] network_flow_endpoint_array 要分配的数组 (array to be allocated)
 * \param[in] size 要分配的数组大小 (size of the array to be allocated)
 * \param[in] allocator 用于分配内存的分配器 (the allocator for allocating memory)
 * \returns `RMW_RET_OK` 初始化成功时 (on successful initialization), or
 * \returns `RMW_RET_INVALID_ARGUMENT` 如果 `network_flow_endpoint_array` 或 `allocator` 为空 (if
 * `network_flow_endpoint_array` or `allocator` is NULL), or \returns `RMW_RET_BAD_ALLOC`
 * 如果内存分配失败 (if memory allocation fails), or \returns `RMW_RET_ERROR` 当发生未指定错误时
 * (when an unspecified error occurs). \remark 失败时设置 RMW 错误状态 (RMW error state is set on
 * failure)
 */
RMW_PUBLIC
rmw_ret_t rmw_network_flow_endpoint_array_init(
    rmw_network_flow_endpoint_array_t*
        network_flow_endpoint_array,  // 要初始化的网络流端点数组指针 (pointer to the network flow
                                      // endpoint array to be initialized)
    size_t size,  // 数组的大小 (size of the array)
    rcutils_allocator_t*
        allocator);  // 分配器指针，用于分配内存 (pointer to the allocator for allocating memory)

/// 释放一个 rmw_network_flow_endpoint_array_t 实例 (Deallocate a rmw_network_flow_endpoint_array_t
/// instance)
/**
 * \param[inout] network_flow_endpoint_array 要释放的数组 (array to be deallocated)
 * \returns `RMW_RET_OK` 成功释放时 (on successfully deallocation), or
 * \returns `RMW_RET_INVALID_ARGUMENT` 如果 `network_flow_endpoint_array` 或其分配器为空 (if
 * `network_flow_endpoint_array` or its allocator is NULL), or \returns `RMW_RET_ERROR`
 * 当发生未指定错误时 (when an unspecified error occurs). \remark 失败时设置 RMW 错误状态 (RMW error
 * state is set on failure)
 */
RMW_PUBLIC
rmw_ret_t rmw_network_flow_endpoint_array_fini(
    rmw_network_flow_endpoint_array_t*
        network_flow_endpoint_array);  // 要释放的网络流端点数组指针 (pointer to the network flow
                                       // endpoint array to be deallocated)

#if __cplusplus
}
#endif

#endif  // RMW__NETWORK_FLOW_ENDPOINT_ARRAY_H_
