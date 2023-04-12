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

#ifndef RMW__TOPIC_ENDPOINT_INFO_ARRAY_H_
#define RMW__TOPIC_ENDPOINT_INFO_ARRAY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rcutils/allocator.h"
#include "rmw/topic_endpoint_info.h"
#include "rmw/visibility_control.h"

/// Array of topic endpoint information (主题端点信息数组)
typedef struct RMW_PUBLIC_TYPE rmw_topic_endpoint_info_array_s {
  /// Size of the array. (数组的大小)
  size_t size;
  /// Contiguous storage for topic endpoint information elements.
  /// (用于存储主题端点信息元素的连续存储空间)
  rmw_topic_endpoint_info_t* info_array;
} rmw_topic_endpoint_info_array_t;

/// Return a zero initialized array of topic endpoint information.
/// (返回一个零初始化的主题端点信息数组)
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_topic_endpoint_info_array_t rmw_get_zero_initialized_topic_endpoint_info_array(void);

/// Check that the given `topic_endpoint_info_array` is zero initialized. (检查给定的
/// `topic_endpoint_info_array` 是否为零初始化)
/**
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety (线程安全性)
 *   Access to the array of topic endpoint information is read-only, but it is not synchronized.
 *   (访问主题端点信息数组是只读的，但不同步)
 *   Concurrent `topic_endpoint_info_array` reads are safe, but concurrent reads
 *   and writes are not. (同时进行的 `topic_endpoint_info_array`
 * 读取是安全的，但同时进行的读写操作则不安全)
 *
 * \param[in] topic_endpoint_info_array Array to be checked. (要检查的数组)
 * \returns `RMW_RET_OK` if array is zero initialized, or (如果数组是零初始化，则返回 `RMW_RET_OK`)
 * \returns `RMW_RET_INVALID_ARGUMENT` if `topic_endpoint_info_array` is NULL, or (如果
 * `topic_endpoint_info_array` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`) \returns `RMW_RET_ERROR`
 * if `topic_endpoint_info_array` is not zero initialized. (如果 `topic_endpoint_info_array`
 * 没有零初始化，则返回 `RMW_RET_ERROR`) \remark This function sets the RMW error state on failure.
 * (在失败时，此函数设置 RMW 错误状态)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_topic_endpoint_info_array_check_zero(
    rmw_topic_endpoint_info_array_t* topic_endpoint_info_array);

/// 初始化一个话题端点信息数组。 (Initialize an array of topic endpoint information.)
/**
 * 此函数为容纳 `size` 个话题端点信息元素分配空间。 (This function allocates space to hold `size`
 * topic endpoint information elements.) 根据情况更新 `info_array` 和 `size` 成员。 (Both
 * `info_array` and `size` members are updated accordingly.)
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
 *   初始化是可重入的过程，但：(Initialization is a reentrant procedure, but:)
 *   - 对话题端点信息数组的访问不同步。 (Access to the array of topic endpoint information is not
 * synchronized.) 在初始化期间，读取或写入 `topic_endpoint_info_array` 是不安全的。 (It is not safe
 * to read or write `topic_endpoint_info_array` during initialization.)
 *   - 默认分配器是线程安全的对象，但任何自定义的 `allocator` 可能不是。 (The default allocators are
 * thread-safe objects, but any custom `allocator` may not be.) 有关详细信息，请参阅分配器文档。
 * (Check your allocator documentation for further reference.)
 *
 * \param[inout] topic_endpoint_info_array 成功时初始化的数组， (Array to be initialized on
 * success,) 但在失败时保持不变。 (but left unchanged on failure.) \param[in] size 数组的大小。
 * (Size of the array.) \param[in] allocator 用于填充 `names_and_types` 的分配器。 (Allocator to be
 * used to populate `names_and_types`.) \returns `RMW_RET_OK` 如果成功，或 (if successful, or)
 * \returns `RMW_RET_INVALID_ARGUMENT` 如果 `topic_endpoint_info_array` 为 NULL，或 (if
 * `topic_endpoint_info_array` is NULL, or) \returns `RMW_RET_INVALID_ARGUMENT` 如果
 * `topic_endpoint_info_array` 不是 (if `topic_endpoint_info_array` is not) 零初始化数组，或 (a zero
 * initialized array, or) \returns `RMW_RET_INVALID_ARGUMENT` 如果 `allocator` 无效， (if
 * `allocator` is invalid,) 根据 rcutils_allocator_is_valid() 定义，或 (by
 * rcutils_allocator_is_valid() definition, or) \returns `RMW_BAD_ALLOC` 如果内存分配失败，或 (if
 * memory allocation fails, or) \returns `RMW_RET_ERROR` 当发生未指定的错误。 (when an unspecified
 * error occurs.) \remark 此函数在失败时设置 RMW 错误状态。 (This function sets the RMW error state
 * on failure.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_topic_endpoint_info_array_init_with_size(
    rmw_topic_endpoint_info_array_t* topic_endpoint_info_array,
    size_t size,
    rcutils_allocator_t* allocator);

/// 完成一个主题端点信息数组的清理操作。
/**
 * 该函数释放给定数组存储空间，然后将其初始化为零。
 * 如果出现逻辑错误，例如 `RMW_RET_INVALID_ARGUMENT`，此函数将提前返回，
 * 保持给定数组不变。否则，即使发生错误，它也将继续执行。
 *
 * <hr>
 * 属性                | 符合性
 * ------------------ | -------------
 * 分配内存            | 否
 * 线程安全            | 否
 * 使用原子操作        | 否
 * 无锁                | 是
 *
 * \par 线程安全性
 *   Finalization 是可重入过程，但是：
 *   - 对主题端点信息数组的访问未同步。
 *     在 finalization 过程中，读取或写入 `topic_endpoint_info_array` 是不安全的。
 *   - 默认分配器是线程安全的对象，但任何自定义的 `allocator` 可能不是。
 *     请查阅您的分配器文档以获取更多参考信息。
 *
 * \pre 给定的 `allocator` 必须与用于初始化给定的 `topic_endpoint_info_array` 相同。
 *
 * \param[inout] topic_endpoint_info_array 要完成的对象。
 * \param[in] allocator 用于填充给定的 `topic_endpoint_info_array` 的分配器。
 * \returns `RMW_RET_OK` 如果成功，或者
 * \returns `RMW_RET_INVALID_ARGUMENT` 如果 `topic_endpoint_info_array` 为 NULL，或者
 * \returns `RMW_RET_INVALID_ARGUMENT` 如果 `allocator` 无效，
 *   根据 rcutils_allocator_is_valid() 定义，或者
 * \returns `RMW_RET_ERROR` 当发生未指定的错误。
 * \remark 此函数在失败时设置 RMW 错误状态。
 */
/// Finalize an array of topic endpoint information.
/**
 * This function deallocates the given array storage, and then zero initializes it.
 * If a logical error, such as `RMW_RET_INVALID_ARGUMENT`, ensues, this function will
 * return early, leaving the given array unchanged.
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
 *   - Access to the array of topic endpoint information is not synchronized.
 *     It is not safe to read or write `topic_endpoint_info_array` during finalization.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.
 *
 * \pre Given `allocator` must be the same used to initialize the given `topic_endpoint_info_array`.
 *
 * \param[inout] topic_endpoint_info_array object to be finalized.
 * \param[in] allocator Allocator used to populate the given `topic_endpoint_info_array`.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `topic_endpoint_info_array` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `allocator` is invalid,
 *   by rcutils_allocator_is_valid() definition, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_topic_endpoint_info_array_fini(
    rmw_topic_endpoint_info_array_t* topic_endpoint_info_array, rcutils_allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif  // RMW__TOPIC_ENDPOINT_INFO_ARRAY_H_
