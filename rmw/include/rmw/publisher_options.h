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

#ifndef RMW__PUBLISHER_OPTIONS_H_
#define RMW__PUBLISHER_OPTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rmw/types.h"

// For now, the rmw_publisher_options_t type is still defined in "rmw/types.h".
/**
 * @brief 返回一个使用默认值初始化的 rmw_publisher_options_t。
 * Return a rmw_publisher_options_t initialized with default values.
 *
 * @return 返回一个具有默认值的 rmw_publisher_options_t 结构体。
 *         Returns a rmw_publisher_options_t structure with default values.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_publisher_options_t rmw_get_default_publisher_options(void);

#ifdef __cplusplus
}
#endif

#endif  // RMW__PUBLISHER_OPTIONS_H_
