// Copyright 2015 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__QOS_PROFILES_H_
#define RMW__QOS_PROFILES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rmw/types.h"

/**
 * @brief 传感器数据的 QoS 配置 (QoS configuration for sensor data)
 * @param history 历史记录策略 (History policy)
 * @param depth 历史记录深度 (History depth)
 * @param reliability 可靠性策略 (Reliability policy)
 * @param durability 持久性策略 (Durability policy)
 * @param deadline Deadline 默认值 (Deadline default value)
 * @param lifespan 生命周期默认值 (Lifespan default value)
 * @param liveliness 活跃度策略 (Liveliness policy)
 * @param lease_duration 租约时长默认值 (Lease duration default value)
 * @param avoid_ros_namespace_conventions 是否避免 ROS 命名空间约定 (Whether to avoid ROS namespace
 * conventions)
 */
static const rmw_qos_profile_t rmw_qos_profile_sensor_data = {
    RMW_QOS_POLICY_HISTORY_KEEP_LAST,
    5,
    RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT,
    RMW_QOS_POLICY_DURABILITY_VOLATILE,
    RMW_QOS_DEADLINE_DEFAULT,
    RMW_QOS_LIFESPAN_DEFAULT,
    RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,
    RMW_QOS_LIVELINESS_LEASE_DURATION_DEFAULT,
    false};

/**
 * @brief 参数 QoS 配置 (QoS configuration for parameters)
 * @param history 历史记录策略 (History policy)
 * @param depth 历史记录深度 (History depth)
 * @param reliability 可靠性策略 (Reliability policy)
 * @param durability 持久性策略 (Durability policy)
 * @param deadline Deadline 默认值 (Deadline default value)
 * @param lifespan 生命周期默认值 (Lifespan default value)
 * @param liveliness 活跃度策略 (Liveliness policy)
 * @param lease_duration 租约时长默认值 (Lease duration default value)
 * @param avoid_ros_namespace_conventions 是否避免 ROS 命名空间约定 (Whether to avoid ROS namespace
 * conventions)
 */
static const rmw_qos_profile_t rmw_qos_profile_parameters = {
    RMW_QOS_POLICY_HISTORY_KEEP_LAST,
    1000,
    RMW_QOS_POLICY_RELIABILITY_RELIABLE,
    RMW_QOS_POLICY_DURABILITY_VOLATILE,
    RMW_QOS_DEADLINE_DEFAULT,
    RMW_QOS_LIFESPAN_DEFAULT,
    RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,
    RMW_QOS_LIVELINESS_LEASE_DURATION_DEFAULT,
    false};

/// 默认的 rmw_qos_profile_t 结构体配置，用于设置 ROS2 通信的 QoS (Quality of Service) 参数。
static const rmw_qos_profile_t rmw_qos_profile_default = {
    RMW_QOS_POLICY_HISTORY_KEEP_LAST,
    10,
    RMW_QOS_POLICY_RELIABILITY_RELIABLE,
    RMW_QOS_POLICY_DURABILITY_VOLATILE,
    RMW_QOS_DEADLINE_DEFAULT,
    RMW_QOS_LIFESPAN_DEFAULT,
    RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,
    RMW_QOS_LIVELINESS_LEASE_DURATION_DEFAULT,
    false};

/// 默认的服务 QoS 配置，用于设置 ROS2 服务通信的 QoS 参数。
static const rmw_qos_profile_t rmw_qos_profile_services_default = {
    RMW_QOS_POLICY_HISTORY_KEEP_LAST,
    10,
    RMW_QOS_POLICY_RELIABILITY_RELIABLE,
    RMW_QOS_POLICY_DURABILITY_VOLATILE,
    RMW_QOS_DEADLINE_DEFAULT,
    RMW_QOS_LIFESPAN_DEFAULT,
    RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,
    RMW_QOS_LIVELINESS_LEASE_DURATION_DEFAULT,
    false};

/// 参数事件 QoS 配置，用于设置 ROS2 参数事件通信的 QoS 参数。
static const rmw_qos_profile_t rmw_qos_profile_parameter_events = {
    RMW_QOS_POLICY_HISTORY_KEEP_LAST,
    1000,
    RMW_QOS_POLICY_RELIABILITY_RELIABLE,
    RMW_QOS_POLICY_DURABILITY_VOLATILE,
    RMW_QOS_DEADLINE_DEFAULT,
    RMW_QOS_LIFESPAN_DEFAULT,
    RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,
    RMW_QOS_LIVELINESS_LEASE_DURATION_DEFAULT,
    false};

static const rmw_qos_profile_t rmw_qos_profile_system_default = {
    RMW_QOS_POLICY_HISTORY_SYSTEM_DEFAULT,
    RMW_QOS_POLICY_DEPTH_SYSTEM_DEFAULT,
    RMW_QOS_POLICY_RELIABILITY_SYSTEM_DEFAULT,
    RMW_QOS_POLICY_DURABILITY_SYSTEM_DEFAULT,
    RMW_QOS_DEADLINE_DEFAULT,
    RMW_QOS_LIFESPAN_DEFAULT,
    RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,
    RMW_QOS_LIVELINESS_LEASE_DURATION_DEFAULT,
    false};

/// 与当前大多数可用端点匹配，同时保持最高服务水平
/**
 * 在创建订阅或发布器时选择可靠性、持久性、截止日期、活跃度和活跃度租期策略。
 *
 * 创建端点后，可以使用 `rmw_get_subscriptions_info_by_topic` 或 `rmw_get_publishers_info_by_topic`
 * 获取实际的 QoS 策略。
 *
 * 即使一个或多个策略与新发现的端点不兼容，中间件也不会在创建订阅或发布器后更新策略。
 */
static const rmw_qos_profile_t rmw_qos_profile_best_available = {
    RMW_QOS_POLICY_HISTORY_KEEP_LAST,
    10,
    RMW_QOS_POLICY_RELIABILITY_BEST_AVAILABLE,
    RMW_QOS_POLICY_DURABILITY_BEST_AVAILABLE,
    RMW_QOS_DEADLINE_BEST_AVAILABLE,
    RMW_QOS_LIFESPAN_DEFAULT,
    RMW_QOS_POLICY_LIVELINESS_BEST_AVAILABLE,
    RMW_QOS_LIVELINESS_LEASE_DURATION_BEST_AVAILABLE,
    false};

static const rmw_qos_profile_t rmw_qos_profile_unknown = {
    RMW_QOS_POLICY_HISTORY_UNKNOWN,
    RMW_QOS_POLICY_DEPTH_SYSTEM_DEFAULT,
    RMW_QOS_POLICY_RELIABILITY_UNKNOWN,
    RMW_QOS_POLICY_DURABILITY_UNKNOWN,
    RMW_QOS_DEADLINE_DEFAULT,
    RMW_QOS_LIFESPAN_DEFAULT,
    RMW_QOS_POLICY_LIVELINESS_UNKNOWN,
    RMW_QOS_LIVELINESS_LEASE_DURATION_DEFAULT,
    false};

/// 定义 QoS 兼容性类型枚举 (Define the QoS compatibility type enumeration)
typedef enum RMW_PUBLIC_TYPE rmw_qos_compatibility_type_e {
  RMW_QOS_COMPATIBILITY_OK = 0,
  RMW_QOS_COMPATIBILITY_WARNING,
  RMW_QOS_COMPATIBILITY_ERROR
} rmw_qos_compatibility_type_t;

/**
 * @brief 检查两个 QoS 配置文件是否兼容 (Check if two QoS profiles are compatible)
 *
 * 如果使用 QoS 策略的发布者和订阅者可以相互通信，则两个 QoS 配置文件是兼容的。
 *
 * 如果任何配置文件策略的值为 "system default" 或 "unknown"，则可能无法确定兼容性。
 * 在这种情况下，输出参数 `compatibility` 被设置为 `RMW_QOS_COMPATIBILITY_WARNING`，并填充
 * `reason`。
 *
 * 如果出现兼容性警告或错误，并为 `reason`
 * 提供缓冲区，则所有警告和错误的解释将填充到缓冲区中，用分号 (`;`) 分隔。
 * 错误将在字符串缓冲区中的警告之前出现。
 * 如果提供的缓冲区不够大，此函数仍会向缓冲区写入，最多达到 `reason_size` 个字符。
 * 因此，如果达到缓冲区大小限制，则可能无法传达所有错误和警告。
 * 缓冲区大小为 2048 应该足以捕获所有可能的错误和警告。
 *
 * @param[in] publisher_profile 发布者使用的 QoS 配置文件 (The QoS profile used for a publisher)
 * @param[in] subscription_profile 订阅者使用的 QoS 配置文件 (The QoS profile used for a
 * subscription)
 * @param[out] compatibility 如果 QoS 配置文件兼容，则为 `RMW_QOS_COMPATIBILITY_OK`，如果 QoS
 * 配置文件可能兼容，则为 `RMW_QOS_COMPATIBILITY_WARNING`，如果 QoS 配置文件不兼容，则为
 * `RMW_QOS_COMPATIBILITY_ERROR`
 * @param[out] reason QoS 不兼容或潜在不兼容的详细原因，必须由调用者预先分配
 * @param[in] reason_size 如果提供了 `reason` 字符串缓冲区，则为其大小
 * @return `RMW_RET_OK` 如果检查成功 (if the check was successful), or
 * @return `RMW_RET_INVALID_ARGUMENT` 如果 `compatiblity` 为 `nullptr` (if `compatiblity` is
 * `nullptr`), or
 * @return `RMW_RET_INVALID_ARGUMENT` 如果 `reason` 为 `NULL` 且 `reason_size` 不为零 (if `reason`
 * is `NULL` and  `reason_size` is not zero), or
 * @return `RMW_RET_ERROR` 如果出现意外错误 (if there is an unexpected error)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_qos_profile_check_compatible(
    const rmw_qos_profile_t publisher_profile,
    const rmw_qos_profile_t subscription_profile,
    rmw_qos_compatibility_type_t* compatibility,
    char* reason,
    size_t reason_size);

#ifdef __cplusplus
}
#endif

#endif  // RMW__QOS_PROFILES_H_
