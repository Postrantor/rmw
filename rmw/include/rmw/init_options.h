// Copyright 2014-2018 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__INIT_OPTIONS_H_
#define RMW__INIT_OPTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "rcutils/allocator.h"
#include "rmw/discovery_options.h"
#include "rmw/domain_id.h"
#include "rmw/localhost.h"
#include "rmw/macros.h"
#include "rmw/ret_types.h"
#include "rmw/security_options.h"
#include "rmw/visibility_control.h"

/// 实现定义的选项结构，用于rmw_init()期间。
/// Implementation defined options structure used during rmw_init().
/**
 * 这应由rmw实现定义。
 * This should be defined by the rmw implementation.
 */
typedef struct rmw_init_options_impl_s rmw_init_options_impl_t;

/// 在rmw_init()期间使用的选项结构。
/// Options structure used during rmw_init().
typedef struct RMW_PUBLIC_TYPE rmw_init_options_s {
  /// 表示此初始化/关闭周期的局部（进程本地）唯一ID。
  /// Locally (process local) unique ID that represents this init/shutdown cycle.
  /**
   * 这应该由`rmw_init()`的调用者设置为在此过程中唯一的数字。
   * This should be set by the caller of `rmw_init()` to a number that is
   * unique within this process.
   * 它被设计用于与`rcl_init()`和`rcl_get_instance_id()`一起使用。
   * It is designed to be used with `rcl_init()` and `rcl_get_instance_id()`.
   */
  uint64_t instance_id;
  /// 实现标识符，用于确保不混合两个不同的实现。
  /// Implementation identifier, used to ensure two different implementations are not being mixed.
  const char* implementation_identifier;
  /// ROS domain id
  size_t domain_id;
  /// 安全选项
  /// Security options
  rmw_security_options_t security_options;
  /// 仅启用本地主机
  /// Enable localhost only
  rmw_localhost_only_t localhost_only;
  /// 配置发现
  /// Configure discovery
  rmw_discovery_options_t discovery_options;
  /// Enclave，用于在sros2密钥库中查找安全工件的名称。
  /// Enclave, name used to find security artifacts in a sros2 keystore.
  char* enclave;

  // TODO(wjwwood): 当这个重构发生时，用rmw_allocator_t替换
  // TODO(wjwwood): replace with rmw_allocator_t when that refactor happens
  /// 如果需要，在init选项的内部分配期间使用的分配器。
  /// Allocator used during internal allocation of init options, if needed.
  rcutils_allocator_t allocator;
  /// 实现定义的初始化选项。
  /// Implementation defined init options.
  /** 如果没有实现定义的选项，则可能为NULL。 */
  /** May be NULL if there are no implementation defined options. */
  rmw_init_options_impl_t* impl;
} rmw_init_options_t;

/// 返回一个零初始化的初始化选项结构。
/// Return a zero initialized init options structure.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_init_options_t rmw_get_zero_initialized_init_options(void);

/// 使用默认值和实现特定值初始化给定的初始化选项。
/// Initialize given init options with the default values and implementation specific values.
/**
 * 如果需要，在设置init选项期间使用给定的分配器，
 * 但在初始化期间也使用它。
 *
 * If required, the given allocator is used during setup of the init options,
 * but is also used during initialization.
 *
 * 在任一情况下，给定的分配器都存储在返回的init选项中。
 *
 * In either case the given allocator is stored in the returned init options.
 *
 * `impl`指针不应手动更改。
 *
 * The `impl` pointer should not be changed manually.
 *
 * \pre 给定的init选项必须为零初始化。
 *
 * \pre The given init options must be zero initialized.
 *
 * \post 如果初始化失败，init选项将保持零初始化。
 *
 * \post If initialization fails, init options will remain zero initialized.
 *
 * \remark 给一个已经初始化的init选项将导致
 *   返回代码为`RMW_RET_INVALID_ARGUMENT`的失败。
 *
 * \remark Giving an already initialized init options will result
 *   in a failure with return code `RMW_RET_INVALID_ARGUMENT`.
 *
 * <hr>
 * 属性          | 遵循
 * -------------- | -------------
 * 分配内存       | 是
 * 线程安全       | 否
 * 使用原子操作   | 是
 * 无锁           | 是
 *
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | Yes
 * Lock-Free          | Yes
 *
 * 这应由rmw实现定义。
 *
 * This should be defined by the rmw implementation.
 *
 * \param[inout] init_options 要设置的对象
 * \param[in] allocator 在设置和初始化期间使用的分配器
 * \return `RMW_RET_OK` 如果设置成功，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果任何参数无效，或
 * \return `RMW_RET_BAD_ALLOC` 如果分配内存失败，或
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。
 *
 * \param[inout] init_options object to be setup
 * \param[in] allocator to be used during setup and during initialization
 * \return `RMW_RET_OK` if setup is successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if any arguments are invalid, or
 * \return `RMW_RET_BAD_ALLOC` if allocating memory failed, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_init_options_init(rmw_init_options_t* init_options, rcutils_allocator_t allocator);

/// 将给定的源初始化选项复制到目标初始化选项。
/// Copy the given source init options to the destination init options.
/**
 * 从源分配器用于任何分配并存储在
 * 目的地。
 *
 * The allocator from the source is used for any allocations and stored in the
 * destination.
 *
 * \pre 源init选项必须已初始化
 *   即已调用`rmw_init_options_init()`。
 * \pre 目标init选项必须为零初始化。
 *
 * \pre The source init options must have been initialized
 *   i.e. had `rmw_init_options_init()` called on.
 * \pre The destination init options must be zero initialized.
 *
 * \post 如果复制失败，目标init选项将保持零初始化。
 *
 * \post If copy fails, destination init options will remain zero initialized.
 *
 * \remark 给一个零初始化的init选项作为源将导致
 *   返回代码为`RMW_RET_INVALID_ARGUMENT`的失败。
 * \remark 给一个已经初始化的init选项作为目标将导致
 *   返回代码为`RMW_RET_INVALID_ARGUMENT`的失败。
 *
 * \remark Giving an zero initialized init options as a source will result
 *   in a failure with return code `RMW_RET_INVALID_ARGUMENT`.
 * \remark Giving an already initialized init options for the destination will result
 *   in a failure with return code `RMW_RET_INVALID_ARGUMENT`.
 *
 * <hr>
 * 属性          | 遵循
 * -------------- | -------------
 * 分配内存       | 是
 * 线程安全       | 否
 * 使用原子操作   | 是
 * 无锁           | 是
 *
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | Yes
 * Lock-Free          | Yes
 *
 * 这应由rmw实现定义。
 *
 * This should be defined by the rmw implementation.
 *
 * \param[in] src 要复制的rcl_init_options_t对象
 * \param[out] dst 要复制到的rcl_init_options_t对象
 * \return `RMW_RET_OK` 如果复制成功，或
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` 如果src的实现
 *   标识符与此函数的实现不匹配，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果任何参数无效，或
 * \return `RMW_RET_BAD_ALLOC` 如果分配内存失败，或
 * \return `RMW_RET_ERROR` 如果发生未指定的错误。
 *
 * \param[in] src rcl_init_options_t object to be copied from
 * \param[out] dst rcl_init_options_t object to be copied into
 * \return `RMW_RET_OK` if the copy is successful, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the implementation
 *   identifier for src does not match the implementation of this function, or
 * \return `RMW_RET_INVALID_ARGUMENT` if any arguments are invalid, or
 * \return `RMW_RET_BAD_ALLOC` if allocating memory failed, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_init_options_copy(const rmw_init_options_t* src, rmw_init_options_t* dst);

/// 结束给定的初始化选项。
/// Finalize the given init options.
/**
 * 如果出现逻辑错误，例如`RMW_RET_INVALID_ARGUMENT`
 * 或`RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，此函数将提前返回，保持给定的init选项
 * 不变。
 * 否则，它将继续进行错误处理，尽可能释放资源并将给定的init选项归零
 * 初始化。
 *
 * This function will return early if a logical error, such as `RMW_RET_INVALID_ARGUMENT`
 * or `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`, ensues, leaving the given init options
 * unchanged.
 * Otherwise, it will proceed despite errors, freeing as much resources as it can and zero
 * initializing the given init options.
 *
 * \pre 给定的init选项必须已初始化
 *   即已调用`rmw_init_options_init()`。
 *
 * \pre The given init options must have been initialized
 *   i.e. had `rmw_init_options_init()` called on.
 *
 * \remarks 如果init选项为零初始化，
 *   则返回`RMW_RET_INVALID_ARGUMENT`。
 *
 * \remarks If init options are zero initialized,
 *   then `RMW_RET_INVALID_ARGUMENT` is returned.
 *
 * <hr>
 * 属性          | 遵循
 * -------------- | -------------
 * 分配内存       | 是
 * 线程安全       | 否
 * 使用原子操作   | 是
 * 无锁           | 是
 *
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | Yes
 * Lock-Free          | Yes
 *
 * 这应由rmw实现定义。
 *
 * This should be defined by the rmw implementation.
 *
 * \param[inout] init_options 要完成的对象
 * \return `RMW_RET_OK` 如果完成成功，或
 * \return `RMW_RET_INVALID_ARGUMENT` 如果任何参数无效，或
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_init_options_fini(rmw_init_options_t* init_options);

#ifdef __cplusplus
}
#endif

#endif  // RMW__INIT_OPTIONS_H_
