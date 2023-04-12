// Copyright 2016-2017 Open Source Robotics Foundation, Inc.
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

#include "rmw/sanity_checks.h"

#include <stdlib.h>
#include <string.h>

#include "rcutils/types.h"
#include "rmw/error_handling.h"
#include "rmw/types.h"

/**
 * @brief 检查字符串数组是否为空（Check if the given string array is empty）
 *
 * @param[in] array 要检查的字符串数组指针（Pointer to the string array to be checked）
 * @return rmw_ret_t 返回检查结果，如果为空则返回 RMW_RET_OK，否则返回 RMW_RET_ERROR（Return check
 * result, RMW_RET_OK if empty, otherwise RMW_RET_ERROR）
 */
rmw_ret_t rmw_check_zero_rmw_string_array(rcutils_string_array_t* array) {
  // 如果数组指针为空，则设置错误信息并返回 RMW_RET_ERROR
  // If the array pointer is null, set the error message and return RMW_RET_ERROR
  if (!array) {
    RMW_SET_ERROR_MSG("array is null");
    return RMW_RET_ERROR;
  }
  // 如果数组大小不为零，则设置错误信息并返回 RMW_RET_ERROR
  // If the array size is not zero, set the error message and return RMW_RET_ERROR
  if (array->size != 0) {
    RMW_SET_ERROR_MSG("array size is not zero");
    return RMW_RET_ERROR;
  }
  // 如果数组数据指针不为空，则设置错误信息并返回 RMW_RET_ERROR
  // If the array data pointer is not null, set the error message and return RMW_RET_ERROR
  if (array->data) {
    RMW_SET_ERROR_MSG("array data is not null");
    return RMW_RET_ERROR;
  }
  // 如果以上条件都不满足，说明字符串数组为空，返回 RMW_RET_OK
  // If none of the above conditions are met, the string array is empty, return RMW_RET_OK
  return RMW_RET_OK;
}
