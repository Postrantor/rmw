// Copyright 2021 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__EVENT_CALLBACK_TYPE_H_
#define RMW__EVENT_CALLBACK_TYPE_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/// 公共事件回调类型签名。
/// Common event callback type signature.
/**
 * 该类型的事件回调可以在各种场景中被调用，例如：
 * 订阅上有数据可用、QoS事件发生或类似情况。
 * Event callbacks of this type can be called in various scenarios, e.g.
 * data becomes available on a subscription, a QoS event has occurred, or
 * something similar.
 *
 * user_data 参数由用户在注册回调时给出，
 * 并在每次回调时返回，以便它可以具有关联的用户定义状态。
 * The user_data argument is given by the user when registering the callback,
 * and is given back to the callback each time so it can have associated,
 * user-defined state.
 *
 * number_of_events 参数表示自回调被调用以来的事件数。
 * 这通常是1，但当事件在回调注册之前发生时，可以是 > 1。
 * 它永远不应该为0。
 * The number_of_events argument indicates the number of events since the
 * callback was called.
 * This is most often 1, but can be > 1 when events occur before the callback
 * is registered.
 * It should never be 0.
 *
 * \sa rmw_subscription_set_on_new_message_callback()
 * \sa rmw_service_set_on_new_request_callback()
 * \sa rmw_client_set_on_new_response_callback()
 * \sa rmw_event_set_callback()
 *
 * @param[in] user_data 用户提供的数据，用于关联用户定义的状态。
 *                      User-provided data for associating user-defined state.
 * @param[in] number_of_events 自回调被调用以来的事件数。
 *                             The number of events since the callback was called.
 */
typedef void (*rmw_event_callback_t)(const void* user_data, size_t number_of_events);

#ifdef __cplusplus
}
#endif

#endif  // RMW__EVENT_CALLBACK_TYPE_H_
