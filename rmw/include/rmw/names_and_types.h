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

#ifndef RMW__NAMES_AND_TYPES_H_
#define RMW__NAMES_AND_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "rcutils/allocator.h"
#include "rcutils/types.h"
#include "rmw/macros.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// 关联主题或服务名称和类型的数组 (Associative array of topic or service names and types)
typedef struct RMW_PUBLIC_TYPE rmw_names_and_types_s {
  /// 名称数组 (Array of names)
  rcutils_string_array_t names;

  /// 类型名的动态数组，长度与 `names` 相同 (Dynamic array of arrays of type names, with the same
  /// length as `names`)
  rcutils_string_array_t* types;
} rmw_names_and_types_t;

/// 返回一个零初始化的名称和类型数组 (Return a zero initialized array of names and types)
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_names_and_types_t rmw_get_zero_initialized_names_and_types(void);

/// 检查给定的 `names_and_types` 数组是否为零初始化 (Check that the given `names_and_types` array is
/// zero initialized)
/**
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   对名称和类型数组的访问是只读的，但不是同步的 (Access to the array of names and types is
 * read-only, but it is not synchronized) 同时进行 `names_and_types`
 * 的读取是安全的，但同时进行读写则不安全 (Concurrent `names_and_types` reads are safe, but
 * concurrent reads and writes are not)
 *
 * \param[in] names_and_types 要检查的数组 (Array to be checked)
 * \return 如果数组为零初始化，则返回 RMW_RET_OK，否则返回 RMW_RET_INVALID_ARGUMENT (RMW_RET_OK if
 * array is zero initialized, RMW_RET_INVALID_ARGUMENT otherwise) \remark 如果失败，此函数会设置 RMW
 * 错误状态 (This function sets the RMW error state on failure)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_names_and_types_check_zero(rmw_names_and_types_t* names_and_types);

/// 初始化名称和类型数组 (Initialize an array of names and types)
/**
 * 此函数为 `names` 的字符串数组进行初始化，并根据给定的大小为 `types` 的字符串数组分配空间，
 * 但不为每个类型集合的字符串数组进行初始化。然而，每个类型集合的字符串数组都是零初始化的。
 * (This function initializes the string array for the names and allocates space
 * for all the string arrays for the types according to the given size, but
 * it does not initialize the string array for each setup of types.
 * However, the string arrays for each set of types is zero initialized.)
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
 *   初始化是可重入的过程，但：(Initialization is a reentrant procedure, but:)
 *   - 对名称和类型数组的访问不是同步的 (Access to arrays of names and types is not synchronized)
 *     在初始化期间读取或写入 `names_and_types` 是不安全的 (It is not safe to read or write
 * `names_and_types` during initialization)
 *   - 默认分配器是线程安全的对象，但任何自定义 `allocator` 可能不是 (The default allocators are
 * thread-safe objects, but any custom `allocator` may not be) 请查阅分配器文档以获取更多参考信息
 * (Check your allocator documentation for further reference)
 *
 * \param[inout] names_and_types 初始化成功时要初始化的数组，但失败时保持不变 (Array to be
 * initialized on success, but left unchanged on failure) \param[in] size 数组的大小 (Size of the
 * array) \param[in] allocator 用于填充 `names_and_types` 的分配器 (Allocator to be used to populate
 * `names_and_types`) \returns 如果成功，则返回 `RMW_RET_OK`，或者 (If successful, or) \returns 如果
 * `names_and_types` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或者 (If `names_and_types` is NULL,
 * or) \returns 如果 `names_and_types` 不是零初始化数组，则返回 `RMW_RET_INVALID_ARGUMENT`，或者 (If
 * `names_and_types` is not a zero initialized array, or) \returns 如果 `allocator` 无效，根据
 * rcutils_allocator_is_valid() 的定义，则返回 `RMW_RET_INVALID_ARGUMENT`，或者 (If `allocator` is
 * invalid, by rcutils_allocator_is_valid() definition, or) \returns 如果内存分配失败，则返回
 * `RMW_BAD_ALLOC`，或者 (If memory allocation fails, or) \returns 当发生未指定的错误时，返回
 * `RMW_RET_ERROR` (When an unspecified error occurs) \remark 如果失败，此函数会设置 RMW 错误状态
 * (This function sets the RMW error state on failure)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_names_and_types_init(
    rmw_names_and_types_t* names_and_types, size_t size, rcutils_allocator_t* allocator);

/// 结束名称和类型数组 (Finalize an array of names and types)
/**
 * 此函数释放名称的字符串数组和类型的字符串数组，并将给定数组置为零初始化。
 * 如果出现逻辑错误，如 `RMW_RET_INVALID_ARGUMENT`，此函数将提前返回，使给定数组保持不变。
 * 否则，它将继续进行，尽管有错误。
 * (This function deallocates the string array of names and the array of string arrays of types,
 * and zero initializes the given array.
 * If a logical error, such as `RMW_RET_INVALID_ARGUMENT`, ensues, this function will return
 * early, leaving the given array unchanged.
 * Otherwise, it will proceed despite errors.)
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
 *   Finalization是可重入的过程，但对名称和类型数组的访问不是同步的 (Finalization is a reentrant
 * procedure, but access to arrays of names and types is not synchronized) 在初始化期间读取或写入
 * `names_and_types` 是不安全的 (It is not safe to read or write `names_and_types` during
 * initialization)
 *
 * \param[inout] names_and_types 要结束的数组 (Array to be finalized)
 * \returns 如果成功，则返回 `RMW_RET_OK`，或者 (If successful, or)
 * \returns 如果 `names_and_types` 为 NULL，则返回 `RMW_RET_INVALID_ARGUMENT`，或者 (If
 * `names_and_types` is NULL, or) \returns 当发生未指定的错误时，返回 `RMW_RET_ERROR` (When an
 * unspecified error occurs) \remark 如果失败，此函数会设置 RMW 错误状态 (This function sets the RMW
 * error state on failure)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_names_and_types_fini(rmw_names_and_types_t* names_and_types);

#ifdef __cplusplus
}
#endif

#endif  // RMW__NAMES_AND_TYPES_H_
