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

#include "rmw/network_flow_endpoint.h"

#include <string.h>

#include "rcutils/snprintf.h"
#include "rmw/error_handling.h"

/**
 * @brief 获取一个零初始化的网络流端点对象 (Get a zero-initialized network flow endpoint object)
 *
 * @return 返回一个零初始化的 rmw_network_flow_endpoint_t 结构体 (Returns a zero-initialized
 * rmw_network_flow_endpoint_t structure)
 */
rmw_network_flow_endpoint_t rmw_get_zero_initialized_network_flow_endpoint(void) {
  // 创建一个零初始化的 rmw_network_flow_endpoint_t 结构体并返回 (Create a zero-initialized
  // rmw_network_flow_endpoint_t structure and return it)
  rmw_network_flow_endpoint_t network_flow_endpoint = {0};
  return network_flow_endpoint;
}

/**
 * @brief 根据传输协议枚举值获取对应的字符串表示 (Get the corresponding string representation based
 * on the transport protocol enumeration value)
 *
 * @param transport_protocol 传输协议枚举值 (Transport protocol enumeration value)
 * @return 对应的传输协议字符串 (Corresponding transport protocol string)
 */
const char* rmw_network_flow_endpoint_get_transport_protocol_string(
    const rmw_transport_protocol_t transport_protocol) {
  // 定义传输协议字符串数组 (Define transport protocol string array)
  const char* transport_protocol_str[RMW_TRANSPORT_PROTOCOL_COUNT] = {
      [RMW_TRANSPORT_PROTOCOL_UNKNOWN] = "Unknown",
      [RMW_TRANSPORT_PROTOCOL_UDP] = "UDP",
      [RMW_TRANSPORT_PROTOCOL_TCP] = "TCP",
  };
  // 如果传入的传输协议枚举值超出范围，则返回未知传输协议字符串 (If the input transport protocol
  // enumeration value is out of range, return the unknown transport protocol string)
  if (transport_protocol >= RMW_TRANSPORT_PROTOCOL_COUNT) {
    return transport_protocol_str[RMW_TRANSPORT_PROTOCOL_UNKNOWN];
  }
  // 返回对应的传输协议字符串 (Return the corresponding transport protocol string)
  return transport_protocol_str[transport_protocol];
}

/**
 * @brief 根据互联网协议枚举值获取对应的字符串表示 (Get the corresponding string representation
 * based on the Internet protocol enumeration value)
 *
 * @param internet_protocol 互联网协议枚举值 (Internet protocol enumeration value)
 * @return 对应的互联网协议字符串 (Corresponding Internet protocol string)
 */
const char* rmw_network_flow_endpoint_get_internet_protocol_string(
    const rmw_internet_protocol_t internet_protocol) {
  // 定义互联网协议字符串数组 (Define Internet protocol string array)
  const char* internet_protocol_str[RMW_INTERNET_PROTOCOL_COUNT] = {
      [RMW_INTERNET_PROTOCOL_UNKNOWN] = "Unknown",
      [RMW_INTERNET_PROTOCOL_IPV4] = "IPv4",
      [RMW_INTERNET_PROTOCOL_IPV6] = "IPv6",
  };
  // 如果传入的互联网协议枚举值超出范围，则返回未知互联网协议字符串 (If the input Internet protocol
  // enumeration value is out of range, return the unknown Internet protocol string)
  if (internet_protocol >= RMW_INTERNET_PROTOCOL_COUNT) {
    return internet_protocol_str[RMW_INTERNET_PROTOCOL_UNKNOWN];
  }
  // 返回对应的互联网协议字符串 (Return the corresponding Internet protocol string)
  return internet_protocol_str[internet_protocol];
}

/**
 * @brief 设置网络流端点的互联网地址 (Set the Internet address of the network flow endpoint)
 *
 * @param network_flow_endpoint 指向网络流端点的指针 (Pointer to the network flow endpoint)
 * @param internet_address 要设置的互联网地址字符串 (The Internet address string to set)
 * @param size 互联网地址字符串的长度 (The length of the Internet address string)
 * @return 设置结果，成功返回 RMW_RET_OK，失败返回相应错误码 (Setting result, success returns
 * RMW_RET_OK, failure returns the corresponding error code)
 */
rmw_ret_t rmw_network_flow_endpoint_set_internet_address(
    rmw_network_flow_endpoint_t* network_flow_endpoint, const char* internet_address, size_t size) {
  // 检查传入的网络流端点指针是否为空 (Check if the input network flow endpoint pointer is null)
  if (!network_flow_endpoint) {
    RMW_SET_ERROR_MSG("network_flow_endpoint is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 检查传入的互联网地址字符串是否为空 (Check if the input Internet address string is null)
  if (!internet_address) {
    RMW_SET_ERROR_MSG("internet_address is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 检查传入的互联网地址字符串长度是否超出范围 (Check if the input Internet address string length
  // is out of range)
  if (size >= RMW_INET_ADDRSTRLEN) {
    RMW_SET_ERROR_MSG("size is not less than RMW_INET_ADDRSTRLEN");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 使用 rcutils_snprintf() 函数将传入的互联网地址字符串复制到网络流端点的互联网地址字段中 (Use the
  // rcutils_snprintf() function to copy the input Internet address string to the Internet address
  // field of the network flow endpoint)
  int ret = rcutils_snprintf(network_flow_endpoint->internet_address, RMW_INET_ADDRSTRLEN, "%s",
                             internet_address);
  // 判断复制操作是否成功 (Determine if the copy operation was successful)
  if (ret < 0) {
    RMW_SET_ERROR_MSG("Copying internet_address using rcutils_snprintf() failed");
    return RMW_RET_ERROR;
  }
  // 返回操作成功的结果码 (Return the result code for a successful operation)
  return RMW_RET_OK;
}
