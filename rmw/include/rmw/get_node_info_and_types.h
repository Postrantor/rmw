// Copyright 2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#ifndef RMW__GET_NODE_INFO_AND_TYPES_H_
#define RMW__GET_NODE_INFO_AND_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rmw/macros.h"
#include "rmw/names_and_types.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// 返回给定远程节点订阅的所有主题名称和类型。
/// Return all topic names and types for which a given remote node has subscriptions.
/**
 * 此函数返回本地节点到目前为止发现的给定远程节点订阅的主题名称和类型数组。
 * This function returns an array of topic names and types for which a given remote
 * node has subscriptions, as discovered so far by the given local node.
 *
 * <hr>
 * 属性                  | 依从性
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存            | 是
 * Allocates Memory   | Yes
 * 线程安全             | 是
 * Thread-Safe        | Yes
 * 使用原子操作         | 可能 [1]
 * Uses Atomics       | Maybe [1]
 * 无锁                | 可能 [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw 实现定义，检查实现文档</i>
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \par 运行时行为
 * \par Runtime behavior
 *   查询 ROS 图是一个同步操作。
 *   To query the ROS graph is a synchronous operation.
 *   它也是非阻塞的，但不能保证是无锁的。
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   一般来说，实现可以使用锁同步访问内部资源，但不允许等待没有保证时间限制的事件（除了由于操作系统调度导致的饥饿效应）。
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par 线程安全性
 * \par Thread-safety
 *   节点是线程安全对象，因此对它们进行的所有操作（除了最终化操作）都是线程安全的。
 *   Nodes are thread-safe objects, and so are all operations on them except for finalization.
 *   因此，使用相同的节点并发查询 ROS 图是安全的。
 *   Therefore, it is safe to query the ROS graph using the same node concurrently.
 *   但是，在查询已订阅主题名称和类型时：
 *   However, when querying subscribed topic names and types:
 *   - 访问名称和类型数组不是同步的。
 *   - Access to the array of names and types is not synchronized.
 *     在 rmw_get_subscriber_names_and_types_by_node() 使用 `topic_names_and_types`
 * 时，读取或写入它是不安全的。 It is not safe to read or write `topic_names_and_types` while
 * rmw_get_subscriber_names_and_types_by_node() uses it.
 *   - 访问节点名称和命名空间是只读的，但不是同步的。
 *   - Access to node name and namespace is read-only but it is not synchronized.
 *     并发 `node_name` 和 `node_namespace` 读取是安全的，但并发读取和写入不是。
 *     Concurrent `node_name` and `node_namespace` reads are safe, but concurrent reads and
 *     writes are not.
 *   - 默认分配器是线程安全对象，但任何自定义 `allocator` 可能不是。
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     请查阅您的分配器文档以获取更多参考信息。
 *     Check your allocator documentation for further reference.
 *
 * \pre 给定的 `node` 必须是一个有效的节点句柄，由 rmw_create_node() 返回。
 * \pre Given `node` must be a valid node handle, as returned by rmw_create_node().
 * \pre 给定的 `topic_names_and_types` 必须是一个零初始化的名称和类型数组，
 *   由 rmw_get_zero_initialized_names_and_types() 返回。
 * \pre Given `topic_names_and_types` must be a zero-initialized array of names and types,
 *   as returned by rmw_get_zero_initialized_names_and_types().
 *
 * \param[in] node 查询 ROS 图的本地节点。
 * \param[in] node Local node to query the ROS graph.
 * \param[in] allocator 用于填充 `topic_names_and_types` 数组的分配器。
 * \param[in] allocator Allocator to be used when populating the `topic_names_and_types` array.
 * \param[in] node_name 要获取信息的远程节点的名称。
 * \param[in] node_name Name of the remote node to get information for.
 * \param[in] node_namespace 要获取信息的远程节点的命名空间。
 * \param[in] node_namespace Namespace of the remote node to get information for.
 * \param[in] no_demangle 是否按照 ROS 约定取消所有主题名称的重整。
 * \param[in] no_demangle Whether to demangle all topic names following ROS conventions or not.
 * \param[out] topic_names_and_types 远程节点为其创建订阅的主题名称和类型数组，
 *   成功时填充，但失败时保持不变。如果填充了该数组，则调用者需要稍后使用 rmw_names_and_types_fini()
 * 对其进行最终处理。 \param[out] topic_names_and_types Array of topic names and types the remote
 * node has created a subscription for, populated on success but left unchanged on failure. If
 * populated, it is up to the caller to finalize this array later on using
 * rmw_names_and_types_fini(). \return `RMW_RET_OK` 如果查询成功，或 \return `RMW_RET_OK` if the
 * query was successful, or \return `RMW_RET_INVALID_ARGUMENT` 如果 `node` 为 NULL，或 \return
 * `RMW_RET_INVALID_ARGUMENT` if `node` is NULL, or \return `RMW_RET_INVALID_ARGUMENT` 如果
 * `allocator` \return `RMW_RET_INVALID_ARGUMENT` if `allocator` is not valid, by
 * rcutils_allocator_is_valid() definition, or \return `RMW_RET_INVALID_ARGUMENT` if `node_name` is
 * not valid, by rmw_validate_node_name() definition, or \return `RMW_RET_INVALID_ARGUMENT` if
 * `node_namespace` is not valid, by rmw_validate_namespace() definition, or \return
 * `RMW_RET_INVALID_ARGUMENT` if `topic_names_and_types` is NULL, or \return
 * `RMW_RET_INVALID_ARGUMENT` if `topic_names_and_types` is not a zero-initialized array, or \return
 * `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `node` implementation identifier does not match
 * this implementation, or \return `RMW_RET_NODE_NAME_NON_EXISTENT` if the node name wasn't found,
 * or \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or \return `RMW_RET_ERROR` if an
 * unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_get_subscriber_names_and_types_by_node(const rmw_node_t* node,
                                                     rcutils_allocator_t* allocator,
                                                     const char* node_name,
                                                     const char* node_namespace,
                                                     bool no_demangle,
                                                     rmw_names_and_types_t* topic_names_and_types);

/// 返回给定远程节点的所有发布者的主题名称和类型 (Return all topic names and types for which a given
/// remote node has publishers).
/**
 * 此函数返回一个数组，其中包含给定远程节点创建的发布者的主题名称和类型（由给定本地节点迄今发现）。
 * (This function returns an array of topic names and types for which a given remote
 * node has created publishers, as discovered so far by the given local node.)
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \par 运行时行为 (Runtime behavior)
 *   查询 ROS 图是一个同步操作。
 *   它也是非阻塞的，但不能保证是无锁的。
 *   一般来说，实现可以使用锁来同步访问内部资源，但不允许等待没有保证时间限制的事件（除了由于操作系统调度导致的饥饿效应）。
 *   (To query the ROS graph is a synchronous operation.
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).)
 *
 * \par 线程安全性 (Thread-safety)
 *   节点是线程安全对象，因此除了最终操作之外，对它们的所有操作都是线程安全的。
 *   因此，使用相同的节点并发查询 ROS 图是安全的。
 *   但是，在查询已发布主题名称和类型时：
 *   - 对名称和类型数组的访问不是同步的。
 *     在 rmw_get_publisher_names_and_types_by_node() 使用它时，读取或写入 `topic_names_and_types`
 * 是不安全的。
 *   - 对节点名称和命名空间的访问是只读的，但不是同步的。
 *     并发的 `node_name` 和 `node_namespace` 读取是安全的，但并发读取和写入是不安全的。
 *   - 默认分配器是线程安全对象，但任何自定义 `allocator` 可能不是。
 *     请查阅您的分配器文档以获取更多参考信息。
 *   (Nodes are thread-safe objects, and so are all operations on them except for finalization.
 *   Therefore, it is safe to query the ROS graph using the same node concurrently.
 *   However, when querying published topic names and types:
 *   - Access to the array of names and types is not synchronized.
 *     It is not safe to read or write `topic_names_and_types`
 *     while rmw_get_publisher_names_and_types_by_node() uses it.
 *   - Access to node name and namespace is read-only but it is not synchronized.
 *     Concurrent `node_name` and `node_namespace` reads are safe, but concurrent reads and
 *     writes are not.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.)
 *
 * \pre 给定的 `node` 必须是有效的节点句柄，由 rmw_create_node() 返回。
 *      (Given `node` must be a valid node handle, as returned by rmw_create_node().)
 * \pre 给定的 `topic_names_and_types` 必须是一个零初始化的名称和类型数组，
 *      由 rmw_get_zero_initialized_names_and_types() 返回。
 *      (Given `topic_names_and_types` must be a zero-initialized array of names and types,
 *      as returned by rmw_get_zero_initialized_names_and_types().)
 *
 * \param[in] node 用于查询 ROS 图的本地节点。(Local node to query the ROS graph.)
 * \param[in] allocator 用于填充 `topic_names_and_types` 数组的分配器。(Allocator to be used when
 * populating the `topic_names_and_types` array.) \param[in] node_name
 * 要获取信息的远程节点的名称。(Name of the remote node to get information for.) \param[in]
 * node_namespace 要获取信息的远程节点的命名空间。(Namespace of the remote node to get information
 * for.) \param[in] no_demangle 是否按照 ROS 规范对所有主题名称进行解扰。(Whether to demangle all
 * topic names following ROS conventions or not.) \param[out] topic_names_and_types
 * 远程节点为其创建了发布者的主题名称和类型数组， 成功时填充，但失败时保持不变。
 *           如果填充了，最后由调用者使用 rmw_names_and_types_fini() 对此数组进行最终处理。
 *           (Array of topic names and types the remote node has created
 *           a publisher for, populated on success but left unchanged on failure.
 *           If populated, it is up to the caller to finalize this array later on
 *           using rmw_names_and_types_fini().)
 * \return `RMW_RET_OK` 如果查询成功，或 (if the query was successful, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node` 为 NULL，或 (if `node` is NULL, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `allocator` 无效，
 *           按 rcutils_allocator_is_valid() 定义，或 (if `allocator` is not valid,
 *           by rcutils_allocator_is_valid() definition, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node_name` 无效，
 *           按 rmw_validate_node_name() 定义，或 (if `node_name` is not valid,
 *           by rmw_validate_node_name() definition, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node_namespace` 无效，
 *           按 rmw_validate_namespace() 定义，或 (if `node_namespace` is not valid,
 *           by rmw_validate_namespace() definition, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `topic_names_and_types` 为 NULL，或 (if
 * `topic_names_and_types` is NULL, or) \return `RMW_RET_INVALID_ARGUMENT` 如果
 * `topic_names_and_types` 不是一个 零初始化数组，或 (if `topic_names_and_types` is not a
 *           zero-initialized array, or)
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `node` 实现的
 *           标识符与此实现不匹配，或 (if the `node` implementation
 *           identifier does not match this implementation, or)
 * \return `RMW_RET_NODE_NAME_NON_EXISTENT` 如果节点名称未找到，或 (if the node name wasn't found,
 * or) \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或 (if memory allocation fails, or) \return
 * `RMW_RET_ERROR` 如果发生未指定的错误。(if an unspecified error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_get_publisher_names_and_types_by_node(const rmw_node_t* node,
                                                    rcutils_allocator_t* allocator,
                                                    const char* node_name,
                                                    const char* node_namespace,
                                                    bool no_demangle,
                                                    rmw_names_and_types_t* topic_names_and_types);

/// 返回给定远程节点所拥有的服务器的所有服务名称和类型。 (Return all service names and types for
/// which a given remote node has servers.)
/**
 * 此函数返回给定本地节点到目前为止发现的给定远程节点具有服务器的服务名称和类型的数组。
 * (This function returns an array of service names and types for which a given remote
 * node has servers, as discovered so far by the given local node.)
 *
 * <hr>
 * 属性 (Attribute)          | 遵循 (Adherence)
 * ------------------ | -------------
 * 分配内存 (Allocates Memory)   | 是 (Yes)
 * 线程安全 (Thread-Safe)        | 是 (Yes)
 * 使用原子操作 (Uses Atomics)       | 可能 (Maybe) [1]
 * 无锁 (Lock-Free)          | 可能 (Maybe) [1]
 * <i>[1] rmw实现定义，检查实现文档 (rmw implementation defined, check the implementation
 * documentation)</i>
 *
 * \par 运行时行为 (Runtime behavior)
 *   查询ROS图是一个同步操作。
 *   (To query the ROS graph is a synchronous operation.)
 *   它也是非阻塞的，但不能保证是无锁的。
 *   (It is also non-blocking, but it is not guaranteed to be lock-free.)
 *   一般来说，实现可以使用锁同步访问内部资源，但不允许等待没有保证时间限制的事件（由于操作系统调度导致的饥饿除外）。
 *   (Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).)
 *
 * \par 线程安全性 (Thread-safety)
 *   节点是线程安全的对象，因此对它们进行的所有操作（最后一步操作除外）都是线程安全的。
 *   (Nodes are thread-safe objects, and so are all operations on them except for finalization.)
 *   因此，使用相同的节点并发查询ROS图是安全的。
 *   (Therefore, it is safe to query the ROS graph using the same node concurrently.)
 *   但是，在查询提供服务的服务名称和类型时：
 *   (However, when querying served service names and types:)
 *   -
 * 访问名称和类型数组没有同步。在rmw_get_service_names_and_types_by_node()使用它时，读取或写入`service_names_and_types`是不安全的。
 *     (Access to the array of names and types is not synchronized.
 *     It is not safe to read or write `service_names_and_types`
 *     while rmw_get_service_names_and_types_by_node() uses it.)
 *   -
 * 访问节点名称和命名空间是只读的，但没有同步。并发`node_name`和`node_namespace`读取是安全的，但并发读写不安全。
 *     (Access to node name and namespace is read-only but it is not synchronized.
 *     Concurrent `node_name` and `node_namespace` reads are safe, but concurrent reads and
 *     writes are not.)
 *   - 默认分配器是线程安全的对象，但任何自定义`allocator`可能不是。查看分配器文档以获取更多参考。
 *     (The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.)
 *
 * \param[in] node 查询ROS图的本地节点。(Local node to query the ROS graph.)
 * \param[in] allocator 用于填充`service_names_and_types`数组的分配器。(Allocator to be used when
 * populating the `service_names_and_types` array.) \param[in] node_name
 * 获取信息的远程节点名称。(Name of the remote node to get information for.) \param[in]
 * node_namespace 获取信息的远程节点的命名空间。(Namespace of the remote node to get information
 * for.) \param[out] service_names_and_types
 * 远程节点为其创建服务服务器的服务名称和类型数组，在成功时填充，但在失败时保持不变。如果填充，由调用者稍后使用rmw_names_and_types_fini()完成此数组。
 *   (Array of service names and types the remote node has
 *   created a service server for, populated on success but left unchanged on failure.
 *   If populated, it is up to the caller to finalize this array later on
 *   using rmw_names_and_types_fini().)
 * \return `RMW_RET_OK` 如果查询成功，或 (if the query was successful, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`node`为NULL，或 (if `node` is NULL, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`allocator`无效，根据rcutils_allocator_is_valid()定义，或
 * (if `allocator` is not valid, by rcutils_allocator_is_valid() definition, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果`node_name`无效，根据rmw_validate_node_name()定义，或 (if
 * `node_name` is not valid, by rmw_validate_node_name() definition, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果`node_namespace`无效，根据rmw_validate_namespace()定义，或 (if
 * `node_namespace` is not valid, by rmw_validate_namespace() definition, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果`service_names_and_types`为NULL，或 (if `service_names_and_types`
 * is NULL, or) \return `RMW_RET_INVALID_ARGUMENT`
 * 如果`service_names_and_types`不是零初始化的数组，或 (if `service_names_and_types` is not a
 *   zero-initialized array, or)
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果`node`实现标识符与此实现不匹配，或 (if the
 * `node` implementation identifier does not match this implementation, or) \return
 * `RMW_RET_NODE_NAME_NON_EXISTENT` 如果找不到节点名称，或 (if the node name wasn't found, or)
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或 (if memory allocation fails, or)
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。(if an unspecified error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_get_service_names_and_types_by_node(const rmw_node_t* node,
                                                  rcutils_allocator_t* allocator,
                                                  const char* node_name,
                                                  const char* node_namespace,
                                                  rmw_names_and_types_t* service_names_and_types);

/// 返回给定远程节点的所有服务名称和类型。 (Return all service names and types for which a given
/// remote node has clients.)
/**
 * 此函数返回一个数组，其中包含给定远程节点的客户端所拥有的服务名称和类型，
 * 这些服务名称和类型是由给定的本地节点发现的。
 * (This function returns an array of service names and types for which a given remote
 * node has clients, as discovered so far by the given local node.)
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \par Runtime behavior
 *   查询ROS图是一个同步操作。
 *   它也是非阻塞的，但不保证是无锁的。
 *   一般来说，实现可以使用锁来同步访问内部资源，但不允许等待没有保证时间上限的事件（不考虑由于操作系统调度引起的饥饿效应）。
 *   (To query the ROS graph is a synchronous operation.
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).)
 *
 * \par Thread-safety
 *   节点是线程安全的对象，除了最终操作之外，对它们的所有操作都是线程安全的。
 *   因此，使用相同的节点并发查询ROS图是安全的。
 *   但是，在查询提供服务的名称和类型时：
 *   - 对名称和类型数组的访问不是同步的。
 *     在rmw_get_client_names_and_types_by_node()使用它时，读取或写入`service_names_and_types`是不安全的。
 *   - 对C风格字符串参数的访问是只读的，但不是同步的。
 *     并发`node_name`和`node_namespace`读取是安全的，但并发读取和写入不是。
 *   - 默认分配器是线程安全的对象，但任何自定义的`allocator`可能不是。
 *     有关进一步参考，请查阅分配器文档。
 *   (Nodes are thread-safe objects, and so are all operations on them except for finalization.
 *   Therefore, it is safe to query the ROS graph using the same node concurrently.
 *   However, when querying served service names and types:
 *   - Access to the array of names and types is not synchronized.
 *     It is not safe to read or write `service_names_and_types`
 *     while rmw_get_client_names_and_types_by_node() uses it.
 *   - Access to C-style string arguments is read-only but it is not synchronized.
 *     Concurrent `node_name` and `node_namespace` reads are safe, but concurrent reads and
 *     writes are not.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.)
 *
 * \param[in] node 用于查询ROS图的本地节点。 (Local node to query the ROS graph.)
 * \param[in] allocator 在填充`service_names_and_types`数组时使用的分配器。 (Allocator to be used
 * when populating the `service_names_and_types` array.) \param[in] node_name
 * 要获取信息的远程节点的名称。 (Name of the remote node to get information for.) \param[in]
 * node_namespace 要获取信息的远程节点的命名空间。 (Namespace of the remote node to get information
 * for.) \param[out] service_names_and_types 远程节点为其创建服务客户端的服务名称和类型的数组，
 *   成功时填充，但失败时保持不变。
 *   如果填充，最终由调用者使用rmw_names_and_types_fini()来完成此数组。
 *   (Array of service names and types the remote node has
 *   created a service client for, populated on success but left unchanged on failure.
 *   If populated, it is up to the caller to finalize this array later on
 *   using rmw_names_and_types_fini().)
 * \return `RMW_RET_OK` 如果查询成功，或 (if the query was successful, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`node`为NULL，或 (if `node` is NULL, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`allocator`无效，
 *   根据rcutils_allocator_is_valid()定义，或 (if `allocator` is not valid,
 *   by rcutils_allocator_is_valid() definition, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`node_name`无效，
 *   根据rmw_validate_node_name()定义，或 (if `node_name` is not valid,
 *   by rmw_validate_node_name() definition, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`node_namespace`无效，
 *   根据rmw_validate_namespace()定义，或 (if `node_namespace` is not valid,
 *   by rmw_validate_namespace() definition, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`service_names_and_types`为NULL，或 (if
 * `service_names_and_types` is NULL, or) \return `RMW_RET_INVALID_ARGUMENT`
 * 如果`service_names_and_types`不是 零初始化数组，或 (if `service_names_and_types` is not a
 *   zero-initialized array, or)
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果`node`实现
 *   标识符与此实现不匹配，或 (if the `node` implementation
 *   identifier does not match this implementation, or)
 * \return `RMW_RET_NODE_NAME_NON_EXISTENT` 如果未找到节点名称，或 (if the node name wasn't found,
 * or) \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或 (if memory allocation fails, or) \return
 * `RMW_RET_ERROR` 如果发生未指定的错误。 (if an unspecified error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_get_client_names_and_types_by_node(const rmw_node_t* node,
                                                 rcutils_allocator_t* allocator,
                                                 const char* node_name,
                                                 const char* node_namespace,
                                                 rmw_names_and_types_t* service_names_and_types);

#ifdef __cplusplus
}
#endif
#endif  // RMW__GET_NODE_INFO_AND_TYPES_H_
