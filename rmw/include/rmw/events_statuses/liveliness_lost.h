// Copyright 2020 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__EVENTS_STATUSES__LIVELINESS_LOST_H_
#define RMW__EVENTS_STATUSES__LIVELINESS_LOST_H_

#include <stdint.h>

#include "rmw/visibility_control.h"

#ifdef __cplusplus
extern "C" {
#endif

/// QoS Liveliness Lost information provided by a publisher.
/// 由发布者提供的 QoS Liveliness Lost 信息。
typedef struct RMW_PUBLIC_TYPE rmw_liveliness_lost_status_s {
  /**
   * Lifetime cumulative number of times that a previously-alive Publisher became not alive due to
   * a failure to actively signal its liveliness within its offered liveliness period.
   * This count does not change when an already not alive Publisher simply remains not alive for
   * another liveliness period.
   *
   * 在其提供的生命周期内，之前活跃的发布者因未能主动发送其活跃信号而变成不活跃的累计次数。
   * 当一个已经不活跃的发布者在另一个生命周期内仍然保持不活跃时，此计数不会改变。
   */
  int32_t total_count;
  /// The change in total_count since the last time the status was last read.
  /// 自上次读取状态以来 total_count 的变化。
  int32_t total_count_change;
} rmw_liveliness_lost_status_t;

#ifdef __cplusplus
}
#endif

#endif  // RMW__EVENTS_STATUSES__LIVELINESS_LOST_H_
