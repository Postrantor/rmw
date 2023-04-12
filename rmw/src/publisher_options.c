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

#include "rmw/publisher_options.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获取默认的发布者选项 (Get the default publisher options)
 *
 * @return rmw_publisher_options_t 默认发布者选项结构体 (Default publisher options structure)
 */
rmw_publisher_options_t rmw_get_default_publisher_options(void) {
  // 初始化一个 rmw_publisher_options_t 结构体，用于存储默认发布者选项
  // Initialize an rmw_publisher_options_t structure to store the default publisher options
  rmw_publisher_options_t publisher_options = {
      // 将 rmw_specific_publisher_payload 设置为 NULL，表示没有特定于 RMW 的数据载荷
      // Set rmw_specific_publisher_payload to NULL, indicating no RMW-specific data payload
      .rmw_specific_publisher_payload = NULL,

      // 设置 require_unique_network_flow_endpoints 为
      // RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_NOT_REQUIRED，
      // 表示不需要唯一的网络流端点
      // Set require_unique_network_flow_endpoints to
      // RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_NOT_REQUIRED,
      // indicating that unique network flow endpoints are not required
      .require_unique_network_flow_endpoints = RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_NOT_REQUIRED,
  };

  // 返回初始化完成的默认发布者选项结构体
  // Return the initialized default publisher options structure
  return publisher_options;
}

#ifdef __cplusplus
}
#endif
