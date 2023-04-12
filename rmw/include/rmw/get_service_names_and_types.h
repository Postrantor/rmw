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

#ifndef RMW__GET_SERVICE_NAMES_AND_TYPES_H_
#define RMW__GET_SERVICE_NAMES_AND_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rmw/macros.h"
#include "rmw/names_and_types.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// 返回 ROS 图中的所有服务名称和类型。
/// Return all service names and types in the ROS graph.
/**
 * 此函数返回 ROS 图中的所有服务名称和类型数组
 * 即，对于存在服务器和/或客户端的服务，按给定的本地节点发现顺序。
 * This function returns an array of all service names and types in the ROS graph
 * i.e. for which a server and/or client exists, as discovered so far by the given
 * local node.
 *
 * <hr>
 * 属性                   | 遵循
 * Attribute              | Adherence
 * ---------------------- | -------------
 * 分配内存               | 是
 * Allocates Memory       | Yes
 * 线程安全               | 是
 * Thread-Safe            | Yes
 * 使用原子操作           | 可能 [1]
 * Uses Atomics           | Maybe [1]
 * 无锁                   | 可能 [1]
 * Lock-Free              | Maybe [1]
 * <i>[1] rmw 实现定义，查看实现文档</i>
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \par 运行时行为
 *   查询 ROS 图是一个同步操作。
 *   它也是非阻塞的，但不能保证是无锁的。
 *   一般来说，实现可以使用锁同步访问内部资源，但不允许等待没有保证时间上限的事件（由于操作系统调度造成的饥饿除外）。
 * \par Runtime behavior
 *   To query the ROS graph is a synchronous operation.
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par 线程安全性
 *   节点是线程安全的对象，除了最终操作以外，对它们的所有操作都是线程安全的。
 *   因此，使用相同的节点并发查询 ROS 图是安全的。
 *   但是，在查询服务名称和类型时：
 *   - 对名称和类型数组的访问不是同步的。
 *     在 rmw_get_service_names_and_types() 使用它时，读取或写入 `service_names_and_types`
 * 是不安全的。
 *   - 默认分配器是线程安全的对象，但任何自定义的 `allocator` 可能不是。
 *     请查阅您的分配器文档以获取更多参考信息。
 * \par Thread-safety
 *   Nodes are thread-safe objects, and so are all operations on them except for finalization.
 *   Therefore, it is safe to query the ROS graph using the same node concurrently.
 *   However, when querying services names and types:
 *   - Access to the array of names and types is not synchronized.
 *     It is not safe to read or write `service_names_and_types`
 *     while rmw_get_service_names_and_types() uses it.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.
 *
 * \pre 给定的 `node` 必须是一个有效的节点句柄，由 rmw_create_node() 返回。
 * \pre Given `node` must be a valid node handle, as returned by rmw_create_node().
 * \pre 给定的 `services_names_and_types` 必须是一个零初始化的名称和类型数组，
 *      由 rmw_get_zero_initialized_names_and_types() 返回。
 * \pre Given `services_names_and_types` must be a zero-initialized array of names and types,
 *   as returned by rmw_get_zero_initialized_names_and_types().
 *
 * \param[in] node 查询 ROS 图的节点。
 * \param[in] node Node to query the ROS graph.
 * \param[in] allocator 用于填充 `service_names_and_types` 数组的分配器。
 * \param[in] allocator Allocator to be used when populating the `service_names_and_types` array.
 * \param[out] service_names_and_types 服务名称及其类型的数组，在成功时填充，但在失败时保持不变。
 *             如果填充了该数组，则调用者需要稍后使用 rmw_names_and_types_fini()
 * 对该数组进行最终处理。 \param[out] service_names_and_types Array of service names and their
 * types, populated on success but left unchanged on failure. If populated, it is up to the caller
 * to finalize this array later on using rmw_names_and_types_fini(). \return `RMW_RET_OK`
 * 如果查询成功，或 \return `RMW_RET_INVALID_ARGUMENT` 如果 `node` 为 NULL，或 \return
 * `RMW_RET_INVALID_ARGUMENT` 如果 `allocator` 不符合 rcutils_allocator_is_valid() 的定义，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `service_names_and_types` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `service_names_and_types` 不是一个零初始化数组，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `node` 实现标识符与此实现不匹配，或
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。
 * \return `RMW_RET_OK` if the query was successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `node` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `allocator` is not valid,
 *   by rcutils_allocator_is_valid() definition, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `service_names_and_types` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `service_names_and_types` is not a
 *   zero-initialized array, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `node` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_get_service_names_and_types(const rmw_node_t* node,
                                          rcutils_allocator_t* allocator,
                                          rmw_names_and_types_t* service_names_and_types);

#ifdef __cplusplus
}
#endif

#endif  // RMW__GET_SERVICE_NAMES_AND_TYPES_H_
