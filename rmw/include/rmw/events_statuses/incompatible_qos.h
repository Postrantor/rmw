// Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#ifndef RMW__EVENTS_STATUSES__INCOMPATIBLE_QOS_H_
#define RMW__EVENTS_STATUSES__INCOMPATIBLE_QOS_H_

#include <stdint.h>

#include "rmw/qos_policy_kind.h"
#include "rmw/visibility_control.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct rmw_qos_incompatible_event_status_s
 * @brief 用于表示 QoS 不兼容事件状态的结构体 (Structure representing the status of a QoS
 * incompatible event)
 */
typedef struct RMW_PUBLIC_TYPE rmw_qos_incompatible_event_status_s {
  /**
   * @var int32_t total_count
   * @brief 订阅者发现与请求的 QoS 不兼容的同一主题发布者的累计总次数
   *        (Total cumulative number of times the concerned subscription discovered a
   *         publisher for the same topic with an offered QoS that was incompatible
   *         with that requested by the subscription.)
   */
  int32_t total_count;

  /**
   * @var int32_t total_count_change
   * @brief 自上次读取状态以来，total_count 的变化值
   *        (The change in total_count since the last time the status was read.)
   */
  int32_t total_count_change;

  /**
   * @var rmw_qos_policy_kind_t last_policy_kind
   * @brief 上次检测到不兼容性时，发现的不兼容策略之一的 QoS 策略类型
   *        (The Qos Policy Kind of one of the policies that was found to be
   *         incompatible the last time an incompatibility was detected.)
   */
  rmw_qos_policy_kind_t last_policy_kind;
} rmw_qos_incompatible_event_status_t;

/**
 * @typedef rmw_requested_qos_incompatible_event_status_t
 * @brief 订阅者的 'RMW_EVENT_REQUESTED_QOS_INCOMPATIBLE' 事件状态
 *        (Event state for a subscription's 'RMW_EVENT_REQUESTED_QOS_INCOMPATIBLE' events.)
 */
typedef rmw_qos_incompatible_event_status_t rmw_requested_qos_incompatible_event_status_t;

/**
 * @typedef rmw_offered_qos_incompatible_event_status_t
 * @brief 发布者的 'RMW_EVENT_OFFERED_QOS_INCOMPATIBLE' 事件状态
 *        (Event state for a publisher's 'RMW_EVENT_OFFERED_QOS_INCOMPATIBLE' events.)
 */
typedef rmw_qos_incompatible_event_status_t rmw_offered_qos_incompatible_event_status_t;

#ifdef __cplusplus
}
#endif

#endif  // RMW__EVENTS_STATUSES__INCOMPATIBLE_QOS_H_
