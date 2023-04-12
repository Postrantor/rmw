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

#ifndef RMW__EVENTS_STATUSES__LIVELINESS_CHANGED_H_
#define RMW__EVENTS_STATUSES__LIVELINESS_CHANGED_H_

#include <stdint.h>

#include "rmw/visibility_control.h"

#ifdef __cplusplus
extern "C" {
#endif

/// QoS Liveliness Changed information provided by a subscription.
// QoS 活跃性变化信息由订阅提供。
typedef struct RMW_PUBLIC_TYPE rmw_liveliness_changed_status_s {
  /**
   * The total number of currently active Publishers which publish to the topic associated with
   * the Subscription.
   * This count increases when a newly matched Publisher asserts its liveliness for the first time
   * or when a Publisher previously considered to be not alive reasserts its liveliness.
   * The count decreases when a Publisher considered alive fails to assert its liveliness and
   * becomes not alive, whether because it was deleted normally or for some other reason.
   */
  // 当前活动的发布者总数，它们将发布到与订阅关联的主题。
  // 当新匹配的发布者首次声明其活跃性时，此计数增加；
  // 或者当先前被认为不活跃的发布者重新声明其活跃性时。
  // 当一个被认为活跃的发布者未能声明其活跃性并变得不活跃时，计数减少，
  // 无论是因为正常删除还是其他原因。
  int32_t alive_count;

  /**
   * The total count of current Publishers which publish to the topic associated with the
   * Subscription that are no longer asserting their liveliness.
   * This count increases when a Publisher considered alive fails to assert its liveliness and
   * becomes not alive for some reason other than the normal deletion of that Publisher.
   * It decreases when a previously not alive Publisher either reasserts its liveliness or is
   * deleted normally.
   */
  // 当前发布者的总数，它们将发布到与订阅关联的主题，但不再声明其活跃性。
  // 当一个被认为活跃的发布者未能声明其活跃性并因非正常删除原因而变得不活跃时，计数增加。
  // 当先前不活跃的发布者重新声明其活跃性或正常删除时，计数减少。
  int32_t not_alive_count;

  /// The change in the alive_count since the status was last read.
  // 自上次读取状态以来，alive_count 的变化。
  int32_t alive_count_change;

  /// The change in the not_alive_count since the status was last read.
  // 自上次读取状态以来，not_alive_count 的变化。
  int32_t not_alive_count_change;
} rmw_liveliness_changed_status_t;

#ifdef __cplusplus
}
#endif

#endif  // RMW__EVENTS_STATUSES__LIVELINESS_CHANGED_H_
