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

#ifndef RMW__DYNAMIC_MESSAGE_TYPE_SUPPORT_H_
#define RMW__DYNAMIC_MESSAGE_TYPE_SUPPORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <rcutils/allocator.h>
#include <rosidl_dynamic_typesupport/api/serialization_support.h>
#include <rosidl_dynamic_typesupport/dynamic_message_type_support_struct.h>
#include <rosidl_dynamic_typesupport/identifier.h>
#include <rosidl_dynamic_typesupport/types.h>
#include <rosidl_runtime_c/message_type_support_struct.h>
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#include <rosidl_runtime_c/type_description/type_source__struct.h>

#include "rmw/features.h"
#include "rmw/serialized_message.h"
#include "rmw/visibility_control.h"

/// Interfaces for runtime interface reflection (运行时接口反射的接口)

// 函数：rmw_take_dynamic_message
// 功能：从订阅中获取动态消息
// Function: rmw_take_dynamic_message
// Purpose: Take a dynamic message from the subscription
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_take_dynamic_message(
    const rmw_subscription_t* subscription,  ///< [in] 订阅对象指针 (Pointer to subscription object)
    rosidl_dynamic_typesupport_dynamic_data_t*
        dynamic_message,  ///< [out] 动态消息数据指针 (Pointer to dynamic message data)
    bool* taken,  ///< [out] 消息是否被成功获取 (Whether the message was successfully taken)
    rmw_subscription_allocation_t*
        allocation);  ///< [in,out] 订阅分配信息 (Subscription allocation info)

// 函数：rmw_take_dynamic_message_with_info
// 功能：从订阅中获取动态消息及其相关信息
// Function: rmw_take_dynamic_message_with_info
// Purpose: Take a dynamic message and its related information from the subscription
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_take_dynamic_message_with_info(
    const rmw_subscription_t* subscription,  ///< [in] 订阅对象指针 (Pointer to subscription object)
    rosidl_dynamic_typesupport_dynamic_data_t*
        dynamic_message,  ///< [out] 动态消息数据指针 (Pointer to dynamic message data)
    bool* taken,  ///< [out] 消息是否被成功获取 (Whether the message was successfully taken)
    rmw_message_info_t* message_info,  ///< [out] 消息相关信息 (Message-related information)
    rmw_subscription_allocation_t*
        allocation);  ///< [in,out] 订阅分配信息 (Subscription allocation info)

// 函数：rmw_serialization_support_init
// 功能：初始化序列化支持
// Function: rmw_serialization_support_init
// Purpose: Initialize serialization support
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_serialization_support_init(
    const char* serialization_lib_name,  ///< [in] 序列化库名称 (Serialization library name)
    rcutils_allocator_t* allocator,      ///< [in] 分配器指针 (Pointer to allocator)
    rosidl_dynamic_typesupport_serialization_support_t*
        serialization_support);  ///< [out] 序列化支持对象指针 (Pointer to serialization support
                                 ///< object)

// TODO(methylDragon): Nice to have only
// RMW_PUBLIC
// RMW_WARN_UNUSED
// rmw_ret_t
// rmw_get_dynamic_type_from_middleware(
//   const rmw_node_t * node,
//   const char * topic_name,
//   const rosidl_message_type_support_t type_support,
//   rosidl_dynamic_typesupport_dynamic_type_t * dynamic_message_type);
//
// RMW_PUBLIC
// RMW_WARN_UNUSED
// rmw_ret_t
// rmw_get_dynamic_data_from_middleware(
//   const rmw_node_t * node,
//   const char * topic_name,
//   const rosidl_message_type_support_t type_support,
//   rosidl_dynamic_typesupport_dynamic_data_t * dynamic_message);

#ifdef __cplusplus
}
#endif

#endif  // RMW__DYNAMIC_MESSAGE_TYPE_SUPPORT_H_
