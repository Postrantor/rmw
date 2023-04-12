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

#ifndef RMW__NETWORK_FLOW_ENDPOINT_H_
#define RMW__NETWORK_FLOW_ENDPOINT_H_

#if __cplusplus
extern "C" {
#endif

#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// 传输协议类型 (Transport protocol types)
typedef enum rmw_transport_protocol_e {
  // 未知传输协议 (Unknown transport protocol)
  RMW_TRANSPORT_PROTOCOL_UNKNOWN = 0,
  // UDP 传输协议 (UDP transport protocol)
  RMW_TRANSPORT_PROTOCOL_UDP,
  // TCP 传输协议 (TCP transport protocol)
  RMW_TRANSPORT_PROTOCOL_TCP,
  // 传输协议计数 (Transport protocol count)
  RMW_TRANSPORT_PROTOCOL_COUNT
} rmw_transport_protocol_t;

/// 互联网协议类型 (Internet protocol types)
typedef enum rmw_internet_protocol_e {
  // 未知互联网协议 (Unknown internet protocol)
  RMW_INTERNET_PROTOCOL_UNKNOWN = 0,
  // IPv4 互联网协议 (IPv4 internet protocol)
  RMW_INTERNET_PROTOCOL_IPV4,
  // IPv6 互联网协议 (IPv6 internet protocol)
  RMW_INTERNET_PROTOCOL_IPV6,
  // 互联网协议计数 (Internet protocol count)
  RMW_INTERNET_PROTOCOL_COUNT
} rmw_internet_protocol_t;

/// 包括终止空的互联网地址字符串的最大长度 (Maximum length of internet address string including
/// terminating null) 受到 linux/inet.h 的启发 (Inspired from linux/inet.h)
#define RMW_INET_ADDRSTRLEN 48

/// 描述发布者或订阅者网络流端点的结构 (Structure that describes network flow endpoint of a
/// publisher or subscription)
typedef struct RMW_PUBLIC_TYPE rmw_network_flow_endpoint_s {
  // 传输协议 (Transport protocol)
  rmw_transport_protocol_t transport_protocol;
  // 互联网协议 (Internet protocol)
  rmw_internet_protocol_t internet_protocol;
  // 端口 (Port)
  uint16_t transport_port;
  // 流标签 (Flow label)
  // TODO(anamud): 考虑专门化，因为流标签只在发布者端设置 (Consider specializing since flow_label is
  // set only at publisher side.)
  uint32_t flow_label;
  // DSCP (Diff. Services Code Point)
  // TODO(anamud): 考虑专门化，因为 DSCP 只在发布者端设置 (Consider specializing since DSCP is set
  // only at publisher side.)
  uint8_t dscp;
  // 互联网地址 (Internet address)
  char internet_address[RMW_INET_ADDRSTRLEN];
} rmw_network_flow_endpoint_t;

/// 返回具有零初始化成员的 rmw_network_flow_endpoint_t 结构 (Return a rmw_network_flow_endpoint_t
/// struct with zero-initialized members)
RMW_PUBLIC
rmw_network_flow_endpoint_t rmw_get_zero_initialized_network_flow_endpoint(void);

/// 转换为传输协议的字符串变体 (Convert to string variant of transport protocol)
/**
 * \param[in] transport_protocol 要转换的 transport_protocol_t (transport_protocol_t to be
 * converted)
 */
RMW_PUBLIC
const char* rmw_network_flow_endpoint_get_transport_protocol_string(
    const rmw_transport_protocol_t transport_protocol);

/// 转换为互联网协议的字符串变体 (Convert to string variant of internet protocol)
/**
 * \param[in] internet_protocol 要转换的 internet_protocol_t (internet_protocol_t to be converted)
 */
RMW_PUBLIC
const char* rmw_network_flow_endpoint_get_internet_protocol_string(
    const rmw_internet_protocol_t internet_protocol);

/// 在 rmw_network_flow_endpoint_t 结构中设置互联网地址 (Set internet address in the
/// rmw_network_flow_endpoint_t struct)
/**
 * \param[in] network_flow_endpoint 要初始化的 network_flow_endpoint_t (network_flow_endpoint_t to
 * be initialized) \param[in] internet_address 作为 C 风格字符串的互联网地址 (the internet address
 * as C-style string) \param[in] size 不包括终止空的 internet_address 的大小 (size of
 * internet_address excluding terminating null) \returns 成功初始化时返回 `RMW_RET_OK`，或 (returns
 * `RMW_RET_OK` on successfull initilization, or) \returns 如果 `network_flow_endpoint` 为
 * NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或 (returns `RMW_RET_INVALID_ARGUMENT` if
 * `network_flow_endpoint` is NULL, or) \returns 如果 `internet_address` 为 NULL，则返回
 * `RMW_RET_INVALID_ARGUMENT`，或 (returns `RMW_RET_INVALID_ARGUMENT` if `internet_address` is NULL,
 * or) \returns 如果 `size` 不小于 RMW_INET_ADDRSTRLEN，则返回 `RMW_RET_INVALID_ARGUMENT`，或
 * (returns `RMW_RET_INVALID_ARGUMENT` if `size` is not less than RMW_INET_ADDRSTRLEN, or) \returns
 * 当发生未指定的错误时返回 `RMW_RET_ERROR`。 (returns `RMW_RET_ERROR` when an unspecified error
 * occurs.)
 */
RMW_PUBLIC
rmw_ret_t rmw_network_flow_endpoint_set_internet_address(
    rmw_network_flow_endpoint_t* network_flow_endpoint, const char* internet_address, size_t size);

#if __cplusplus
}
#endif

#endif  // RMW__NETWORK_FLOW_ENDPOINT_H_
