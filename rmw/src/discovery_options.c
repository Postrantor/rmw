// Copyright 2022 Open Source Robotics Foundation, Inc.
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

#include "rmw/discovery_options.h"

#include <stddef.h>
#include <string.h>

#include "rmw/error_handling.h"

/**
 * @brief 获取一个零初始化的发现选项结构体 (Get a zero-initialized discovery options structure)
 *
 * @return 返回一个零初始化的发现选项结构体 (Return a zero-initialized discovery options structure)
 */
rmw_discovery_options_t rmw_get_zero_initialized_discovery_options(void) {
  // 初始化 result 结构体，并设置默认值 (Initialize the result structure and set default values)
  rmw_discovery_options_t result = (rmw_discovery_options_t){
      .automatic_discovery_range = RMW_AUTOMATIC_DISCOVERY_RANGE_NOT_SET,
      .static_peers_count = 0,
  };  // NOLINT(readability/braces): false positive
  // 返回零初始化的发现选项结构体 (Return the zero-initialized discovery options structure)
  return result;
}

/**
 * @brief 初始化发现选项 (Initialize discovery options)
 *
 * @param[out] discovery_options 发现选项指针 (Pointer to discovery options)
 * @param[in] size 静态对等点数量 (Number of static peers)
 * @param[in] allocator 分配器指针 (Pointer to allocator)
 *
 * @return 成功返回 RMW_RET_OK，否则返回相应的错误代码 (Return RMW_RET_OK on success, otherwise
 * return corresponding error code)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_discovery_options_init(rmw_discovery_options_t* discovery_options,
                                     size_t size,
                                     rcutils_allocator_t* allocator) {
  // 检查可能的无效参数和内存分配错误 (Check for possible invalid arguments and memory allocation
  // errors)
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_BAD_ALLOC);

  // 检查 discovery_options 是否为空 (Check if discovery_options is NULL)
  RMW_CHECK_ARGUMENT_FOR_NULL(discovery_options, RMW_RET_INVALID_ARGUMENT);
  // 检查分配器是否有效 (Check if the allocator is valid)
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);

  // 检查发现选项是否已经初始化 (Check if discovery options have already been initialized)
  if (0 != discovery_options->static_peers_count || NULL != discovery_options->static_peers) {
    RMW_SET_ERROR_MSG("discovery_options must be zero intialized");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 设置自动发现范围为默认值 (localhost) (Set the automatic discovery range to the default value
  // (localhost))
  if (RMW_AUTOMATIC_DISCOVERY_RANGE_NOT_SET == discovery_options->automatic_discovery_range) {
    discovery_options->automatic_discovery_range = RMW_AUTOMATIC_DISCOVERY_RANGE_LOCALHOST;
  }

  // 如果 size 为 0，则无需执行任何操作 (If size is 0, no action is required)
  if (0u == size) {
    return RMW_RET_OK;
  }

  // 为静态对等点分配内存 (Allocate memory for static peers)
  discovery_options->static_peers =
      allocator->zero_allocate(size, sizeof(rmw_peer_address_t), allocator->state);

  // 检查静态对等点内存分配是否成功 (Check if memory allocation for static peers was successful)
  if (NULL == discovery_options->static_peers) {
    RMW_SET_ERROR_MSG("failed to allocate memory for static_peers");
    return RMW_RET_BAD_ALLOC;
  }

  // 设置静态对等点数量和分配器 (Set the static peers count and allocator)
  discovery_options->static_peers_count = size;
  discovery_options->allocator = *allocator;

  // 返回成功代码 (Return success code)
  return RMW_RET_OK;
}

/**
 * @brief 比较两个rmw_discovery_options_t结构体是否相等 (Compare two rmw_discovery_options_t
 * structures for equality)
 *
 * @param[in] left 第一个rmw_discovery_options_t结构体指针 (Pointer to the first
 * rmw_discovery_options_t structure)
 * @param[in] right 第二个rmw_discovery_options_t结构体指针 (Pointer to the second
 * rmw_discovery_options_t structure)
 * @param[out] result 指向布尔值的指针，用于存储比较结果 (Pointer to a boolean value to store the
 * comparison result)
 * @return RMW_RET_OK 如果成功，其他错误代码表示失败 (RMW_RET_OK if successful, other error codes
 * indicate failure)
 */
rmw_ret_t rmw_discovery_options_equal(const rmw_discovery_options_t* const left,
                                      const rmw_discovery_options_t* const right,
                                      bool* result) {
  // 检查输入参数是否有效 (Check if input arguments are valid)
  if (!left || !right || !result) {
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 比较automatic_discovery_range成员 (Compare automatic_discovery_range members)
  if (left->automatic_discovery_range != right->automatic_discovery_range) {
    *result = false;
    return RMW_RET_OK;
  }

  // 比较static_peers_count成员 (Compare static_peers_count members)
  if (left->static_peers_count != right->static_peers_count) {
    *result = false;
    return RMW_RET_OK;
  }

  // 如果static_peers_count为0，则无需检查static_peers数组 (If static_peers_count is 0, no need to
  // check the static_peers arrays)
  if (left->static_peers_count == 0) {
    *result = true;
    return RMW_RET_OK;
  }

  // 检查static_peers指针是否有效 (Check if static_peers pointers are valid)
  if (!left->static_peers || !right->static_peers) {
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 遍历并比较static_peers数组中的元素 (Iterate and compare elements in the static_peers arrays)
  for (size_t ii = 0; ii < left->static_peers_count; ++ii) {
    if (strncmp(left->static_peers[ii].peer_address, right->static_peers[ii].peer_address,
                RMW_DISCOVERY_OPTIONS_STATIC_PEERS_MAX_LENGTH) != 0) {
      *result = false;
      return RMW_RET_OK;
    }
  }

  // 如果所有成员相等，则设置结果为true (Set result to true if all members are equal)
  *result = true;
  return RMW_RET_OK;
}

/**
 * @brief 复制发现选项 (Copy discovery options)
 *
 * @param[in] src 源发现选项结构体指针 (Pointer to the source discovery options structure)
 * @param[in] allocator 分配器指针，用于内存管理 (Pointer to the allocator for memory management)
 * @param[out] dst 目标发现选项结构体指针 (Pointer to the destination discovery options structure)
 * @return rmw_ret_t 返回操作结果 (Returns the operation result)
 */
rmw_ret_t rmw_discovery_options_copy(const rmw_discovery_options_t* src,
                                     rcutils_allocator_t* allocator,
                                     rmw_discovery_options_t* dst) {
  // 检查传入的源发现选项是否为空 (Check if the source discovery options are NULL)
  RMW_CHECK_ARGUMENT_FOR_NULL(src, RMW_RET_INVALID_ARGUMENT);
  // 检查分配器是否有效 (Check if the allocator is valid)
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);
  // 检查传入的目标发现选项是否为空 (Check if the destination discovery options are NULL)
  RMW_CHECK_ARGUMENT_FOR_NULL(dst, RMW_RET_INVALID_ARGUMENT);
  // 检查源和目标发现选项是否相同 (Check if the source and destination discovery options are the
  // same)
  if (src == dst) {
    RMW_SET_ERROR_MSG("src and dst must be different");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 初始化目标发现选项 (Initialize the destination discovery options)
  rmw_ret_t ret = rmw_discovery_options_init(dst, src->static_peers_count, allocator);
  if (RMW_RET_OK != ret) {
    return ret;
  }

  // 复制自动发现范围 (Copy the automatic discovery range)
  dst->automatic_discovery_range = src->automatic_discovery_range;

  // 遍历静态节点并复制地址 (Iterate through static peers and copy their addresses)
  for (size_t i = 0; i < src->static_peers_count; i++) {
#ifdef _WIN32
    strncpy_s(dst->static_peers[i].peer_address, RMW_DISCOVERY_OPTIONS_STATIC_PEERS_MAX_LENGTH,
              src->static_peers[i].peer_address, RMW_DISCOVERY_OPTIONS_STATIC_PEERS_MAX_LENGTH);
#else
    strncpy(dst->static_peers[i].peer_address, src->static_peers[i].peer_address,
            RMW_DISCOVERY_OPTIONS_STATIC_PEERS_MAX_LENGTH);
    // 确保字符串以空字符结尾 (Ensure the string ends with a null character)
    dst->static_peers[i].peer_address[RMW_DISCOVERY_OPTIONS_STATIC_PEERS_MAX_LENGTH - 1] = '\0';
#endif
  }

  return RMW_RET_OK;
}

/**
 * @brief 终止发现选项 (Finalize discovery options)
 *
 * @param[in,out] discovery_options 要终止的发现选项结构体指针 (Pointer to the discovery options
 * structure to finalize)
 * @return rmw_ret_t 返回操作结果 (Returns the operation result)
 */
rmw_ret_t rmw_discovery_options_fini(rmw_discovery_options_t* discovery_options) {
  // 检查传入的发现选项是否为空 (Check if the discovery options are NULL)
  RMW_CHECK_ARGUMENT_FOR_NULL(discovery_options, RMW_RET_INVALID_ARGUMENT);

  // 如果有静态节点，释放其内存 (If there are static peers, deallocate their memory)
  if (discovery_options->static_peers_count > 0) {
    RCUTILS_CHECK_ALLOCATOR(&(discovery_options->allocator), return RMW_RET_INVALID_ARGUMENT);
    discovery_options->allocator.deallocate(discovery_options->static_peers,
                                            discovery_options->allocator.state);
  }
  // 将发现选项设置为零初始化状态 (Set the discovery options to a zero-initialized state)
  *discovery_options = rmw_get_zero_initialized_discovery_options();

  return RMW_RET_OK;
}
