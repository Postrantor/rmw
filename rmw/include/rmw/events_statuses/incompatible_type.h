// Copyright 2022 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__EVENTS_STATUSES__INCOMPATIBLE_TYPE_H_
#define RMW__EVENTS_STATUSES__INCOMPATIBLE_TYPE_H_

#include <stdint.h>

#include "rmw/visibility_control.h"

#ifdef __cplusplus
extern "C" {
#endif

/// 不兼容类型信息由主题提供。
/// Incompatible type information provided by a topic.
typedef struct RMW_PUBLIC_TYPE rmw_incompatible_type_status_s {
  /// 生命周期内累计检测到的不兼容类型数量。
  /// Lifetime cumulative number of incompatible types detected.
  int32_t total_count;

  /// 自从状态被读取以来，检测到的不兼容类型的增量数量。
  /// The incremental number of incompatible types detected since the status was read.
  int32_t total_count_change;
} rmw_incompatible_type_status_t;

#ifdef __cplusplus
}
#endif

#endif  // RMW__EVENTS_STATUSES__INCOMPATIBLE_TYPE_H_
