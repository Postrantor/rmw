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

#include "rmw/network_flow_endpoint_array.h"

#include "rmw/error_handling.h"

/**
 * @brief 返回一个零初始化的网络流端点数组 (Return a zero-initialized network flow endpoint array)
 *
 * @return rmw_network_flow_endpoint_array_t 零初始化的网络流端点数组 (Zero-initialized network flow
 * endpoint array)
 */
rmw_network_flow_endpoint_array_t rmw_get_zero_initialized_network_flow_endpoint_array(void) {
  // 初始化一个网络流端点数组并将其所有成员设为0 (Initialize a network flow endpoint array and set
  // all its members to 0)
  rmw_network_flow_endpoint_array_t network_flow_endpoint_array = {0};
  // 返回零初始化的网络流端点数组 (Return the zero-initialized network flow endpoint array)
  return network_flow_endpoint_array;
}

/**
 * @brief 检查网络流端点数组是否为零初始化 (Check if the network flow endpoint array is
 * zero-initialized)
 *
 * @param[in] network_flow_endpoint_array 要检查的网络流端点数组指针 (Pointer to the network flow
 * endpoint array to be checked)
 * @return rmw_ret_t 检查结果，如果为零初始化则返回 RMW_RET_OK (Check result, return RMW_RET_OK if
 * zero-initialized)
 */
rmw_ret_t rmw_network_flow_endpoint_array_check_zero(
    const rmw_network_flow_endpoint_array_t* network_flow_endpoint_array) {
  // 判断输入的网络流端点数组指针是否为空 (Check if the input network flow endpoint array pointer is
  // null)
  if (!network_flow_endpoint_array) {
    RMW_SET_ERROR_MSG("network_flow_endpoint_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 判断网络流端点数组是否为零初始化 (Check if the network flow endpoint array is zero-initialized)
  if (network_flow_endpoint_array->size != 0 ||
      network_flow_endpoint_array->network_flow_endpoint != NULL ||
      network_flow_endpoint_array->allocator != NULL) {
    RMW_SET_ERROR_MSG("network_flow_endpoint_array is not zeroed");
    return RMW_RET_ERROR;
  }
  return RMW_RET_OK;
}

/**
 * @brief 初始化网络流端点数组 (Initialize the network flow endpoint array)
 *
 * @param[out] network_flow_endpoint_array 要初始化的网络流端点数组指针 (Pointer to the network flow
 * endpoint array to be initialized)
 * @param[in] size 网络流端点数组的大小 (Size of the network flow endpoint array)
 * @param[in] allocator 内存分配器 (Memory allocator)
 * @return rmw_ret_t 初始化结果，如果成功则返回 RMW_RET_OK (Initialization result, return RMW_RET_OK
 * if successful)
 */
rmw_ret_t rmw_network_flow_endpoint_array_init(
    rmw_network_flow_endpoint_array_t* network_flow_endpoint_array,
    size_t size,
    rcutils_allocator_t* allocator) {
  // 检查输入的内存分配器是否为空 (Check if the input memory allocator is null)
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 检查输入的网络流端点数组指针是否为空 (Check if the input network flow endpoint array pointer is
  // null)
  if (!network_flow_endpoint_array) {
    RMW_SET_ERROR_MSG("network_flow_endpoint_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 分配内存空间给网络流端点数组 (Allocate memory space for the network flow endpoint array)
  network_flow_endpoint_array->network_flow_endpoint =
      allocator->allocate(sizeof(rmw_network_flow_endpoint_t) * size, allocator->state);
  // 检查分配的内存空间是否为空 (Check if the allocated memory space is null)
  if (!network_flow_endpoint_array->network_flow_endpoint) {
    RMW_SET_ERROR_MSG("failed to allocate memory for network_flow_endpoint_array");
    return RMW_RET_BAD_ALLOC;
  }
  // 设置网络流端点数组的大小 (Set the size of the network flow endpoint array)
  network_flow_endpoint_array->size = size;
  // 初始化网络流端点数组中的每个元素 (Initialize each element in the network flow endpoint array)
  for (size_t i = 0; i < size; i++) {
    network_flow_endpoint_array->network_flow_endpoint[i] =
        rmw_get_zero_initialized_network_flow_endpoint();
  }
  // 设置网络流端点数组的内存分配器 (Set the memory allocator for the network flow endpoint array)
  network_flow_endpoint_array->allocator = allocator;
  return RMW_RET_OK;
}

/**
 * @brief 清理网络流端点数组 (Clean up the network flow endpoint array)
 *
 * @param[out] network_flow_endpoint_array 要清理的网络流端点数组指针 (Pointer to the network flow
 * endpoint array to be cleaned up)
 * @return rmw_ret_t 清理结果，如果成功则返回 RMW_RET_OK (Cleanup result, return RMW_RET_OK if
 * successful)
 */
rmw_ret_t rmw_network_flow_endpoint_array_fini(
    rmw_network_flow_endpoint_array_t* network_flow_endpoint_array) {
  // 检查输入的网络流端点数组指针是否为空 (Check if the input network flow endpoint array pointer is
  // null)
  if (!network_flow_endpoint_array) {
    RMW_SET_ERROR_MSG("network_flow_endpoint_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 检查网络流端点数组的内存分配器是否为空 (Check if the memory allocator of the network flow
  // endpoint array is null)
  if (network_flow_endpoint_array->allocator == NULL) {
    RMW_SET_ERROR_MSG("network_flow_endpoint_array->allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 释放网络流端点数组的内存空间 (Release the memory space of the network flow endpoint array)
  network_flow_endpoint_array->allocator->deallocate(
      network_flow_endpoint_array->network_flow_endpoint,
      network_flow_endpoint_array->allocator->state);
  // 将网络流端点数组的成员设为零初始化值 (Set the members of the network flow endpoint array to
  // zero-initialized values)
  network_flow_endpoint_array->network_flow_endpoint = NULL;
  network_flow_endpoint_array->size = 0;
  network_flow_endpoint_array->allocator = NULL;
  return RMW_RET_OK;
}
