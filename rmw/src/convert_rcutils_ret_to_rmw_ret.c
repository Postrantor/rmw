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

#include "rmw/convert_rcutils_ret_to_rmw_ret.h"

/**
 * @brief 将 rcutils_ret_t 类型转换为 rmw_ret_t 类型 (Convert rcutils_ret_t type to rmw_ret_t type)
 *
 * @param[in] rcutils_ret 输入的 rcutils_ret_t 类型值 (The input value of type rcutils_ret_t)
 * @return 返回对应的 rmw_ret_t 类型值 (Return the corresponding value of type rmw_ret_t)
 */
rmw_ret_t rmw_convert_rcutils_ret_to_rmw_ret(rcutils_ret_t rcutils_ret) {
  // 使用 switch 语句进行类型转换 (Use switch statement for type conversion)
  switch (rcutils_ret) {
    // 如果 rcutils_ret 的值为 RCUTILS_RET_OK (If the value of rcutils_ret is RCUTILS_RET_OK)
    case RCUTILS_RET_OK:
      // 返回 RMW_RET_OK (Return RMW_RET_OK)
      return RMW_RET_OK;
    // 如果 rcutils_ret 的值为 RCUTILS_RET_INVALID_ARGUMENT (If the value of rcutils_ret is
    // RCUTILS_RET_INVALID_ARGUMENT)
    case RCUTILS_RET_INVALID_ARGUMENT:
      // 返回 RMW_RET_INVALID_ARGUMENT (Return RMW_RET_INVALID_ARGUMENT)
      return RMW_RET_INVALID_ARGUMENT;
    // 如果 rcutils_ret 的值为 RCUTILS_RET_BAD_ALLOC (If the value of rcutils_ret is
    // RCUTILS_RET_BAD_ALLOC)
    case RCUTILS_RET_BAD_ALLOC:
      // 返回 RMW_RET_BAD_ALLOC (Return RMW_RET_BAD_ALLOC)
      return RMW_RET_BAD_ALLOC;
    // 如果 rcutils_ret 的值为 RCUTILS_RET_ERROR (If the value of rcutils_ret is RCUTILS_RET_ERROR)
    case RCUTILS_RET_ERROR:
      // 返回 RMW_RET_ERROR (Return RMW_RET_ERROR)
      return RMW_RET_ERROR;
    // 其他情况 (Other cases)
    default:
      // 返回 RMW_RET_ERROR (Return RMW_RET_ERROR)
      return RMW_RET_ERROR;
  }
}
