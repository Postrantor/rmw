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

#include "rmw/subscription_options.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获取默认的订阅选项 (Get default subscription options)
 *
 * @return 返回一个 rmw_subscription_options_t 结构体，包含了默认的订阅选项 (Return an
 * rmw_subscription_options_t structure containing the default subscription options)
 */
rmw_subscription_options_t rmw_get_default_subscription_options(void) {
  // 定义并初始化一个 rmw_subscription_options_t 结构体变量 (Define and initialize an
  // rmw_subscription_options_t structure variable)
  rmw_subscription_options_t subscription_options = {
      .rmw_specific_subscription_payload =
          NULL,  // rmw 特定的订阅负载设置为 NULL (Set rmw specific subscription payload to NULL)
      .ignore_local_publications = false,  // 不忽略本地发布 (Do not ignore local publications)
      .require_unique_network_flow_endpoints =
          RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_NOT_REQUIRED,  // 不需要唯一的网络流端点 (Unique network
                                                           // flow endpoints not required)
      .content_filter_options =
          NULL,  // 内容过滤选项设置为 NULL (Set content filter options to NULL)
  };

  // 返回已经初始化的订阅选项 (Return the initialized subscription options)
  return subscription_options;
}

#ifdef __cplusplus
}
#endif
