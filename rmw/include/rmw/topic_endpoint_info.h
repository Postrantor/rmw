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

#ifndef RMW__TOPIC_ENDPOINT_INFO_H_
#define RMW__TOPIC_ENDPOINT_INFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rcutils/allocator.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"
#include "rosidl_runtime_c/type_hash.h"

/**
 * @brief 数据结构，封装了发布者和订阅者的节点名称、节点命名空间、topic_type、gid 和 qos_profile。
 *        A data structure that encapsulates the node name, node namespace,
 *        topic_type, gid, and qos_profile of publishers and subscriptions
 *        for a topic.
 */
typedef struct RMW_PUBLIC_TYPE rmw_topic_endpoint_info_s {
  /// 节点名称 (Node name)
  const char* node_name;
  /// 节点命名空间 (Node namespace)
  const char* node_namespace;
  /// 关联的主题类型名称 (The associated topic type's name)
  const char* topic_type;
  /// 主题类型描述的哈希值 (Hashed value for topic type's description)
  rosidl_type_hash_t topic_type_hash;
  /// 端点类型 (Endpoint type)
  rmw_endpoint_type_t endpoint_type;
  /// 端点的 GID (The GID of the endpoint)
  uint8_t endpoint_gid[RMW_GID_STORAGE_SIZE];
  /// 端点的 QoS 配置文件 (QoS profile of the endpoint)
  rmw_qos_profile_t qos_profile;
} rmw_topic_endpoint_info_t;

/**
 * @brief 返回零初始化的主题端点信息数据结构。
 *        Return zero initialized topic endpoint info data structure.
 *
 * 端点类型将是无效的。 (Endpoint type will be invalid.)
 * 端点 QoS 配置文件将是系统默认值。 (Endpoint QoS profile will be the system default.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_topic_endpoint_info_t rmw_get_zero_initialized_topic_endpoint_info(void);

/// 完成主题端点信息数据结构的初始化。
/**
 * 释放给定数据结构的所有已分配成员，
 * 然后将其初始化为零。
 * 如果出现逻辑错误，如 `RMW_RET_INVALID_ARGUMENT`，此函数
 * 将提前返回，保持给定的数据结构不变。
 * 否则，将继续执行，尽管存在错误。
 *
 * <hr>
 * 属性              | 遵循
 * ------------------ | -------------
 * 分配内存          | 否
 * 线程安全          | 否
 * 使用原子操作      | 否
 * 无锁              | 是
 *
 * \par 线程安全性
 *   Finalization 是可重入的过程，但是：
 *   - 对主题端点信息数据结构的访问没有同步。
 *     在 finalization 期间，读取或写入 `topic_endpoint` 是不安全的。
 *   - 默认的分配器是线程安全的对象，但任何自定义的 `allocator` 可能不是。
 *     请查阅您的分配器文档以获取更多参考信息。
 *
 * \param[inout] topic_endpoint_info 要完成的数据结构。
 * \param[in] allocator 用于填充给定的 `topic_endpoint_info` 的分配器。
 * \returns `RMW_RET_OK` 如果成功，或者
 * \returns `RMW_RET_INVALID_ARGUMENT` 如果 `topic_endpoint_info` 为 NULL，或者
 * \returns `RMW_RET_INVALID_ARGUMENT` 如果 `allocator` 无效，
 *   按照 rcutils_allocator_is_valid() 的定义，或者
 * \returns `RMW_RET_ERROR` 当发生未指定的错误。
 * \remark 此函数在失败时设置 RMW 错误状态。
 */
/// Finalize a topic endpoint info data structure.
/**
 * Deallocates all allocated members of the given data structure,
 * and then zero initializes it.
 * If a logical error, such as `RMW_RET_INVALID_ARGUMENT`, ensues, this function
 * will return early, leaving the given data structure unchanged.
 * Otherwise, it will proceed despite errors.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Finalization is a reentrant procedure, but:
 *   - Access to the topic endpoint info data structure is not synchronized.
 *     It is not safe to read or write `topic_endpoint` during finalization.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.
 *
 * \param[inout] topic_endpoint_info Data structure to be finalized.
 * \param[in] allocator Allocator used to populate the given `topic_endpoint_info`.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `topic_endpoint_info` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `allocator` is invalid,
 *   by rcutils_allocator_is_valid() definition, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_topic_endpoint_info_fini(
    rmw_topic_endpoint_info_t* topic_endpoint_info, rcutils_allocator_t* allocator);

/// 设置给定主题端点信息数据结构中的主题类型。 (Set the topic type in the given topic endpoint info
/// data structure.)
/**
 * 分配内存并复制 `topic_type` 参数的值，以设置数据结构的 `topic_type` 成员。
 * (Allocates memory and copies the value of the `topic_type`
 * argument to set the data structure's `topic_type` member.)
 *
 * <hr>
 * 属性 (Attribute)          | 遵循 (Adherence)
 * ------------------ | -------------
 * 分配内存 (Allocates Memory)   | 是 (Yes)
 * 线程安全 (Thread-Safe)        | 否 (No)
 * 使用原子操作 (Uses Atomics)       | 否 (No)
 * 无锁 (Lock-Free)          | 是 (Yes)
 *
 * \par 线程安全性 (Thread-safety)
 *   设置成员是可重入过程，但：(Setting a member is a reentrant procedure, but:)
 *   - 对主题端点信息数据结构的访问未同步。(Access to the topic endpoint info data structure is not
 * synchronized.) 在设置时，读取或写入给定 `topic_endpoint` 的 `topic_type` 成员是不安全的。(It is
 * not safe to read or write the `topic_type` member of the given `topic_endpoint` while setting
 * it.)
 *   - 对 C 风格字符串参数的访问是只读的，但未同步。(Access to C-style string arguments is read-only
 * but it is not synchronized.) 并发 `topic_type` 读取是安全的，但并发读写不安全。(Concurrent
 * `topic_type` reads are safe, but concurrent reads and writes are not.)
 *   - 默认分配器是线程安全的对象，但任何自定义的 `allocator` 可能不是。(The default allocators are
 * thread-safe objects, but any custom `allocator` may not be.)
 *     请查阅您的分配器文档以获取更多参考信息。(Check your allocator documentation for further
 * reference.)
 *
 * \pre 给定的 `topic_type` 是有效的 C 风格字符串，即以 NULL 结尾。(Given `topic_type` is a valid
 * C-style string i.e. NULL terminated.)
 *
 * \param[inout] topic_endpoint_info 要填充的数据结构。(Data structure to be populated.)
 * \param[in] topic_type 要设置的类型名称。(Type name to be set.)
 * \param[in] allocator 要使用的分配器。(Allocator to be used.)
 * \returns `RMW_RET_OK` 如果成功，或 (if successful, or)
 * \returns `RMW_RET_INVALID_ARGUMENT` 如果 `topic_endpoint_info` 为 NULL，或 (if
 * `topic_endpoint_info` is NULL, or) \returns `RMW_RET_INVALID_ARGUMENT` 如果 `topic_type` 为
 * NULL，或 (if `topic_type` is NULL, or) \returns `RMW_RET_BAD_ALLOC` 如果内存分配失败，或 (if
 * memory allocation fails, or) \returns `RMW_RET_ERROR` 当发生未指定的错误时。(when an unspecified
 * error occurs.) \remark 当失败时，此函数设置 RMW 错误状态。(This function sets the RMW error state
 * on failure.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_topic_endpoint_info_set_topic_type(
    rmw_topic_endpoint_info_t* topic_endpoint_info,
    const char* topic_type,
    rcutils_allocator_t* allocator);

/// 设置给定主题端点信息数据结构中的主题类型哈希。
/// Set the topic type hash in the given topic endpoint info data structure.
/**
 * 将 `topic_type_hash` 参数的值分配给数据结构的
 * `topic_type_hash` 成员。
 * Assigns the value of the `topic_type_hash` argument to the data structure's
 * `topic_type_hash` member.
 *
 * <hr>
 * 属性                 | 遵循
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存            | 否
 * Allocates Memory   | No
 * 线程安全             | 否
 * Thread-Safe        | No
 * 使用原子操作         | 否
 * Uses Atomics       | No
 * 无锁                 | 是
 * Lock-Free          | Yes
 *
 * \par 线程安全性
 * \par Thread-safety
 *   设置成员是一个可重入过程，但：
 *   Setting a member is a reentrant procedure, but:
 *   - 对主题端点信息数据结构的访问不是同步的。
 *     在设置它时，读取或写入给定 `topic_endpoint` 的 `topic_type_hash` 成员是不安全的。
 *     并发的 `topic_type_hash` 读取是安全的，但并发的读和写是不安全的。
 *   - Access to the topic endpoint info data structure is not synchronized.
 *     It is not safe to read or write the `topic_type_hash` member of the given `topic_endpoint`
 *     while setting it.
 *     Concurrent `topic_type_hash` reads are safe, but concurrent reads and writes are not.
 *
 * \param[inout] topic_endpoint_info 要填充的数据结构。
 * \param[inout] topic_endpoint_info Data structure to be populated.
 * \param[in] topic_type_hash 要复制的主题类型哈希。
 * \param[in] topic_type_hash Topic type hash to be copied.
 * \returns 如果成功，则返回 `RMW_RET_OK`，或
 * \returns `RMW_RET_OK` if successful, or
 * \returns 如果 `topic_endpoint_info` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或
 * \returns `RMW_RET_INVALID_ARGUMENT` if `topic_endpoint_info` is NULL, or
 * \returns 如果 `topic_type_hash` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或
 * \returns `RMW_RET_INVALID_ARGUMENT` if `topic_type_hash` is NULL, or
 * \returns 当发生未指定的错误时，返回 `RMW_RET_ERROR`。
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark 当失败时，此函数设置 RMW 错误状态。
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_topic_endpoint_info_set_topic_type_hash(
    rmw_topic_endpoint_info_t* topic_endpoint_info, const rosidl_type_hash_t* type_hash);

/// 设置给定主题端点信息数据结构中的节点名称。 (Set the node_name in the given topic endpoint info
/// data structure.)
/**
 * 分配内存并复制 `node_name`
 * 参数以设置数据结构的 `node_name` 成员。 (Allocates memory and copies the value of the `node_name`
 * argument to set the data structure's `node_name` member.)
 *
 * <hr>
 * 属性(Attribute)          | 遵循(Adherence)
 * ------------------ | -------------
 * 分配内存(Allocates Memory)   | 是(Yes)
 * 线程安全(Thread-Safe)        | 否(No)
 * 使用原子操作(Uses Atomics)       | 否(No)
 * 无锁(Lock-Free)          | 是(Yes)
 *
 * \par 线程安全性 (Thread-safety)
 *   设置成员是可重入过程，但是：(Setting a member is a reentrant procedure, but:)
 *   - 对主题端点信息数据结构的访问不是同步的。(Access to the topic endpoint info data structure is
 * not synchronized.) 在设置它时，读取或写入给定的 `topic_endpoint` 的 `node_name`
 * 成员是不安全的。(It is not safe to read or write the `node_name` member of the given
 * `topic_endpoint` while setting it.)
 *   - 对 C 风格字符串参数的访问是只读的，但不是同步的。 (Access to C-style string arguments is
 * read-only but it is not synchronized.) 并发的 `node_name` 读取是安全的，但并发的读取和写入不是。
 * (Concurrent `node_name` reads are safe, but concurrent reads and writes are not.)
 *   - 默认分配器是线程安全的对象，但任何自定义的 `allocator` 可能不是。(The default allocators are
 * thread-safe objects, but any custom `allocator` may not be.) 有关详细信息，请参阅您的分配器文档。
 * (Check your allocator documentation for further reference.)
 *
 * \pre 给定的 `node_name` 是一个有效的 C 风格字符串，即以 NULL 结尾。(Given `node_name` is a valid
 * C-style string i.e. NULL terminated.)
 *
 * \param[inout] topic_endpoint_info 要填充的数据结构。(Data structure to be populated.)
 * \param[in] node_name 要设置的节点名称。(Node name to be set.)
 * \param[in] allocator 要使用的分配器。(Allocator to be used.)
 * \returns `RMW_RET_OK` 如果成功，或 (if successful, or)
 * \returns `RMW_RET_INVALID_ARGUMENT` 如果 `topic_endpoint_info` 为 NULL，或 (if
 * `topic_endpoint_info` is NULL, or) \returns `RMW_RET_INVALID_ARGUMENT` 如果 `node_name` 为
 * NULL，或 (if `node_name` is NULL, or) \returns `RMW_RET_BAD_ALLOC` 如果内存分配失败，或 (if
 * memory allocation fails, or) \returns `RMW_RET_ERROR` 当发生未指定的错误时。(when an unspecified
 * error occurs.) \remark 当出现失败时，此函数设置 RMW 错误状态。 (This function sets the RMW error
 * state on failure.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_topic_endpoint_info_set_node_name(
    rmw_topic_endpoint_info_t* topic_endpoint_info,
    const char* node_name,
    rcutils_allocator_t* allocator);

/// 设置给定主题端点信息数据结构中的节点命名空间。
/// Set the node namespace in the given topic endpoint info data structure.
/**
 * 为 `node_namespace` 成员分配内存并复制 `node_namespace`
 * 参数的值以设置数据结构的 `node_namespace` 成员。
 * Allocates memory and copies the value of the `node_namespace`
 * argument to set the data structure's `node_namespace` member.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   设置成员是可重入的过程，但：
 *   Setting a member is a reentrant procedure, but:
 *   - 对主题端点信息数据结构的访问不同步。
 *     在设置时，读取或写入给定 `topic_endpoint` 的 `node_namespace` 成员是不安全的。
 *   - Access to the topic endpoint info data structure is not synchronized.
 *     It is not safe to read or write the `node_namespace` member of the given `topic_endpoint`
 *     while setting it.
 *   - 访问 C 样式字符串参数是只读的，但不同步。
 *     同时进行 `node_namespace` 读取是安全的，但同时进行读取和写入则不安全。
 *   - Access to C-style string arguments is read-only but it is not synchronized.
 *     Concurrent `node_namespace` reads are safe, but concurrent reads and writes are not.
 *   - 默认分配器是线程安全的对象，但任何自定义的 `allocator` 可能不是。
 *     有关更多信息，请查阅您的分配器文档。
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.
 *
 * \pre 给定的 `node_namespace` 是有效的 C 样式字符串，即以 NULL 结尾。
 * \pre Given `node_namespace` is a valid C-style string i.e. NULL terminated.
 *
 * \param[inout] topic_endpoint_info 要填充的数据结构。
 * \param[inout] topic_endpoint_info Data structure to be populated.
 * \param[in] node_namespace 要设置的节点命名空间。
 * \param[in] node_namespace Node namespace to be set.
 * \param[in] allocator 要使用的分配器。
 * \param[in] allocator Allocator to be used.
 * \returns 如果成功，则返回 `RMW_RET_OK`，或
 * \returns `RMW_RET_OK` if successful, or
 * \returns 如果 `topic_endpoint_info` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或
 * \returns `RMW_RET_INVALID_ARGUMENT` if `topic_endpoint_info` is NULL, or
 * \returns 如果 `node_namespace` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或
 * \returns `RMW_RET_INVALID_ARGUMENT` if `node_namespace` is NULL, or
 * \returns 如果内存分配失败，则返回 `RMW_RET_BAD_ALLOC`，或
 * \returns `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \returns 当发生未指定的错误时，返回 `RMW_RET_ERROR`。
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark 在失败时，此函数设置 RMW 错误状态。
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_topic_endpoint_info_set_node_namespace(
    rmw_topic_endpoint_info_t* topic_endpoint_info,
    const char* node_namespace,
    rcutils_allocator_t* allocator);

/// 设置给定主题端点信息数据结构中的端点类型。 (Set the endpoint type in the given topic endpoint
/// info data structure.)
/**
 * 将 `type` 参数的值分配给数据结构的 `endpoint_type` 成员。 (Assigns the value of the `type`
 * argument to the data structure's `endpoint_type` member.)
 *
 * <hr>
 * 属性 (Attribute)          | 遵循 (Adherence)
 * ------------------ | -------------
 * 分配内存 (Allocates Memory)   | 否 (No)
 * 线程安全 (Thread-Safe)        | 否 (No)
 * 使用原子操作 (Uses Atomics)   | 否 (No)
 * 无锁 (Lock-Free)          | 是 (Yes)
 *
 * \par 线程安全性 (Thread-safety)
 *   设置成员是可重入过程，但访问主题端点信息数据结构没有同步。 (Setting a member is a reentrant
 * procedure, but access to the topic endpoint info data structure is not synchronized.)
 *   在设置时，不安全地读取或写入给定 `topic_endpoint` 的 `endpoint_type` 成员。 (It is not safe to
 * read or write the `endpoint_type` member of the given `topic_endpoint` while setting it.)
 *
 * \param[inout] topic_endpoint_info 要填充的数据结构。 (Data structure to be populated.)
 * \param[in] type 要设置的端点类型。 (Endpoint type to be set.)
 * \returns 如果成功，则返回 `RMW_RET_OK` ，或 (if successful, or)
 * \returns 如果 `topic_endpoint_info` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT` ，或 (if
 * `topic_endpoint_info` is NULL, or) \returns 当出现未指定错误时，返回 `RMW_RET_ERROR` 。 (when an
 * unspecified error occurs.) \remark 在失败时，此函数设置 RMW 错误状态。 (This function sets the
 * RMW error state on failure.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_topic_endpoint_info_set_endpoint_type(
    rmw_topic_endpoint_info_t* topic_endpoint_info, rmw_endpoint_type_t type);

/// 设置给定主题端点信息数据结构中的端点 gid。 (Set the endpoint gid in the given topic endpoint
/// info data structure.)
/**
 * 将 `gid` 参数的值复制到数据结构的 `endpoint_gid` 成员。 (Copies the value of the `gid` argument
 * to the data structure's `endpoint_gid` member.)
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   设置成员是可重入过程，但对主题端点信息数据结构的访问未同步。 (Setting a member is a reentrant
 * procedure, but access to the topic endpoint info data structure is not synchronized.)
 *   在设置时，不安全地读取或写入给定 `topic_endpoint` 的 `gid` 成员。 (It is not safe to read or
 * write the `gid` member of the given `topic_endpoint` while setting it.)
 *
 * \param[inout] topic_endpoint_info 要填充的数据结构。 (Data structure to be populated.)
 * \param[in] gid 要设置的端点 gid。 (Endpoint gid to be set.)
 * \param[in] size 给定 `gid` 的大小。 (Size of the given `gid`.)
 * \returns `RMW_RET_OK` 如果成功，或 (if successful, or)
 * \returns `RMW_RET_INVALID_ARGUMENT` 如果 `topic_endpoint_info` 为 NULL，或 (if
 * `topic_endpoint_info` is NULL, or) \returns `RMW_RET_INVALID_ARGUMENT` 如果 `gid` 为 NULL，或 (if
 * `gid` is NULL, or) \returns `RMW_RET_INVALID_ARGUMENT` 如果 `size` 大于 RMW_GID_STORAGE_SIZE，或
 * (if `size` is greater than RMW_GID_STORAGE_SIZE, or) \returns `RMW_RET_ERROR`
 * 当发生未指定的错误。 (when an unspecified error occurs.) \remark 此函数在失败时设置 RMW
 * 错误状态。 (This function sets the RMW error state on failure.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_topic_endpoint_info_set_gid(
    rmw_topic_endpoint_info_t* topic_endpoint_info, const uint8_t* gid, size_t size);

/// 设置给定主题端点信息数据结构中的端点 QoS 配置文件。 (Set the endpoint QoS profile in the given
/// topic endpoint info data structure.)
/**
 * 将 `qos_profile` 参数的值分配给数据结构的 `qos_profile` 成员。
 * (Assigns the value of the `qos_profile` argument to the data structure's `qos_profile` member.)
 *
 * <hr>
 * 属性 (Attribute)          | 符合性 (Adherence)
 * ------------------ | -------------
 * 分配内存 (Allocates Memory)   | 否 (No)
 * 线程安全 (Thread-Safe)        | 否 (No)
 * 使用原子操作 (Uses Atomics)       | 否 (No)
 * 无锁 (Lock-Free)          | 是 (Yes)
 *
 * \par 线程安全性 (Thread-safety)
 *   设置成员是可重入过程，但对主题端点信息数据结构的访问未同步。
 *   (Setting a member is a reentrant procedure, but access to the topic endpoint info data
 * structure is not synchronized.) 在设置时，读取或写入给定 `topic_endpoint` 的 `qos_profile`
 * 成员是不安全的。 (It is not safe to read or write the `qos_profile` member of the given
 * `topic_endpoint` while setting it.)
 *
 * \param[inout] topic_endpoint_info 要填充的数据结构。 (Data structure to be populated.)
 * \param[in] qos_profile 要设置的 QoS 配置文件。 (QoS profile to be set.)
 * \returns 如果成功，则返回 `RMW_RET_OK`，或 (If successful, or)
 * \returns 如果 `topic_endpoint_info` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或 (If
 * `topic_endpoint_info` is NULL, or) \returns 如果 `qos_profile` 为 NULL，则返回
 * `RMW_RET_INVALID_ARGUMENT`，或 (If `qos_profile` is NULL, or) \returns 当发生未指定的错误时返回
 * `RMW_RET_ERROR`。 (When an unspecified error occurs.) \remark 当失败时，此函数设置 RMW 错误状态。
 * (This function sets the RMW error state on failure.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_topic_endpoint_info_set_qos_profile(
    rmw_topic_endpoint_info_t* topic_endpoint_info, const rmw_qos_profile_t* qos_profile);

#ifdef __cplusplus
}
#endif

#endif  // RMW__TOPIC_ENDPOINT_INFO_H_
