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

#ifndef RMW__SANITY_CHECKS_H_
#define RMW__SANITY_CHECKS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rcutils/types.h"
#include "rmw/macros.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// 检查字符串数组结构是否为零初始化 (Check that a string_array struct is zero initialized)
/**
 * 如果数组未零初始化，则设置错误消息并返回错误代码
 * (This sets error message and returns error code if array is not zero initialized)
 *
 * \param[in] array 要检查的字符串数组 (The string array to check)
 * \return 如果数组已零初始化则返回 RMW_RET_OK，否则返回 RMW_RET_ERROR
 *         (RMW_RET_OK if array is zero initialized, otherwise RMW_RET_ERROR)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_check_zero_rmw_string_array(rcutils_string_array_t* array);

#ifdef __cplusplus
}
#endif

#endif  // RMW__SANITY_CHECKS_H_
