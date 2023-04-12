// Copyright 2023 Sony Group Corporation.
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

#ifndef RMW__EVENTS_STATUSES__MATCHED_H_
#define RMW__EVENTS_STATUSES__MATCHED_H_

#include <stddef.h>
#include <stdint.h>

#include "rmw/visibility_control.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief RMW_PUBLIC_TYPE rmw_matched_status_s 结构体
 *
 * 用于存储与发布者和订阅者相关的匹配状态信息。
 * Used to store the matched status information related to publishers and subscribers.
 */
typedef struct RMW_PUBLIC_TYPE rmw_matched_status_s {
  /**
   * @brief 对于发布者，与关注的发布者匹配的订阅者的总累积计数。
   *        对于订阅者，与关注的订阅者匹配的发布者的总累积计数。
   *
   * For publisher, the total cumulative count of subscribers matched to the concerned publisher.
   * For subscriber, the total cumulative count of publishers matched to the concerned subscriber.
   */
  size_t total_count;

  /**
   * @brief 自上次读取状态以来的 total_count 变化。
   *
   * The change in total_count since the last time the status was read.
   */
  size_t total_count_change;

  /**
   * @brief 对于发布者，当前与关注的发布者匹配的订阅者数量。
   *        对于订阅者，当前与关注的订阅者匹配的发布者数量。
   *
   * For publisher, the number of subscribers currently matched to the concerned publisher.
   * For subscriber, the number of publishers currently matched to the concerned subscriber.
   */
  size_t current_count;

  /**
   * @brief 自上次读取状态以来的 current_count 变化。
   *
   * The change in current_count since the last time the status was read.
   */
  int32_t current_count_change;
} rmw_matched_status_t;

#ifdef __cplusplus
}
#endif

#endif  // RMW__EVENTS_STATUSES__MATCHED_H_
