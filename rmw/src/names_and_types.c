// Copyright 2017 Open Source Robotics Foundation, Inc.
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

#include "rmw/names_and_types.h"

#include "rcutils/logging_macros.h"
#include "rcutils/macros.h"
#include "rcutils/types/string_array.h"
#include "rmw/convert_rcutils_ret_to_rmw_ret.h"
#include "rmw/error_handling.h"
#include "rmw/types.h"

/**
 * @brief 获取一个已初始化为零的rmw_names_and_types_t结构体
 * Get a zero-initialized rmw_names_and_types_t structure
 *
 * @return 一个已初始化为零的rmw_names_and_types_t结构体实例
 * An instance of the zero-initialized rmw_names_and_types_t structure
 */
rmw_names_and_types_t rmw_get_zero_initialized_names_and_types(void) {
  // 初始化一个静态的rmw_names_and_types_t变量为零值
  // Initialize a static rmw_names_and_types_t variable to zero
  static rmw_names_and_types_t zero = {
      .names = {0, NULL, {NULL, NULL, NULL, NULL, NULL}},
      .types = NULL,
  };
  // 将names字段设置为rcutils_get_zero_initialized_string_array()的返回值
  // Set the names field to the return value of rcutils_get_zero_initialized_string_array()
  zero.names = rcutils_get_zero_initialized_string_array();
  // 返回这个已初始化为零的rmw_names_and_types_t变量
  // Return this zero-initialized rmw_names_and_types_t variable
  return zero;
}

/**
 * @brief 检查rmw_names_and_types_t结构体是否已初始化为零
 * Check if an rmw_names_and_types_t structure is initialized to zero
 *
 * @param[in] names_and_types 要检查的rmw_names_and_types_t结构体指针
 * Pointer to the rmw_names_and_types_t structure to check
 * @return RMW_RET_OK 如果成功，否则返回相应的错误代码
 * RMW_RET_OK if successful, otherwise return the corresponding error code
 */
rmw_ret_t rmw_names_and_types_check_zero(rmw_names_and_types_t* names_and_types) {
  // 宏，用于检查函数是否可以返回RMW_RET_INVALID_ARGUMENT错误
  // Macro to check if the function can return RMW_RET_INVALID_ARGUMENT error
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  // 检查names_and_types参数是否为NULL
  // Check if the names_and_types argument is NULL
  if (!names_and_types) {
    RMW_SET_ERROR_MSG("names_and_types is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 检查names字段是否已初始化为零
  // Check if the names field is initialized to zero
  if (names_and_types->names.data != 0 || names_and_types->names.size != 0) {
    RMW_SET_ERROR_MSG("names array is not zeroed");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 检查types字段是否为NULL
  // Check if the types field is NULL
  if (names_and_types->types != 0) {
    RMW_SET_ERROR_MSG("types array is not NULL");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 如果成功，返回RMW_RET_OK
  // Return RMW_RET_OK if successful
  return RMW_RET_OK;
}

/**
 * @brief 初始化rmw_names_and_types_t结构体
 * Initialize an rmw_names_and_types_t structure
 *
 * @param[out] names_and_types 要初始化的rmw_names_and_types_t结构体指针
 * Pointer to the rmw_names_and_types_t structure to initialize
 * @param[in] size 初始化names数组的大小
 * Size to initialize the names array
 * @param[in] allocator 分配器用于分配内存
 * Allocator for memory allocation
 * @return RMW_RET_OK 如果成功，否则返回相应的错误代码
 * RMW_RET_OK if successful, otherwise return the corresponding error code
 */
rmw_ret_t rmw_names_and_types_init(rmw_names_and_types_t* names_and_types,
                                   size_t size,
                                   rcutils_allocator_t* allocator) {
  // 宏，用于检查函数是否可以返回RMW_RET_INVALID_ARGUMENT和RMW_RET_BAD_ALLOC错误
  // Macro to check if the function can return RMW_RET_INVALID_ARGUMENT and RMW_RET_BAD_ALLOC errors
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_BAD_ALLOC);

  // 检查allocator参数是否为NULL
  // Check if the allocator argument is NULL
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 检查names_and_types参数是否为NULL
  // Check if the names_and_types argument is NULL
  if (!names_and_types) {
    RMW_SET_ERROR_MSG("names_and_types is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 使用rcutils_string_array_init()函数初始化names字段
  // Initialize the names field using the rcutils_string_array_init() function
  rcutils_ret_t rcutils_ret = rcutils_string_array_init(&names_and_types->names, size, allocator);
  // 检查rcutils_string_array_init()函数是否成功执行
  // Check if the rcutils_string_array_init() function executed successfully
  if (rcutils_ret != RCUTILS_RET_OK) {
    RMW_SET_ERROR_MSG(rcutils_get_error_string().str);
    return rmw_convert_rcutils_ret_to_rmw_ret(rcutils_ret);
  }
  // 使用分配器的zero_allocate方法为types字段分配内存
  // Allocate memory for the types field using the allocator's zero_allocate method
  names_and_types->types =
      allocator->zero_allocate(size, sizeof(rcutils_string_array_t), allocator->state);
  // 检查分配是否成功，如果不成功且size不为0，则清理names字段并返回RMW_RET_BAD_ALLOC错误
  // Check if the allocation was successful, if not and size is not 0, clean up the names field and
  // return RMW_RET_BAD_ALLOC error
  if (!names_and_types->types && size != 0) {
    rcutils_ret = rcutils_string_array_fini(&names_and_types->names);
    if (rcutils_ret != RCUTILS_RET_OK) {
      RCUTILS_LOG_ERROR("error while reporting error: %s", rcutils_get_error_string().str);
    }
    RMW_SET_ERROR_MSG("failed to allocate memory for types");
    return RMW_RET_BAD_ALLOC;
  }
  // 如果成功，返回RMW_RET_OK
  // Return RMW_RET_OK if successful
  return RMW_RET_OK;
}

/**
 * @brief 释放 rmw_names_and_types_t 结构体的内存 (Free the memory of a rmw_names_and_types_t
 * structure)
 *
 * @param[in] names_and_types 指向要释放内存的 rmw_names_and_types_t 结构体指针 (Pointer to the
 * rmw_names_and_types_t structure whose memory is to be freed)
 * @return 返回一个 rmw_ret_t 类型的状态，表示函数执行的成功或失败 (Returns an rmw_ret_t type status
 * indicating the success or failure of the function execution)
 */
rmw_ret_t rmw_names_and_types_fini(rmw_names_and_types_t* names_and_types) {
  // 判断 names_and_types 是否为空指针 (Check if names_and_types is a null pointer)
  if (!names_and_types) {
    RMW_SET_ERROR_MSG("names_and_types is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  rcutils_ret_t rcutils_ret;

  // 判断 names_and_types->types 是否为空指针 (Check if names_and_types->types is a null pointer)
  if (names_and_types->types) {
    // 检查分配器是否有效 (Check if allocator is valid)
    RCUTILS_CHECK_ALLOCATOR_WITH_MSG(&names_and_types->names.allocator, "allocator is invalid",
                                     return RMW_RET_INVALID_ARGUMENT);

    // 首先清理类型的字符串数组 (First clean up the string arrays for types)
    size_t i;
    for (i = 0; i < names_and_types->names.size; ++i) {
      rcutils_ret = rcutils_string_array_fini(&names_and_types->types[i]);
      if (rcutils_ret != RCUTILS_RET_OK) {
        RMW_SET_ERROR_MSG(rcutils_get_error_string().str);
        return rmw_convert_rcutils_ret_to_rmw_ret(rcutils_ret);
      }
    }

    // 使用 names 字符串数组中的分配器 (Use the allocator in the names string array)
    // (防止此数据结构也必须存储它) (Prevents this data structure from having to also store it)
    names_and_types->names.allocator.deallocate(names_and_types->types,
                                                names_and_types->names.allocator.state);
    names_and_types->types = NULL;
  }

  // 清理 names 字符串数组 (Clean up the names string array)
  rcutils_ret = rcutils_string_array_fini(&names_and_types->names);
  if (rcutils_ret != RCUTILS_RET_OK) {
    RMW_SET_ERROR_MSG(rcutils_get_error_string().str);
    return rmw_convert_rcutils_ret_to_rmw_ret(rcutils_ret);
  }
  return RMW_RET_OK;
}
