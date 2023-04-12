// Copyright 2018 Open Source Robotics Foundation, Inc.
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

#include "rmw/init.h"

#include <stddef.h>

#include "rmw/domain_id.h"
#include "rmw/init_options.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获取一个初始化为零的 rmw_context_t 结构体实例 (Get a zero-initialized rmw_context_t
 * instance)
 *
 * @return 返回一个初始化为零的 rmw_context_t 结构体实例 (Return a zero-initialized rmw_context_t
 * instance)
 */
rmw_context_t rmw_get_zero_initialized_context(void) {
  // 使用列表初始化器创建一个 rmw_context_t 结构体实例，并将其字段设置为零或 NULL
  // (Create an rmw_context_t instance using a designated initializer and set its fields to zero or
  // NULL)
  return (const rmw_context_t){
      .instance_id = 0,                   // 将 instance_id 设置为 0 (Set the instance_id to 0)
      .implementation_identifier = NULL,  // 将 implementation_identifier 设置为 NULL (Set the
                                          // implementation_identifier to NULL)
      .options =
          rmw_get_zero_initialized_init_options(),  // 使用 rmw_get_zero_initialized_init_options
                                                    // 函数获取一个初始化为零的 rmw_init_options_t
                                                    // 结构体实例并设置 options 字段 (Get a
                                                    // zero-initialized rmw_init_options_t instance
                                                    // using rmw_get_zero_initialized_init_options
                                                    // function and set the options field)
      .actual_domain_id = 0u,  // 将 actual_domain_id 设置为 0 (Set the actual_domain_id to 0)
      .impl = NULL             // 将 impl 设置为 NULL (Set the impl to NULL)
  };                           // NOLINT(readability/braces): false positive
}

#ifdef __cplusplus
}
#endif
