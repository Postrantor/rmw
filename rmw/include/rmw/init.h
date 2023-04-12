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

#ifndef RMW__INIT_H_
#define RMW__INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "rmw/init_options.h"
#include "rmw/macros.h"
#include "rmw/ret_types.h"
#include "rmw/visibility_control.h"

/// 实现定义的由 rmw_init() 返回的上下文结构。
/// Implementation defined context structure returned by rmw_init().
/**
 * 这应该由 rmw 实现来定义。
 * This should be defined by the rmw implementation.
 */
typedef struct rmw_context_impl_s rmw_context_impl_t;

/// 用于存储初始化特定信息的初始化上下文结构。
/// Initialization context structure which is used to store init specific information.
typedef struct RMW_PUBLIC_TYPE rmw_context_s {
  /// 表示此初始化/关闭周期的本地（进程内）唯一 ID。
  /// Locally (process local) unique ID that represents this init/shutdown cycle.
  uint64_t instance_id;
  /// 实现标识符，用于确保不混合使用两个不同的实现。（区分底层使用的哪个dds吧）
  /// Implementation identifier, used to ensure two different implementations are not being mixed.
  const char* implementation_identifier;
  /// 用于初始化上下文的选项。
  /// Options used to initialize the context.
  rmw_init_options_t options;
  /// 正在使用的域 ID。
  /// Domain id that is being used.
  size_t actual_domain_id;
  /// 实现定义的上下文信息。
  /// Implementation defined context information.
  /** 如果没有实现定义的上下文信息，则可能为 NULL。*/
  /** May be NULL if there is no implementation defined context information. */
  rmw_context_impl_t* impl;
} rmw_context_t;

/// 返回一个零初始化的上下文结构。
/// Return a zero initialized context structure.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_context_t rmw_get_zero_initialized_context(void);

/// 使用给定的选项初始化中间件，并生成一个上下文。
/// Initialize the middleware with the given options, and yielding an context.
/**
 * 如果此函数成功，则上下文将填充有关中间件的特定数据。
 * Context is filled with middleware specific data upon success of this function.
 * 上下文在初始化诸如节点和保护条件等实体时使用，还需要正确调用 `rmw_shutdown()`。
 * The context is used when initializing some entities like nodes and
 * guard conditions, and is also required to properly call `rmw_shutdown()`.
 *
 * \pre 给定的选项必须已经初始化
 *   即对其调用了 `rmw_init_options_init()` 并设置了一个围场。
 *   i.e. `rmw_init_options_init()` called on it and
 *   an enclave set.
 * \pre 给定的上下文必须为零初始化。
 * \pre The given context must be zero initialized.
 *
 * \post 如果初始化失败，上下文将保持零初始化。
 * \post If initialization fails, context will remain zero initialized.
 * \post `context->actual_domain_id` 将设置为 rmw 实现正在使用的域 ID。
 *  如果它不是 RMW_DEFAULT_DOMAIN_ID，则与 `options->domain_id` 匹配。
 *  否则，该值取决于 rmw 实现。
 *  This matches `options->domain_id` if it is not RMW_DEFAULT_DOMAIN_ID.
 *  In other case, the value is rmw implementation dependent.
 *
 * \remarks 如果选项为零初始化，则返回 `RMW_RET_INVALID_ARGUMENT`。
 *   如果选项已初始化但未提供围场，则返回 `RMW_RET_INVALID_ARGUMENT`。
 *   如果上下文已经初始化（已在其上调用 `rmw_init()`），则返回 `RMW_RET_INVALID_ARGUMENT`。
 * \remarks If options are zero-initialized, then `RMW_RET_INVALID_ARGUMENT` is returned.
 *   If options are initialized but no enclave is provided, then `RMW_RET_INVALID_ARGUMENT`
 *   is returned.
 *   If context has been already initialized (`rmw_init()` was called on it), then
 *   `RMW_RET_INVALID_ARGUMENT` is returned.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * 这应该由 rmw 实现来定义。
 * This should be defined by the rmw implementation.
 *
 * \param[in] options 初始化期间使用的初始化选项
 * \param[out] context 生成的上下文结构
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if any arguments are invalid, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the implementation
 *   identifier does not match, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_init(const rmw_init_options_t* options, rmw_context_t* context);

/// 关闭给定上下文的中间件。
/// Shutdown the middleware for a given context.
/**
 * \pre 给定的上下文必须是一个有效的上下文，已经使用 `rmw_init()` 初始化。
 * \pre The given context must be a valid context which has been initialized with `rmw_init()`.
 *
 * \remarks 如果上下文为零初始化，则返回 `RMW_RET_INVALID_ARGUMENT`。
 *   如果上下文已经无效（已在其上调用 `rmw_shutdown()`），则此函数为无操作，并返回 `RMW_RET_OK`。
 * \remarks If context is zero initialized, then `RMW_RET_INVALID_ARGUMENT` is returned.
 *   If context has been already invalidated (`rmw_shutdown()` was called on it), then
 *   this function is a no-op and `RMW_RET_OK` is returned.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * 这应该由 rmw 实现来定义。
 * This should be defined by the rmw implementation.
 *
 * \param[in] context 生成的上下文结构
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if any argument are invalid, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the implementation
 *   identifier does not match, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_shutdown(rmw_context_t* context);

/// 结束一个上下文。
/// Finalize a context.
/**
 * 如果出现逻辑错误，例如 `RMW_RET_INVALID_ARGUMENT`
 * 或 `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`，则此函数将提前返回，保持给定的上下文不变。
 * 否则，它将尽管出现错误，释放尽可能多的资源并将给定的上下文归零初始化。
 * This function will return early if a logical error, such as `RMW_RET_INVALID_ARGUMENT`
 * or `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`, ensues, leaving the given context unchanged.
 * Otherwise, it will proceed despite errors, freeing as much resources as it can and zero
 * initializing the given context.
 *
 * \pre 要完成的上下文必须先用
 *   `rmw_init()` 初始化，然后再用 `rmw_shutdown()` 使其无效。
 * \pre The context to be finalized must have been previously initialized with
 *   `rmw_init()`, and then later invalidated with `rmw_shutdown()`.
 *
 * \remarks 如果上下文为零初始化，则返回 `RMW_RET_INVALID_ARGUMENT`。
 *   如果上下文已初始化且有效（未在其上调用 `rmw_shutdown()`），则返回 `RMW_RET_INVALID_ARGUMENT`。
 * \remarks If context is zero initialized, then `RMW_RET_INVALID_ARGUMENT` is returned.
 *   If context is initialized and valid (`rmw_shutdown()` was not called on it), then
 *   `RMW_RET_INVALID_ARGUMENT` is returned.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | Yes
 * Lock-Free          | Yes [1]
 * <i>[1] if `atomic_is_lock_free()` returns true for `atomic_uint_least64_t`</i>
 *
 * 这应该由 rmw 实现来定义。
 * This should be defined by the rmw implementation.
 *
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if any arguments are invalid, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the implementation
 *   identifier does not match, or
 * \return `RMW_RET_ERROR` if an unspecified error occur.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_context_fini(rmw_context_t* context);

#ifdef __cplusplus
}
#endif

#endif  // RMW__INIT_H_
