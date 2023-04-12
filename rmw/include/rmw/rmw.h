// Copyright 2014-2017 Open Source Robotics Foundation, Inc.
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

/** \mainpage rmw: ROS Middleware Abstraction Interface
 *
 * `rmw` 定义了由更高级别的 ROS API 使用的中间件原语接口。
 * 它包括以下主要组件：
 *
 * `rmw` defines an interface of middleware primitives that are used by the higher level ROS API's.
 * It consists of these main components:
 *
 * - 初始化和关闭 (Initialization and Shutdown):
 *   - rmw/init.h
 * - 节点 (Nodes)
 *   - rmw/rmw.h
 * - 发布者 (Publisher)
 *   - rmw/rmw.h
 * - 订阅者 (Subscription)
 *   - rmw/rmw.h
 * - 服务客户端 (Service Client)
 *   - rmw/rmw.h
 * - 服务服务器 (Service Server)
 *   - rmw/rmw.h
 *
 * 与"主题"或"服务"结合使用的一些通用实用功能：
 *
 * There are some common utility functions in combination with "Topics" or "Services":
 * - 验证全限定主题或服务名称的函数
 *   - rmw_validate_full_topic_name()
 *   - rmw/validate_full_topic_name.h
 * - 验证节点命名空间的函数
 *   - rmw_validate_namespace()
 *   - rmw/validate_namespace.h
 * - 验证节点名称的函数
 *   - rmw_validate_node_name()
 *   - rmw/validate_node_name.h
 * - 验证两个 QoS 配置文件兼容性的函数
 *   - rmw_qos_profile_check_compatible()
 *   - rmw/qos_profiles.h
 *
 * 它还具有等待和处理这些概念所需的一些机制：
 *
 * It also has some machinery that is necessary to wait on and act on these concepts:
 *
 * - 初始化和关闭管理（目前为全局）
 *   - rmw/rmw.h
 * - 等待集，用于等待消息和服务请求/响应准备就绪
 *   - rmw/rmw.h
 * - 用于异步唤醒等待集的保护条件
 *   - rmw/rmw.h
 * - ROS 图的内省
 *   - rmw_names_and_types_t
 *   - rmw_get_topic_names_and_types()
 *   - rmw_get_service_names_and_types()
 *   - rmw/names_and_types.h
 *   - rmw/get_topic_names_and_types.h
 *   - rmw/get_topic_endpoint_info.h
 *   - rmw/get_service_names_and_types.h
 *
 * 此外还有一些有用的抽象和实用程序：
 *
 * Further still there are some useful abstractions and utilities:
 *
 * - 各种类型的分配器函数
 *   - rmw/allocators.h
 * - 错误处理功能（C 风格）
 *   - rmw/error_handling.h
 * - 宏
 *   - rmw/macros.h
 * - 返回代码类型和其他实用类型
 *   - rmw/types.h
 * - 一些类型的完整性检查
 *   - rmw/sanity_checks.h
 * - 控制库上符号可见性的宏
 *   - rmw/visibility_control.h
 * - 将类型解构为人类可读字符串的实用程序函数（C++ 特定）：
 *   - rmw/impl/cpp/demangle.hpp
 */

#ifndef RMW__RMW_H_
#define RMW__RMW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "rcutils/allocator.h"
#include "rcutils/macros.h"
#include "rcutils/types.h"
#include "rmw/dynamic_message_type_support.h"
#include "rmw/event.h"
#include "rmw/event_callback_type.h"
#include "rmw/init.h"
#include "rmw/macros.h"
#include "rmw/message_sequence.h"
#include "rmw/publisher_options.h"
#include "rmw/qos_profiles.h"
#include "rmw/subscription_options.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_runtime_c/sequence_bound.h"
#include "rosidl_runtime_c/service_type_support_struct.h"

/// 获取正在使用的 rmw 实现的名称 (Get the name of the rmw implementation being used)
/**
 * \return rmw 实现的名称 (Name of rmw implementation)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
const char* rmw_get_implementation_identifier(void);

/// 获取此中间件的唯一序列化格式 (Get the unique serialization format for this middleware)
/**
 * 返回二进制数据的序列化格式 (Return the format in which binary data is serialized)
 * 一个中间件只能有一种编码 (One middleware can only have one encoding)
 * 与实现标识符不同，多个 RMW 实现之间可以具有相同的序列化格式 (In contrast to the implementation
 * identifier, the serialization format can be equal between multiple RMW implementations)
 * 这意味着，相同的二进制消息可以由具有相同格式的 RMW 实现进行反序列化 (This means that the same
 * binary messages can be deserialized by RMW implementations with the same format) \sa
 * rmw_serialize \sa rmw_deserialize \return 序列化格式 (serialization format)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
const char* rmw_get_serialization_format(void);

// TODO(wjwwood): refactor this API to return a return code when updated to use an allocator
/// 创建一个节点并返回指向该节点的句柄 (Create a node and return a handle to that node)
/**
 * 如果以下情况之一发生，此函数可能会失败，并返回 `NULL`：
 *   - name 不是有效的非空节点名称 (name is not a valid non-null node name)
 *   - namespace_ 不是有效的非空命名空间 (namespace_ is not a valid non-null namespace)
 *   - context 无效，即它是零初始化的，或者
 *     其实现标识符与此 API 实现不匹配，或者已被
 *     `rmw_shutdown()` 使之失效 (context is not valid i.e. it is zero-initialized, or its
 * implementation identifier does not match that of this API implementation, or has been invalidated
 * by `rmw_shutdown()`)
 *   - 节点创建过程中内存分配失败 (memory allocation fails during node creation)
 *   - 发生未指定的错误 (an unspecified error occurs)
 *
 * <hr>
 * 属性                | 遵循
 * ------------------ | -------------
 * 分配内存            | 是 (Yes)
 * 线程安全            | 否 (No)
 * 使用原子操作        | 否 [1] (No [1])
 * 无锁                | 否 [1] (No [1])
 * <i>[1] rmw 实现定义，查阅实现文档</i> (<i>[1] rmw implementation defined, check the
 * implementation documentation</i>)
 *
 * 这应该由 rmw 实现来定义 (This should be defined by the rmw implementation)
 *
 * \param[in] context 此节点应与之关联的初始化上下文 (init context that this node should be
 * associated with)
 * \param[in] name 节点名称 (the node name)
 * \param[in] namespace_ 节点命名空间 (the node namespace)
 * \return rmw 节点句柄，如果出现错误则返回 `NULL` (rmw node handle, or `NULL` if
 * there was an error)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_node_t* rmw_create_node(rmw_context_t* context, const char* name, const char* namespace_);

/// 最终确定给定的节点句柄，回收资源并释放节点句柄。
/// Finalize a given node handle, reclaim the resources, and deallocate the node handle.
/**
 * 如果出现逻辑错误，如 `RMW_RET_INVALID_ARGUMENT`
 * 或 `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，此函数将提前返回，
 * 保持给定节点句柄不变。否则，它将忽略错误，尽可能释放更多资源，包括
 * 节点句柄。使用已释放的节点句柄是未定义的行为。
 *
 * If a logical error, such as `RMW_RET_INVALID_ARGUMENT`
 * or `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`, ensues, this function will return early,
 * leaving the given node handle unchanged. Otherwise, it will proceed despite errors,
 * freeing as many resources as it can, including the node handle.
 * Usage of a deallocated node handle is undefined behavior.
 *
 * \pre 在此调用之前，必须先销毁从此节点创建的所有发布者、订阅者、服务和客户端。
 *   某些 rmw 实现可能会验证这一点，如果从该节点创建的任何实体尚未被销毁，
 *   则返回 `RMW_RET_ERROR` 并设置一个人类可读的错误消息。但是，这不能保证
 *   所以调用者应确保在调用此函数之前确保这一点。
 *
 * \pre All publishers, subscribers, services, and clients created from this node must
 *   have been destroyed prior to this call. Some rmw implementations may verify this,
 *   returning `RMW_RET_ERROR` and setting a human readable error message if any entity
 *   created from this node has not yet been destroyed. However, this is not guaranteed
 *   and so callers should ensure that this is the case before calling this function.
 *
 * \param[in] node 要销毁的节点句柄
 * \param[in] node the node handle to be destroyed
 * \return 如果成功，则返回 `RMW_RET_OK`，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果节点无效，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果实现标识符不匹配，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_destroy_node(rmw_node_t* node);

/**
 * \deprecated `rmw_node_assert_liveliness` 实现已删除。
 *   如果需要手动生命周期断言，请使用 MANUAL_BY_TOPIC。
 *
 * \deprecated `rmw_node_assert_liveliness` implementation was removed.
 *   If manual liveliness assertion is needed, use MANUAL_BY_TOPIC.
 */
RMW_PUBLIC
RCUTILS_DEPRECATED_WITH_MSG(
    "rmw_node_assert_liveliness implementation was removed."
    " If manual liveliness assertion is needed, use MANUAL_BY_TOPIC.")
rmw_ret_t rmw_node_assert_liveliness(const rmw_node_t* node);

/// 返回一个触发器条件，当 ROS 图发生更改时触发。
/// Return a guard condition which is triggered when the ROS graph changes.
/**
 * 任何时候只要 ROS 图发生变化，保护条件就会触发。
 * 当以下情况发生时，ROS 图将发生变化：
 * - 节点加入或离开 ROS 图。
 *   此更改将反映在 rmw_get_node_names() 和
 *   rmw_get_node_names_with_enclaves() 结果中。
 * - 话题订阅加入或离开 ROS 图。
 *   此更改将反映在 rmw_get_topic_names_and_types()，
 *   rmw_get_subscriber_names_and_types_by_node() 和
 *   rmw_get_subscriptions_info_by_topic() 结果中。
 * - 话题发布者加入或离开 ROS 图。
 *   此更改将反映在 rmw_get_topic_names_and_types()，
 *   rmw_get_publisher_names_and_types_by_node() 和
 *   rmw_get_publishers_info_by_topic() 结果中。
 * - 具有兼容 QoS 策略的话题订阅与话题发布者匹配。
 *   此更改将反映在 rmw_subscription_count_matched_publishers() 结果中。
 * - 具有兼容 QoS 策略的话题发布者与话题订阅匹配。
 *   此更改将反映在 rmw_publisher_count_matched_subscriptions() 结果中。
 * - 服务服务器加入或离开 ROS 图。
 *   此更改将反映在 rmw_get_service_names_and_types() 和
 *   rmw_get_service_names_and_types_by_node() 结果中。
 * - 服务客户端加入或离开 ROS 图。
 *   此更改将反映在 rmw_get_service_names_and_types() 和
 *   rmw_get_client_names_and_types_by_node() 结果中。
 * - 具有兼容 QoS 策略的服务客户端与服务服务器匹配。
 *   此更改将反映在 rmw_service_server_is_available() 结果中。
 *
 * The guard condition will be triggered anytime a change to the ROS graph occurs.
 * A ROS graph change occurs whenever:
 * - A node joins or leaves the ROS graph.
 *   This change will be reflected in rmw_get_node_names() and
 *   rmw_get_node_names_with_enclaves() outcome.
 * - A topic subscription joins or leaves the ROS graph.
 *   This change will be reflected in rmw_get_topic_names_and_types(),
 *   rmw_get_subscriber_names_and_types_by_node(), and
 *   rmw_get_subscriptions_info_by_topic() outcome.
 * - A topic publisher joins or leaves the ROS graph.
 *   This change will be reflected in rmw_get_topic_names_and_types(),
 *   rmw_get_publisher_names_and_types_by_node(), and
 *   rmw_get_publishers_info_by_topic() outcome.
 * - A topic subscription matches a topic publisher with compatible QoS policies.
 *   This change will be reflected in rmw_subscription_count_matched_publishers() outcome.
 * - A topic publisher matches a topic subscription with compatible QoS policies.
 *   This change will be reflected in rmw_publisher_count_matched_subscriptions() outcome.
 * - A service server joins or leaves the ROS graph.
 *   This change will be reflected in rmw_get_service_names_and_types() and
 *   rmw_get_service_names_and_types_by_node() outcome.
 * - A service client joins or leaves the ROS graph.
 *   This change will be reflected in rmw_get_service_names_and_types() and
 *   rmw_get_client_names_and_types_by_node() outcome.
 * - A service client matches a service server with compatible QoS policies.
 *   This change will be reflected in rmw_service_server_is_available() outcome.
 *
 * \note 与关联的 `node` 相关的 ROS 图的状态和可能发生的任何更改都会报告。
 * \note The state of the ROS graph, and any changes that may take place,
 *   are reported as seen by the associated `node`.
 *
 * 保护条件由 `node` 拥有并在内部持有。
 * 如果使用 rmw_destroy_node() 对 `node` 进行最终确定，它将被使无效。
 * 使用无效的保护条件是未定义的行为。
 *
 * The guard condition is owned and internally held by the `node`.
 * It will be invalidated if `node` is finalized using rmw_destroy_node().
 * It is undefined behavior to use an invalidated guard condition.
 *
 * <hr>
 * 属性            | 遵循
 * ------------------ | -------------
 * 分配内存   | 否
 * 线程安全        | 是
 * 使用原子       | 否
 * 无锁          | 是
 *
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \pre 给定的 `node` 必须是一个有效的节点句柄，由 rmw_create_node() 返回。
 * \pre Given `node` must be a valid node handle, as returned by rmw_create_node().
 *
 * \param[in] node 从中检索保护条件的节点。
 * \param[in] node Node to retrieve the guard condition from.
 * \return 如果成功，则返回保护条件，如果
 *   `node` 为 `NULL`，或发生未指定的错误，则返回 `NULL`。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
const rmw_guard_condition_t* rmw_node_get_graph_guard_condition(const rmw_node_t* node);

/// 初始化一个用于后续发布的发布者分配。
/**
 * 这将创建一个分配对象，该对象可以与 rmw_publish 方法结合使用，
 * 以更仔细地控制内存分配。
 *
 * 这将允许中间件为给定的消息类型和消息边界预分配正确数量的内存。
 * 因为在此方法中执行了分配，所以不需要在 `rmw_publish` 方法中分配。
 *
 * \param[in] type_support 要预分配的消息的类型支持。
 * \param[in] message_bounds 要预分配的消息的边界结构。
 * \param[out] allocation 将传递给 `rmw_publish` 的分配结构。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_UNSUPPORTED` 如果未实现
 * \return `RMW_RET_INVALID_ARGUMENT` 如果参数为空，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
/// Initialize a publisher allocation to be used with later publications.
/**
 * This creates an allocation object that can be used in conjunction with
 * the rmw_publish method to perform more carefully control memory allocations.
 *
 * This will allow the middleware to preallocate the correct amount of memory
 * for a given message type and message bounds.
 * As allocation is performed in this method, it will not be necessary to allocate
 * in the `rmw_publish` method.
 *
 * \param[in] type_support Type support of the message to be preallocated.
 * \param[in] message_bounds Bounds structure of the message to be preallocated.
 * \param[out] allocation Allocation structure to be passed to `rmw_publish`.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_UNSUPPORTED` if it's unimplemented
 * \return `RMW_RET_INVALID_ARGUMENT` if an argument is null, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_init_publisher_allocation(
    const rosidl_message_type_support_t* type_support,
    const rosidl_runtime_c__Sequence__bound* message_bounds,
    rmw_publisher_allocation_t* allocation);

/// 销毁一个发布者分配对象。
/**
 * 这将释放由 `rmw_init_publisher_allocation` 分配的任何内存。
 *
 * \param[in] allocation 要销毁的分配对象。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_UNSUPPORTED` 如果未实现
 * \return `RMW_RET_INVALID_ARGUMENT` 如果参数为空，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
/// Destroy a publisher allocation object.
/**
 * This deallocates any memory allocated by `rmw_init_publisher_allocation`.
 *
 * \param[in] allocation Allocation object to be destroyed.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_UNSUPPORTED` if it's unimplemented
 * \return `RMW_RET_INVALID_ARGUMENT` if argument is null, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_fini_publisher_allocation(rmw_publisher_allocation_t* allocation);

/// 创建一个发布者并返回指向该发布者的句柄。
/// Create a publisher and return a handle to that publisher.
/**
 * 本函数可能失败，因此在以下情况下会返回 `NULL`：
 * This function can fail, and therefore return `NULL`, if:
 *   - 节点不是由 `rmw_create_node()` 返回的有效非空句柄
 *   - node is not a valid non-null handle for this rmw implementation,
 *     as returned by `rmw_create_node()`
 *   - type_support 不是有效的非空消息类型支持，由 `ROSIDL_GET_MSG_TYPE_SUPPORT()` 返回
 *   - type_support is a not valid non-null message type support, as returned by
 *     `ROSIDL_GET_MSG_TYPE_SUPPORT()`
 *   - topic_name 不是根据 `rmw_validate_full_topic_name()` 的有效非空主题名称
 *   - topic_name is not a valid non-null topic name, according to
 *     `rmw_validate_full_topic_name()`
 *   - qos_profile 不是完全指定的非空配置文件，即没有未知策略
 *   - qos_profile is not a fully specified non-null profile i.e. no UNKNOWN policies
 *   - publisher_options 不是有效的非空选项集，由 `rmw_get_default_publisher_options()` 返回
 *   - publisher_options is not a valid non-null option set, as returned by
 *     `rmw_get_default_publisher_options()`
 *   - 发布者创建过程中内存分配失败
 *   - memory allocation fails during publisher creation
 *   - 发生未指定错误
 *   - an unspecified error occurs
 *
 * <hr>
 * 属性                | 遵循
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存            | 是
 * Allocates Memory   | Yes
 * 线程安全            | 否
 * Thread-Safe        | No
 * 使用原子操作        | 也许[1]
 * Uses Atomics       | Maybe [1]
 * 无锁                | 也许[1]
 * Lock-Free          | Maybe [1]
 * <i>[1] 由 rmw 实现定义，请查阅实现文档</i>
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] node 用于注册此发布者的节点句柄
 * \param[in] node Handle to node with which to register this publisher
 * \param[in] type_support 要发布的消息的类型支持
 * \param[in] type_support Type support for the messages to be published
 * \param[in] topic_name 要发布到的主题名称，通常是一个完全限定的主题名称，除非 `qos_profile`
 * 配置为避免 ROS 命名空间约定，即创建本地主题发布者
 * \param[in] topic_name Name of the topic to
 * publish to, often a fully qualified topic name unless `qos_profile` is configured to avoid ROS
 * namespace conventions i.e. to create a native topic publisher
 * \param[in] qos_profile 此发布者的 QoS 策略
 * \param[in] qos_profile QoS policies for this publisher
 * \param[in] publisher_options 配置此发布者的选项
 * \param[in] publisher_options Options to configure this publisher
 * \return rmw 发布者句柄，如果出现错误则返回 `NULL`
 * \return rmw publisher handle, or `NULL` if there was an error
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_publisher_t* rmw_create_publisher(
    const rmw_node_t* node,
    const rosidl_message_type_support_t* type_support,
    const char* topic_name,
    const rmw_qos_profile_t* qos_profile,
    const rmw_publisher_options_t* publisher_options);

/// 结束给定的发布者句柄，回收资源，并释放发布者句柄。
/// Finalize a given publisher handle, reclaim the resources, and deallocate the publisher handle.
/**
 * 如果发生逻辑错误，如 `RMW_RET_INVALID_ARGUMENT`
 * 或 `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，此函数将提前返回，
 * 保持给定的发布者句柄不变。否则，它将忽略错误，尽可能释放资源，包括
 * 发布者句柄。使用已释放的发布者句柄是未定义的行为。
 * This function will return early if a logical error, such as `RMW_RET_INVALID_ARGUMENT`
 * or `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`, ensues, leaving the given publisher handle unchanged.
 * Otherwise, it will proceed despite errors, freeing as many resources as it can, including
 * the publisher handle. Usage of a deallocated publisher handle is undefined behavior.
 *
 * \pre 给定的节点必须是注册发布者的节点。
 * \pre Given node must be the one the publisher was registered with.
 *
 * <hr>
 * 属性                  | 遵循
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存              | 否
 * Allocates Memory   | No
 * 线程安全              | 否
 * Thread-Safe        | No
 * 使用原子操作         | 可能[1]
 * Uses Atomics       | Maybe [1]
 * 无锁                 | 可能[1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw 实现定义，检查实现文档</i>
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] node 与给定发布者注册的节点句柄
 * \param[in] node Handle to node with which the given publisher is registered
 * \param[in] publisher 要完成的发布者句柄
 * \param[in] publisher Handle to publisher to be finalized
 * \return 如果成功，则返回 `RMW_RET_OK`，或
 * \return `RMW_RET_OK` if successful, or
 * \return 如果节点或发布者为 `NULL`，则返回 `RMW_RET_INVALID_ARGUMENT`，或
 * \return `RMW_RET_INVALID_ARGUMENT` if node or publisher is `NULL`, or
 * \return 如果节点或发布者实现标识符不匹配，则返回 `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if node or publisher
 *   implementation identifier does not match, or
 * \return 如果发生意外错误，则返回 `RMW_RET_ERROR`。
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_destroy_publisher(rmw_node_t* node, rmw_publisher_t* publisher);

/// 借用一个已借出的 ROS 消息。
/// Borrow a loaned ROS message.
/**
 * 此 ROS 消息由中间件拥有，直到调用者使用 rmw_publish_loaned_message() 发布它或
 * 使用 rmw_return_loaned_message_from_publisher()
 * 返回它为止，中间件将使其保持活动状态（即在有效内存空间中）。 This ROS message is owned by the
 * middleware, which will keep it alive (i.e., in valid memory space) until the caller publishes it
 * using rmw_publish_loaned_message() or returns it using
 * rmw_return_loaned_message_from_publisher().
 *
 * <hr>
 * 属性                  | 遵循性
 * Attribute             | Adherence
 * --------------------  | -------------
 * 分配内存               | 可能
 * Allocates Memory      | Maybe
 * 线程安全               | 是
 * Thread-Safe           | Yes
 * 使用原子操作           | 可能 [1]
 * Uses Atomics          | Maybe [1]
 * 无锁                   | 可能 [1]
 * Lock-Free             | Maybe [1]
 *
 * <i>[1] 实现定义，请查阅实现文档。</i>
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par 运行时行为
 *   借用 ROS 消息是同步操作。
 *   To borrow a ROS message is a synchronous operation.
 *   它也是非阻塞的，但不能保证是无锁的。
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   一般来说，实现可以使用锁来同步访问内部资源，
 *   但不允许等待没有保证时间范围的事件（除非由于操作系统调度导致饥饿）。
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par 内存分配
 *   实现定义是否在借用时分配内存。
 *   It is implementation defined whether memory will be allocated on borrow or not.
 *   查阅实现文档以了解在使用 ROS 消息贷款支持时的内存分配保证。
 *   Check the implementation documentation to learn about memory allocation
 *   guarantees when using ROS message loaning support.
 *
 * \par 线程安全性
 *   发布者是线程安全对象，因此对它们的所有操作（除最终化之外）都是线程安全的。
 *   Publishers are thread-safe objects, and so are all operations on them except for finalization.
 *   因此，从同一个发布者并发地借用 ROS 消息是安全的。
 *   Therefore, it is safe to borrow ROS messages from the same publisher concurrently.
 *
 * \pre 给定的 `publisher` 必须是有效的发布者，如 rmw_create_publisher() 返回的那样。
 * \pre Given `publisher` must be a valid publisher, as returned by rmw_create_publisher().
 * \pre 给定的 `type_support` 必须是有效的 `rosidl` 消息类型支持，与在创建时向 `publisher`
 * 注册的类型匹配。 \pre Given `type_support` must be a valid `rosidl` message type support,
 * matching the one registered with the `publisher` on creation.
 *
 * \param[in] publisher 被借出的 ROS 消息将与之关联的发布者。
 * \param[in] publisher Publisher to which the loaned ROS message will be associated.
 * \param[in] type_support 被借出的 ROS 消息的消息类型支持。
 * \param[in] type_support Message type support of the loaned ROS message.
 * \param[out] ros_message 指向由中间件贷款的类型擦除 ROS 消息的指针。
 * \param[out] ros_message Pointer to type erased ROS message loaned by the middleware.
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `publisher` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `publisher` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `type_support` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `type_support` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `ros_message` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `ros_message` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `*ros_message` 不为 NULL（以防止泄漏），或
 * \return `RMW_RET_INVALID_ARGUMENT` if `*ros_message` is not NULL (to prevent leaks), or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `publisher` 实现标识符与此实现不匹配，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if `publisher` implementation identifier
 *   does not match this implementation, or
 * \return `RMW_RET_UNSUPPORTED` 如果实现不支持 ROS 消息贷款，或
 * \return `RMW_RET_UNSUPPORTED` if the implementation does not support ROS message loaning, or
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 * \return `RMW_RET_ERROR` if an unexpected error occurred.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_borrow_loaned_message(
    const rmw_publisher_t* publisher,
    const rosidl_message_type_support_t* type_support,
    void** ros_message);

/// 返回一个之前从发布者借用的消息。 (Return a loaned message previously borrowed from a publisher.)
/**
 * 告诉中间件调用者不再需要借用的ROS消息。 (Tells the middleware that a borrowed ROS message is no
 * longer needed by the caller.) 将ROS消息的所有权归还给中间件。 (Ownership of the ROS message is
 * given back to the middleware.) 如果此函数由于逻辑错误（如无效参数）导致提前失败， (If this
 * function fails early due to a logical error, such as an invalid argument,)
 * 则借用的ROS消息将保持不变。 (the loaned ROS message will be left unchanged.)
 * 否则，将ROS消息的所有权归还给中间件。 (Otherwise, ownership of the ROS message will be given back
 * to the middleware.) 中间件将决定返回的ROS消息的处理方式。 (It is up to the middleware what will
 * be made of the returned ROS message.) 使用归还后的借用ROS消息是未定义行为。 (It is undefined
 * behavior to use a loaned ROS message after returning it.)
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par 运行时行为 (Runtime behavior)
 *   返回ROS消息是一个同步操作。 (To return a ROS message is a synchronous operation.)
 *   它也是非阻塞的，但不能保证是无锁的。 (It is also non-blocking, but it is not guaranteed to be
 * lock-free.) 一般来说，实现可能使用锁来同步访问内部资源， (Generally speaking, implementations may
 * synchronize access to internal resources using)
 *   但不允许等待没有保证时间限制的事件（不考虑由于操作系统调度造成的饥饿效应）。 (locks but are not
 * allowed to wait for events with no guaranteed time bound (barring the effects of starvation due
 * to OS scheduling).)
 *
 * \par 线程安全 (Thread-safety)
 *   发布者是线程安全对象，对它们的所有操作（除了最终化）都是线程安全的。 (Publishers are
 * thread-safe objects, and so are all operations on them except for finalization.)
 *   因此，可以并发地将借用的ROS消息返回给相同的发布者。 (Therefore, it is safe to return borrowed
 * ROS messages to the same publisher concurrently.)
 *   但是，由于将借用的ROS消息的所有权归还给中间件且此传输未同步， (However, since ownership of the
 * loaned ROS message is given back to the middleware and this transfer is not synchronized,)
 *   并发返回相同的借用ROS消息是不安全的。 (it is not safe to return the same loaned ROS message
 * concurrently.)
 *
 * \pre 给定的`publisher`必须是有效的发布者，如由rmw_create_publisher()返回。 (Given `publisher`
 * must be a valid publisher, as returned by rmw_create_publisher().) \pre
 * 给定的`loaned_message`必须之前已从相同发布者借用， (Given `loaned_message` must have been
 * previously borrowed from the same publisher using rmw_borrow_loaned_message().)
 *
 * \param[in] publisher 与借用的ROS消息关联的发布者。 (Publisher to which the loaned ROS message is
 * associated.) \param[in] loaned_message 要返回的类型擦除的借用ROS消息。 (Type erased loaned ROS
 * message to be returned.) \return `RMW_RET_OK` 如果成功，或 (if successful, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果 `publisher` 为 NULL，或 (if `publisher` is NULL, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果 `loaned_message` 为 NULL，或 (if `loaned_message` is NULL, or)
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `publisher` 实现标识符与此实现不匹配，或 (if
 * `publisher` implementation identifier does not match this implementation, or) \return
 * `RMW_RET_UNSUPPORTED` 如果实现不支持ROS消息借用，或 (if the implementation does not support ROS
 * message loaning, or) \return `RMW_RET_ERROR` 如果发生意外错误且无法初始化消息。 (if an unexpected
 * error occurs and no message can be initialized.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_return_loaned_message_from_publisher(
    const rmw_publisher_t* publisher, void* loaned_message);

/// 发布一个 ROS 消息。 (Publish a ROS message.)
/**
 * 使用给定的发布者将 ROS 消息发送到所有具有匹配 QoS 策略的订阅。 (Send a ROS message to all
 * subscriptions with matching QoS policies using the given publisher.)
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   发布 ROS 消息是同步还是异步、阻塞还是非阻塞操作，这取决于实现。 (It is implementation defined
 * whether to publish a ROS message is a synchronous or asynchronous, blocking or non-blocking
 * operation.) 但是，异步实现不允许在此函数返回后访问给定的 ROS 消息。 (However, asynchronous
 * implementations are not allowed to access the given ROS message after this function returns.)
 *   查看实现文档以了解关于发布行为的信息。 (Check the implementation documentation to learn about
 * publish behavior.)
 *
 * \par Memory allocation
 *   是否在发布时分配内存取决于实现。 (It is implementation defined whether memory will be allocated
 * on publish or not.) 例如，序列化 ROS
 * 消息以通过线路发送的实现可能需要在处理无界（动态大小）字段时执行额外的内存分配。 (For instance,
 * implementations that serialize ROS messages to send it over the wire may need to perform
 * additional memory allocations when dealing with unbounded (dynamically-sized) fields.)
 *   可能会也可能不会使用提供的发布者分配。 (A publisher allocation, if provided, may or may not be
 * used.) 查看实现文档以了解在发布 ROS 消息时带有和没有发布者分配的内存分配保证。 (Check the
 * implementation documentation to learn about memory allocation guarantees when publishing ROS
 * messages with and without publisher allocations.)
 *
 * \par Thread-safety
 *   发布者是线程安全对象，因此除了最终化之外，对它们的所有操作都是线程安全的。 (Publishers are
 * thread-safe objects, and so are all operations on them except for finalization.)
 *   因此，使用相同的发布者并发发布是安全的。 (Therefore, it is safe to publish using the same
 * publisher concurrently.) 但是，在发布常规 ROS 消息时： (However, when publishing regular ROS
 * messages:)
 *   - 对 ROS 消息的访问是只读的，但不是同步的。 (Access to the ROS message is read-only but it is
 * not synchronized.) 并发 `ros_message` 读取是安全的，但并发读取和写入不是。 (Concurrent
 * `ros_message` reads are safe, but concurrent reads and writes are not.)
 *   - 对发布者分配的访问不是同步的，除非实现特别说明。 (Access to the publisher allocation is not
 * synchronized, unless specifically stated otherwise by the implementation.) 因此，在 rmw_publish()
 * 使用它时，通常不安全地读取或写入 `allocation` 是不安全的。 (Thus, it is generally not safe to
 * read or write `allocation` while rmw_publish() uses it.)
 *     查看实现文档以了解关于发布者分配线程安全性的信息。 (Check the implementation documentation to
 * learn about publisher allocations' thread-safety.)
 *
 * \pre 给定的 `publisher` 必须是一个有效的发布者，由 rmw_create_publisher() 返回。 (Given
 * `publisher` must be a valid publisher, as returned by rmw_create_publisher().) \pre 给定的
 * `ros_message` 必须是一个有效的消息，其类型与 `publisher` 在创建时注册的消息类型支持相匹配。
 * (Given `ros_message` must be a valid message, whose type matches the message type support the
 * `publisher` was registered with on creation.) \pre 如果不为 NULL，则给定的 `allocation`
 * 必须是一个有效的发布者分配，使用与在创建时向 `publisher` 注册的消息类型支持匹配的
 * rmw_publisher_allocation_init() 初始化。 (If not NULL, given `allocation` must be a valid
 * publisher allocation, initialized with rmw_publisher_allocation_init() with a message type
 * support that matches the one registered with `publisher` on creation.)
 *
 * \param[in] publisher 用于发送消息的发布者。 (Publisher to be used to send message.)
 * \param[in] ros_message 要发送的类型擦除 ROS 消息。 (Type erased ROS message to be sent.)
 * \param[in] allocation 要使用的预先分配的内存。可以为 NULL。 (Pre-allocated memory to be used. May
 * be NULL.) \return `RMW_RET_OK` if successful, or \return `RMW_RET_INVALID_ARGUMENT` if
 * `publisher` is NULL, or \return `RMW_RET_INVALID_ARGUMENT` if `ros_message` is NULL, or \return
 * `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if `publisher` implementation identifier does not match
 * this implementation, or \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_publish(
    const rmw_publisher_t* publisher,
    const void* ros_message,
    rmw_publisher_allocation_t* allocation);

/// 发布一个借用的 ROS 消息。
/**
 * 使用给定的发布者将之前借用的 ROS 消息发送到所有具有匹配 QoS 策略的订阅，
 * 然后将 ROS 消息所有权返回给中间件。
 *
 * 如果此函数由于逻辑错误（如无效参数）而提前失败，
 * 借用的 ROS 消息将保持不变。
 * 否则，ROS 消息的所有权将归还给中间件。
 * 中间件将决定如何处理返回的 ROS 消息。
 * 发布借用的 ROS 消息后使用它是未定义的行为。
 *
 * <hr>
 * 属性              | 遵循
 * ------------------ | -------------
 * 分配内存          | 可能
 * 线程安全          | 是
 * 使用原子操作      | 可能 [1]
 * 无锁              | 可能 [1]
 *
 * <i>[1] 由实现定义，检查实现文档。</i>
 *
 * \par 运行时行为
 *   发布借用的 ROS 消息是同步还是异步，阻塞还是非阻塞操作由实现定义。
 *   查看实现文档以了解发布行为。
 *
 * \par 内存分配
 *   实现定义是否在发布时分配内存。
 *   例如，在通过网络发送序列化 ROS 消息时可能需要在处理
 *   无界（动态大小）字段时执行额外的内存分配。
 *   可能使用或不使用提供的发布者分配。
 *   查看实现文档以了解在发布借用的 ROS 消息时有无发布者分配的内存分配保证。
 *
 * \par 线程安全
 *   发布者是线程安全对象，除最终化外，对它们的所有操作都是线程安全的。
 *   因此，使用相同的发布者并发发布是安全的。
 *   但是，在发布借用的 ROS 消息时：
 *   - 将借用的 ROS 消息的所有权归还给中间件。
 *     此传输未同步，因此并发发布相同的借用的 ROS 消息是不安全的。
 *   - 对发布者分配的访问不是同步的，除非实现特别声明
 *     否则。
 *     因此，在 rmw_publish() 使用 `allocation` 时，通常不安全地读取或写入 `allocation`。
 *     查看实现文档以了解发布者分配的线程安全性。
 *
 * \pre 给定的 `publisher` 必须是有效的发布者，由 rmw_create_publisher() 返回。
 * \pre 给定的 `ros_message` 必须是有效的消息，从使用相同发布者借用
 *   rmw_borrow_loaned_message()。
 * \pre 如果不为 NULL，则给定的 `allocation` 必须是有效的发布者分配，使用
 *   rmw_publisher_allocation_init() 与在创建时向 `publisher` 注册的消息类型支持进行初始化。
 *
 * \param[in] publisher 用于发送消息的发布者。
 * \param[in] ros_message 要发送的借用类型擦除 ROS 消息。
 * \param[in] allocation 要使用的预分配内存。可以为 NULL。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `publisher` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `ros_message` 为 NULL，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `publisher` 实现
 *   标识符与此实现不匹配，或
 * \return `RMW_RET_UNSUPPORTED` 如果实现不支持 ROS 消息借用，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_publish_loaned_message(
    const rmw_publisher_t* publisher, void* ros_message, rmw_publisher_allocation_t* allocation);

/// 获取与发布者匹配的订阅数量。
/// Retrieve the number of matched subscriptions to a publisher.
/**
 * 查询底层中间件以确定有多少订阅与给定的发布者匹配。
 * Query the underlying middleware to determine how many subscriptions are
 * matched to a given publisher.
 *
 * <hr>
 * 属性                | 遵循情况
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存            | 否
 * Allocates Memory   | No
 * 线程安全            | 否
 * Thread-Safe        | No
 * 使用原子操作        | 也许 [1]
 * Uses Atomics       | Maybe [1]
 * 无锁                | 也许 [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw 实现定义的，请查看实现文档</i>
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] publisher 要检查的发布者对象
 * \param[out] subscription_count 匹配的订阅数量
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果任一参数为 null，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果发布者
 *   实现标识符不匹配，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_publisher_count_matched_subscriptions(
    const rmw_publisher_t* publisher, size_t* subscription_count);

/// 获取发布者的实际 qos 设置。
/// Retrieve the actual qos settings of the publisher.
/**
 * 查询底层中间件以确定发布者的 qos 设置。
 * 当使用 RMW_*_SYSTEM_DEFAULT 时，只有在创建发布者之后才能解析实际应用的配置，
 * 并且它取决于底层的 rmw 实现。如果正在使用的底层设置不能用 ROS 术语表示，
 * 它将被设置为 RMW_*_UNKNOWN。
 *
 * \note 此函数不会解析 avoid_ros_namespace_conventions 字段的值。
 *   rcl 函数 `rcl_publisher_get_actual_qos()` 解析它。
 *
 * <hr>
 * 属性                | 遵循情况
 * ------------------ | -------------
 * 分配内存            | 也许 [1]
 * 线程安全            | 否
 * 使用原子操作        | 也许 [1]
 * 无锁                | 也许 [1]
 * <i>[1] rmw 实现定义的，请查看实现文档</i>
 *
 * \param[in] publisher 要检查的发布者对象
 * \param[out] qos 实际的 qos 设置
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果任一参数为 null，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果发布者
 *   实现标识符不匹配，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 * ---
 * Query the underlying middleware to determine the qos settings
 * of the publisher.
 * The actual configuration applied when using RMW_*_SYSTEM_DEFAULT
 * can only be resolved after the creation of the publisher, and it
 * depends on the underlying rmw implementation.
 * If the underlying setting in use can't be represented in ROS terms,
 * it will be set to RMW_*_UNKNOWN.
 *
 * \note The value of avoid_ros_namespace_conventions field is not resolved
 *   with this function. The rcl function `rcl_publisher_get_actual_qos()`
 *   resolves it.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe [1]
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] publisher The publisher object to inspect
 * \param[out] qos The actual qos settings
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if any arguments are null, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the publisher
 *   implementation identifier does not match, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_publisher_get_actual_qos(const rmw_publisher_t* publisher, rmw_qos_profile_t* qos);

/// 发布一个序列化为字节流的 ROS 消息。
/**
 * 使用给定的发布器将一个序列化为字节流的 ROS 消息发送到所有具有匹配 QoS 策略的订阅。
 * 可以使用 rmw_serialize() 手动序列化 ROS 消息。
 *
 * <hr>
 * 属性                  | 符合性
 * -------------------- | -------------
 * 分配内存              | 可能
 * 线程安全              | 是
 * 使用原子操作          | 可能 [1]
 * 无锁                  | 可能 [1]
 *
 * <i>[1] 取决于实现，查看实现文档。</i>
 *
 * \par 运行时行为
 *   将已借用的 ROS 消息发布是否为同步或异步、阻塞或非阻塞操作取决于实现。
 *   但是，异步实现不允许在此函数返回后访问给定的字节流。
 *   查看实现文档了解发布行为。
 *
 * \par 内存分配
 *   在发布时是否分配内存取决于实现。
 *   即使提供了发布器分配，实现也可能忽略它。
 *   查看实现文档了解在使用和不使用发布器分配发布序列化消息时的内存分配保证。
 *
 * \par 线程安全性
 *   发布器是线程安全对象，因此它们上的所有操作（最终化除外）都是线程安全的。
 *   因此，使用相同的发布器并发发布是安全的。
 *   但是，在发布序列化的 ROS 消息时：
 *   - 对字节流的访问是只读的，但不是同步的。
 *     并发的 `serialized_message` 读取是安全的，但并发读写则不然。
 *   - 发布器分配的访问不是同步的，除非实现明确另有说明。
 *     因此，在 rmw_publish() 使用它时，通常不安全地读取或写入 `allocation`。
 *     查看实现文档了解发布器分配的线程安全性。
 *
 * \pre 给定的 `publisher` 必须是有效的发布器，由 rmw_create_publisher() 返回。
 * \pre 给定的 `serialized_message` 必须是有效的序列化消息，由 rmw_serialized_message_init()
 * 初始化， 并包含与创建时在 `publisher` 上注册的消息类型支持匹配的 ROS 消息的序列化。 \pre 如果非
 * NULL，则给定的 `allocation` 必须是有效的发布器分配，通过 rmw_publisher_allocation_init()
 *   与与创建时在 `publisher` 上注册的消息类型支持匹配的消息类型支持进行初始化。
 *
 * \param[in] publisher 用于发送消息的发布器。
 * \param[in] serialized_message 要发送的序列化 ROS 消息。
 * \param[in] allocation 要使用的预分配内存。可以为 NULL。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `publisher` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `serialized_message` 为 NULL，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `publisher` 实现标识符与此实现不匹配，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_publish_serialized_message(
    const rmw_publisher_t* publisher,
    const rmw_serialized_message_t* serialized_message,
    rmw_publisher_allocation_t* allocation);

/// 计算序列化消息的大小。 (Compute the size of a serialized message.)
/**
 * 根据消息定义和边界，计算序列化大小。 (Given a message definition and bounds, compute the
 * serialized size.)
 *
 * \param[in] type_support 要计算的消息的类型支持。 (The type support of the message to compute.)
 * \param[in] message_bounds 用于无界字段的人工边界。 (Artificial bounds to use on unbounded
 * fields.) \param[out] size 计算出的序列化消息的大小。 (The computed size of the serialized
 * message.) \return `RMW_RET_OK` 如果成功，或 (if successful, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果参数为空，或 (if either argument is null, or) \return
 * `RMW_RET_UNSUPPORTED` 如果未实现，或 (if it's unimplemented, or) \return `RMW_RET_ERROR`
 * 如果发生意外错误。 (if an unexpected error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_get_serialized_message_size(
    const rosidl_message_type_support_t* type_support,
    const rosidl_runtime_c__Sequence__bound* message_bounds,
    size_t* size);

/// 手动声明此发布者是活跃的（针对 RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC）(Manually assert that
/// this Publisher is alive (for RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC))
/**
 * 如果 rmw Liveliness 策略设置为
 * RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC，则此发布者的创建者可以在某个时间点手动调用
 * `assert_liveliness`，以向系统的其余部分发出信号，表明此节点仍然活跃。 (If the rmw Liveliness
 * policy is set to RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC, the creator of this publisher may
 * manually call `assert_liveliness` at some point in time to signal to the rest of the system that
 * this Node is still alive.)
 *
 * <hr>
 * 属性 (Attribute)          | 遵循性 (Adherence)
 * ------------------ | -------------
 * 分配内存 (Allocates Memory)   | 否 (No)
 * 线程安全 (Thread-Safe)        | 是 (Yes)
 * 使用原子操作 (Uses Atomics)       | 否 (No)
 * 无锁 (Lock-Free)          | 是 (Yes)
 *
 * \param[in] publisher 需要声明活跃性的发布者句柄 (handle to the publisher that needs liveliness to
 * be asserted) \return `RMW_RET_OK` 如果活跃性声明成功完成，或 (if the liveliness assertion was
 * completed successfully, or) \return `RMW_RET_ERROR` 如果发生未指定的错误，或 (if an unspecified
 * error occurs, or) \return `RMW_RET_UNSUPPORTED` 如果 rmw 实现不支持声明活跃性。 (if the rmw
 * implementation does not support asserting liveliness.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_publisher_assert_liveliness(const rmw_publisher_t* publisher);

/// 等待所有已发布的消息数据被确认或直到指定的超时时间过去。
/// Wait until all published message data is acknowledged or until the specified timeout elapses.
/**
 * 此函数等待所有已发布的消息数据被对等节点确认或超时。
 * This function waits until all published message data were acknowledged by peer node or timeout.
 *
 * 该功能只在发布者的 QOS 配置文件设置为 RELIABLE 时有效。
 * This function only works effectively while QOS profile of publisher is set to RELIABLE.
 * 否则，此函数将立即返回 RMW_RET_OK。
 * Otherwise this function will immediately return RMW_RET_OK.
 *
 * <hr>
 * 属性          | 遵循
 * Attribute     | Adherence
 * ------------- | -------------
 * 分配内存       | 可能 [1]
 * Allocates Memory | Maybe [1]
 * 线程安全       | 是
 * Thread-Safe   | Yes
 * 使用原子操作   | 可能 [1]
 * Uses Atomics  | Maybe [1]
 * 无锁           | 可能 [1]
 * Lock-Free     | Maybe [1]
 * <i>[1] rmw 实现定义，检查实现文档</i>
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \par 运行时行为
 * \par Runtime behavior
 *   等待所有确认是同步操作。
 *   Waiting for all acknowledgments is synchronous operation.
 *   因此，调用线程将阻塞，直到所有已发布的消息数据被确认或指定的持续时间过去。
 *   So the calling thread is blocked until all published message data is acknowledged or specified
 *   duration elapses.
 *
 * \par 线程安全性
 * \par Thread-Safety
 *   发布者是线程安全对象，因此对它们的所有操作（除了最终确定）都是线程安全的。
 *   Publishers are thread-safe objects, and so are all operations on them except for finalization.
 *   因此，使用相同的发布者并发调用此函数是安全的。
 *   Therefore, it is safe to call this function using the same publisher concurrently.
 *
 * \pre 给定的 `publisher` 必须是有效的发布者，由 rmw_create_publisher() 返回。
 * \pre Given `publisher` must be a valid publisher, as returned by rmw_create_publisher().
 *
 * \param[in] publisher 需要等待所有确认的发布者句柄。
 * \param[in] publisher handle to the publisher that needs to wait for all acked.
 * \param[in] wait_timeout 表示等待所有已发布消息数据被确认的最长时间。
 * \param[in] wait_timeout represents the maximum amount of time to wait for all published message
 *   data were acknowledged.
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_TIMEOUT` 如果等待超时，或
 * \return `RMW_RET_TIMEOUT` if wait timed out, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `publisher` 为 `NULL`，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `publisher` is `NULL`, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `publisher` 实现标识符与此实现不匹配，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `publisher` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` 如果发生未指定的错误，或
 * \return `RMW_RET_ERROR` if an unspecified error occurs, or
 * \return `RMW_RET_UNSUPPORTED` 如果 rmw 实现未实现。
 * \return `RMW_RET_UNSUPPORTED` if the rmw implementation is unimplemented.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_publisher_wait_for_all_acked(
    const rmw_publisher_t* publisher, rmw_time_t wait_timeout);

/// 将 ROS 消息序列化为 rmw_serialized_message_t。
/// Serialize a ROS message into a rmw_serialized_message_t.
/**
 * ROS 消息被序列化为包含在 rmw_serialized_message_t 结构内的字节流。
 * The ROS message is serialized into a byte stream contained within the
 * rmw_serialized_message_t structure.
 * 序列化格式取决于底层实现。
 * The serialization format depends on the underlying implementation.
 *
 * \pre 给定的 ROS 消息必须是有效的非空实例，由调用者初始化并与提供的类型支持匹配。
 * \pre Given ROS message must be a valid non-null instance, initialized
 *   by the caller and matching the provided typesupport.
 * \pre 给定的类型支持必须是有效的非空实例，如 `rosidl` API 所提供。
 * \pre Given typesupport must be a valid non-null instance, as provided
 *   by `rosidl` APIs.
 * \pre 给定的序列化消息必须是有效的非空实例，由调用者初始化。
 * \pre Given serialized message must be a valid non-null instance, initialized
 *   by the caller.
 *
 * <hr>
 * 属性                   | 符合性
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存              | 可能 [1]
 * Allocates Memory   | Maybe [1]
 * 线程安全               | 否
 * Thread-Safe        | No
 * 使用原子操作           | 可能 [2]
 * Uses Atomics       | Maybe [2]
 * 无锁                  | 可能 [2]
 * Lock-Free          | Maybe [2]
 * <i>[1] 如果给定的序列化消息没有足够的容量来容纳 ROS 消息序列化</i>
 * <i>[1] if the given serialized message does not have enough capacity to hold
 *        the ROS message serialization</i>
 * <i>[2] rmw 实现定义，检查实现文档</i>
 * <i>[2] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] ros_message 已输入的 ROS 消息类型
 * \param[in] type_support 输入的 ROS 消息的类型支持
 * \param[out] serialized_message 序列化 ROS 消息的目标
 * \return `RMW_RET_OK` 如果成功，或者
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或者
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_serialize(
    const void* ros_message,                            // 输入的 ROS 消息
    const rosidl_message_type_support_t* type_support,  // 输入的 ROS 消息的类型支持
    rmw_serialized_message_t* serialized_message);      // 序列化 ROS 消息的目标

/// 反序列化一个 ROS 消息。
/**
 * 将给定的 rmw_serialized_message_t 的内部字节流缓冲区反序列化为给定的 ROS 消息。
 * rmw_serialized_message_t 中预期的序列化格式取决于底层实现。
 *
 * \pre 给定的序列化消息必须是有效的非空实例，
 *   例如由 `rmw_serialize()` 返回的实例，与提供的类型支持和 ROS 消息匹配。
 * \pre 给定的类型支持必须是有效的非空实例，由 `rosidl` API 提供。
 * \pre 给定的 ROS 消息必须是有效的非空实例，由调用者初始化。
 *
 * <hr>
 * 属性              | 符合性
 * ------------------ | -------------
 * 分配内存           | 可能 [1]
 * 线程安全           | 否
 * 使用原子操作       | 可能 [2]
 * 无锁               | 可能 [2]
 * <i>[1] 如果给定的 ROS 消息包含无界字段</i>
 * <i>[2] rmw 实现定义，检查实现文档</i>
 *
 * \param[in] serialized_message 包含字节流的序列化消息
 * \param[in] type_support 已键入 ros 消息的类型支持
 * \param[out] ros_message 反序列化 ROS 消息的目标
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
/// Deserialize a ROS message.
/**
 * The given rmw_serialized_message_t's internal byte stream buffer is deserialized
 * into the given ROS message.
 * The serialization format expected in the rmw_serialized_message_t depends on the
 * underlying implementation.
 *
 * \pre Given serialized message must be a valid non-null instance, such
 *   as that returned by `rmw_serialize()`, matching provided typesupport
 *   and ROS message.
 * \pre Given typesupport must be a valid non-null instance, as provided
 *   by `rosidl` APIs.
 * \pre Given ROS message must be a valid non-null instance, initialized
 *   by the caller.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe [1]
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [2]
 * Lock-Free          | Maybe [2]
 * <i>[1] if the given ROS message contains unbounded fields</i>
 * <i>[2] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] serialized_message the serialized message holding the byte stream
 * \param[in] type_support the typesupport for the typed ros message
 * \param[out] ros_message destination for the deserialized ROS message
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation failed, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_deserialize(
    const rmw_serialized_message_t* serialized_message,
    const rosidl_message_type_support_t* type_support,
    void* ros_message);

/// 初始化一个用于后续 `take` 的订阅分配。 (Initialize a subscription allocation to be used with
/// later `take`s.)
/**
 * 这将创建一个分配对象，该对象可以与 rmw_take 方法结合使用，
 * 以更精细地控制内存分配。 (This creates an allocation object that can be used in conjunction with
 * the rmw_take method to perform more carefully control memory allocations.)
 *
 * 这将允许中间件为给定的消息类型和消息边界预先分配正确的内存量。
 * (This will allow the middleware to preallocate the correct amount of memory for a given message
 * type and message bounds.) 由于在此方法中执行分配，因此不需要在 `rmw_take` 方法中分配。 (As
 * allocation is performed in this method, it will not be necessary to allocate in the `rmw_take`
 * method.)
 *
 * \param[in] type_support 要预先分配的消息的类型支持。 (Type support of the message to be
 * preallocated.) \param[in] message_bounds 要预先分配的消息的边界结构。 (Bounds structure of the
 * message to be preallocated.) \param[out] allocation 要传递给 `rmw_take` 的分配结构。 (Allocation
 * structure to be passed to `rmw_take`.) \return 如果成功，则返回 `RMW_RET_OK`，或者 (if
 * successful, or) \return 如果未实现，则返回 `RMW_RET_UNSUPPORTED` (if it's unimplemented) \return
 * 如果参数为空，则返回 `RMW_RET_INVALID_ARGUMENT`，或者 (if an argument is null, or) \return
 * 如果发生意外错误，则返回 `RMW_RET_ERROR`。 (if an unexpected error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_init_subscription_allocation(
    const rosidl_message_type_support_t* type_support,
    const rosidl_runtime_c__Sequence__bound* message_bounds,
    rmw_subscription_allocation_t* allocation);

/// 销毁一个发布者分配对象。 (Destroy a publisher allocation object.)
/**
 * 这将释放由 `rmw_init_subscription_allocation` 分配的内存。
 * (This deallocates memory allocated by `rmw_init_subscription_allocation`.)
 *
 * \param[in] allocation 要销毁的分配对象。 (Allocation object to be destroyed.)
 * \return 如果成功，则返回 `RMW_RET_OK`，或者 (if successful, or)
 * \return 如果未实现，则返回 `RMW_RET_UNSUPPORTED` (if it's unimplemented)
 * \return 如果参数为空，则返回 `RMW_RET_INVALID_ARGUMENT`，或者 (if argument is null, or)
 * \return 如果发生意外错误，则返回 `RMW_RET_ERROR`。 (if an unexpected error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_fini_subscription_allocation(rmw_subscription_allocation_t* allocation);

/// 创建一个订阅并返回该订阅的句柄。
/// Create a subscription and return a handle to that subscription.
/**
 * 此函数可能会失败，并因此返回`NULL`，原因可能有：
 * This function can fail, and therefore return `NULL`, if:
 *   - node 不是此 rmw 实现的有效非空句柄，如由 `rmw_create_node()` 返回
 *   - node is not a valid non-null handle for this rmw implementation,
 *     as returned by `rmw_create_node()`
 *   - type_support 不是有效的非空消息类型支持，如由 `ROSIDL_GET_MSG_TYPE_SUPPORT()` 返回
 *   - type_support is a not valid non-null message type support, as returned by
 *     `ROSIDL_GET_MSG_TYPE_SUPPORT()`
 *   - topic_name 不是有效的非空主题名称，根据 `rmw_validate_full_topic_name()`，如果 ROS
 * 命名空间约定适用
 *   - topic_name is not a valid non-null topic name, according to
 *     `rmw_validate_full_topic_name()` if ROS namespace conventions apply
 *   - qos_profile 不是完全指定的非空配置文件，即没有 UNKNOWN 策略
 *   - qos_profile is not a fully specified non-null profile i.e. no UNKNOWN policies
 *   - subscription_options 不是有效的非空选项集，例如由 `rmw_get_default_subscription_options()`
 * 返回的选项集
 *   - subscription_options is not a valid non-null option set, such as the one
 *     returned by `rmw_get_default_subscription_options()`
 *   - 在订阅创建期间内存分配失败
 *   - memory allocation fails during subscription creation
 *   - 发生未指定的错误
 *   - an unspecified error occurs
 *
 * <hr>
 * 属性                    | 遵循性
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存            | 是
 * Allocates Memory   | Yes
 * 线程安全             | 否
 * Thread-Safe        | No
 * 使用原子操作         | 可能 [1]
 * Uses Atomics       | Maybe [1]
 * 无锁                 | 可能 [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw 实现定义，检查实现文档</i>
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] node 用于注册此订阅的节点句柄
 * \param[in] node Handle to node with which to register this subscription
 * \param[in] type_support 要订阅的消息的类型支持
 * \param[in] type_support Type support for the messages to be subscribed to
 * \param[in] topic_name 要订阅的主题名称，通常是一个完全限定的主题名称，除非 `qos_profile`
 * 配置为避免 ROS 命名空间约定，即创建本地主题订阅
 * \param[in] topic_name Name of the topic to
 * subscribe to, often a fully qualified topic name unless `qos_profile` is configured to avoid ROS
 * namespace conventions i.e. to create a native topic subscription
 * \param[in] qos_policies 此订阅的 QoS 策略
 * \param[in] qos_policies QoS policies for this subscription
 * \param[in] subscription_options 配置此订阅的选项
 * \param[in] subscription_options Options for configuring this subscription
 * \return rmw 订阅句柄，如果出现错误，则返回 `NULL`
 * \return rmw subscription handle, or `NULL` if there was an error
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_subscription_t* rmw_create_subscription(
    const rmw_node_t* node,
    const rosidl_message_type_support_t* type_support,
    const char* topic_name,
    const rmw_qos_profile_t* qos_policies,
    const rmw_subscription_options_t* subscription_options);

/// 释放给定的订阅句柄，回收资源，并释放订阅句柄。
/// Finalize a given subscription handle, reclaim the resources, and deallocate the subscription
/// handle.
/**
 * 如果出现逻辑错误（如 `RMW_RET_INVALID_ARGUMENT` 或
 * `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`），此函数将提前返回，
 * 让给定的订阅句柄保持不变。否则，它将忽略错误，尽可能释放更多资源（包括订阅句柄），并返回
 * `RMW_RET_ERROR`。
 * If a logical error ensues, namely `RMW_RET_INVALID_ARGUMENT` or
 * `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`, this function will return early, leaving the given
 * subscription handle unchanged. Otherwise, it will proceed despite errors, freeing as many
 * resources as it can, including the subscription handle, and return `RMW_RET_ERROR`.
 *
 * 使用已释放的订阅句柄是未定义行为。
 * Usage of a deallocated subscription handle is undefined behavior.
 *
 * \pre 给定的节点必须是订阅注册时使用的节点。
 * \pre Given node must be the one the subscription was registered with.
 *
 * <hr>
 * Attribute            | Adherence
 * -------------------- | -------------
 * Allocates Memory     | No
 * Thread-Safe          | No
 * Uses Atomics         | Maybe [1]
 * Lock-Free            | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] node 注册给定订阅的节点句柄
 * \param[in] node Handle to node with which the given subscription is registered
 * \param[in] subscription 要完成的订阅句柄
 * \param[in] subscription Handle to subscription to be finalized
 * \return 成功时返回 `RMW_RET_OK`，或者
 * \return `RMW_RET_OK` if successful, or
 * \return 如果节点或订阅为 `NULL`，则返回 `RMW_RET_INVALID_ARGUMENT`，或者
 * \return `RMW_RET_INVALID_ARGUMENT` if node or subscription is `NULL`, or
 * \return 如果节点或订阅实现标识符不匹配，则返回 `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，或者
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if node or subscription
 *   implementation identifier does not match, or
 * \return 如果发生意外错误，则返回 `RMW_RET_ERROR`。
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_destroy_subscription(rmw_node_t* node, rmw_subscription_t* subscription);

/// 检索与订阅匹配的发布者数量。 (Retrieve the number of matched publishers to a subscription.)
/**
 * 查询底层中间件以确定有多少发布者与给定订阅匹配。
 * (Query the underlying middleware to determine how many publishers are
 * matched to a given subscription.)
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] subscription 要检查的订阅对象 (the subscription object to inspect)
 * \param[out] publisher_count 匹配的发布者数量 (the number of publishers matched)
 * \return `RMW_RET_OK` 如果成功，或 (if successful, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果任一参数为空，或 (if either argument is null, or)
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果订阅实现标识符不匹配，或 (if subscription
 *   implementation identifier does not match, or)
 * \return `RMW_RET_ERROR` 如果发生意外错误。 (if an unexpected error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_subscription_count_matched_publishers(
    const rmw_subscription_t* subscription, size_t* publisher_count);

/// 获取订阅的实际QoS设置。 (Retrieve the actual qos settings of the subscription.)
/**
 * 查询底层中间件以确定订阅的qos设置。使用RMW_*_SYSTEM_DEFAULT时，只有在创建订阅后才能解析实际配置，并且它取决于底层rmw实现。
 * 如果正在使用的底层设置无法用ROS术语表示，它将设置为RMW_*_UNKNOWN。
 * (Query the underlying middleware to determine the qos settings of the subscription.)
 * (The actual configuration applied when using RMW_*_SYSTEM_DEFAULT can only be resolved after the
 * creation of the subscription, and it depends on the underlying rmw implementation.)
 * (If the underlying setting in use can't be represented in ROS terms, it will be set to
 * RMW_*_UNKNOWN.)
 *
 * \note
 * 该函数并未解析avoid_ros_namespace_conventions字段的值。rcl函数`rcl_subscription_get_actual_qos()`会对其进行解析。
 * (The value of avoid_ros_namespace_conventions field is not resolved with this function.)
 * (The rcl function `rcl_subscription_get_actual_qos()` resolves it.)
 *
 * <hr>
 * 属性 (Attribute)          | 遵循 (Adherence)
 * ------------------ | -------------
 * 分配内存 (Allocates Memory)   | 可能 (Maybe) [1]
 * 线程安全 (Thread-Safe)        | 否 (No)
 * 使用原子操作 (Uses Atomics)   | 可能 (Maybe) [1]
 * 无锁 (Lock-Free)          | 可能 (Maybe) [1]
 * <i>[1] 由rmw实现定义，查阅实现文档 (rmw implementation defined, check the implementation
 * documentation)</i>
 *
 * \param[in] subscription 要检查的订阅对象 (the subscription object to inspect)
 * \param[out] qos 实际的qos设置 (the actual qos settings)
 * \return `RMW_RET_OK` 如果成功 (if successful), or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果任一参数为空 (if either argument is null), or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果订阅实现标识符不匹配 (if subscription
 * implementation identifier does not match), or \return `RMW_RET_ERROR` 如果发生意外错误 (if an
 * unexpected error occurs).
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_subscription_get_actual_qos(
    const rmw_subscription_t* subscription, rmw_qos_profile_t* qos);

/// 设置订阅的内容过滤选项。 (Set the content filter options for the subscription.)
/**
 * 此函数将为给定的订阅设置过滤表达式和表达式参数数组。
 * (This function will set a filter expression and an array of expression parameters
 * for the given subscription.)
 *
 * <hr>
 * 属性（Attribute）          | 遵循性（Adherence）
 * ------------------ | -------------
 * 分配内存（Allocates Memory）   | 否（No）
 * 线程安全（Thread-Safe）        | 否（No）
 * 使用原子操作（Uses Atomics）   | 可能（Maybe [1]）
 * 无锁（Lock-Free）          | 可能（Maybe [1]）
 * <i>[1] 根据实现定义，查看实现文档</i> (implementation defined, check the implementation
 * documentation)
 *
 * \param[in] subscription 要设置内容过滤选项的订阅。（The subscription to set content filter
 * options.） \param[in] options 内容过滤选项。 使用空字符串("") 的 `options.filter_expression`
 * 来重置/清除订阅的内容过滤主题。 (Use `options.filter_expression` with an empty("") string to
 *   reset/clean content filtered topic for the subscription.)
 * \return `RMW_RET_OK` 如果成功，或 (if successful, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果参数为空，或 (if an argument is null, or)
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `subscription` 实现标识符与此实现不匹配，或
 * (if the `subscription` implementation identifier does not match this implementation, or) \return
 * `RMW_RET_UNSUPPORTED` 如果实现不支持内容过滤主题，或 (if the implementation does not support
 * content filtered topic, or) \return `RMW_RET_ERROR` 如果发生未指定的错误。 (if an unspecified
 * error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_subscription_set_content_filter(
    rmw_subscription_t* subscription, const rmw_subscription_content_filter_options_t* options);

/// 检索订阅的内容过滤器选项。 (Retrieve the content filter options of the subscription.)
/**
 * 此函数将根据给定的订阅返回一个内容过滤器选项。 (This function will return a content filter
 * options by the given subscription.)
 *
 * <hr>
 * 属性（Attribute）          | 符合性（Adherence）
 * ------------------ | -------------
 * 分配内存（Allocates Memory）   | 是（Yes）
 * 线程安全（Thread-Safe）        | 否（No）
 * 使用原子操作（Uses Atomics）       | 可能（Maybe）[1]
 * 无锁（Lock-Free）          | 可能（Maybe）[1]
 * <i>[1] 实现定义，检查实现文档（implementation defined, check the implementation
 * documentation）</i>
 *
 * \param[in] subscription 要检查的订阅对象。 (The subscription object to inspect.)
 * \param[in] allocator 用于填充内容过滤器选项的分配器。 (Allocator to be used when populating the
 * content filter options.)
 * \param[out] options 内容过滤器选项。 (The content filter options.)
 * \return `RMW_RET_OK` 如果成功，或 (if successful, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果参数为空，或 (if an argument is null, or)
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `subscription` 实现标识符与此实现不匹配，或
 * (if the `subscription` implementation identifier does not match this implementation, or)
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或 (if memory allocation fails, or)
 * \return `RMW_RET_UNSUPPORTED` 如果实现不支持内容过滤主题，或
 * (if the implementation does not support content filtered topic, or)
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。 (if an unspecified
 * error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_subscription_get_content_filter(
    const rmw_subscription_t* subscription,
    rcutils_allocator_t* allocator,
    rmw_subscription_content_filter_options_t* options);

/// 接收一个传入的 ROS 消息。
/**
 * 从给定订阅中获取已接收的 ROS 消息，将其从内部队列中删除。
 * 即使没有收到 ROS 消息，此函数也会成功，但 `taken` 将为 false。
 *
 * \remarks 同一个 ROS 消息不能被接收两次。
 *   调用者不必处理重复项。
 *
 * <hr>
 * 属性            | 符合性
 * ------------------ | -------------
 * 分配内存         | 可能
 * 线程安全         | 是
 * 使用原子操作     | 可能 [1]
 * 无锁             | 可能 [1]
 *
 * <i>[1] 由实现定义，请查阅实现文档。</i>
 *
 * \par 运行时行为
 *   接收一个 ROS 消息是一个同步操作。
 *   它也是非阻塞的，因为它不会等待新的 ROS 消息到达，
 *   但不能保证是无锁的。
 *   一般来说，实现可能使用锁来同步访问内部资源，
 *   但不允许等待没有保证时间限制的事件（不考虑由于操作系统调度导致的饥饿）。
 *
 * \par 内存分配
 *   是否在接收时分配内存由实现定义。
 *   例如，对通过网络接收的 ROS 消息进行反序列化的实现可能需要在处理
 *   无界（动态大小）字段时执行额外的内存分配。
 *   提供的订阅分配可能会被使用，也可能不会被使用。
 *   查阅实现文档了解在接收 ROS 消息时使用和不使用订阅分配的内存分配保证。
 *
 * \par 线程安全性
 *   订阅是线程安全的对象，除了最终化之外，所有操作都是线程安全的。
 *   因此，从同一个订阅并发地获取消息是安全的。
 *   但是，在接收常规 ROS 消息时：
 *   - 对给定 ROS 消息的访问没有同步。
 *     在 rmw_take() 使用 `ros_message` 时读取或写入它是不安全的。
 *   - 对给定原始数据类型参数的访问没有同步。
 *     在 rmw_take() 使用 `taken` 时读取或写入它是不安全的。
 *   - 对给定的订阅分配的访问没有同步，
 *     除非实现明确另行声明。
 *     因此，在 rmw_take() 使用 `allocation` 时，通常读取或写入它是不安全的。
 *     查阅实现文档了解订阅分配的线程安全性。
 *
 * \pre 给定的 `subscription` 必须是有效的订阅，由 rmw_create_subscription() 返回。
 * \pre 给定的 `ros_message` 必须是有效的消息，其类型与在创建时与 `subscription`
 * 注册的消息类型支持相匹配。 \pre 如果不为 NULL，则给定的 `allocation`
 * 必须是有效的订阅分配，使用与创建时与 `subscription` 注册的消息类型支持相匹配的
 * rmw_subscription_allocation_init() 初始化。 \post 给定的 `ros_message` 将保持有效的消息。
 *   如果此函数由于逻辑错误（如无效参数）或运行时错误而提前失败，则它将保持不变，但如果此函数成功而
 * `taken` 为 false，则它将保持不变。
 *
 * \param[in] subscription 要从中获取消息的订阅。
 * \param[out] ros_message 要写入的类型擦除 ROS 消息。
 * \param[out] taken 指示是否接收了 ROS 消息的布尔标志。
 * \param[in] allocation 要使用的预分配内存。可以为 NULL。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `subscription` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `ros_message` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `taken` 为 NULL，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `subscription`
 *   实现标识符与此实现不匹配，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_take(
    const rmw_subscription_t* subscription,
    void* ros_message,
    bool* taken,
    rmw_subscription_allocation_t* allocation);

/// 接收一个带有元数据的 ROS 消息。
/**
 * 与 rmw_take() 相同，但它还接收 ROS 消息元数据。
 *
 * <hr>
 * 属性              | 遵循
 * ------------------ | -------------
 * 分配内存          | 可能
 * 线程安全          | 是
 * 使用原子操作      | 可能 [1]
 * 无锁              | 可能 [1]
 *
 * <i>[1] 由实现定义，检查实现文档。</i>
 *
 * \par 运行时行为
 *   带有元数据的取 ROS 消息是一个同步操作。
 *   它也是非阻塞的，因为它不会等待新的 ROS 消息到达，
 *   但不能保证是无锁的。
 *   一般来说，实现可以使用锁来同步访问内部资源，
 *   但不允许等待没有保证的时间界限的事件（不考虑由于操作系统调度导致的饥饿效应）。
 *
 * \par 内存分配
 *   是否在取操作中分配内存由实现定义。
 *   例如，对通过网络接收的 ROS 消息进行反序列化的实现可能需要在处理
 *   无界（动态大小）字段时执行额外的内存分配。
 *   如果提供了订阅分配，可能会或者可能不会使用。
 *   查看实现文档以了解在带有和不带订阅分配的情况下获取 ROS 消息时的内存分配保证。
 *
 * \par 线程安全
 *   订阅是线程安全的对象，因此对它们进行的所有操作（除了最后的操作）都是线程安全的。
 *   因此，从同一个订阅并发地获取是安全的。
 *   但是，在获取带有元数据的常规 ROS 消息时：
 *   - 对给定 ROS 消息的访问不是同步的。
 *     在 rmw_take_with_info() 使用它时，读取或写入 `ros_message` 是不安全的。
 *   - 对给定基本数据类型参数的访问不是同步的。
 *     在 rmw_take_with_info() 使用它时，读取或写入 `taken` 是不安全的。
 *   - 对给定 ROS 消息元数据的访问不是同步的。
 *     在 rmw_take_with_info() 使用它时，读取或写入 `message_info` 是不安全的。
 *   - 对给定订阅分配的访问不是同步的，
 *     除非实现特别声明。
 *     因此，在 rmw_take_with_info() 使用它时，通常不安全地读取或写入 `allocation`。
 *     查看实现文档以了解订阅分配的线程安全性。
 *
 * \pre 给定的 `subscription` 必须是有效的订阅，由 rmw_create_subscription() 返回。
 * \pre 给定的 `ros_message` 必须是有效的消息，其类型与在创建时使用 `subscription`
 * 注册的消息类型支持匹配。 \pre 如果不为 NULL，则给定的 `allocation` 必须是有效的订阅分配
 *   使用与在创建时使用 `subscription` 注册的相同消息类型支持的 rmw_subscription_allocation_init()
 * 初始化。 \post 给定的 `ros_message` 将保持为有效消息， `message_info` 为有效消息元数据。
 *   如果此函数由于逻辑错误（如无效参数）提前失败，或者由于运行时错误导致失败，那么这两者都将保持不变，但状态未知。
 *   如果此函数成功，但 `taken` 为 false，这两者也将保持不变。
 *
 * \param[in] subscription 要从中获取 ROS 消息的订阅。
 * \param[out] ros_message 要写入的类型擦除 ROS 消息。
 * \param[out] taken 表示是否获取了 ROS 消息的布尔标志。
 * \param[out] message_info 获取的 ROS 消息元数据。
 * \param[in] allocation 预先分配的内存供使用。可以为 NULL。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `subscription` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `ros_message` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `taken` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `message_info` 为 NULL，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `subscription`
 *   实现标识符与此实现不匹配，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_take_with_info(
    const rmw_subscription_t* subscription,
    void* ros_message,
    bool* taken,
    rmw_message_info_t* message_info,
    rmw_subscription_allocation_t* allocation);

/// 从给定的订阅中获取多个连续的 ROS 消息及其元数据。
/// Take multiple incoming ROS messages with their metadata.
/**
 * 从给定订阅处获取已接收到的一系列连续的 ROS 消息，并将它们从内部队列中移除。
 * Take a sequence of consecutive ROS messages already received by the given
 * subscription, removing them from internal queues.
 * 虽然可能请求 `count` 个 ROS 消息，但订阅可能接收到更少的消息。
 * While `count` ROS messages may be requested, fewer messages may have been
 * received by the subscription.
 * 此函数仅获取已经接收到的内容，并在接收到较少（或零）消息时仍然成功。
 * This function will only take what has been already received, and it will
 * succeed even if fewer (or zero) messages were received.
 * 在这种情况下，只会返回当前可用的消息。
 * In this case, only currently available messages will be returned.
 * `taken` 输出变量表示实际获取的 ROS 消息数量。
 * The `taken` output variable indicates the number of ROS messages actually taken.
 *
 * \remarks 一旦获取，序列中的 ROS 消息不能再次获取。调用者不必处理重复项。
 * \remarks Once taken, ROS messages in the sequence cannot be taken again.
 *   Callers do not have to deal with duplicates.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par 运行时行为
 * \par Runtime behavior
 *   获取一系列 ROS 消息是同步操作。
 *   To take a sequence of ROS messages is a synchronous operation.
 *   它还是非阻塞的，因为它不会等待新的 ROS 消息到达，
 *   It is also non-blocking, to the extent it will not wait for new ROS messages to arrive,
 *   但不能保证是无锁的。
 *   but it is not guaranteed to be lock-free.
 *   一般来说，实现可能使用锁来同步访问内部资源，但不允许等待没有保证时间限制的事件（除了由于 OS
 * 调度导致的饥饿效应）。 Generally speaking, implementations may synchronize access to internal
 * resources using locks but are not allowed to wait for events with no guaranteed time bound
 * (barring the effects of starvation due to OS scheduling).
 *
 * \par 内存分配
 * \par Memory allocation
 *   是否在获取时分配内存是由实现定义的。
 *   It is implementation defined whether memory will be allocated on take or not.
 *   例如，在处理有界（动态大小）字段时，对接收到的 ROS
 * 消息进行反序列化的实现可能需要执行额外的内存分配。 For instance, implementations that deserialize
 * ROS messages received over the wire may need to perform additional memory allocations when
 * dealing with unbounded (dynamically-sized) fields. 可能会使用提供的订阅分配，也可能不使用。 A
 * subscription allocation, if provided, may or may not be used. 检查实现文档以了解在获取 ROS
 * 消息时使用和不使用订阅分配的内存分配保证。 Check the implementation documentation to learn about
 * memory allocation guarantees when taking ROS messages with and without subscription allocations.
 *
 * \par 线程安全性
 * \par Thread-safety
 *   订阅是线程安全的对象，除了最终化操作之外，对它们的所有操作都是线程安全的。
 *   Subscriptions are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   因此，从同一订阅并发获取是安全的。
 *   Therefore, it is safe to take from the same subscription concurrently.
 *   此外，保证获取的 ROS 消息序列是连续的，并保留订阅队列中的顺序，尽管有任何并发获取。
 *   Moreover, the sequence of ROS messages taken is guaranteed to be consecutive and to
 *   preserve the order in the subscription queues, despite any concurrent takes.
 *   但是，在获取带有元数据的 ROS 消息序列时：
 *   However, when taking a sequence of ROS messages with metadata:
 *   - 对给定的 ROS 消息序列的访问不是同步的。在 rmw_take_sequence() 使用 `message_sequence`
 * 时，读取或写入它是不安全的。
 *   - Access to the given ROS message sequence is not synchronized.
 *     It is not safe to read or write `message_sequence` while rmw_take_sequence() uses it.
 *   - 对给定的 ROS 消息元数据序列的访问不是同步的。在 rmw_take_sequence() 使用
 * `message_info_sequence` 时，读取或写入它是不安全的。
 *   - Access to the given ROS message metadata sequence is not synchronized.
 *     It is not safe to read or write `message_info_sequence` while rmw_take_sequence() uses it.
 *   - 对给定基本数据类型参数的访问不是同步的。在 rmw_take_sequence() 使用 `taken`
 * 时，读取或写入它是不安全的。
 *   - Access to given primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `taken` while rmw_take_sequence() uses it.
 *   - 对给定的订阅分配的访问不是同步的，除非实现明确另行声明。
 *   - Access to the given subscription allocation is not synchronized,
 *     unless specifically stated otherwise by the implementation.
 *     因此，在 rmw_take_sequence() 使用 `allocation` 时，通常不安全地读取或写入它。
 *     Thus, it is generally not safe to read or write `allocation` while rmw_take_sequence()
 *     uses it.
 *     查看实现文档以了解订阅分配的线程安全性。
 *     Check the implementation documentation to learn about subscription allocations'
 *     thread-safety.
 *
 * \pre 给定的 `subscription` 必须是有效的订阅，由 rmw_create_subscription() 返回。
 * \pre Given `subscription` must be a valid subscription, as returned
 *   by rmw_create_subscription().
 *
 * \pre 给定的 `message_sequence` 必须是有效的消息序列，由 rmw_message_sequence_init()
 * 初始化并填充与 `subscription` 创建时注册的消息类型支持匹配的 ROS 消息。
 * \pre Given `message_sequence` must be a valid message sequence, initialized by
 * rmw_message_sequence_init() and populated with ROS messages whose type matches the message type
 * support registered with the `subscription` on creation.
 *
 * \pre 给定的 `message_info_sequence` 必须是有效的消息元数据序列，由
 * rmw_message_info_sequence_init() 初始化。
 * \pre Given `message_info_sequence` must be a valid
 * message metadata sequence, initialized by rmw_message_info_sequence_init().
 *
 * \pre 如果不为 NULL，则给定的 `allocation` 必须是有效的订阅分配，使用与 `subscription`
 * 创建时注册的相同消息类型支持初始化的 rmw_subscription_allocation_init()。
 * \pre If not NULL, given `allocation` must be a valid subscription allocation initialized with
 * rmw_subscription_allocation_init() with a message type support that matches the one registered
 * with `subscription` on creation.
 *
 * \post 给定的 `message_sequence` 将保持为有效的消息序列，`message_info_sequence`
 * 为有效的消息元数据序列。如果此函数由于逻辑错误（如无效参数）而提前失败，或由于运行
 * 时错误而失败，两者都将保持不变。如果此函数成功但 `taken` 为零，两者也将保持不变。
 * \post Given `message_sequence` will remain a valid message
 * sequence, and `message_info_sequence`, a valid message metadata sequence. Both will be left
 * unchanged if this function fails early due to a logical error, such as an invalid argument, or in
 * an unknown yet valid state if it fails due to a runtime error. Both will also be left unchanged
 * if this function succeeds but `taken` is zero.
 *
 * \param[in] subscription 从中获取 ROS 消息的订阅。
 * \param[in] count 尝试获取的消息数量。
 * \param[out] message_sequence 要写入的类型擦除 ROS
 * 消息序列。消息序列容量必须足以容纳所有请求的消息，即容量必须等于或大于 `count`。它不必与
 * `message_info_sequence` 匹配。
 * \param[out] message_info_sequence 存储额外消息元数据的序列
 * (Sequence of additional message metadata)
 *   消息信息序列的容量必须足够容纳所有请求的消息元数据，即容量必须等于或大于`count`。
 *   (Message info sequence capacity has to be enough to hold all requested messages
 *   metadata i.e. capacity has to be equal or greater than `count`)
 *   它不需要与`message_sequence`相匹配。(It does not have to match that of `message_sequence`)
 * \param[out] taken 实际从订阅中获取的消息数量 (Number of messages actually taken from
 * subscription)
 * \param[in] allocation 预先分配的内存以供使用，可以为 NULL (Pre-allocated memory to
 * use, may be NULL)
 * \return `RMW_RET_OK` 如果成功 (if successful), or
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败 (if memory allocation fails), or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果
 * `subscription` 为 NULL (if `subscription` is NULL), or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果
 * `message_sequence` 为 NULL (if `message_sequence` is NULL), or
 * \return `RMW_RET_INVALID_ARGUMENT`
 * 如果 `message_info_sequence` 为 NULL (if `message_info_sequence` is NULL), or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `taken` 为 NULL (if `taken` is NULL), or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `count` 为 0 (if `count` is 0), or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `message_sequence` 容量小于 `count` (if
 * `message_sequence` capacity is less than `count`), or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果
 * `message_info_sequence` 容量小于 `count` (if `message_info_sequence` capacity is less than
 * `count`), or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `subscription`
 * 实现标识符与此实现不匹配 (if the `subscription` implementation identifier does not match this
 * implementation), or
 * \return `RMW_RET_ERROR` 如果发生意外错误 (if an unexpected error occurs)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_take_sequence(
    const rmw_subscription_t* subscription,
    size_t count,
    rmw_message_sequence_t* message_sequence,
    rmw_message_info_sequence_t* message_info_sequence,
    size_t* taken,
    rmw_subscription_allocation_t* allocation);

/// 以字节流形式获取传入的 ROS 消息。
/**
 * 获取已经由给定订阅接收的 ROS 消息，并将其从内部队列中移除。
 * 即使没有收到 ROS 消息，此函数也会成功执行，但 `taken` 将为 false。
 * 与 rmw_take() 不同，ROS 消息以序列化形式（字节流）获取。
 * 如果需要，可以使用 rmw_deserialize() 将此字节流反序列化为 ROS 消息。
 *
 * \remarks 同一个 ROS 消息（无论是否序列化）不能被获取两次。
 *   调用者不必处理重复项。
 *
 * <hr>
 * 属性              | 遵循
 * ------------------ | -------------
 * 分配内存          | 可能
 * 线程安全          | 是
 * 使用原子操作      | 可能 [1]
 * 无锁              | 可能 [1]
 *
 * <i>[1] 实现定义，请查阅实现文档。</i>
 *
 * \par 运行时行为
 *   以字节流形式获取 ROS 消息是同步操作。
 *   它还是非阻塞的，因为它不会等待新的 ROS 消息到达，
 *   但不能保证无锁。
 *   一般来说，实现可能使用锁同步访问内部资源，
 *   但不允许等待没有确保时间限制的事件（不考虑由于操作系统调度导致的饥饿）。
 *
 * \par 内存分配
 *   实现定义是否在获取时分配内存。
 *   例如，实现可能需要在处理包含无界（动态大小）字段的 ROS 消息时执行额外的内存分配，
 *   即这些实现可能需要调整给定的字节流大小。
 *   订阅分配（如果提供）可能会或可能不会被使用。
 *   查阅实现文档了解在获取序列化 ROS 消息时有无订阅分配的内存分配保证。
 * \par
 *   对于只包含有界（固定大小）字段的 ROS 消息，调用者可以使用 rmw_get_serialized_message_size()
 * 查询其大小， 并使用 rmw_serialized_message_resize() 相应地调整 `serialized_message`
 * 的大小，以防止在获取时调整字节流的大小。 尽管如此，调整字节流大小并非唯一的内存操作。
 *
 * \par 线程安全性
 *   订阅是线程安全的对象，除了最终化操作，它们上的所有操作都是线程安全的。
 *   因此，从同一个订阅并发获取是安全的。
 *   但是，在获取序列化 ROS 消息时：
 *   - 对于序列化 ROS 消息的给定字节流的访问不是同步的。
 *     在 rmw_take_serialized_message() 使用它时，读取或写入 `serialized_message` 是不安全的。
 *   - 对给定原始数据类型参数的访问不是同步的。
 *     在 rmw_take_serialized_message() 使用它时，读取或写入 `taken` 是不安全的。
 *   - 对给定的订阅分配的访问不是同步的，
 *     除非实现明确声明。
 *     因此，通常在 rmw_take_serialized_message() 使用它时，读取或写入 `allocation` 是不安全的。
 *     查阅实现文档了解订阅分配的线程安全性。
 *
 * \pre 给定的 `subscription` 必须是一个有效的订阅，由 rmw_create_subscription() 返回。
 * \pre 给定的 `serialized_message` 必须是一个有效的序列化消息，由 rmw_serialized_message_init()
 * 初始化。 \pre 如果不为 NULL，则给定的 `allocation` 必须是一个有效的订阅分配，使用与创建时注册到
 * `subscription` 的消息类型支持匹配的 rmw_subscription_allocation_init() 初始化。 \post 给定的
 * `serialized_message` 将保持为有效的序列化消息。
 *   如果此函数由于逻辑错误（如无效参数）提前失败，或者由于运行时错误而失败，它将保持不变。
 *   如果此函数成功，但 `taken` 为 false，它也将保持不变。
 *
 * \param[in] subscription 从中获取 ROS 消息的订阅。
 * \param[out] serialized_message 写入的字节流。
 * \param[out] taken 表示是否获取了 ROS 消息的布尔标志。
 * \param[in] allocation 要使用的预分配内存。可以为 NULL。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `subscription` 为空，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `serialized_message` 为空，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `taken` 为空，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `subscription` 实现标识符与此实现不匹配，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_take_serialized_message(
    const rmw_subscription_t* subscription,
    rmw_serialized_message_t* serialized_message,
    bool* taken,
    rmw_subscription_allocation_t* allocation);

/// 以字节流及其元数据的形式接收传入的 ROS 消息。
/// Take an incoming ROS message as a byte stream with its metadata.
/**
 * 与 rmw_take_serialized_message() 相同，但它还接受 ROS 消息元数据。
 * Same as rmw_take_serialized_message(), except it also takes ROS message metadata.
 *
 * <hr>
 * 属性                  | 符合性
 * -------------------- | -------------
 * 分配内存              | 可能
 * 线程安全              | 是
 * 使用原子操作          | 可能 [1]
 * 无锁                  | 可能 [1]
 *
 * <i>[1] 由实现定义，检查实现文档。</i>
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par 运行时行为
 *   接收一个字节流及其元数据的 ROS 消息是一个同步操作。
 *   To take a ROS message a byte stream with its metadata is a synchronous operation.
 *   它也是非阻塞的，因为它不会等待新的 ROS 消息到达，
 *   It is also non-blocking, to the extent it will not wait for new ROS messages to arrive,
 *   但不能保证无锁。
 *   but it is not guaranteed to be lock-free.
 *   一般来说，实现可能会使用锁来同步访问内部资源，
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par 内存分配
 *   是否在接收时分配内存由实现定义。
 *   It is implementation defined whether memory will be allocated on take or not.
 *   例如，实现可能需要在处理包含无界（动态大小）字段的 ROS 消息时执行额外的内存分配，
 *   For instance, implementations may have to perform additional memory allocations
 *   when dealing with ROS messages that contain unbounded (dynamically-sized) fields
 *   即这些实现可能需要调整给定的字节流大小。
 *   i.e. these implementations may have to resize the given byte stream.
 *   可能会也可能不会使用提供的订阅分配。
 *   A subscription allocation, if provided, may or may not be used.
 *   查看实现文档以了解在接收序列化 ROS 消息时有无订阅分配的内存分配保证。
 *   Check the implementation documentation to learn about memory allocation guarantees
 *   when taking serialized ROS messages with and without subscription allocations.
 * \par
 *   对于只包含有界（固定大小）字段的 ROS 消息，调用者可以使用 rmw_get_serialized_message_size()
 * 查询 their size using rmw_get_serialized_message_size() and resize `serialized_message`
 *   它们的大小，并相应地使用 rmw_serialized_message_resize() 调整 `serialized_message` 的大小，
 *   using rmw_serialized_message_resize() accordingly to prevent byte stream resizing
 *   以防止在接收时调整字节流大小。
 *   on take.
 *   尽管如此，调整字节流大小并不保证是唯一的内存操作。
 *   Nonetheless, byte stream resizing is not guaranteed to be the sole memory operation.
 *
 * \par 线程安全
 *   订阅是线程安全对象，因此除了最终化之外，它们上的所有操作都是线程安全的。
 *   Subscriptions are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   因此，可以同时从同一个订阅中获取。
 *   Therefore, it is safe to take from the same subscription concurrently.
 *   但是，在获取序列化 ROS 消息及其元数据时：
 *   However, when taking serialized ROS messages with metadata:
 *   - 对于序列化 ROS 消息的给定字节流的访问不是同步的。
 *     It is not safe to read or write `serialized_message` while
 *     rmw_take_serialized_message_with_info() uses it.
 *   - 对给定的 ROS 消息元数据的访问不是同步的。
 *     It is not safe to read or write `message_info` while
 *     rmw_take_serialized_message_with_info() uses it.
 *   - 对给定的基本数据类型参数的访问不是同步的。
 *     It is not safe to read or write `taken` while rmw_take_serialized_message_with_info()
 *     uses it.
 *   - 对给定的订阅分配的访问不是同步的，
 *     Access to the given subscription allocation is not synchronized,
 *     除非实现特别说明。
 *     unless specifically stated otherwise by the implementation.
 *     因此，在 rmw_take_serialized_message_with_info() 使用它时，通常不安全地读取或写入
 * `allocation`。 Thus, it is generally not safe to read or write `allocation` while
 *     rmw_take_serialized_message_with_info() uses it.
 *     查看实现文档以了解订阅分配的线程安全性。
 *     Check the implementation documentation to learn about subscription allocations'
 *     thread-safety.
 *
 * \pre 给定的 `serialized_message` 必须是一个有效的序列化消息，由 rmw_serialized_message_init()
 * 初始化。 \pre Given `serialized_message` must be a valid serialized message, initialized by
 *   rmw_serialized_message_init().
 * \pre 如果不为 NULL，则给定的 `allocation` 必须是一个有效的订阅分配，使用与在创建 `subscription`
 * 时注册的消息类型支持匹配的 rmw_subscription_allocation_init() 初始化。 \pre If not NULL, given
 * `allocation` must be a valid subscription allocation initialized with
 * rmw_subscription_allocation_init() with a message type support that matches the one registered
 * with `subscription` on creation. \post 给定的 `serialized_message` 将保持为有效的序列化消息，而
 * `message_info` 为有效的消息元数据。 \post Given `serialized_message` will remain a valid
 * serialized message, and `message_info`, valid message metadata.
 *   如果此函数由于逻辑错误（如无效参数）而提前失败，或者由于运行时错误而失败，则两者都将保持不变，
 *   Both will be left unchanged if this function fails early due to a logical error,
 *   such as an invalid argument, or in an unknown yet valid state if it fails due to a
 *   runtime error.
 *   如果此函数成功但 `taken` 为 false，它们也将保持不变。
 *   It will also be left unchanged if this function succeeds but `taken` is false.
 *
 * \param[in] subscription 从中获取 ROS 消息的订阅。
 * \param[out] serialized_message 要写入的字节流。
 * \param[out] taken 表示是否获取了 ROS 消息的布尔标志。
 * \param[out] message_info 获取的 ROS 消息元数据。
 * \param[in] allocation 要使用的预分配内存。可能为 NULL。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `subscription` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `serialized_message` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `taken` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `message_info` 为 NULL，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `subscription` 实现
 *   标识符与此实现不匹配，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_take_serialized_message_with_info(
    const rmw_subscription_t* subscription,
    rmw_serialized_message_t* serialized_message,
    bool* taken,
    rmw_message_info_t* message_info,
    rmw_subscription_allocation_t* allocation);

/// 通过中间件借用的传入ROS消息。
/// Take an incoming ROS message, loaned by the middleware.
/**
 * 获取已由给定订阅接收的ROS消息，并从内部队列中删除它。
 * Take a ROS message already received by the given subscription, removing it from internal queues.
 * 即使没有收到ROS消息，此函数也会成功，但“taken”将为false。
 * This function will succeed even if no ROS message was received, but `taken` will be false.
 * 被借用的ROS消息由中间件拥有，直到调用者使用rmw_return_loaned_message_from_subscription()返回它，中间件将保持其活动状态（即有效内存空间）。
 * The loaned ROS message is owned by the middleware, which will keep it alive (i.e. in valid
 * memory space) until the caller returns it using rmw_return_loaned_message_from_subscription().
 *
 * \remarks 同一个ROS消息，无论是否借用，都不能被获取两次。调用者不必处理重复项。
 * \remarks The same ROS message, loaned or not, cannot be taken twice.
 *   Callers do not have to deal with duplicates.
 *
 * <hr>
 * 属性          | 遵循
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存   | 可能
 * Allocates Memory   | Maybe
 * 线程安全        | 是
 * Thread-Safe        | Yes
 * 使用原子       | 可能 [1]
 * Uses Atomics       | Maybe [1]
 * 无锁          | 可能 [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] 实现定义，检查实现文档。</i>
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par 运行时行为
 *   获取借用的ROS消息是一个同步操作。
 *   To take a loaned ROS message is a synchronous operation.
 *   它还是非阻塞的，因为它不会等待新的ROS消息到达，也不会等待内部内存借用池（如果有）补充，但不能保证无锁。
 *   It is also non-blocking, to the extent it will not wait for new ROS messages to arrive
 *   nor for internal memory loaning pools, if any, to be replenished, but it is not
 *   guaranteed to be lock-free.
 *   一般来说，实现可能使用锁同步访问内部资源，但不允许等待没有保证时间界限的事件（除了由于操作系统调度导致的饥饿效应）。
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par 内存分配
 *   是否在获取时分配内存由实现定义。
 *   It is implementation defined whether memory will be allocated on take or not.
 *   例如，对接收到的ROS消息进行反序列化的实现可能需要在处理无界（动态大小）字段时执行额外的内存分配。
 *   For instance, implementations that deserialize ROS messages received over
 *   the wire may need to perform additional memory allocations when dealing with
 *   unbounded (dynamically-sized) fields.
 *   可能会使用也可能不会使用订阅分配（如果提供的话）。
 *   A subscription allocation, if provided, may or may not be used.
 *   查看实现文档，了解在获取借用的ROS消息时，有无订阅分配的内存分配保证。
 *   Check the implementation documentation to learn about memory allocation
 *   guarantees when taking loaned ROS messages with and without subscription allocations.
 *
 * \par 线程安全性
 *   订阅是线程安全的对象，因此所有对它们的操作（除了最终化）都是线程安全的。
 *   Subscriptions are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   因此，从同一订阅并发获取是安全的。
 *   Therefore, it is safe to take from the same subscription concurrently.
 *   但是，在获取借用的ROS消息时：
 *   However, when taking loaned ROS messages:
 *   -
 * 对给定基本数据类型参数的访问不是同步的。在rmw_take_loaned_message()使用它们时，读取或写入“taken”和“loaned_message”是不安全的。
 *   - Access to given primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `taken` nor `loaned_message`
 *     while rmw_take_loaned_message() uses them.
 *   - 对给定订阅分配的访问不是同步的，除非实现特别声明。
 *   - Access to the given subscription allocation is not synchronized,
 *     unless specifically stated otherwise by the implementation.
 *     因此，在rmw_take_loaned_message()使用它时，通常不安全地读取或写入“allocation”。
 *     Thus, it is generally not safe to read or write `allocation` while
 *     rmw_take_loaned_message() uses it.
 *     查看实现文档以了解订阅分配的线程安全性。
 *     Check the implementation documentation to learn about subscription allocations'
 *     thread-safety.
 *
 * \pre 给定的“subscription”必须是有效的订阅，由rmw_create_subscription()返回。
 * \pre Given `subscription` must be a valid subscription, as returned
 *   by rmw_create_subscription().
 * \pre
 * 如果不为NULL，则给定的“allocation”必须是一个有效的订阅分配，用rmw_subscription_allocation_init()初始化，并具有与创建时在“subscription”上注册的消息类型支持匹配的消息类型支持。
 * \pre If not NULL, given `allocation` must be a valid subscription allocation initialized
 *   with rmw_subscription_allocation_init() with a message type support that matches the
 *   one registered with `subscription` on creation.
 * \post 如果此函数成功且“taken”为true，则给定的“loaned_message”将保持不变，或指向有效的消息。
 * \post Given `loaned_message` will remain unchanged, or point to a valid message if
 *   this function was successful and `taken` is true.
 *
 * \param[in] subscription 从中获取ROS消息的订阅。
 * \param[in] subscription Subscription to take ROS message from.
 * \param[inout] loaned_message 指向由中间件借出并取走的类型擦除ROS消息的指针。
 * \param[inout] loaned_message Pointer to type erased ROS message taken
 *   and loaned by the middleware.
 * \param[out] taken 指示是否获取了ROS消息的布尔标志。
 * \param[out] taken Boolean flag indicating if a ROS message was taken or not.
 * \param[in] allocation 使用预先分配的内存。可能为NULL。
 * \param[in] allocation Pre-allocated memory to use. May be NULL.
 * \return 如果成功，则返回“RMW_RET_OK”，或
 * \return `RMW_RET_OK` if successful, or
 * \return 如果内存分配失败，则返回“RMW_RET_BAD_ALLOC”，或
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return 如果“subscription”为空，则返回“RMW_RET_INVALID_ARGUMENT”，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `subscription` is NULL, or
 * \return 如果“loaned_message”为空，则返回“RMW_RET_INVALID_ARGUMENT”，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `loaned_message` is NULL, or
 * \return 如果“*loaned_message”不为空（以防止泄漏），则返回“RMW_RET_INVALID_ARGUMENT”，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `*loaned_message` is not NULL (to prevent leaks), or
 * \return 如果“taken”为空，则返回“RMW_RET_INVALID_ARGUMENT”，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `taken` is NULL, or
 * \return 如果订阅实现标识符与该实现不匹配，则返回“RMW_RET_INCORRECT_RMW_IMPLEMENTATION”，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `subscription` implementation
 *   identifier does not match this implementation, or
 * \return 如果实现不支持借用的ROS消息，则返回“RMW_RET_UNSUPPORTED”，或
 * \return `RMW_RET_UNSUPPORTED` if the implementation does not support loaned ROS messages, or
 * \return 如果发生意外错误，则返回“RMW_RET_ERROR”。
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_take_loaned_message(
    const rmw_subscription_t* subscription,
    void** loaned_message,
    bool* taken,
    rmw_subscription_allocation_t* allocation);

/// 借用一条消息并附带其额外的消息信息。
/**
 * 与 rmw_take_loaned_message() 相同，但它还获取 ROS 消息元数据。
 *
 * <hr>
 * 属性                | 遵循
 * ------------------ | -------------
 * 分配内存            | 可能
 * 线程安全            | 是
 * 使用原子操作        | 可能 [1]
 * 无锁                | 可能 [1]
 *
 * <i>[1] 实现定义，请检查实现文档。</i>
 *
 * \par 运行时行为
 *   借用带有元数据的 ROS 消息是一个同步操作。
 *   它还是非阻塞的，因为它不会等待新的 ROS 消息到达
 *   也不会等待内部内存借用池（如果有）被补充，但不能保证
 *   是无锁的。
 *   一般来说，实现可以使用锁来同步访问内部资源
 *   但不允许等待没有保证时间界限的事件（除了
 *   由于操作系统调度导致的饥饿效应）。
 *
 * \par 内存分配
 *   是否在 take 时分配内存取决于实现。
 *   例如，对接收到的 ROS 消息进行反序列化的实现
 *   在处理无边界（动态大小）字段时可能需要执行额外的内存分配。
 *   订阅分配（如果提供）可能会或可能不会被使用。
 *   查看实现文档以了解在借用带有和没有订阅分配的 ROS 消息时
 *   内存分配保证。
 *
 * \par 线程安全性
 *   订阅是线程安全对象，除了最终化操作，
 *   对它们的所有操作都是如此。
 *   因此，从同一订阅并发获取是安全的。
 *   但是，在获取带元数据的借用 ROS 消息时：
 *   - 对给定基本数据类型参数的访问不是同步的。
 *     在 rmw_take_loaned_message_with_info() 使用它们时，读取或写入 `taken` 或 `loaned_message`
 *     是不安全的。
 *   - 对给定的 ROS 消息元数据的访问不是同步的。
 *     在 rmw_take_loaned_message_with_info() 使用它时，读取或写入 `message_info` 是不安全的。
 *   - 对给定的订阅分配的访问不是同步的，
 *     除非实现特别另行声明。
 *     因此，在 rmw_take_loaned_message_with_info() 使用它时，通常情况下，读取或写入 `allocation`
 * 是不安全的。 查看实现文档以了解订阅分配的线程安全性。
 *
 * \pre 给定的 `subscription` 必须是一个有效的订阅，由 rmw_create_subscription() 返回。
 * \pre 如果不为 NULL，则给定的 `allocation` 必须是一个有效的订阅分配，使用与 `subscription`
 * 创建时注册的相同消息类型支持初始化的 rmw_subscription_allocation_init()。 \post
 * 如果此函数成功并且 `taken` 为 true，则给定的 `loaned_message` 将保持不变，或指向一个有效的消息。
 * \post 给定的 `message_info` 将保持有效的消息元数据。
 *   如果此函数由于逻辑错误（如无效参数）而提前失败，它将保持不变，
 *   或者如果由于运行时错误而失败，它将处于未知但有效的状态。
 *   如果此函数成功但 `taken` 为 false，它也将保持不变。
 *
 * \param[in] subscription 从中获取 ROS 消息的订阅。
 * \param[inout] loaned_message 中间件借用和借出的类型擦除 ROS 消息的指针。
 * \param[out] taken 布尔标志，表示是否获取了 ROS 消息。
 * \param[out] message_info 获取的 ROS 消息元数据。
 * \param[in] allocation 要使用的预先分配的内存。可能为 NULL。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `subscription` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `loaned_message` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `*loaned_message` 不为 NULL 以防止泄漏，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `taken` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `message_info` 为 NULL，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `subscription` 实现
 *   标识符与此实现不匹配，或
 * \return `RMW_RET_UNSUPPORTED` 如果实现不支持借用的 ROS 消息，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_take_loaned_message_with_info(
    const rmw_subscription_t* subscription,
    void** loaned_message,
    bool* taken,
    rmw_message_info_t* message_info,
    rmw_subscription_allocation_t* allocation);

/// 返回一个从订阅中借用的 ROS 消息。 (Return a loaned ROS message previously taken from a
/// subscription.)
/**
 * 告诉中间件，先前借用的 ROS 消息不再需要调用者使用。
 * (Tells the middleware that previously loaned ROS message is no longer needed by the caller.)
 * 如果此函数由于逻辑错误（如无效参数）而提前失败，
 * (If this function fails early due to a logical error, such as an invalid argument,)
 * 则借用的 ROS 消息将保持不变。
 * (the loaned ROS message will be left unchanged.)
 * 否则，ROS 消息的所有权将归还给中间件。
 * (Otherwise, ownership of the ROS message will be given back to the middleware.)
 * 中间件将决定如何处理返回的 ROS 消息。
 * (It is up to the middleware what will be made of the returned ROS message.)
 * 在归还后再使用借用的 ROS 消息是未定义的行为。
 * (It is undefined behavior to use a loaned ROS message after returning it.)
 *
 * <hr>
 * 属性                | 遵循性 (Attribute          | Adherence)
 * ------------------ | -------------
 * 分配内存            | 否 (Allocates Memory   | No)
 * 线程安全            | 是 (Thread-Safe        | Yes)
 * 使用原子操作        | 可能 [1] (Uses Atomics       | Maybe [1])
 * 无锁                | 可能 [1] (Lock-Free          | Maybe [1])
 *
 * <i>[1] 由实现定义，检查实现文档。</i>
 * (implementation defined, check implementation documentation.)
 *
 * \par 运行时行为 (Runtime behavior)
 *   归还借用的 ROS 消息是一个同步操作。
 *   (To return a loaned ROS message is a synchronous operation.)
 *   它也是非阻塞的，但不能保证是无锁的。
 *   (It is also non-blocking, but it is not guaranteed to be lock-free.)
 *   一般来说，实现可能会使用锁同步访问内部资源，
 *   (Generally speaking, implementations may synchronize access to internal resources using)
 *   但不允许等待没有保证时间界限的事件（不考虑由于操作系统调度引起的饥饿效应）。
 *   (locks but are not allowed to wait for events with no guaranteed time bound (barring)
 *   (the effects of starvation due to OS scheduling).)
 *
 * \par 线程安全性 (Thread-safety)
 *   订阅是线程安全的对象，除最终化外，它们上的所有操作都是线程安全的。
 *   (Subscriptions are thread-safe objects, and so are all operations on them except for)
 *   (finalization.)
 *   因此，将借用的 ROS 消息并发归还给相同的订阅是安全的。
 *   (Therefore, it is safe to return loaned ROS messages to the same subscription concurrently.)
 *   但是，由于借用的 ROS 消息的所有权归还给中间件且此传输未同步，
 *   (However, since ownership of the loaned ROS message is given back to middleware and this)
 *   (transfer is not synchronized,)
 *   并发归还相同的借用的 ROS 消息是不安全的。
 *   (it is not safe to return the same loaned ROS message concurrently.)
 *
 * \pre 给定的 `subscription` 必须是有效订阅，如 rmw_create_subscription() 返回的订阅。
 * (\pre Given `subscription` must be a valid subscription, as returned)
 * (by rmw_create_subscription().)
 * \pre 给定的 `loaned_message` 必须是借用的 ROS 消息，之前从使用
 * (rmw_take_loaned_message() 或 rmw_take_loaned_message_with_info() 的 `subscription` 中获取。
 * (\pre Given `loaned_message` must be a loaned ROS message, previously taken from)
 * (`subscription` using rmw_take_loaned_message() or rmw_take_loaned_message_with_info().)
 *
 * \param[in] subscription 从中获取并借用的 ROS 消息的订阅。
 * (\param[in] subscription Subscription the ROS message was taken and loaned from.)
 * \param[in] loaned_message 要归还给中间件的借用类型擦除的 ROS 消息。
 * (\param[in] loaned_message Loaned type erased ROS message to be returned to the middleware.)
 * \return 如果成功，则返回 `RMW_RET_OK`，或
 * (\return `RMW_RET_OK` if successful, or)
 * \return 如果 `subscription` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或
 * (\return `RMW_RET_INVALID_ARGUMENT` if `subscription` is NULL, or)
 * \return 如果 `loaned_message` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或
 * (\return `RMW_RET_INVALID_ARGUMENT` if `loaned_message` is NULL, or)
 * \return 如果 `subscription` 实现标识符与此实现不匹配，则返回
 * `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，或
 * (\return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `subscription` implementation)
 * (identifier does not match this implementation, or)
 * \return 如果实现不支持借用的 ROS 消息，则返回 `RMW_RET_UNSUPPORTED`，或
 * (\return `RMW_RET_UNSUPPORTED` if the implementation does not support loaned ROS messages, or)
 * \return 如果发生意外错误，则返回 `RMW_RET_ERROR`。
 * (\return `RMW_RET_ERROR` if an unexpected error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_return_loaned_message_from_subscription(
    const rmw_subscription_t* subscription, void* loaned_message);

/// 创建一个服务客户端，可以向服务服务器发送请求并接收回复。
/// Create a service client that can send requests to and receive replies from a service server.
/**
 * 此函数可能失败，并因此返回 `NULL`，具体原因如下：
 * This function can fail, and therefore return `NULL`, if:
 *   - `node` 是 `NULL`，或者
 *   - `node` is `NULL`, or
 *   - `node` 不属于此实现
 *      即它没有匹配的实现标识符，或者
 *   - `node` does not belong to this implementation
 *      i.e. it does not have a matching implementation identifier, or
 *   - `type_support` 是 `NULL`，或者
 *   - `type_support` is `NULL`, or
 *   - `service_name` 是 `NULL`，或者
 *   - `service_name` is `NULL`, or
 *   - `service_name` 是空字符串，或者
 *   - `service_name` is an empty string, or
 *   - （如果适用 ROS 命名空间约定）根据 rmw_validate_full_topic_name() 的定义，`service_name`
 * 无效，或者
 *   - (if ROS namespace conventions apply) `service_name` is invalid by
 *     rmw_validate_full_topic_name() definition, or
 *   - `qos_profile` 是 `NULL`，或者
 *   - `qos_profile` is `NULL`, or
 *   - `qos_profile` 具有无效或未知策略，或者
 *   - `qos_profile` has invalid or unknown policies, or
 *   - 在服务客户端创建期间内存分配失败，或者
 *   - memory allocation fails during service client creation, or
 *   - 发生未指定的错误。
 *   - an unspecified error occurs.
 *
 * <hr>
 * 属性                  | 符合
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存             | 是
 * Allocates Memory   | Yes
 * 线程安全             | 否
 * Thread-Safe        | No
 * 使用原子操作         | 可能[1]
 * Uses Atomics       | Maybe [1]
 * 无锁                 | 可能[1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw 实现定义，检查实现文档</i>
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \pre 给定的 `node` 必须是一个有效节点，由 rmw_create_node() 返回。
 * \pre Given `node` must be a valid node, as returned by rmw_create_node().
 * \pre 给定的 `type_support` 必须是一个有效的 `rosidl` 服务类型支持，如
 * ROSIDL_GET_SRV_TYPE_SUPPORT() 返回。 \pre Given `type_support` must be a valid `rosidl` service
 * type support, as returned by ROSIDL_GET_SRV_TYPE_SUPPORT().
 *
 * \param[in] node 用于注册此服务客户端的节点。
 * \param[in] node Node with which to register this service client.
 * \param[in] type_support 要使用的服务的类型支持。
 * \param[in] type_support Type support of the service to be used.
 * \param[in] service_name 要使用的服务名称，通常是完全限定的服务名称，除非 `qos_profile` 配置为避免
 * ROS 命名空间约定，即创建本地服务客户端。 \param[in] service_name Name of the service to be used,
 * often a fully qualified service name unless `qos_profile` is configured to avoid ROS namespace
 * conventions i.e. to create a native service client. \param[in] qos_policies 此服务客户端连接的
 * QoS 策略。 \param[in] qos_policies QoS policies for this service client's connections. \return
 * rmw 服务客户端句柄，如果出现错误，则返回 `NULL`。 \return rmw service client handle, or `NULL` if
 * there was an error.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_client_t* rmw_create_client(
    const rmw_node_t* node,
    const rosidl_service_type_support_t* type_support,
    const char* service_name,
    const rmw_qos_profile_t* qos_policies);

/// 销毁并从其节点中注销服务客户端 (Destroy and unregister a service client from its node).
/**
 * 此函数将回收所有相关资源，包括服务客户端本身 (This function will reclaim all associated
 * resources, including the service client itself). 使用已销毁的服务客户端是未定义行为 (Use of a
 * destroyed service client is undefined behavior). 如果出现逻辑错误，例如
 * `RMW_RET_INVALID_ARGUMENT` 或
 * `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，此函数将提前返回，保持给定的服务客户端不变 (This function
 * will return early if a logical error, such as `RMW_RET_INVALID_ARGUMENT` or
 * `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`, ensues, leaving the given service client unchanged).
 * 否则，即使发生错误，它也会继续进行 (Otherwise, it will proceed despite errors).
 *
 * <hr>
 * 属性 (Attribute)          | 符合性 (Adherence)
 * ------------------ | -------------
 * 分配内存 (Allocates Memory)   | 否 (No)
 * 线程安全 (Thread-Safe)        | 否 (No)
 * 使用原子操作 (Uses Atomics)       | 可能 [1] (Maybe [1])
 * 无锁 (Lock-Free)          | 可能 [1] (Maybe [1])
 * <i>[1] rmw 实现定义,请查阅实现文档 (rmw implementation defined, check the implementation
 * documentation)</i>
 *
 * \pre 给定的 `node` 必须是服务客户端注册的节点 (Given `node` must be the one the service client
 * was registered with).
 * \pre 给定的 `client` 必须是有效的服务客户端，由 rmw_create_service() 返回
 * (Given `client` must be a valid service client, as returned by rmw_create_service()).
 *
 * \param[in] node 注册了给定服务客户端的节点 (Node with which the given service client is
 * registered).
 * \param[in] client 要销毁的服务客户端 (Service client to be destroyed).
 * \return 如果成功，则返回 `RMW_RET_OK`, 或者 (if successful, or)
 * \return 如果 `node` 为 `NULL`，则返回
 * `RMW_RET_INVALID_ARGUMENT`, 或者 (if `node` is `NULL`, or)
 * \return 如果 `client` 为
 * `NULL`，则返回 `RMW_RET_INVALID_ARGUMENT`, 或者 (if `client` is `NULL`, or)
 * \return 如果 `node`
 * 的实现标识符与此实现不匹配，则返回 `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`, 或者 (if the `node`
 *   implementation identifier does not match this implementation, or)
 * \return 如果 `client` 的实现标识符与此实现不匹配，则返回 `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`,
 * 或者 (if the `client` implementation identifier does not match this implementation, or)
 * \return 如果发生未指定的错误，则返回 `RMW_RET_ERROR`. (if an unspecified error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_destroy_client(rmw_node_t* node, rmw_client_t* client);

/// 发送一个 ROS 服务请求 (Send a ROS service request).
/**
 * 使用给定的客户端，向一个或多个具有匹配 QoS 策略的服务服务器发送 ROS 服务请求。
 * (Send a ROS service request to one or more service servers, with matching QoS policies, using the
 * given client.)
 *
 * \note 实现定义了可能有多少个服务服务器可以获得，并可能对相同的请求做出反应，考虑到 ROS
 * 图中可能有多个相同服务的服务器。 (It is implementation defined how many service servers may get,
 * and potentially react to, the same request, considering there may be more than one server for the
 * same service in the ROS graph.)
 *
 * 成功时，此函数将返回一个序列号。(On success, this function will return a sequence number.)
 * 调用者需要保存返回的序列号，以便将刚发送的 ROS 服务请求与将来的 ROS 服务响应（使用
 * rmw_take_response() 获取）配对。 (It is up to callers to save the returned sequence number to
 * pair the ROS service request just sent with future ROS service responses (taken using
 * rmw_take_response()).)
 *
 * <hr>
 * 属性 (Attribute)          | 遵循 (Adherence)
 * ------------------ | -------------
 * 分配内存 (Allocates Memory)   | 可能 (Maybe)
 * 线程安全 (Thread-Safe)        | 是 (Yes)
 * 使用原子操作 (Uses Atomics)   | 可能 [1] (Maybe [1])
 * 无锁 (Lock-Free)          | 可能 [1] (Maybe [1])
 *
 * <i>[1] 实现定义，检查实现文档。(implementation defined, check implementation documentation.)</i>
 *
 * \par 运行时行为 (Runtime behavior)
 *   发送 ROS 服务请求是同步还是异步、阻塞还是非阻塞操作，由实现定义。
 *   (It is implementation defined whether sending a ROS service request is a synchronous or
 * asynchronous, and blocking or non-blocking, operation.)
 *   但是，异步实现不允许在此函数返回后访问给定的 ROS 服务请求。
 *   (However, asynchronous implementations are not allowed to access the given ROS service request
 * after this function returns.) 查看实现文档以了解请求行为。(Check the implementation documentation
 * to learn about request behavior.)
 *
 * \par 内存分配 (Memory allocation)
 *   发送时是否分配内存由实现定义。(It is implementation defined whether memory will be allocated on
 * send or not.) 例如，序列化 ROS
 * 服务请求的实现可能需要在处理无界（动态大小）字段时执行额外的内存分配。(For instance,
 * implementations that serialize ROS service requests may need to perform additional memory
 * allocations when dealing with unbounded (dynamically-sized) fields.)
 *
 * \par 线程安全性 (Thread-safety)
 *   服务客户端是线程安全对象，除了最终确定之外，所有对它们的操作都是线程安全的。
 *   (Service clients are thread-safe objects, and so are all operations on them except for
 * finalization.) 因此，使用相同的服务客户端并发发送请求是安全的。(Therefore, it is safe to send
 * requests using the same service client concurrently.) 但是 (However)：
 *   - 对给定 ROS 服务请求的访问是只读的，但不是同步的。并发 `ros_request`
 * 读取是安全的，但并发读取和写入不是。 (Access to the given ROS service request is read-only but it
 * is not synchronized. Concurrent `ros_request` reads are safe, but concurrent reads and writes are
 * not.)
 *   - 对给定原始数据类型参数的访问不是同步的。在 rmw_send_request() 使用 `sequence_id`
 * 时，读取或写入 `sequence_id` 是不安全的。 (Access to given primitive data-type arguments is not
 * synchronized. It is not safe to read or write `sequence_id` while rmw_send_request() uses it.)
 *
 * \pre 给定的 `client` 必须是有效的客户端，由 rmw_create_client() 返回。(Given `client` must be a
 * valid client, as returned by rmw_create_client().) \pre 给定的 `ros_request`
 * 必须是有效的服务请求，其类型与创建时使用 `client` 注册的服务类型支持匹配。 (Given `ros_request`
 * must be a valid service request, whose type matches the service type support registered with the
 * `client` on creation.)
 *
 * \param[in] client 发送请求的服务客户端。(Service client to send a request with.)
 * \param[in] ros_request 要发送的 ROS 服务请求。(ROS service request to be sent.)
 * \param[out] sequence_id 刚发送的 `ros_request` 的序列号，即唯一标识号，在成功时填充。(Sequence
 * number for the `ros_request` just sent i.e. a unique identification number for it, populated on
 * success.) \return `RMW_RET_OK` 如果成功，或 (if successful, or) \return `RMW_RET_BAD_ALLOC`
 * 如果内存分配失败，或 (if memory allocation fails, or) \return `RMW_RET_INVALID_ARGUMENT` 如果
 * `client` 为 NULL，或 (if `client` is NULL, or) \return `RMW_RET_INVALID_ARGUMENT` 如果
 * `ros_request` 为 NULL，或 (if `ros_request` is NULL, or) \return `RMW_RET_INVALID_ARGUMENT` 如果
 * `sequence_id` 为 NULL，或 (if `sequence_id` is NULL, or) \return
 * `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `client` 的实现标识符与此实现不匹配，或 (if the
 * `client` implementation identifier does not match this implementation, or) \return
 * `RMW_RET_ERROR` 如果发生意外错误。(if an unexpected error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_send_request(
    const rmw_client_t* client, const void* ros_request, int64_t* sequence_id);

/// 接收一个传入的ROS服务响应。
/**
 * 从给定的服务服务器接收一个已经收到的ROS服务响应，将其从内部队列中移除。
 * 响应头（即元数据）也将被检索，至少包含写入者guid和序列号。
 * 写入者guid和序列号允许调用者为每个远程服务服务器配对一个ROS服务响应及其相应的ROS服务请求，
 * 之前使用rmw_send_request()发送。
 *
 * \note 实现定义了一个给定请求可能获得多少响应，考虑到在ROS图中同一个服务可能有多个服务器。
 *
 * 即使没有收到ROS服务请求，此函数也会成功，但`taken`将为false。
 *
 * \remarks 不能两次获取相同的ROS服务响应。调用者不必处理重复项。
 *
 * <hr>
 * 属性              | 遵循
 * ----------------- | -------------
 * 分配内存          | 可能
 * 线程安全          | 是
 * 使用原子操作      | 可能 [1]
 * 无锁              | 可能 [1]
 *
 * <i>[1] 实现定义，请检查实现文档。</i>
 *
 * \par 运行时行为
 *   获取 ROS 服务响应是一个同步操作。
 *   它还是非阻塞的，因为它不会等待新的 ROS 服务响应到达，但不能保证是无锁的。
 *   一般来说，实现可以使用锁来同步访问内部资源，但不允许等待没有保证时间限制的事件
 *   （由于操作系统调度引起的饥饿效应除外）。
 *
 * \par 内存分配
 *   实现定义是否在 take 上分配内存。
 *   例如，对接收到的 ROS 服务响应进行反序列化的实现可能需要在处理无界（动态大小）字段时
 *   执行额外的内存分配。
 *
 * \par 线程安全
 *   服务客户端是线程安全的对象，因此对它们的所有操作都是线程安全的，除了最终化操作。
 *   因此，从同一个服务客户端同时获取响应是安全的。
 *   但是：
 *   - 对给定的ROS服务响应的访问未同步。在 rmw_take_request() 使用它时，读取或写入 `ros_response`
 * 是不安全的。
 *   - 对给定的ROS服务响应头的访问未同步。在 rmw_take_response() 使用它时，读取或写入
 * `response_header` 是不安全的。
 *   - 对给定原始数据类型参数的访问未同步。在 rmw_take_response() 使用它时，读取或写入 `taken`
 * 是不安全的。
 *
 * \pre 给定的 `client` 必须是有效的客户端，由 rmw_create_client() 返回。
 * \pre 给定的 `ros_response` 必须是有效的服务响应，其类型与在创建时使用 `client`
 * 注册的服务类型支持匹配。 \post 给定的 `ros_response` 将保持有效的服务响应。
 *   如果由于逻辑错误（如无效参数）导致此函数提前失败，或者由于运行时错误导致未知但有效状态失败，它将保持不变。
 *   如果此函数成功但 `taken` 为 false，它也将保持不变。
 *
 * \param[in] client 要从中获取响应的服务客户端。
 * \param[out] request_header 要写入的服务响应头。
 * \param[out] ros_response 要写入的类型擦除 ROS 服务响应。
 * \param[out] taken 布尔标志，表示是否获取了 ROS 服务响应。
 * \return `RMW_RET_OK`，如果成功，或
 * \return `RMW_RET_BAD_ALLOC`，如果内存分配失败，或
 * \return `RMW_RET_INVALID_ARGUMENT`，如果 `client` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT`，如果 `response_header` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT`，如果 `ros_response` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT`，如果 `taken` 为 NULL，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，如果 `client`
 *   实现标识符与此实现不匹配，或
 * \return `RMW_RET_ERROR`，如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_take_response(
    const rmw_client_t* client,
    rmw_service_info_t* request_header,
    void* ros_response,
    bool* taken);

/// 获取客户端请求发布器的实际qos设置。 (Retrieve the actual qos settings of the client's request
/// publisher.)
/**
 * 查询底层中间件以确定客户端请求发布器的qos设置。
 * (Query the underlying middleware to determine the qos settings of the client's request
 * publisher.) 使用 RMW_*_SYSTEM_DEFAULT 时，只有在创建客户端后才能解析实际配置， (The actual
 * configuration applied when using RMW_*_SYSTEM_DEFAULT can only be resolved after the creation of
 * the client,) 并且它取决于底层的rmw实现。 (and it depends on the underlying rmw implementation.)
 * 如果正在使用的底层设置无法用ROS术语表示，
 * (If the underlying setting in use can't be represented in ROS terms,)
 * 它将被设置为 RMW_*_UNKNOWN。
 * (it will be set to RMW_*_UNKNOWN.)
 *
 * \note 使用此函数未解析avoid_ros_namespace_conventions字段的值。
 *   (\note The value of avoid_ros_namespace_conventions field is not resolved with this function.)
 *   rcl函数`rcl_client_request_publisher_get_actual_qos()`解析它。
 *   (The rcl function `rcl_client_request_publisher_get_actual_qos()` resolves it.)
 *
 * <hr>
 * 属性              | 遵循
 * ------------------ | -------------
 * 分配内存            | 可能 [1]
 * 线程安全            | 否
 * 使用原子操作        | 可能 [1]
 * 无锁                | 可能 [1]
 * <i>[1] 由rmw实现定义，检查实现文档</i>
 * (<i>[1] rmw implementation defined, check the implementation documentation</i>)
 *
 * \param[in] client 要检查的客户端对象
 * (\param[in] client the client object to inspect)
 * \param[out] qos 实际qos设置
 * (\param[out] qos the actual qos settings)
 * \return `RMW_RET_OK` 如果成功, 或者
 * (\return `RMW_RET_OK` if successful, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果任一参数为null, 或者
 * (\return `RMW_RET_INVALID_ARGUMENT` if either argument is null, or)
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果客户端实现标识符不匹配, 或者
 * (\return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if client implementation identifier does not
 * match, or) \return `RMW_RET_ERROR` 如果发生意外错误。
 * (\return `RMW_RET_ERROR` if an unexpected error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_client_request_publisher_get_actual_qos(
    const rmw_client_t* client, rmw_qos_profile_t* qos);

/// 获取客户端响应订阅的实际qos设置。 (Retrieve the actual qos settings of the client's response
/// subscription.)
/**
 * 查询底层中间件以确定客户端响应订阅的qos设置。
 * (Query the underlying middleware to determine the qos settings of the client's response
 * subscription.) 使用 RMW_*_SYSTEM_DEFAULT 时，只有在创建客户端后才能解析实际配置， (The actual
 * configuration applied when using RMW_*_SYSTEM_DEFAULT can only be resolved after the creation of
 * the client,) 并且它取决于底层的rmw实现。 (and it depends on the underlying rmw implementation.)
 * 如果正在使用的底层设置无法用ROS术语表示，
 * (If the underlying setting in use can't be represented in ROS terms,)
 * 它将被设置为 RMW_*_UNKNOWN。
 * (it will be set to RMW_*_UNKNOWN.)
 *
 * \note 使用此函数未解析avoid_ros_namespace_conventions字段的值。
 *   (\note The value of avoid_ros_namespace_conventions field is not resolved with this function.)
 *   rcl函数`rcl_client_request_publisher_get_actual_qos()`解析它。
 *   (The rcl function `rcl_client_request_publisher_get_actual_qos()` resolves it.)
 *
 * <hr>
 * 属性              | 遵循
 * ------------------ | -------------
 * 分配内存            | 可能 [1]
 * 线程安全            | 否
 * 使用原子操作        | 可能 [1]
 * 无锁                | 可能 [1]
 * <i>[1] 由rmw实现定义，检查实现文档</i>
 * (<i>[1] rmw implementation defined, check the implementation documentation</i>)
 *
 * \param[in] client 要检查的客户端对象
 * (\param[in] client the client object to inspect)
 * \param[out] qos 实际qos设置
 * (\param[out] qos the actual qos settings)
 * \return `RMW_RET_OK` 如果成功, 或者
 * (\return `RMW_RET_OK` if successful, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果任一参数为null, 或者
 * (\return `RMW_RET_INVALID_ARGUMENT` if either argument is null, or)
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果客户端实现标识符不匹配, 或者
 * (\return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if client implementation identifier does not
 * match, or) \return `RMW_RET_ERROR` 如果发生意外错误。
 * (\return `RMW_RET_ERROR` if an unexpected error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_client_response_subscription_get_actual_qos(
    const rmw_client_t* client, rmw_qos_profile_t* qos);

/// 创建一个服务服务器，可以从服务客户端接收请求并发送回复。
/// Create a service server that can receive requests from and send replies to a service client.
/**
 * 此函数可能失败，并返回 `NULL`，原因如下：
 * This function can fail, and therefore return `NULL`, if:
 *   - `node` 是 `NULL`
 *   - `node` is `NULL`, or
 *   - `node` 不属于此实现，即它没有匹配的实现标识符
 *   - `node` does not belong to this implementation
 *     i.e. it does not have a matching implementation identifier, or
 *   - `type_support` 是 `NULL`
 *   - `type_support` is `NULL`, or
 *   - `service_name` 是 `NULL`
 *   - `service_name` is `NULL`, or
 *   - `service_name` 是空字符串
 *   - `service_name` is an empty string, or
 *   - （如果适用 ROS 命名空间约定）根据 rmw_validate_full_topic_name() 定义，`service_name` 无效
 *   - (if ROS namespace conventions apply) `service_name` is invalid by
 *     rmw_validate_full_topic_name() definition, or
 *   - `qos_profile` 是 `NULL`
 *   - `qos_profile` is `NULL`, or
 *   - `qos_profile` 具有无效或未知策略
 *   - `qos_profile` has invalid or unknown policies, or
 *   - 在服务服务器创建期间内存分配失败
 *   - memory allocation fails during service server creation, or
 *   - 发生未指定的错误
 *   - an unspecified error occurs
 *
 * <hr>
 * 属性                   | 遵循性
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存            | 是
 * Allocates Memory   | Yes
 * 线程安全             | 否
 * Thread-Safe        | No
 * 使用原子操作         | 可能[1]
 * Uses Atomics       | Maybe [1]
 * 无锁                 | 可能[1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw 实现定义，检查实现文档</i>
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \pre 给定的 `node` 必须是有效节点，由 rmw_create_node() 返回。
 * \pre Given `node` must be a valid node, as returned by rmw_create_node().
 * \pre 给定的 `type_support` 必须是有效的 `rosidl` 服务类型支持，由 ROSIDL_GET_SRV_TYPE_SUPPORT()
 * 返回。 \pre Given `type_support` must be a valid `rosidl` service type support, as returned by
 * ROSIDL_GET_SRV_TYPE_SUPPORT().
 *
 * \param[in] node 用于注册此服务服务器的节点。
 * \param[in] node Node with which to register this service server.
 * \param[in] type_support 要提供的服务的类型支持。
 * \param[in] type_support Type support of the service to be served.
 * \param[in] service_name 要提供的服务的名称，通常是完全限定的服务名称，除非 `qos_profile`
 * 配置为避免 ROS 命名空间约定，即创建本地服务服务器。 \param[in] service_name Name of the service
 * to be served, often a fully qualified service name unless `qos_profile` is configured to avoid
 * ROS namespace conventions i.e. to create a native service server. \param[in] qos_profile
 * 此服务服务器连接的 QoS 策略。 \param[in] qos_profile QoS policies for this service server's
 * connections. \return rmw 服务句柄，如果有错误，则返回 `NULL`。 \return rmw service handle, or
 * `NULL` if there was an error.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_service_t* rmw_create_service(
    const rmw_node_t* node,
    const rosidl_service_type_support_t* type_support,
    const char* service_name,
    const rmw_qos_profile_t* qos_profile);

/// 销毁并从节点中取消注册一个服务服务器 (Destroy and unregister a service server from its node).
/**
 * 此函数将回收所有相关资源，包括服务服务器本身。
 * (This function will reclaim all associated resources, including the service server itself.)
 * 使用已销毁的服务服务器是未定义行为。
 * (Use of a destroyed service server is undefined behavior.)
 * 如果出现逻辑错误，例如 `RMW_RET_INVALID_ARGUMENT`
 * 或 `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，此函数将提前返回，保持给定服务服务器不变。
 * (This function will return early if a logical error, such as `RMW_RET_INVALID_ARGUMENT`
 * or `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`, ensues, leaving the given service server unchanged.)
 * 否则，它将忽略错误继续执行。
 * (Otherwise, it will proceed despite errors.)
 *
 * <hr>
 * 属性                 | 遵循情况
 * ------------------   | -------------
 * 分配内存             | 否 (No)
 * 线程安全             | 否 (No)
 * 使用原子操作         | 可能 [1] (Maybe [1])
 * 无锁                 | 可能 [1] (Maybe [1])
 * <i>[1] rmw 实现定义，请查阅实现文档</i>
 * (<i>[1] rmw implementation defined, check the implementation documentation</i>)
 *
 * \pre 给定的 `node` 必须是服务服务器注册时使用的节点。
 * (\pre Given `node` must be the one the service server was registered with.)
 * \pre 给定的 `service` 必须是一个有效的服务服务器，由 rmw_create_service() 返回。
 * (\pre Given `service` must be a valid service server, as returned by rmw_create_service().)
 *
 * \param[in] node 注册给定服务服务器的节点。
 * (\param[in] node Node with which the given service server is registered.)
 * \param[in] service 要销毁的服务服务器。
 * (\param[in] service Service server to be destroyed.)
 * \return 如果成功，则返回 `RMW_RET_OK`，或
 * (\return `RMW_RET_OK` if successful, or)
 * \return 如果 `node` 为 `NULL`，则返回 `RMW_RET_INVALID_ARGUMENT`，或
 * (\return `RMW_RET_INVALID_ARGUMENT` if `node` is `NULL`, or)
 * \return 如果 `service` 为 `NULL`，则返回 `RMW_RET_INVALID_ARGUMENT`，或
 * (\return `RMW_RET_INVALID_ARGUMENT` if `service` is `NULL`, or)
 * \return 如果 `node` 的实现标识符与此实现不匹配，则返回 `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，或
 * (\return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `node`
 *   implementation identifier does not match this implementation, or)
 * \return 如果 `service` 的实现标识符与此实现不匹配，则返回
 * `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，或
 * (\return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `service`
 *   implementation identifier does not match this implementation, or)
 * \return 如果发生未指定的错误，则返回 `RMW_RET_ERROR`。
 * (\return `RMW_RET_ERROR` if an unspecified error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_destroy_service(rmw_node_t* node, rmw_service_t* service);

/// 接收一个传入的ROS服务请求。
/**
 * 获取已由给定服务服务器接收的ROS服务请求，将其从内部队列中移除。
 * 请求头（即元数据），至少包含写入者guid和序列号，也会被检索出来。
 * 写入者guid和序列号允许调用者为每个远程服务客户端配对一个ROS服务请求及其相应的ROS服务响应，以便稍后使用rmw_send_response()发送。
 *
 * 即使没有收到ROS服务请求，此函数也会成功，但是`taken`将为false。
 *
 * \remarks 不能两次获取相同的ROS服务请求。
 *   调用者不必处理重复项。
 *
 * <hr>
 * 属性                 | 符合性
 * ------------------ | -------------
 * 分配内存            | 可能
 * 线程安全             | 是
 * 使用原子操作          | 可能 [1]
 * 无锁                | 可能 [1]
 *
 * <i>[1] 实现定义的，请查阅实现文档。</i>
 *
 * \par 运行时行为
 *   获取ROS服务请求是一个同步操作。
 *   它还是非阻塞的，因为它不会等待新的ROS服务请求到达，但不能保证是无锁的。
 *   一般来说，实现可以使用锁来同步访问内部资源，但不允许等待没有确保时间限制的事件（除了由于操作系统调度导致的饥饿效应）。
 *
 * \par 内存分配
 *   是否在获取时分配内存由实现定义。
 *   例如，通过线路接收的反序列化ROS服务请求的实现可能需要在处理无界（动态大小）字段时进行额外的内存分配。
 *
 * \par 线程安全性
 *   服务服务器是线程安全的对象，除了最终化操作之外，对它们的所有操作都是线程安全的。
 *   因此，从同一个服务服务器并发获取请求是安全的。
 *   但是：
 *   - 对给定的ROS服务请求的访问不是同步的。
 *     在rmw_take_request()使用`ros_request`时，读取或写入`ros_request`是不安全的。
 *   - 对给定的ROS服务请求头的访问不是同步的。
 *     在rmw_take_request()使用`request_header`时，读取或写入`request_header`是不安全的。
 *   - 对给定基本数据类型参数的访问不是同步的。
 *     在rmw_take_request()使用`taken`时，读取或写入`taken`是不安全的。
 *
 * \pre 给定的`service`必须是有效的服务，由rmw_create_service()返回。
 * \pre
 * 给定的`ros_request`必须是有效的服务请求，其类型与在创建时注册到`service`的服务类型支持相匹配。
 * \post 给定的`ros_request`将保持为有效的服务请求。
 *   如果由于逻辑错误（如无效参数）导致此函数提前失败，或者由于运行时错误导致失败，则它将保持不变，但处于未知的有效状态。
 *   如果此函数成功但`taken`为false，它也将保持不变。
 *
 * \param[in] service 要从中获取请求的服务服务器。
 * \param[out] request_header 要写入的服务请求头。
 * \param[out] ros_request 要写入的类型擦除的ROS服务请求。
 * \param[out] taken 表示是否获取了ROS服务请求的布尔标志。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`service`为NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`request_header`为NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`ros_request`为NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`taken`为NULL，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果`service`
 *   实现标识符与此实现不匹配，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_take_request(
    const rmw_service_t* service,
    rmw_service_info_t* request_header,
    void* ros_request,
    bool* taken);

/// 发送 ROS 服务响应。
/**
 * 将 ROS 服务响应发送给服务客户端，匹配的 QoS 策略，
 * 从中原始发送之前获取的 ROS 服务请求。
 *
 * <hr>
 * 属性              | 遵循
 * ------------------ | -------------
 * 分配内存          | 可能
 * 线程安全          | 是
 * 使用原子操作      | 可能 [1]
 * 无锁              | 可能 [1]
 *
 * <i>[1] 实现定义，检查实现文档。</i>
 *
 * \par 运行时行为
 *   发送 ROS 服务响应是同步还是异步，阻塞还是非阻塞操作，取决于实现。
 *   但是，异步实现在此函数返回后不允许访问给定的 ROS 服务请求。
 *   检查实现文档以了解请求行为。
 *
 * \par 内存分配
 *   是否在发送时分配内存取决于实现。
 *   例如，序列化 ROS 服务响应的实现可能需要在处理无界（动态大小）字段时进行额外的内存分配。
 *
 * \par 线程安全性
 *   服务服务器是线程安全对象，因此除最终化之外，对它们的所有操作都是线程安全的。
 *   因此，使用相同的服务服务器并发发送响应是安全的。
 *   但是：
 *   - 对给定的 ROS 服务请求头的访问是只读的，但没有同步。
 *     并发 `request_header` 读取是安全的，但并发读写不安全。
 *   - 对给定的 ROS 服务响应的访问是只读的，但没有同步。
 *     并发 `ros_request` 读取是安全的，但并发读写不安全。
 *
 * \pre 给定的 `service` 必须是有效的服务服务器，由 rmw_create_service() 返回。
 * \pre 给定的 `request_header` 必须是先前获取的与我们回复的 ROS 服务请求关联的那个。
 * \pre 给定的 `ros_response` 必须是有效的服务响应，其类型与在创建时使用 `service`
 * 注册的服务类型支持匹配。
 *
 * \param[in] service 要发送响应的服务服务器。
 * \param[in] request_header 服务响应头，与相应的 ROS 服务请求一起获取。
 * \param[in] ros_response 要发送的 ROS 服务响应。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `service` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `request_header` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `ros_response` 为 NULL，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `service`
 *   实现标识符与此实现不匹配，或
 * \return `RMW_RET_TIMEOUT` 如果响应读取器尚未准备好，或
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_send_response(
    const rmw_service_t* service, rmw_request_id_t* request_header, void* ros_response);

/// 获取服务请求订阅的实际 qos 设置。 (Retrieve the actual qos settings of the service's request
/// subscription.)
/**
 * 查询底层中间件以确定服务请求订阅的 qos 设置。
 * (Query the underlying middleware to determine the qos settings
 * of the service's request subscription.)
 * 使用 RMW_*_SYSTEM_DEFAULT 时，实际配置只能在创建服务后解析，
 * 并且它取决于底层 rmw 实现。
 * (The actual configuration applied when using RMW_*_SYSTEM_DEFAULT
 * can only be resolved after the creation of the service, and it
 * depends on the underlying rmw implementation.)
 * 如果正在使用的底层设置无法用 ROS 术语表示，则将其设置为 RMW_*_UNKNOWN。
 * (If the underlying setting in use can't be represented in ROS terms,
 * it will be set to RMW_*_UNKNOWN.)
 *
 * \note 此函数不会解析 avoid_ros_namespace_conventions 字段的值。
 *   rcl 函数 `rcl_service_request_subscription_get_actual_qos()` 解析它。
 *   (The value of avoid_ros_namespace_conventions field is not resolved
 *   with this function. The rcl function `rcl_service_request_subscription_get_actual_qos()`
 *   resolves it.)
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe [1]
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] service 要检查的服务对象 (the service object to inspect)
 * \param[out] qos 实际的 qos 设置 (the actual qos settings)
 * \return `RMW_RET_OK` 如果成功，或 (if successful, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果任一参数为 null，或 (if either argument is null, or)
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果服务实现标识符不匹配，或 (if service
 *   implementation identifier does not match, or)
 * \return `RMW_RET_ERROR` 如果发生意外错误。 (if an unexpected error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_service_request_subscription_get_actual_qos(
    const rmw_service_t* service, rmw_qos_profile_t* qos);

/// 检索服务响应发布器的实际 qos 设置。
/// Retrieve the actual qos settings of the service's response publisher.
/**
 * 查询底层中间件以确定服务响应发布器的 qos 设置。
 * Query the underlying middleware to determine the qos settings
 * of the service's response publisher.
 * 当使用 RMW_*_SYSTEM_DEFAULT 时，只有在创建服务后才能解析实际配置，
 * and it depends on the underlying rmw implementation.
 * 如果正在使用的底层设置无法用 ROS 术语表示，
 * it will be set to RMW_*_UNKNOWN.
 *
 * \note 此函数不会解析 avoid_ros_namespace_conventions 字段的值。
 *   The rcl function `rcl_service_request_subscription_get_actual_qos()`
 *   resolves it.
 *
 * <hr>
 * 属性              | 遵循
 * ------------------ | -------------
 * 分配内存            | 可能 [1]
 * 线程安全            | 否
 * 使用原子操作        | 可能 [1]
 * 无锁                | 可能 [1]
 * <i>[1] rmw 实现定义，检查实现文档</i>
 *
 * \param[in] service 要检查的服务对象
 * \param[out] qos 实际的 qos 设置
 * \return `RMW_RET_OK` 如果成功，或者
 * \return `RMW_RET_INVALID_ARGUMENT` 如果任一参数为 null，或者
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果服务
 *   实现标识符不匹配，或者
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_service_response_publisher_get_actual_qos(
    const rmw_service_t* service, rmw_qos_profile_t* qos);

// TODO(wjwwood): 当更新使用分配器时，重构此 API 以返回返回代码
/// 创建一个守卫条件并返回该守卫条件的句柄。
/// Create a guard condition and return a handle to that guard condition.
/**
 * 此函数可能会失败，因此返回 `NULL`，如果：
 *   - context 为 `NULL`
 *   - context 无效
 *   - 在守卫条件创建期间内存分配失败
 *   - 发生未指定错误
 *
 * 上下文必须为非空且有效，即已通过 `rmw_init()` 初始化，
 * 并且尚未通过 `rmw_shutdown()` 最终确定。
 *
 * <hr>
 * 属性              | 遵循
 * ------------------ | -------------
 * 分配内存            | 是
 * 线程安全            | 否
 * 使用原子操作        | 否 [1]
 * 无锁                | 否 [1]
 * <i>[1] rmw 实现定义，检查实现文档</i>
 *
 * 这应由 rmw 实现定义。
 *
 * \param[in] context 与此节点关联的初始化上下文
 * \return rmw 守卫条件句柄或 `NULL`（如果有错误）
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_guard_condition_t* rmw_create_guard_condition(rmw_context_t* context);

/// 最终确定给定的监视条件句柄，回收资源，并释放句柄。
/// Finalize a given guard condition handle, reclaim the resources, and deallocate the handle.
/**
 * \param[in] guard_condition 要销毁的监视条件句柄
 * \param[in] guard_condition the guard condition handle to be destroyed
 * \return `RMW_RET_OK` 如果成功，或者
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 guard_condition 为空，或者
 * \return `RMW_RET_ERROR` 如果发生意外错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_destroy_guard_condition(rmw_guard_condition_t* guard_condition);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_trigger_guard_condition(const rmw_guard_condition_t* guard_condition);

/// 创建一个等待集，用于存储中间件可以等待的条件。
/// Create a wait set to store conditions that the middleware can wait on.
/**
 * 此函数可能会失败，因此返回 `NULL`，如果：
 * This function can fail, and therefore return `NULL`, if:
 *   - context 是 `NULL`
 *   - context is `NULL`
 *   - context 是零初始化的，由 rmw_get_zero_initialized_context() 提供
 *   - context is zero initialized, as provided by rmw_get_zero_initialized_context()
 *   - context 不属于此实现，即没有匹配的实现标识符
 *   - context does not belong to this implementation i.e. does not have a matching
 *     implementation identifier
 *   - 在等待集创建期间内存分配失败
 *   - memory allocation fails during wait set creation
 *   - 发生未指定的错误
 *   - an unspecified error occurs
 *
 * <hr>
 * 属性 | 依从性
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存 | 是
 * Allocates Memory   | Yes
 * 线程安全 | 是
 * Thread-Safe        | Yes
 * 使用原子操作 | 可能 [1]
 * Uses Atomics       | Maybe [1]
 * 无锁 | 可能 [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw 实现定义，检查实现文档</i>
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \par 线程安全性
 * \par Thread-safety
 *   上下文是线程安全对象，因此除了最终确定之外，对它们的所有操作都是线程安全的。
 *   Contexts are thread-safe objects, and so are all operations on them except for finalization.
 *   因此，在同一上下文中同时创建多个等待集是安全的。
 *   Therefore, it is safe to create multiple wait sets in the same context concurrently.
 *
 * \pre 给定的 `context` 必须是有效的上下文，由 rmw_init() 初始化。
 * \pre Given `context` must be a valid context, initialized by rmw_init().
 *
 * \param[in] context 与等待集关联的上下文。
 * \param[in] context Context to associate the wait set with.
 * \param[in] max_conditions
 *   可以附加到等待集并由其存储的最大条件数。
 *   The maximum number of conditions that can be attached to, and stored by, the wait set.
 *   可以将其设置为零 (0)，以便等待集支持无限数量的条件。
 *   Can be set to zero (0) for the wait set to support an unbounded number of conditions.
 * \return 一个 rmw 等待集，如果发生错误则返回 `NULL`。
 * \return An rmw wait set, or `NULL` if an error occurred.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_wait_set_t* rmw_create_wait_set(rmw_context_t* context, size_t max_conditions);

/// 销毁一个等待集 (Destroy a wait set).
/**
 * 此函数将回收所有相关资源，包括等待集。
 * (This function will reclaim all associated resources, including the wait set.)
 * 在销毁后使用等待集是未定义行为。
 * (Use of a wait set after destruction is undefined behavior.)
 * 如果出现逻辑错误，如 `RMW_RET_INVALID_ARGUMENT`
 * 或 `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，此函数将提前返回，保持给定的等待集不变。
 * (This function will return early if a logical error, such as `RMW_RET_INVALID_ARGUMENT`
 * or `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`, ensues, leaving the given wait set unchanged.)
 * 否则，它将忽略错误继续执行。
 * (Otherwise, it will proceed despite errors.)
 *
 * <hr>
 * 属性                | 依从性
 * ------------------ | -------------
 * 分配内存            | 否
 * 线程安全            | 否
 * 使用原子操作        | 可能 [1]
 * 无锁                | 可能 [1]
 * <i>[1] rmw 实现定义，检查实现文档</i>
 * (Attribute          | Adherence)
 * (------------------ | -------------)
 * (Allocates Memory   | No)
 * (Thread-Safe        | No)
 * (Uses Atomics       | Maybe [1])
 * (Lock-Free          | Maybe [1])
 * (<i>[1] rmw implementation defined, check the implementation documentation</i>)
 *
 * \pre 给定的 `wait_set` 必须是一个有效的等待集，由 rmw_create_wait_set() 返回。
 * (\pre Given `wait_set` must be a valid wait set, as returned by rmw_create_wait_set().)
 *
 * \param[in] wait_set 要完成的等待集。
 * (\param[in] wait_set Wait set to be finalized.)
 * \return 如果成功，则返回 `RMW_RET_OK`，或
 * (\return `RMW_RET_OK` if successful, or)
 * \return 如果 `wait_set` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或
 * (\return `RMW_RET_INVALID_ARGUMENT` if `wait_set` is NULL, or)
 * \return 如果 `wait_set` 实现标识符与此实现不匹配，则返回
 * `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，或
 * (\return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `wait_set` implementation)
 *   identifier does not match this implementation, or
 * \return 如果发生未指定的错误，则返回 `RMW_RET_ERROR`。
 * (\return `RMW_RET_ERROR` if an unspecified error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_destroy_wait_set(rmw_wait_set_t* wait_set);

/// 等待不同实体的集合，并在有一个准备好时返回。
/// Waits on sets of different entities and returns when one is ready.
/**
 * 此函数将中间件特定的条件添加到等待集合中并等待，
 * 直到一个或多个变得可用，或者直达超时。
 * This function adds middleware-specific conditions to the wait set and waits
 * until one or more become ready, or until the timeout is reached.
 *
 * \remark 应使用单调时钟测量经过的时间，
 *   尽管 rmw 实现可能会使用其他时钟。
 *   因此，超时粒度受限于底层实现所使用的时钟，
 *   以及用于休眠和/或等待的平台特定 API。
 * \remark Elapsed time should be measured using a monotonic clock,
 *   though rmw implementations could use a different one.
 *   Timeout granularity is thus bound to that of the clock used by the underlying implementation,
 *   and to the platform-specific APIs used to sleep and/or wait.
 *
 * \remark 此函数实际等待的时间可能高于或低于指定的超时时间。
 * \remark
 *   The amount of time this function actually waits may be either above or
 *   below the specified timeout.
 *
 * 数组包含与可等待实体关联的类型擦除、中间件特定的条件，
 * 此函数将其转换并添加到等待集合中。
 * 等待前数组中的 `NULL` 条目被认为是无效的。
 * 等待结束后，与没有触发的条件相对应的每个数组中的条目将设置为 `NULL`。
 * Arrays contain type-erased, middleware-specific conditions associated with
 * waitable entities, which this function casts and adds to the wait set.
 * `NULL` entries in arrays prior to wait are considered invalid.
 * When the wait is over, entries in each array that correspond to
 * conditions that were \b not triggered are set to `NULL`.
 *
 * \remark 数组的内存管理在此函数之外。
 * \remark Arrays' memory management is external to this function.
 *
 * <hr>
 * 属性                  | 遵循性
 * ------------------   | -------------
 * 分配内存              | 可能 [1]
 * 线程安全                | 否
 * 使用原子操作            | 可能 [1]
 * 无锁                    | 可能 [1]
 * <i>[1] rmw 实现定义，检查实现文档</i>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe [1]
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \par 线程安全性
 *   等待是一个可重入过程，但：
 *   - 在两个或多个线程中同时使用相同的等待集合进行等待是不安全的。
 *   - 在两个或多个线程中使用不同的等待集合等待相同的实体是不安全的。
 *   - 对给定超时的访问是只读的，但未同步。
 *     并发 `wait_timeout` 读取是安全的，但并发读写不安全。
 * \par Thread-safety
 *   To wait is a reentrant procedure, but:
 *   - It is not safe to use the same wait set to wait in two or more threads concurrently.
 *   - It is not safe to wait for the same entity using different wait sets in two or
 *     more threads concurrently.
 *   - Access to the given timeout is read-only but it is not synchronized.
 *     Concurrent `wait_timeout` reads are safe, but concurrent reads and writes are not.
 *
 * \pre 给定的 `wait_set` 必须是有效的等待集合，如 rmw_create_wait_set() 返回的那样。
 * \pre 所有给定的实体必须与节点关联，而这些节点又必须与创建时给定的 `wait_set`
 * 注册在同一个上下文中。 \pre Given `wait_set` must be a valid wait set, as returned by
 * rmw_create_wait_set(). \pre All given entities must be associated with nodes that, in turn, were
 * registered with the same context the given `wait_set` was registered with on creation.
 *
 * \param[inout] subscriptions 要等待的订阅数组。
 *   如果没有要等待的订阅，则可以为 `NULL`。
 * \param[inout] guard_conditions 要等待的保护条件数组
 *   如果没有要等待的保护条件，则可以为 `NULL`。
 * \param[inout] services 要等待的服务数组。
 *   如果没有要等待的服务，则可以为 `NULL`。
 * \param[inout] clients 要等待的客户端数组。
 *   如果没有要等待的客户端，则可以为 `NULL`。
 * \param[inout] events 要等待的事件数组。
 *   如果没有要等待的事件，则可以为 `NULL`。
 * \param[in] wait_set 用于等待的等待集合。
 * \param[in] wait_timeout 如果为 `NULL`，则无限期阻塞，直到实体变得可用。
 *   如果为零，则不阻塞——仅检查立即可用的实体。
 *   否则，这表示等待实体变得可用的最大时间。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_TIMEOUT` 如果等待超时，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `wait_set` 为 `NULL`，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果数组条目为 `NULL`，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `wait_set` 实现
 *   标识符与此实现不匹配，或
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_wait(
    rmw_subscriptions_t* subscriptions,
    rmw_guard_conditions_t* guard_conditions,
    rmw_services_t* services,
    rmw_clients_t* clients,
    rmw_events_t* events,
    rmw_wait_set_t* wait_set,
    const rmw_time_t* wait_timeout);

/// 返回 ROS 图中所有节点的名称和命名空间。
/// Return the name and namespace of all nodes in the ROS graph.
/**
 * 此函数将返回一个节点名称数组和一个节点命名空间数组，
 * 根据给定节点迄今为止发现的情况。
 * 这两个数组表示每个已发现节点的名称和命名空间对。
 * 两个数组长度相同，相同索引指向同一节点。
 * This function will return an array of node names and an array of node namespaces,
 * as discovered so far by the given node.
 * The two arrays represent name and namespace pairs for each discovered node.
 * Both arrays will be the same length and the same index will refer to the same node.
 *
 * <hr>
 * 属性                  | 遵循性
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存              | 是
 * Allocates Memory   | Yes
 * 线程安全              | 否
 * Thread-Safe        | No
 * 使用原子操作          | 也许 [1]
 * Uses Atomics       | Maybe [1]
 * 无锁                  | 也许 [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] 实现定义，检查实现文档</i>
 * <i>[1] implementation defined, check the implementation documentation</i>
 *
 * \par 运行时行为
 *   查询 ROS 图是一个同步操作。
 *   它也是非阻塞的，但不能保证是无锁的。
 *   一般来说，实现可能使用锁来同步访问内部资源，
 *   但不允许等待没有保证时间限制的事件（不考虑
 *   由于操作系统调度引起的饥饿效应）。
 * \par Runtime behavior
 *   To query the ROS graph is a synchronous operation.
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par 线程安全性
 *   节点是线程安全对象，除最后化操作外，对它们的所有操作都是线程安全的。
 *   因此，使用相同的节点并发查询 ROS 图是安全的。
 *   但是，对字符串数组的访问不是同步的。
 *   在 rmw_get_node_names() 使用它们时，读取或写入 `node_names` 和 `node_namespaces` 是不安全的。
 * \par Thread-safety
 *   Nodes are thread-safe objects, and so are all operations on them except for finalization.
 *   Therefore, it is safe to query the ROS graph using the same node concurrently.
 *   However, access to string arrays is not synchronized.
 *   It is not safe to read or write `node_names` nor `node_namespaces`
 *   while rmw_get_node_names() uses them.
 *
 * \pre 给定的 `node` 必须是一个有效的节点句柄，由 rmw_create_node() 返回。
 * \pre Given `node` must be a valid node handle, as returned by rmw_create_node().
 * \pre 给定的 `node_names` 必须是一个有效的字符串数组，由
 * rcutils_get_zero_initialized_string_array() 返回的零初始化。 \pre Given `node_names` must be a
 * valid string array, zero-initialized as returned by rcutils_get_zero_initialized_string_array().
 * \pre 给定的 `node_namespaces` 必须是一个有效的字符串数组，由
 * rcutils_get_zero_initialized_string_array() 返回的零初始化。 \pre Given `node_namespaces` must be
 * a valid string array, zero-initialized as returned by
 * rcutils_get_zero_initialized_string_array(). \post 给定的 `node_names` 和 `node_namespaces`
 * 将保持为有效数组。 如果此函数由于逻辑错误（如无效参数）而提前失败，
 *   或者由于运行时错误而失败，这些将保持不变，但处于未知且有效状态。
 * \post Given `node_names` and `node_namespaces` will remain valid arrays.
 *   These will be left unchanged if this function fails early due to a logical error,
 *   such as an invalid argument, or in an unknown yet valid state if it fails due to
 *   a runtime error.
 *
 * \param[in] node 查询 ROS 图的节点。
 * \param[out] node_names 成功时填充已发现节点名称的数组。
 *   之后，由调用者使用 rcutils_string_array_fini() 对此数组进行最终处理。
 * \param[out] node_namespaces 成功时填充已发现节点命名空间的数组。
 *   之后，由调用者使用 rcutils_string_array_fini() 对此数组进行最终处理。
 * \return `RMW_RET_OK` 如果查询成功，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node_names` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node_names` 不是零初始化数组，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node_namespaces` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node_namespaces` 不是零初始化数组，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `node` 实现
 *   标识符与此实现不匹配，或
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_get_node_names(
    const rmw_node_t* node,
    rcutils_string_array_t* node_names,
    rcutils_string_array_t* node_namespaces);

/// 返回 ROS 图中所有节点的名称、命名空间和围场(enclave)名称。
/// Return the name, namespace, and enclave name of all nodes in the ROS graph.
/**
 * 这与 rmw_get_node_names() 类似，但它还提供了围场(enclave)名称。
 * This is similar to rmw_get_node_names(), but it also provides enclave names.
 *
 * <hr>
 * 属性                  | 遵循情况
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存            | 是
 * Allocates Memory   | Yes
 * 线程安全            | 是
 * Thread-Safe        | Yes
 * 使用原子操作        | 可能 [1]
 * Uses Atomics       | Maybe [1]
 * 无锁                | 可能 [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] 实现定义，查看实现文档</i>
 * <i>[1] implementation defined, check the implementation documentation</i>
 *
 * \par 运行时行为
 * \par Runtime behavior
 *   查询 ROS 图是一个同步操作。
 *   To query the ROS graph is a synchronous operation.
 *   它也是非阻塞的，但不能保证是无锁的。
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   一般来说，实现可以使用锁同步访问内部资源，
 *   但不允许等待没有保证时间范围的事件（除了由于操作系统调度导致的饿死效应）。
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par 线程安全性
 * \par Thread-safety
 *   节点是线程安全对象，除了最后一步操作之外，对它们的所有操作都是线程安全的。
 *   Nodes are thread-safe objects, and so are all operations on them except for finalization.
 *   因此，使用相同的节点同时查询 ROS 图是安全的。
 *   Therefore, it is safe to query the ROS graph using the same node concurrently.
 *   但是，对字符串数组的访问不是同步的。
 *   However, access to string arrays is not synchronized.
 *   在 rmw_get_node_names_with_enclaves() 使用它们时，
 *   读取或写入 `node_names`、`node_namespaces` 和 `enclaves` 是不安全的。
 *   It is not safe to read or write `node_names`, `node_namespaces`, nor `enclaves`
 *   while rmw_get_node_names_with_enclaves() uses them.
 *
 * \pre 给定的 `node` 必须是有效的节点句柄，由 rmw_create_node() 返回。
 * \pre Given `node` must be a valid node handle, as returned by rmw_create_node().
 * \pre 给定的 `node_names` 必须是有效的字符串数组，由 rcutils_get_zero_initialized_string_array()
 * 返回的零初始化。 \pre Given `node_names` must be a valid string array, zero-initialized as
 * returned by rcutils_get_zero_initialized_string_array(). \pre 给定的 `node_namespaces`
 * 必须是有效的字符串数组，由 rcutils_get_zero_initialized_string_array() 返回的零初始化。 \pre
 * Given `node_namespaces` must be a valid string array, zero-initialized as returned by
 * rcutils_get_zero_initialized_string_array(). \pre 给定的 `enclaves` 必须是零初始化的字符串数组，
 *   由 rcutils_get_zero_initialized_string_array() 返回。
 * \pre Given `enclaves` must be a zero-initialized string array,
 *   as returned by rcutils_get_zero_initialized_string_array().
 * \post 给定的 `node_names`、`node_namespaces` 和 `enclaves` 将保持为有效数组。
 * \post Given `node_names`, `node_namespaces`, and `enclaves` will remain valid arrays.
 *   如果此函数由于逻辑错误（如无效参数）而提前失败，
 *   或者由于运行时错误而失败，则这些值将保持不变。
 *   These will be left unchanged if this function fails early due to a logical error,
 *   such as an invalid argument, or in an unknown yet valid state if it fails due to
 *   a runtime error.
 *
 * \param[in] node 要查询 ROS 图的节点。
 * \param[in] node Node to query the ROS graph.
 * \param[out] node_names 成功时填充的已发现节点名称数组。
 * \param[out] node_names Array of discovered node names, populated on success.
 *   调用方稍后需要使用 rcutils_string_array_fini() 完成此数组。
 *   It is up to the caller to finalize this array later on, using rcutils_string_array_fini().
 * \param[out] node_namespaces 成功时填充的已发现节点命名空间数组。
 * \param[out] node_namespaces Array of discovered node namespaces, populated on success.
 *   调用方稍后需要使用 rcutils_string_array_fini() 完成此数组。
 *   It is up to the caller to finalize this array later on, using rcutils_string_array_fini().
 * \param[out] enclaves 成功时填充的已发现节点围场(enclave)名称数组。
 * \param[out] enclaves Array of discovered node enclave names, populated on success.
 *   调用方稍后需要使用 rcutils_string_array_fini() 完成此数组。
 *   It is up to the caller to finalize this array later on, using rcutils_string_array_fini().
 * \return `RMW_RET_OK` 如果查询成功，或
 * \return `RMW_RET_OK` if the query was successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `node` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node_names` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `node_names` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node_names` 不是零初始化数组，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `node_names` is not a zero-initialized array, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node_namespaces` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `node_namespaces` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node_namespaces` 不是零初始化数组，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `node_namespaces` is not a zero-initialized array, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `enclaves` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `enclaves` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `enclaves` 不是零初始化数组，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `enclaves` is not a zero-initialized array, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `node` 实现标识符与此实现不匹配，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `node` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_BAD_ALLOC` 如果内存分配失败，或
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_get_node_names_with_enclaves(
    const rmw_node_t* node,
    rcutils_string_array_t* node_names,
    rcutils_string_array_t* node_namespaces,
    rcutils_string_array_t* enclaves);

/// 计算匹配主题名称的已知发布者数量。 (Count the number of known publishers matching a topic name.)
/**
 * 此函数返回给定节点到目前为止在 ROS 图中发现的给定主题的发布者数量。
 * (This function returns the numbers of publishers of a given topic in the ROS graph,
 * as discovered so far by the given node.)
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] implementation defined, check the implementation documentation</i>
 *
 * \par 运行时行为 (Runtime behavior)
 *   查询 ROS 图是一个同步操作。(To query the ROS graph is a synchronous operation.)
 *   它也是非阻塞的，但不能保证是无锁的。(It is also non-blocking, but it is not guaranteed to be
 * lock-free.)
 *   一般来说，实现可以使用锁同步访问内部资源，但不允许等待没有保证时间限制的事件(不考虑由于操作系统调度导致的饥饿效应)。
 *   (Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).)
 *
 * \par 线程安全性 (Thread-safety)
 *   节点是线程安全对象，对它们的所有操作都是线程安全的，除了最终化操作。
 *   (Nodes are thread-safe objects, and so are all operations on them except for finalization.)
 *   因此，使用相同的节点并发查询 ROS 图是安全的。(Therefore, it is safe to query the ROS graph
 * using the same node concurrently.) 但是，对基本数据类型参数的访问没有同步。(However, access to
 * primitive data-type arguments is not synchronized.) 当 rmw_count_publishers()
 * 使用它们时，读取或写入 `topic_name` 或 `count` 是不安全的。 (It is not safe to read or write
 * `topic_name` or `count` while rmw_count_publishers() uses them.)
 *
 * \pre 给定的 `node` 必须是一个有效的节点句柄，由 rmw_create_node() 返回。
 *      (Given `node` must be a valid node handle, as returned by rmw_create_node().)
 *
 * \param[in] node 用于查询 ROS 图的节点句柄。(Handle to node to use to query the ROS graph.)
 * \param[in] topic_name 完全限定的 ROS 主题名称。(Fully qualified ROS topic name.)
 * \param[out] count 匹配给定主题名称的发布者数量。(Number of publishers matching the given topic
 * name.) \return `RMW_RET_OK` 如果查询成功，或 (if the query was successful, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果 `node` 为 NULL，或 (if `node` is NULL, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果 `topic_name` 为 NULL，或 (if `topic_name` is NULL, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果 `topic_name` 不是一个完全限定的主题名称， 按
 * rmw_validate_full_topic_name() 的定义，或 (if `topic_name` is not a fully qualified topic name,
 *   by rmw_validate_full_topic_name() definition, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `count` 为 NULL，或 (if `count` is NULL, or)
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `node` 实现
 *   标识符与此实现不匹配，或 (if the `node` implementation
 *   identifier does not match this implementation, or)
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。(if an unspecified error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_count_publishers(const rmw_node_t* node, const char* topic_name, size_t* count);

/// 计算匹配主题名称的已知订阅者数量。 (Count the number of known subscribers matching a topic
/// name.)
/**
 * 此函数返回给定节点到目前为止在 ROS 图中发现的给定主题的订阅者数量。 (This function returns the
 * numbers of subscribers of a given topic in the ROS graph, as discovered so far by the given
 * node.)
 *
 * <hr>
 * 属性（Attribute）          | 遵循性（Adherence）
 * ------------------ | -------------
 * 分配内存（Allocates Memory）   | 否（No）
 * 线程安全（Thread-Safe）        | 是（Yes）
 * 使用原子操作（Uses Atomics）   | 可能（Maybe [1]）
 * 无锁（Lock-Free）          | 可能（Maybe [1]）
 * <i>[1] 实现定义，检查实现文档（implementation defined, check the implementation
 * documentation）</i>
 *
 * \par 运行时行为（Runtime behavior）
 *   查询 ROS 图是一个同步操作。 (To query the ROS graph is a synchronous operation.)
 *   它也是非阻塞的，但不能保证无锁。 (It is also non-blocking, but it is not guaranteed to be
 * lock-free.)
 *   一般来说，实现可以使用锁同步访问内部资源，但不允许等待没有保证时间范围的事件（不考虑由于操作系统调度导致的饥饿）。(Generally
 * speaking, implementations may synchronize access to internal resources using locks but are not
 * allowed to wait for events with no guaranteed time bound (barring the effects of starvation due
 * to OS scheduling).)
 *
 * \par 线程安全性（Thread-safety）
 *   节点是线程安全对象，除了最终化操作，所有对它们的操作都是线程安全的。 (Nodes are thread-safe
 * objects, and so are all operations on them except for finalization.)
 *   因此，可以使用相同的节点并发查询 ROS 图。 (Therefore, it is to query the ROS graph using the
 * same node concurrently.) 但是，对基本数据类型参数的访问不是同步的。 (However, access to primitive
 * data-type arguments is not synchronized.) 在 rmw_count_subscribers() 使用 `topic_name` 或 `count`
 * 时，读取或写入它们是不安全的。 (It is not safe to read or write `topic_name` or `count` while
 * rmw_count_subscribers() uses them.)
 *
 * \pre 给定的 `node` 必须是一个有效的节点句柄，由 rmw_create_node() 返回。 (Given `node` must be a
 * valid node handle, as returned by rmw_create_node().)
 *
 * \param[in] node 用于查询 ROS 图的节点句柄。 (Handle to node to use to query the ROS graph.)
 * \param[in] topic_name 完全限定的 ROS 主题名称。 (Fully qualified ROS topic name.)
 * \param[out] count 匹配给定主题名称的订阅者数量。 (Number of subscribers matching the given topic
 * name.) \return `RMW_RET_OK` 如果查询成功，或 (if the query was successful, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果 `node` 为 NULL，或 (if `node` is NULL, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果 `topic_name` 为 NULL，或 (if `topic_name` is NULL, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果 `topic_name` 不是完全限定的主题名称，由
 * rmw_validate_full_topic_name() 定义，或 (if `topic_name` is not a fully qualified topic name, by
 * rmw_validate_full_topic_name() definition, or) \return `RMW_RET_INVALID_ARGUMENT` 如果 `count` 为
 * NULL，或 (if `count` is NULL, or) \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `node`
 * 实现标识符与此实现不匹配，或 (if the `node` implementation identifier does not match this
 * implementation, or) \return `RMW_RET_ERROR` 如果发生未指定的错误。 (if an unspecified error
 * occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_count_subscribers(const rmw_node_t* node, const char* topic_name, size_t* count);

/// 计算匹配服务名称的已知客户端数量。 (Count the number of known clients matching a service name.)
/**
 * 此函数返回给定节点到目前为止在 ROS 图中发现的给定服务的客户端数量。
 * (This function returns the numbers of clients of a given service in the ROS graph,
 * as discovered so far by the given node.)
 *
 * <hr>
 * 属性 (Attribute)          | 遵守 (Adherence)
 * ------------------ | -------------
 * 分配内存 (Allocates Memory)   | 否 (No)
 * 线程安全 (Thread-Safe)        | 是 (Yes)
 * 使用原子操作 (Uses Atomics)       | 可能 (Maybe [1])
 * 无锁 (Lock-Free)          | 可能 (Maybe [1])
 * <i>[1] 实现定义，检查实现文档 (implementation defined, check the implementation
 * documentation)</i>
 *
 * \par 运行时行为 (Runtime behavior)
 *   查询 ROS 图是一个同步操作。
 *   (To query the ROS graph is a synchronous operation.)
 *   它也是非阻塞的，但不能保证是无锁的。
 *   (It is also non-blocking, but it is not guaranteed to be lock-free.)
 *   一般来说，实现可以使用锁来同步对内部资源的访问，但不允许等待没有保证时间限制的事件（除了由于操作系统调度导致的饥饿效应）。
 *   (Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).)
 *
 * \par 线程安全性 (Thread-safety)
 *   节点是线程安全对象，除了最终化之外，对它们的所有操作都是安全的。
 *   (Nodes are thread-safe objects, and so are all operations on them except for finalization.)
 *   因此，使用相同的节点并发查询 ROS 图是安全的。
 *   (Therefore, it is safe to query the ROS graph using the same node concurrently.)
 *   但是，对基本数据类型参数的访问不是同步的。
 *   (However, access to primitive data-type arguments is not synchronized.)
 *   在 rmw_count_clients() 使用它们时，读取或写入 `service_name` 或 `count` 是不安全的。
 *   (It is not safe to read or write `service_name` or `count` while rmw_count_clients()
 *   uses them.)
 *
 * \pre 给定的 `node` 必须是一个有效的节点句柄，由 rmw_create_node() 返回。
 *      (Given `node` must be a valid node handle, as returned by rmw_create_node().)
 *
 * \param[in] node 用于查询 ROS 图的节点句柄。 (Handle to node to use to query the ROS graph.)
 * \param[in] service_name 完全限定的 ROS 服务名称。 (Fully qualified ROS service name.)
 * \param[out] count 匹配给定服务名称的客户端数量。 (Number of clients matching the given service
 * name.) \return 如果查询成功，则返回 `RMW_RET_OK`，或 (if the query was successful, or) \return
 * 如果 `node` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或 (if `node` is NULL, or) \return 如果
 * `service_name` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或 (if `service_name` is NULL, or)
 * \return 如果 `service_name` 不是一个完全限定的主题名称，则根据 rmw_validate_full_topic_name()
 * 的定义，返回 `RMW_RET_INVALID_ARGUMENT`，或 (if `service_name` is not a fully qualified topic
 * name, by rmw_validate_full_topic_name() definition, or) \return 如果 `count` 为 NULL，则返回
 * `RMW_RET_INVALID_ARGUMENT`，或 (if `count` is NULL, or) \return
 * 如果节点实现标识符与此实现不匹配，则返回 `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，或 (if the
 * `node` implementation identifier does not match this implementation, or) \return
 * 如果发生未指定的错误，则返回 `RMW_RET_ERROR`。 (if an unspecified error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_count_clients(const rmw_node_t* node, const char* service_name, size_t* count);

/// 计算匹配服务名的已知服务器数量。
/// Count the number of known servers matching a service name.
/**
 * 这个函数返回给定节点到目前为止在ROS图中发现的给定服务的服务器数量。
 * This function returns the numbers of servers of a given service in the ROS graph,
 * as discovered so far by the given node.
 *
 * <hr>
 * 属性                | 遵循情况
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存            | 否
 * Allocates Memory   | No
 * 线程安全            | 是
 * Thread-Safe        | Yes
 * 使用原子操作        | 可能 [1]
 * Uses Atomics       | Maybe [1]
 * 无锁                | 可能 [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] 实现定义，检查实现文档</i>
 * <i>[1] implementation defined, check the implementation documentation</i>
 *
 * \par 运行时行为
 *   查询ROS图是一个同步操作。
 *   To query the ROS graph is a synchronous operation.
 *   它也是非阻塞的，但不能保证是无锁的。
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   一般来说，实现可能使用锁来同步访问内部资源，
 *   但不允许等待没有保证时间限制的事件（除了由于操作系统调度导致的饥饿效应）。
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par 线程安全性
 *   节点是线程安全的对象，对它们进行的所有操作（除了最后的清理）都是线程安全的。
 *   Nodes are thread-safe objects, and so are all operations on them except for finalization.
 *   因此，使用相同的节点并发查询ROS图是安全的。
 *   Therefore, it is safe to query the ROS graph using the same node concurrently.
 *   但是，对基本数据类型参数的访问不是同步的。
 *   However, access to primitive data-type arguments is not synchronized.
 *   当rmw_count_services()使用它们时，读取或写入`service_name`或`count`是不安全的。
 *   It is not safe to read or write `service_name` or `count` while rmw_count_services()
 *   uses them.
 *
 * \pre 给定的`node`必须是一个有效的节点句柄，由rmw_create_node()返回。
 * \pre Given `node` must be a valid node handle, as returned by rmw_create_node().
 *
 * \param[in] node 用于查询ROS图的节点句柄。
 * \param[in] node Handle to node to use to query the ROS graph.
 * \param[in] service_name 完全限定的ROS服务名。
 * \param[in] service_name Fully qualified ROS topic name.
 * \param[out] count 匹配给定主题名的服务数量。
 * \param[out] count Number of services matching the given topic name.
 * \return `RMW_RET_OK` 如果查询成功，或
 * \return `RMW_RET_OK` if the query was successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`node`为NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `node` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`service_name`为NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `service_name` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`service_name`不是一个完全限定的服务名，
 *   按照rmw_validate_full_topic_name()的定义，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `service_name` is not a fully qualified service name,
 *   by rmw_validate_full_topic_name() definition, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果`count`为NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `count` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果`node`实现
 *   标识符与此实现不匹配，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `node` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_count_services(const rmw_node_t* node, const char* service_name, size_t* count);

/// 获取发布者的全局唯一标识符 (GID)。
/// Get the globally unique identifier (GID) of a publisher.
/**
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par 线程安全性 (Thread-safety)
 *   发布者是线程安全的对象，因此对它们的所有操作（除了终止操作）都是线程安全的。
 *   Publishers are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   因此，从同一个发布者并发获取唯一标识符是安全的。
 *   Therefore, it is safe to get the unique identifier from the same publisher concurrently.
 *   但是，对 GID 的访问不是同步的。
 *   However, access to the GID is not synchronized.
 *   在 rmw_get_gid_for_publisher() 使用它时，读取或写入 `gid` 是不安全的。
 *   It is not safe to read or write `gid` while rmw_get_gid_for_publisher() uses it.
 *
 * \pre 给定的 `publisher` 必须是有效的发布者，由 rmw_create_publisher() 返回。
 * \pre Given `publisher` must be a valid publisher, as returned by rmw_create_publisher().
 *
 * 这预计在 ROS 域内全球唯一。
 * This is expected to be globally unique within a ROS domain.
 * 当在本地（创建实体的位置）和远程主机或进程中报告时，标识符应该相同。
 * The identifier should be the same when reported both locally (where the entity was created)
 * and on remote hosts or processes.
 *
 * \param[in] publisher 从中获取 GID 的发布者。
 * \param[in] publisher Publisher to get a GID from.
 * \param[out] gid 发布者的唯一标识符，成功时填充，但失败时保持不变。
 * \param[out] gid Publisher's unique identifier, populated on success
 *   but left unchanged on failure.
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `publisher` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `publisher` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `gid` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `gid` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `publisher` 实现
 *   标识符与此实现不匹配，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `publisher` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_get_gid_for_publisher(const rmw_publisher_t* publisher, rmw_gid_t* gid);

/// 获取服务客户端的全局唯一标识符 (GID)。 (Get the globally unique identifier (GID) of a service
/// client.)
/**
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] 实现定义，检查实现文档。 (implementation defined, check implementation documentation.)</i>
 *
 * \par 线程安全性 (Thread-safety)
 *   服务客户端是线程安全的对象，因此除了最终化操作之外，对它们的所有操作都是线程安全的。
 *   (Service clients are thread-safe objects, and so are all operations on them except for
 *   finalization.)
 *   因此，可以并发地从同一个客户端获取唯一标识符。(Therefore, it is safe to get the unique
 * identifier from the same client concurrently.) 但是，对 GID 的访问没有同步。(However, access to
 * the GID is not synchronized.) 当 rmw_get_gid_for_client() 使用它时，读取或写入 `gid`
 * 是不安全的。(It is not safe to read or write `gid` while rmw_get_gid_for_client() uses it.)
 *
 * \pre 给定的 `client` 必须是由 rmw_create_client() 返回的有效服务客户端。(Given `client` must be a
 * valid service client, as returned by rmw_create_client().)
 *
 * 这预期在 ROS 域内是全局唯一的。(This is expected to be globally unique within a ROS domain.)
 * 当在本地（创建实体的地方）和远程主机或进程中报告时，标识符应该相同。(The identifier should be the
 * same when reported both locally (where the entity was created) and on remote hosts or processes.)
 * \param[in] client 要从中获取 GID 的服务客户端。 (Service client to get a GID from.)
 * \param[out] gid 服务客户端的唯一标识符，成功时填充，但失败时保持不变。 (Service client's unique
 * identifier, populated on success but left unchanged on failure.)
 * \return `RMW_RET_OK` 如果成功，或 (if successful, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `publisher` 为 NULL，或
 * (if `publisher` is NULL, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `gid` 为 NULL，或 (if `gid`
 * is NULL, or)
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `client` 实现
 *   标识符与此实现不匹配，或 (if the `client` implementation
 *   identifier does not match this implementation, or)
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。 (if an unspecified error occurs.)
 */
/// 检查两个全局唯一标识符（GIDs）是否相等。
/// Check if two globally unique identifiers (GIDs) are equal.
/**
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par 线程安全性
 * \par Thread-safety
 *   唯一标识符比较是一个可重入函数，但：
 *   Unique identifier comparison is a reentrant function, but:
 *   - 访问两个 GID 是只读的，但不是同步的。
 *     同时对 `gid1` 和 `gid2` 的读取是安全的，但同时读写则不然。
 *   - Access to both GIDs is read-only but it is not synchronized.
 *     Concurrent `gid1` and `gid2` reads are safe, but concurrent reads and writes are not.
 *   - 对基本数据类型参数的访问不是同步的。
 *     在 rmw_compare_gids_equal() 使用 `result` 时，读取或写入 `result` 是不安全的。
 *   - Access to primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `result` while rmw_compare_gids_equal() uses it.
 *
 * \param[in] gid1 要比较的第一个唯一标识符。
 * \param[in] gid1 First unique identifier to compare.
 * \param[in] gid2 要比较的第二个唯一标识符。
 * \param[in] gid2 Second unique identifier to compare.
 * \param[out] result 如果两个 GID 相等，则为 true，否则为 false。
 * \param[out] result true if both GIDs are equal, false otherwise.
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `gid1` 或 `gid2` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `gid1` or `gid2` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `gid1` 或 `gid2` 的实现
 *   标识符与此实现不匹配，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the implementation
 *   identifier of `gid1` or `gid2` does not match this implementation, or
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_compare_gids_equal(const rmw_gid_t* gid1, const rmw_gid_t* gid2, bool* result);

RMW_WARN_UNUSED
rmw_ret_t rmw_get_gid_for_client(const rmw_client_t* client, rmw_gid_t* gid);

/// 检查两个全局唯一标识符（GIDs）是否相等。
/// Check if two globally unique identifiers (GIDs) are equal.
/**
 * <hr>
 * 属性              | 遵循
 * ------------------ | -------------
 * 分配内存          | 否
 * 线程安全          | 是
 * 使用原子操作      | 可能 [1]
 * 无锁              | 可能 [1]
 *
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] 实现定义，检查实现文档。</i>
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par 线程安全性
 *   唯一标识符比较是可重入函数，但：
 *   - 对两个 GID 的访问是只读的，但不是同步的。
 *     并发的 `gid1` 和 `gid2` 读取是安全的，但并发读写不是。
 *   - 对基本数据类型参数的访问不是同步的。
 *     在 rmw_compare_gids_equal() 使用 `result` 时，读取或写入 `result` 是不安全的。
 *
 * \par Thread-safety
 *   Unique identifier comparison is a reentrant function, but:
 *   - Access to both GIDs is read-only but it is not synchronized.
 *     Concurrent `gid1` and `gid2` reads are safe, but concurrent reads and writes are not.
 *   - Access to primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `result` while rmw_compare_gids_equal() uses it.
 *
 * \param[in] gid1 要比较的第一个唯一标识符。
 * \param[in] gid2 要比较的第二个唯一标识符。
 * \param[out] result 如果两个 GID 相等，则为 true，否则为 false。
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `gid1` 或 `gid2` 为 NULL，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `gid1` 或 `gid2`
 * 的实现标识符与此实现不匹配，或 \return `RMW_RET_ERROR` 如果发生未指定的错误。
 *
 * \param[in] gid1 First unique identifier to compare.
 * \param[in] gid2 Second unique identifier to compare.
 * \param[out] result true if both GIDs are equal, false otherwise.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `gid1` or `gid2` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the implementation
 *   identifier of `gid1` or `gid2` does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_compare_gids_equal(const rmw_gid_t* gid1, const rmw_gid_t* gid2, bool* result);

/// 检查给定服务客户端是否有可用的服务服务器。
/// Check if a service server is available for the given service client.
/**
 * 此函数检查 ROS 图中是否存在与给定服务客户端匹配的一个或多个服务，
 * 该服务是由给定的本地节点迄今为止发现的。
 * This function checks whether one or more service services matching the
 * given service client exist in the ROS graph, as discovered so far by the
 * given local node.
 *
 * <hr>
 * 属性                  | 遵循性
 * Attribute          | Adherence
 * --------------------| -------------
 * 分配内存            | 否
 * Allocates Memory   | No
 * 线程安全            | 是
 * Thread-Safe        | Yes
 * 使用原子操作       | 可能[1]
 * Uses Atomics       | Maybe [1]
 * 无锁                | 可能[1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] 实现定义，检查实现文档。</i>
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par 运行时行为
 * \par Runtime behavior
 *   查询 ROS 图是同步操作。
 *   To query the ROS graph is a synchronous operation.
 *   它也是非阻塞的，但不能保证是无锁的。
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   一般来说，实现可能使用锁来同步访问内部资源，
 *   但不允许等待没有保证的时间范围的事件（不考虑由于操作系统调度引起的饥饿效应）。
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \pre 给定的 `node` 必须是一个有效的节点，由 rmw_create_node() 返回。
 * \pre Given `node` must be a valid node, as returned by rmw_create_node().
 * \pre 给定的 `client` 必须是一个有效的客户端，由 rmw_create_client() 返回。
 * \pre Given `client` must be a valid client, as returned by rmw_create_client().
 * \pre 给定的 `node` 必须是 `client` 注册时使用的节点。
 * \pre Given `node` must be the one the `client` was registered with.
 *
 * \param[in] node 查询 ROS 图的节点。
 * \param[in] node Node to query the ROS graph.
 * \param[in] client 查找匹配服务服务器的服务客户端。
 * \param[in] client Service client to look for matching service servers.
 * \param[out] is_available 如果有可用的服务服务器，则为 true，否则为 false。
 * \param[out] is_available True if there is a service server available, else false.
 * \return `RMW_RET_OK` 如果成功，或
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `node` 为 `NULL`，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `node` is `NULL`, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `client` 为 `NULL`，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `client` is `NULL`, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `is_available` 为 `NULL`，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `is_available` is `NULL`, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果 `node` 或 `client`
 * 的实现标识符与此实现不匹配，或 \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the
 * implementation identifier of `node` or `client` does not match this implementation, or \return
 * `RMW_RET_ERROR` 如果发生未指定的错误。 \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_service_server_is_available(
    const rmw_node_t* node, const rmw_client_t* client, bool* is_available);

/// 设置当前日志严重级别 (Set the current log severity)
/**
 * \param[in] severity 要设置的日志严重级别 (The log severity to set)
 * \return 如果成功，则返回 RMW_RET_OK，否则返回适当的错误代码 (RMW_RET_OK if successful, otherwise
 * an appropriate error code)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_set_log_severity(rmw_log_severity_t severity);

/// 为订阅设置新消息回调函数 (Set the on new message callback function for the subscription)
/**
 * 此 API 设置在订阅收到新消息时调用的回调函数。
 * (This API sets the callback function to be called whenever the
 * subscription is notified about a new message.)
 *
 * 每次订阅接收到新消息时都会调用此回调。
 * (This callback is called for each new message received by the subscription.
 * If messages arrive before the callback is registered, the number_of_events
 * argument given to the callback may be > 1.)
 *
 * 回调可能是从 rmw 实现创建的线程中调用的，而不是用户拥有的线程，
 * 即除了调用 rmw_wait() 或 rmw_publish() 等 rmw 函数的用户拥有的线程之外的其他线程。
 * (The callback may be called from a thread that the rmw implementation
 * created, rather than a thread owned by the user, i.e. some thread other
 * than user owned threads calling rmw functions such as rmw_wait() or
 * rmw_publish().)
 *
 * 此函数是线程安全的。
 * (This function is thread-safe.)
 * 因为回调可能会从任何中间件线程调用，而用户可能随时调用此函数，
 * 所以 rmw 实现需要这样做。
 * (This is required of the rmw implementation because the callback may be called
 * from any middleware thread, and this function could be called by the user
 * at any time.)
 *
 * \param[in] subscription 要设置回调的订阅 (The subscription on which to set the callback)
 * \param[in] callback 当新消息到达时要调用的回调，
 *   可以是 NULL 以清除已注册的回调 (The callback to be called when new messages arrive,
 *   can be NULL to clear the registered callback)
 * \param[in] user_data 后续调用回调时给定的数据，可以是 NULL (Given to the callback when called
 * later, may be NULL) \return `RMW_RET_OK` 如果成功，或 (if successful, or) \return
 * `RMW_RET_INVALID_ARGUMENT` 如果 `subscription` 为 NULL，或 (if `subscription` is NULL, or)
 * \return `RMW_RET_UNSUPPORTED` 如果 dds 实现中未实现该 API (if the API is not implemented in the
 * dds implementation)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_subscription_set_on_new_message_callback(
    rmw_subscription_t* subscription, rmw_event_callback_t callback, const void* user_data);

/// 设置服务的新请求回调函数。
/// Set the on new request callback function for the service.
/**
 * 此 API 用于设置在服务收到新请求通知时调用的回调函数。
 * This API sets the callback function to be called whenever the
 * service is notified about a new request.
 *
 * 对于服务接收到的每个新请求，都会调用此回调。
 * This callback is called for each new request received by the service.
 * 如果请求在注册回调之前到达，则给回调的 number_of_events 参数可能 >1。
 * If requests arrive before the callback is registered, the number_of_events
 * argument given to the callback may be > 1.
 *
 * 回调可能会从 rmw 实现创建的线程中调用，而不是用户拥有的线程，例如调用 rmw_wait() 或
 * rmw_send_request() 的用户拥有的线程。 The callback may be called from a thread that the rmw
 * implementation created, rather than a thread owned by the user, i.e. some thread other than user
 * owned threads calling rmw functions such as rmw_wait() or rmw_send_request().
 *
 * 此函数是线程安全的。
 * This function is thread-safe.
 * 因为回调可能会从任何中间件线程调用，且此函数可能会在任何时间被用户调用，所以 rmw
 * 实现需要满足这一要求。 This is required of the rmw implementation because the callback may be
 * called from any middleware thread, and this function could be called by the user at any time.
 *
 * \param[in] service 要设置回调的服务
 * \param[in] service The service on which to set the callback
 * \param[in] callback 当新请求到达时调用的回调，可以为 NULL 以清除已注册的回调
 * \param[in] callback The callback to be called when new requests arrive,
 *   can be NULL to clear the registered callback
 * \param[in] user_data 后续调用时传递给回调的数据，可以为 NULL
 * \param[in] user_data Given to the callback when called later, may be NULL
 * \return `RMW_RET_OK` 如果回调已设置为监听器，或
 * \return `RMW_RET_OK` if callback was set to the listener, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `service` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `service` is NULL, or
 * \return `RMW_RET_UNSUPPORTED` 如果 API 在 dds 实现中未实现
 * \return `RMW_RET_UNSUPPORTED` if the API is not implemented in the dds implementation
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_service_set_on_new_request_callback(
    rmw_service_t* service, rmw_event_callback_t callback, const void* user_data);

/// 设置客户端的新响应回调函数。
/// Set the on new response callback function for the client.
/**
 * 此 API 用于设置在客户端收到新响应通知时调用的回调函数。
 * This API sets the callback function to be called whenever the
 * client is notified about a new response.
 *
 * 对于客户端接收到的每个新响应，都会调用此回调。
 * This callback is called for each new response received by the client.
 * 如果响应在注册回调之前到达，则给回调的 number_of_events 参数可能 >1。
 * If responses arrive before the callback is registered, the number_of_events
 * argument given to the callback may be > 1.
 *
 * 回调可能会从 rmw 实现创建的线程中调用，而不是用户拥有的线程，例如调用 rmw_wait() 或
 * rmw_take_response() 的用户拥有的线程。 The callback may be called from a thread that the rmw
 * implementation created, rather than a thread owned by the user, i.e. some thread other than user
 * owned threads calling rmw functions such as rmw_wait() or rmw_take_response().
 *
 * 此函数是线程安全的。
 * This function is thread-safe.
 * 因为回调可能会从任何中间件线程调用，且此函数可能会在任何时间被用户调用，所以 rmw
 * 实现需要满足这一要求。 This is required of the rmw implementation because the callback may be
 * called from any middleware thread, and this function could be called by the user at any time.
 *
 * \param[in] client 要设置回调的客户端
 * \param[in] client The client on which to set the callback
 * \param[in] callback 当新响应到达时调用的回调，可以为 NULL 以清除已注册的回调
 * \param[in] callback The callback to be called when new responses arrive,
 *   can be NULL to clear the registered callback
 * \param[in] user_data 后续调用时传递给回调的数据，可以为 NULL
 * \param[in] user_data Given to the callback when called later, may be NULL
 * \return `RMW_RET_OK` 如果回调已设置为监听器，或
 * \return `RMW_RET_OK` if callback was set to the listener, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `client` 为 NULL，或
 * \return `RMW_RET_INVALID_ARGUMENT` if `client` is NULL, or
 * \return `RMW_RET_UNSUPPORTED` 如果 API 在 dds 实现中未实现
 * \return `RMW_RET_UNSUPPORTED` if the API is not implemented in the dds implementation
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_client_set_on_new_response_callback(
    rmw_client_t* client, rmw_event_callback_t callback, const void* user_data);

/// 设置事件的回调函数。 (Set the callback function for the event.)
/**
 * 此 API 设置在事件通知新实例时调用的回调函数。 (This API sets the callback function to be called
 * whenever the event is notified about a new instance of the event.)
 *
 * 例如，当检测到不兼容的 QoS 时，或者错过了截止时间，或者其他任何 QoS 事件。 (For example, this
 * could be called when incompatible QoS is detected, or a deadline is missed, or any other QoS
 * event.)
 *
 * 对于此 rmw_event_t 实例发生的每个新事件，都会调用此回调。 (This callback is called for each new
 * event that occurs for this rmw_event_t instance.) 如果在注册回调之前发生事件，则提供给回调的
 * number_of_events 参数可能大于 1。 (If events occur before the callback is registered, the
 * number_of_events argument given to the callback may be > 1.)
 *
 * 回调可能是从 rmw 实现创建的线程中调用的，而不是用户拥有的线程，即除了调用 rmw 函数（如 rmw_wait()
 * 或 rmw_publish()）的用户拥有的线程之外的某个线程。 (The callback may be called from a thread that
 * the rmw implementation created, rather than a thread owned by the user, i.e. some thread other
 * than user owned threads calling rmw functions such as rmw_wait() or
 * rmw_publish().)
 *
 * 此函数是线程安全的。 (This function is thread-safe.)
 * 由于回调可能是从任何中间件线程调用的，而且用户可能随时调用此函数，因此 rmw 实现需要这样做。 (This
 * is required of the rmw implementation because the callback may be called from any middleware
 * thread, and this function could be called by the user at any time.)
 *
 * \param[in] event 设置回调的事件 (The event on which to set the callback)
 * \param[in] callback 发生新事件时要调用的回调，可以为 NULL 以清除已注册的回调 (The callback to be
 * called when new events occur, can be NULL to clear the registered callback) \param[in] user_data
 * 后续调用时提供给回调的，可以为 NULL (Given to the callback when called later, may be NULL)
 * \return `RMW_RET_OK` 如果回调已设置为侦听器，或 (if callback was set to the listener, or)
 * \return `RMW_RET_INVALID_ARGUMENT` 如果 `event` 为 NULL，或 (if `event` is NULL, or)
 * \return `RMW_RET_UNSUPPORTED` 如果 API 在 dds 实现中未实现 (if the API is not implemented in the
 * dds implementation)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_event_set_callback(
    rmw_event_t* event, rmw_event_callback_t callback, const void* user_data);

#ifdef __cplusplus
}
#endif

#endif  // RMW__RMW_H_
