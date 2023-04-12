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

#include "rmw/init_options.h"

#include <stddef.h>

#include "rmw/discovery_options.h"
#include "rmw/localhost.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获取一个初始化为零值的 rmw_init_options_t 结构体 (Get a zero-initialized
 * rmw_init_options_t structure)
 *
 * @return 返回一个初始化为零值的 rmw_init_options_t 结构体实例 (Return a zero-initialized
 * rmw_init_options_t structure instance)
 */
rmw_init_options_t rmw_get_zero_initialized_init_options(void) {
  // 使用初始化列表创建一个新的 rmw_init_options_t 结构体实例，并设置其各个字段的默认值
  // (Create a new rmw_init_options_t structure instance using an initializer list and set the
  // default values for its fields)
  return (const rmw_init_options_t){
      .domain_id =
          RMW_DEFAULT_DOMAIN_ID,  // 设置域 ID 为默认值 (Set the domain ID to the default value)
      .localhost_only = RMW_LOCALHOST_ONLY_DEFAULT,  // 设置本地主机限制为默认值 (Set the localhost
                                                     // restriction to the default value)
      .discovery_options =
          rmw_get_zero_initialized_discovery_options(),  // 设置发现选项为零初始化的默认值 (Set the
                                                         // discovery options to the
                                                         // zero-initialized default value)
      .implementation_identifier =
          NULL,      // 将实现标识符设置为 NULL (Set the implementation identifier to NULL)
      .impl = NULL,  // 将实现指针设置为 NULL (Set the implementation pointer to NULL)
      .instance_id = 0,  // 将实例 ID 设置为 0 (Set the instance ID to 0)
      .enclave = NULL,   // 将固定区设置为 NULL (Set the enclave to NULL)
      .security_options =
          rmw_get_default_security_options(),  // 设置安全选项为默认值 (Set the security options to
                                               // the default value)
  };  // NOLINT(readability/braces): false positive
}

#ifdef __cplusplus
}
#endif
