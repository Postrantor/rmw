// Copyright 2020 Ericsson AB
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

#ifndef RMW__GET_NETWORK_FLOW_ENDPOINTS_H_
#define RMW__GET_NETWORK_FLOW_ENDPOINTS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rmw/network_flow_endpoint_array.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// 获取发布者的网络流端点 (Get network flow endpoints of a publisher)
/**
 * 查询给定发布者的底层中间件以获取网络流端点 (Query the underlying middleware for a given
 * publisher's network flow endpoints)
 *
 * \param[in] publisher 要检查的发布者实例 (the publisher instance to inspect)
 * \param[in] allocator 用于为 network_flow_endpoint_array_t 分配空间时使用的分配器 (allocator to be
 * used when allocating space for network_flow_endpoint_array_t) \param[out]
 * network_flow_endpoint_array 网络流端点 (the network flow endpoints) \return `RMW_RET_OK`
 * 如果成功，或 (if successful, or) \return `RMW_RET_INVALID_ARGUMENT` 如果任何参数为空，或 (if any
 * argument is null, or) \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或 (if memory allocation
 * fails, or) \return `RMW_RET_UNSUPPORTED` 如果不支持，或 (if not supported, or) \return
 * `RMW_RET_ERROR` 如果发生意外错误。 (if an unexpected error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_publisher_get_network_flow_endpoints(
    const rmw_publisher_t* publisher,
    rcutils_allocator_t* allocator,
    rmw_network_flow_endpoint_array_t* network_flow_endpoint_array);

/// 获取订阅的网络流端点 (Get network flow endpoints of a subscription)
/**
 * 查询给定订阅的底层中间件以获取网络流端点 (Query the underlying middleware for a given
 * subscription's network flow endpoints)
 *
 * \param[in] subscription 要检查的订阅实例 (the subscription instance to inspect)
 * \param[in] allocator 用于为 network_flow_endpoint_array_t 分配空间时使用的分配器 (allocator to be
 * used when allocating space for network_flow_endpoint_array_t) \param[out]
 * network_flow_endpoint_array 网络流端点 (the network flow endpoints) \return `RMW_RET_OK`
 * 如果成功，或 (if successful, or) \return `RMW_RET_INVALID_ARGUMENT` 如果任何参数为空，或 (if any
 * argument is null, or) \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或 (if memory allocation
 * fails, or) \return `RMW_RET_UNSUPPORTED` 如果不支持，或 (if not supported, or) \return
 * `RMW_RET_ERROR` 如果发生意外错误。 (if an unexpected error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_subscription_get_network_flow_endpoints(
    const rmw_subscription_t* subscription,
    rcutils_allocator_t* allocator,
    rmw_network_flow_endpoint_array_t* network_flow_endpoint_array);

#ifdef __cplusplus
}
#endif

#endif  // RMW__GET_NETWORK_FLOW_ENDPOINTS_H_
