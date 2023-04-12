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

#ifndef RMW__EVENT_H_
#define RMW__EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "rcutils/allocator.h"
#include "rmw/macros.h"
#include "rmw/ret_types.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// 定义发布者/订阅者事件 (Define publisher/subscription events)
typedef enum rmw_event_type_e {
  // 订阅者事件 (subscription events)
  RMW_EVENT_LIVELINESS_CHANGED,         ///< 活跃度改变 (Liveliness changed)
  RMW_EVENT_REQUESTED_DEADLINE_MISSED,  ///< 请求的截止日期未满足 (Requested deadline missed)
  RMW_EVENT_REQUESTED_QOS_INCOMPATIBLE,  ///< 请求的 QoS 不兼容 (Requested QoS incompatible)
  RMW_EVENT_MESSAGE_LOST,                ///< 消息丢失 (Message lost)
  RMW_EVENT_SUBSCRIPTION_INCOMPATIBLE_TYPE,  ///< 订阅者不兼容类型 (Subscription incompatible type)
  RMW_EVENT_SUBSCRIPTION_MATCHED,            ///< 订阅者匹配 (Subscription matched)

  // 发布者事件 (publisher events)
  RMW_EVENT_LIVELINESS_LOST,           ///< 活跃度丢失 (Liveliness lost)
  RMW_EVENT_OFFERED_DEADLINE_MISSED,   ///< 提供的截止日期未满足 (Offered deadline missed)
  RMW_EVENT_OFFERED_QOS_INCOMPATIBLE,  ///< 提供的 QoS 不兼容 (Offered QoS incompatible)
  RMW_EVENT_PUBLISHER_INCOMPATIBLE_TYPE,  ///< 发布者不兼容类型 (Publisher incompatible type)
  RMW_EVENT_PUBLICATION_MATCHED,          ///< 发布者匹配 (Publication matched)

  // 哨兵值 (sentinel value)
  RMW_EVENT_INVALID  ///< 无效事件 (Invalid event)
} rmw_event_type_t;

/// 封装 RMW 事件实现、数据和类型。 (Encapsulate the RMW event implementation, data, and type.)
typedef struct RMW_PUBLIC_TYPE rmw_event_s {
  /// 实现标识符，用于确保不混合使用两个不同的实现。 (Implementation identifier, used to ensure two
  /// different implementations are not being mixed.)
  const char* implementation_identifier;
  /// 来自发布者或订阅者的特定于此事件类型的数据。 (Data specific to this event type from either the
  /// publisher or subscriber.)
  void* data;
  /// 发生的事件类型。 (The event type that occurred.)
  rmw_event_type_t event_type;
} rmw_event_t;

/// 返回一个零初始化的事件结构。 (Return a zero initialized event structure.)
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_event_t rmw_get_zero_initialized_event(void);

/// 初始化 rmw 发布者事件。 (Initialize a rmw publisher event.)
/**
 * \param[inout] rmw_event 要初始化的事件 (Event to initialize)
 * \param[in] publisher 要与之一起初始化的发布者 (Publisher to initialize with)
 * \param[inout] event_type 要初始化的事件类型 (Event type for the event to initialize)
 * \return `RMW_RET_OK` 如果成功，或 (if successful, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果参数无效，或 (if invalid argument, or)
 * \return `RMW_RET_UNSUPPORTED` 如果不支持 event_type，或 (if event_type is not supported, or)
 * \return `RMW_RET_ERROR` 如果发生意外错误。 (if an unexpected error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_publisher_event_init(rmw_event_t* rmw_event,
                                   const rmw_publisher_t* publisher,
                                   rmw_event_type_t event_type);

/// 初始化一个 rmw 订阅事件 (Initialize a rmw subscription event)
/**
 * \param[inout] rmw_event 要初始化的 rmw_event (rmw_event to initialize)
 * \param[in] subscription 要与之关联的订阅 (subscription to initialize with)
 * \param[inout] event_type 事件要处理的事件类型 (event_type for the event to handle)
 * \return `RMW_RET_OK` 如果成功 (if successful), or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果参数无效 (if invalid argument), or
 * \return `RMW_RET_UNSUPPORTED` 如果不支持 event_type (if event_type is not supported), or
 * \return `RMW_RET_ERROR` 如果发生意外错误 (if an unexpected error occurs)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_subscription_event_init(rmw_event_t* rmw_event,
                                      const rmw_subscription_t* subscription,
                                      rmw_event_type_t event_type);

/// 从事件句柄中获取事件 (Take an event from the event handle)
/**
 * \param[in] event_handle 要获取的事件对象 (event object to take from)
 * \param[inout] event_info 用于写入获取到的数据的事件信息对象 (event info object to write taken
 * data into) \param[out] taken 指示是否获取到事件的布尔标志 (boolean flag indicating if an event
 * was taken or not) \return `RMW_RET_OK` 如果成功 (if successful), or \return `RMW_RET_BAD_ALLOC`
 * 如果内存分配失败 (if memory allocation failed), or \return `RMW_RET_ERROR` 如果发生意外错误 (if
 * an unexpected error occurs)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_take_event(const rmw_event_t* event_handle, void* event_info, bool* taken);

/// 结束一个 rmw_event_t (Finalize an rmw_event_t)
/**
 * \param[in] event 要结束的事件 (event to finalize)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_event_fini(rmw_event_t* event);

#ifdef __cplusplus
}
#endif

#endif  // RMW__EVENT_H_
