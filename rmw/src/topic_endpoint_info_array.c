// Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#include "rmw/topic_endpoint_info_array.h"

#include "rmw/error_handling.h"
#include "rmw/types.h"

/**
 * @brief 初始化一个空的主题端点信息数组
 *        Initialize an empty topic endpoint info array
 *
 * @return 返回一个已初始化为零的主题端点信息数组
 *         Return a zero-initialized topic endpoint info array
 */
rmw_topic_endpoint_info_array_t rmw_get_zero_initialized_topic_endpoint_info_array(void) {
  // 如果使用Clang编译器，忽略-Wmissing-field-initializers警告
  // If using Clang compiler, ignore -Wmissing-field-initializers warning
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#endif

  // 初始化一个空的主题端点信息数组
  // Initialize an empty topic endpoint info array
  rmw_topic_endpoint_info_array_t zero = {0};

  // 恢复Clang编译器的警告设置
  // Restore Clang compiler's warning settings
#ifdef __clang__
#pragma clang diagnostic pop
#endif

  // 返回已初始化为零的主题端点信息数组
  // Return the zero-initialized topic endpoint info array
  return zero;
}

/**
 * @brief 检查主题端点信息数组是否已初始化为零
 *        Check if a topic endpoint info array is zero-initialized
 *
 * @param[in] topic_endpoint_info_array 要检查的主题端点信息数组指针
 *                                      Pointer to the topic endpoint info array to check
 * @return 返回RMW_RET_OK表示数组已初始化为零，否则返回相应的错误代码
 *         Return RMW_RET_OK if the array is zero-initialized, otherwise return the corresponding
 * error code
 */
rmw_ret_t rmw_topic_endpoint_info_array_check_zero(
    rmw_topic_endpoint_info_array_t* topic_endpoint_info_array) {
  // 检查主题端点信息数组指针是否为空
  // Check if the topic endpoint info array pointer is null
  if (!topic_endpoint_info_array) {
    RMW_SET_ERROR_MSG("topic_endpoint_info_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 检查主题端点信息数组是否已初始化为零
  // Check if the topic endpoint info array is zero-initialized
  if (topic_endpoint_info_array->size != 0 || topic_endpoint_info_array->info_array != NULL) {
    RMW_SET_ERROR_MSG("topic_endpoint_info_array is not zeroed");
    return RMW_RET_ERROR;
  }

  // 返回RMW_RET_OK表示数组已初始化为零
  // Return RMW_RET_OK to indicate the array is zero-initialized
  return RMW_RET_OK;
}

/**
 * @brief 使用给定的大小和分配器初始化主题端点信息数组
 *        Initialize a topic endpoint info array with the given size and allocator
 *
 * @param[in,out] topic_endpoint_info_array 要初始化的主题端点信息数组指针
 *                                          Pointer to the topic endpoint info array to initialize
 * @param[in] size 初始化数组的大小
 *                 Size to initialize the array with
 * @param[in] allocator 用于分配内存的分配器指针
 *                      Pointer to the allocator for memory allocation
 * @return 返回RMW_RET_OK表示成功初始化，否则返回相应的错误代码
 *         Return RMW_RET_OK if successfully initialized, otherwise return the corresponding error
 * code
 */
rmw_ret_t rmw_topic_endpoint_info_array_init_with_size(
    rmw_topic_endpoint_info_array_t* topic_endpoint_info_array,
    size_t size,
    rcutils_allocator_t* allocator) {
  // 检查分配器指针是否为空
  // Check if the allocator pointer is null
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 检查主题端点信息数组指针是否为空
  // Check if the topic endpoint info array pointer is null
  if (!topic_endpoint_info_array) {
    RMW_SET_ERROR_MSG("topic_endpoint_info_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 使用给定的分配器为主题端点信息数组分配内存
  // Allocate memory for the topic endpoint info array using the given allocator
  topic_endpoint_info_array->info_array =
      allocator->allocate(sizeof(*topic_endpoint_info_array->info_array) * size, allocator->state);

  // 检查分配的内存是否为空
  // Check if the allocated memory is null
  if (!topic_endpoint_info_array->info_array) {
    RMW_SET_ERROR_MSG("failed to allocate memory for info_array");
    return RMW_RET_BAD_ALLOC;
  }

  // 设置主题端点信息数组的大小
  // Set the size of the topic endpoint info array
  topic_endpoint_info_array->size = size;

  // 使用零初始化的主题端点信息填充数组
  // Fill the array with zero-initialized topic endpoint info
  for (size_t i = 0; i < size; i++) {
    topic_endpoint_info_array->info_array[i] = rmw_get_zero_initialized_topic_endpoint_info();
  }

  // 返回RMW_RET_OK表示成功初始化
  // Return RMW_RET_OK to indicate successful initialization
  return RMW_RET_OK;
}

/**
 * @brief 释放 topic_endpoint_info_array 的内存资源 (Free memory resources of
 * topic_endpoint_info_array)
 *
 * @param[in,out] topic_endpoint_info_array 要释放的 topic_endpoint_info_array_t 结构体指针 (Pointer
 * to the topic_endpoint_info_array_t structure to be freed)
 * @param[in] allocator 用于分配和释放内存的 rcutils_allocator_t 结构体指针 (Pointer to the
 * rcutils_allocator_t structure for allocating and deallocating memory)
 * @return rmw_ret_t 返回操作结果 (Return the operation result)
 */
rmw_ret_t rmw_topic_endpoint_info_array_fini(
    rmw_topic_endpoint_info_array_t* topic_endpoint_info_array, rcutils_allocator_t* allocator) {
  // 检查 allocator 是否为空，如果为空则返回 RMW_RET_INVALID_ARGUMENT 错误 (Check if allocator is
  // null, if it is null, return RMW_RET_INVALID_ARGUMENT error)
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 检查 topic_endpoint_info_array 是否为空，如果为空则返回 RMW_RET_INVALID_ARGUMENT 错误 (Check if
  // topic_endpoint_info_array is null, if it is null, return RMW_RET_INVALID_ARGUMENT error)
  if (!topic_endpoint_info_array) {
    RMW_SET_ERROR_MSG("topic_endpoint_info_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  rmw_ret_t ret;
  // 遍历 topic_endpoint_info_array->info_array，并释放其内部的 const char* 资源 (Iterate through
  // topic_endpoint_info_array->info_array and free its internal const char* resources)
  for (size_t i = 0u; i < topic_endpoint_info_array->size; i++) {
    ret = rmw_topic_endpoint_info_fini(&topic_endpoint_info_array->info_array[i], allocator);
    // 如果释放资源失败，返回相应的错误代码 (If resource release fails, return the corresponding
    // error code)
    if (ret != RMW_RET_OK) {
      return ret;
    }
  }

  // 使用 allocator 释放 topic_endpoint_info_array->info_array 的内存 (Use allocator to free the
  // memory of topic_endpoint_info_array->info_array)
  allocator->deallocate(topic_endpoint_info_array->info_array, allocator->state);
  // 将 topic_endpoint_info_array->info_array 设置为 NULL (Set topic_endpoint_info_array->info_array
  // to NULL)
  topic_endpoint_info_array->info_array = NULL;
  // 将 topic_endpoint_info_array->size 设置为 0 (Set topic_endpoint_info_array->size to 0)
  topic_endpoint_info_array->size = 0;
  // 返回操作成功的结果代码 (Return the result code for successful operation)
  return RMW_RET_OK;
}
