// Copyright 2019 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__SUBSCRIPTION_OPTIONS_H_
#define RMW__SUBSCRIPTION_OPTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rmw/types.h"

// For now, the rmw_subscription_options_t type is still defined in "rmw/types.h".
// 目前，rmw_subscription_options_t 类型仍在 "rmw/types.h" 中定义
// At present, the rmw_subscription_options_t type is still defined in "rmw/types.h".

/// Return a rmw_subscription_options_t initialized with default values.
/// 返回一个用默认值初始化的 rmw_subscription_options_t。
/**
 * @return An rmw_subscription_options_t structure with default values.
 * @返回一个具有默认值的 rmw_subscription_options_t 结构体。
 */
RMW_PUBLIC        // 定义为公共实用程序的宏。 Macro defining as a public utility.
  RMW_WARN_UNUSED // 警告未使用的宏。 Macro warning for unused.

    // 函数原型: rmw_get_default_subscription_options()
    // Function prototype: rmw_get_default_subscription_options()
    rmw_subscription_options_t
    rmw_get_default_subscription_options(void);

#ifdef __cplusplus
}
#endif

#endif // RMW__SUBSCRIPTION_OPTIONS_H_
