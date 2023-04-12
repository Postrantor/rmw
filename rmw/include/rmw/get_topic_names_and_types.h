// Copyright 2017 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__GET_TOPIC_NAMES_AND_TYPES_H_
#define RMW__GET_TOPIC_NAMES_AND_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rmw/macros.h"
#include "rmw/names_and_types.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// 返回 ROS 图中的所有主题名称和类型。
/**
 * 此函数返回 ROS 图中的所有主题名称和类型数组
 * 即存在发布者和/或订阅者，到目前为止由给定的本地节点发现。
 *
 * 除非 `no_demangle` 为 true，否则在列出主题时可能会进行一些去重和过滤，
 * 这是由中间件实现的。
 * 是否应用去重以及如何应用取决于底层实现。
 * 请参阅 http://design.ros2.org/articles/topic_and_service_names.html 查看一个示例，
 * 了解它在基于 DDS 和 RTPS 的实现中是如何使用的。
 *
 * <hr>
 * 属性                 | 遵循性
 * ------------------ | -------------
 * 分配内存             | 是
 * 线程安全             | 是
 * 使用原子操作          | 可能 [1]
 * 无锁                 | 可能 [1]
 * <i>[1] rmw 实现定义，查看实现文档</i>
 *
 * \par 运行时行为
 *   查询 ROS 图是一个同步操作。
 *   它也是非阻塞性的，但不能保证是无锁的。
 *   一般来说，实现可以使用锁来同步访问内部资源，但不允许等待没有保证时间界限的事件（不考虑由于操作系统调度引起的饥饿效应）。
 *
 * \par 线程安全性
 *   节点是线程安全对象，因此除了最终化之外，对它们的所有操作都是线程安全的。
 *   因此，可以使用相同的节点并发查询 ROS 图。
 *   但是，在查询主题名称和类型时：
 *   - 对名称和类型数组的访问不是同步的。
 *     在 rmw_get_topic_names_and_types() 使用 `topic_names_and_types` 时，
 *     读取或写入它是不安全的。
 *   - 默认分配器是线程安全对象，但任何自定义 `allocator` 可能不是。
 *     请查阅您的分配器文档以获取更多参考信息。
 *
 * \pre 给定的 `node` 必须是有效的节点句柄，由 rmw_create_node() 返回。
 * \pre 给定的 `topic_names_and_types` 必须是零初始化的名称和类型数组，
 *   由 rmw_get_zero_initialized_names_and_types() 返回。
 *
 * \param[in] node 查询 ROS 图的节点。
 * \param[in] allocator 在填充 `topic_names_and_types` 数组时要使用的分配器。
 * \param[in] no_demangle 是否按照 ROS 规范对所有主题名称进行去重。
 * \param[out] topic_names_and_types 主题名称及其类型的数组，
 *   成功时填充，但失败时保持不变。
 *   如果填充，调用者需要稍后使用 rmw_names_and_types_fini() 来完成此数组。
 * \return `RMW_RET_OK` 如果查询成功，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `allocator` 不是有效的，根据 rcutils_allocator_is_valid()
 * 的定义，或 \return `RMW_RET_INVALID_ARGUMENT` 如果 `topic_names_and_types` 为 NULL，或 \return
 * `RMW_RET_INVALID_ARGUMENT` 如果 `topic_names_and_types` 不是 零初始化数组，或 \return
 * `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `node` 实现 标识符与此实现不匹配，或 \return
 * `RMW_RET_BAD_ALLOC` 如果内存分配失败，或 \return `RMW_RET_ERROR` 如果发生未指定的错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_get_topic_names_and_types(const rmw_node_t* node,
                                        rcutils_allocator_t* allocator,
                                        bool no_demangle,
                                        rmw_names_and_types_t* topic_names_and_types);

#ifdef __cplusplus
}
#endif

#endif  // RMW__GET_TOPIC_NAMES_AND_TYPES_H_
