// Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#ifndef RMW__GET_TOPIC_ENDPOINT_INFO_H_
#define RMW__GET_TOPIC_ENDPOINT_INFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rmw/topic_endpoint_info_array.h"
#include "rmw/visibility_control.h"

/// 获取给定主题的每个已知发布者的端点信息。
/**
 * 此函数返回给定节点到目前为止发现的给定主题的每个发布者的端点信息数组。
 * 端点信息包括发布者的节点名称和命名空间、关联的主题类型、发布者的 gid 和发布者 QoS 配置文件。
 * 允许使用不存在的主题名称，在这种情况下将返回一个空数组。
 *
 * 根据正在使用的 RMW，发现可能是异步的。因此，创建一个发布者然后调用此 API
 * 可能不会立即显示新创建的发布者。
 *
 * \par 成功读取的 QoS
 * 返回的 QoS 配置文件可能有一些无效字段。
 * rmw 实现必须将无效字段设置为 `RMW_QOS_POLICY_*_UNKNOWN`。
 * 对于基于 DDS 的实现，唯一保证在发现过程中共享的 QoS 策略是参与端点匹配的策略。
 * 从当前可用的 QoS 设置中，基于 DDS 的实现未共享的唯一策略是 `history` 和 `history_depth`。
 *
 * <hr>
 * 属性          | 遵循
 * --------------|-------------
 * 分配内存       | 是
 * 线程安全       | 是
 * 使用原子操作   | 或许 [1]
 * 无锁           | 或许 [1]
 * <i>[1] rmw 实现定义，请检查实现文档</i>
 *
 * \par 运行时行为
 *   查询 ROS 图是一个同步操作。
 *   它也是非阻塞的，但不能保证是无锁的。
 *   一般来说，实现可以使用锁同步访问内部资源，但不允许等待没有保证时间限制的事件（不考虑由于操作系统调度引起的饥饿效应）。
 *
 * \par 线程安全性
 *   节点是线程安全对象，对它们进行的所有操作除了最终化都是线程安全的。
 *   因此，使用相同的节点并发查询 ROS 图是安全的。
 *   但是，在查询主题名称和类型时：
 *   - 对主题端点信息数组的访问没有同步。
 *     在 rmw_get_publishers_info_by_topic() 使用 `publishers_info` 时，
 *     读取或写入 `publishers_info` 是不安全的。
 *   - 对 C 风格字符串参数的访问是只读的，但没有同步。
 *     并发 `topic_name` 读取是安全的，但并发读写不安全。
 *   - 默认分配器是线程安全对象，但任何自定义 `allocator` 可能不是。
 *     请查阅您的分配器文档以获取更多参考。
 *
 * \pre 给定的 `node` 必须是有效的节点句柄，如 rmw_create_node() 返回的。
 * \pre 给定的 `publishers_info` 必须是端点信息的零初始化数组，
 *   如 rmw_get_zero_initialized_topic_endpoint_info_array() 返回的。
 *
 * \param[in] node 用于查询 ROS 图的节点。
 * \param[in] allocator 用于在填充 `publishers_info` 数组时使用的分配器。
 * \param[in] topic_name 发布者查找的主题名称，通常是完全限定的
 *   主题名称，但不一定（请参阅 rmw_create_publisher()）。
 * \param[in] no_mangle 在发布者查找之前是否对主题名称进行混淆。
 * \param[out] publishers_info 发布者信息数组，在成功时填充，
 *   在失败时保持不变。
 *   如果填充了，调用者需要稍后使用 rmw_topic_endpoint_info_array_fini() 对此数组进行最终处理。
 *   info 数组中的 QoS 配置文件将使用 RMW_DURATION_INFINITE 表示无限持续时间，
 *   避免暴露任何特定于实现的值。
 * \return `RMW_RET_OK` 如果查询成功，或者
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node` 为 NULL，或者
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `allocator` 无效，
 *   根据 rcutils_allocator_is_valid() 的定义，或者
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `topic_name` 为 NULL，或者
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `publishers_info` 为 NULL，或者
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `publishers_info` 不是
 *   零初始化数组，或者
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `node` 实现
 *   标识符与此实现不匹配，或者
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或者
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_get_publishers_info_by_topic(
    const rmw_node_t* node,
    rcutils_allocator_t* allocator,
    const char* topic_name,
    bool no_mangle,
    rmw_topic_endpoint_info_array_t* publishers_info);

/// 获取给定主题的每个已知订阅的端点信息。（Retrieve endpoint information for each known
/// subscription of a given topic.）
/**
 * 此函数返回给定节点迄今为止发现的给定主题的每个订阅的端点信息数组。
 * 端点信息包括订阅的节点名称和命名空间，关联的主题类型，订阅的 gid 和订阅 QoS 配置文件。
 * 允许使用不存在的主题名称，在这种情况下，将返回一个空数组。
 *
 * 根据所使用的 RMW，发现可能是异步的。因此，创建订阅然后调用此 API 可能不会立即显示新创建的订阅。
 *
 * \par 正确读取的 QoS（QoS that are correctly read）
 * 并非所有 QoS 都可以正确读取，请参阅 rmw_get_publishers_info_by_topic() 以获取更多详细信息。
 *
 * <hr>
 * 属性                    | 遵循
 * ------------------ | -------------
 * 分配内存              | 是
 * 线程安全              | 是
 * 使用原子              | 可能 [1]
 * 无锁                  | 可能 [1]
 * <i>[1] rmw 实现定义，检查实现文档</i>
 *
 * \par 运行时行为（Runtime behavior）
 *   查询 ROS 图是同步操作。
 *   它也是非阻塞的，但不能保证是无锁的。
 *   一般来说，实现可以使用锁同步访问内部资源，但不允许等待没有保证时间限制的事件（除了由于操作系统调度导致的饥饿效应）。
 *
 * \par 线程安全性（Thread-safety）
 *   节点是线程安全对象，因此对它们进行的所有操作（最终化除外）都是线程安全的。
 *   因此，使用相同的节点并发查询 ROS 图是安全的。
 *   但是，在查询订阅信息时：
 *   - 对主题端点信息数组的访问未同步。
 *     在 rmw_get_subscriptions_info_by_topic() 使用它时，读取或写入 `subscriptions_info`
 * 是不安全的。
 *   - 默认分配器是线程安全对象，但任何自定义 `allocator` 可能不是。
 *     请查阅您的分配器文档以获取更多参考。
 *
 * \pre 给定的 `node` 必须是有效的节点句柄，如 rmw_create_node() 返回的句柄。
 * \pre 给定的 `subscriptions_info` 必须是端点信息的零初始化数组，
 *   如 rmw_get_zero_initialized_topic_endpoint_info_array() 返回的数组。
 *
 * \param[in] node 查询 ROS 图的节点。（Node to query the ROS graph.）
 * \param[in] allocator 用于填充 `subscriptions_info` 数组的分配器。（Allocator to be used when
 * populating the `subscriptions_info` array.） \param[in] topic_name
 * 订阅查找的主题名称，通常是完全限定的主题名称，但不一定如此（请参阅
 * rmw_create_subscription()）。（Name of the topic for subscription lookup, often a fully qualified
 * topic name but not necessarily (see rmw_create_subscription()).） \param[in] no_mangle
 * 在订阅查找之前是否修改主题名称。（Whether to mangle the topic name before subscription lookup or
 * not.） \param[out] subscriptions_info 订阅信息数组，在成功时填充，在失败时保持不变。
 *   如果填充，则调用者稍后需要使用 rmw_topic_endpoint_info_array_fini() 对此数组进行最终处理。
 *   信息数组中的 QoS 配置文件将对无限持续时间使用 RMW_DURATION_INFINITE，
 *   避免暴露任何实现特定的值。
 * \return `RMW_RET_OK` 如果查询成功，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `allocator` 无效，
 *   按 rcutils_allocator_is_valid() 定义，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `topic_name` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `subscriptions_info` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `subscriptions_info` 不是
 *   零初始化数组，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `node` 实现
 *   标识符与此实现不匹配，或
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_get_subscriptions_info_by_topic(
    const rmw_node_t* node,
    rcutils_allocator_t* allocator,
    const char* topic_name,
    bool no_mangle,
    rmw_topic_endpoint_info_array_t* subscriptions_info);

#ifdef __cplusplus
}
#endif

#endif  // RMW__GET_TOPIC_ENDPOINT_INFO_H_
