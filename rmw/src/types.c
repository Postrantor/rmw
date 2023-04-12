// Copyright 2020 Robert Bosch GmbH. All Rights Reserved.
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

#include "rmw/types.h"

/**
 * @brief 初始化并返回一个全零的 rmw_message_info_t 结构体。Initialize and return an all-zero
 * rmw_message_info_t structure.
 *
 * @return 返回一个全零的 rmw_message_info_t 结构体。Return an all-zero rmw_message_info_t
 * structure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_message_info_t rmw_get_zero_initialized_message_info(void) {
  // 创建一个名为 zero_initialized_message_info 的 rmw_message_info_t
  // 结构体变量并将其所有成员初始化为 0。 Create an rmw_message_info_t structure variable named
  // zero_initialized_message_info and initialize all its members to 0.
  rmw_message_info_t zero_initialized_message_info = {0};

  // 返回已初始化的 zero_initialized_message_info 变量。Return the initialized
  // zero_initialized_message_info variable.
  return zero_initialized_message_info;
}
