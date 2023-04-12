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

#ifndef RMW__EVENTS_STATUSES__REQUESTED_DEADLINE_MISSED_H_
#define RMW__EVENTS_STATUSES__REQUESTED_DEADLINE_MISSED_H_

#include <stdint.h>

#include "rmw/visibility_control.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief QoS 请求截止日期未满足的信息，由订阅提供。 (QoS Requested Deadline Missed information
/// provided by a subscription.)
typedef struct RMW_PUBLIC_TYPE rmw_requested_deadline_missed_status_s {
  /**
   * @brief 订阅读取的任何实例的生命周期累积未满足的截止日期数量。
   * 未满足的截止日期会累积；也就是说，每个截止日期周期，对于未收到数据的每个实例，total_count
   * 将增加一个。 (Lifetime cumulative number of missed deadlines detected for any instance read by
   * the subscription. Missed deadlines accumulate; that is, each deadline period the total_count
   * will be incremented by one for each instance for which data was not received.)
   */
  int32_t total_count;

  /// @brief 自上次读取状态以来检测到的截止日期的增量数量。 (The incremental number of deadlines
  /// detected since the status was read.)
  int32_t total_count_change;
} rmw_requested_deadline_missed_status_t;

#ifdef __cplusplus
}
#endif

#endif  // RMW__EVENTS_STATUSES__REQUESTED_DEADLINE_MISSED_H_
