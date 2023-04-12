// Copyright 2014 Open Source Robotics Foundation, Inc.
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

/**
 * @file error_handling.h
 * @brief Functions and macros to interact with error handling mechanisms
 *
 * This logic pretty much renames many `rcutils` functions and macros. Also check out that
 * package's documentation for more information.
 */

#ifndef RMW__ERROR_HANDLING_H_
#define RMW__ERROR_HANDLING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <rcutils/error_handling.h>

/// 结构体包装一个固定大小的c字符串，用于返回格式化的错误字符串。
/// Struct wrapping a fixed-size c string used for returning the formatted error string.
typedef rcutils_error_string_t rmw_error_string_t;

/// 封装由 RMW_SET_ERROR_MSG() 设置的错误状态的结构体。
/// Struct which encapsulates the error state set by RMW_SET_ERROR_MSG().
typedef rcutils_error_state_t rmw_error_state_t;

/// 在 `fwrite` 调用中限制缓冲区大小，以防止在非空终止的 `msg` 的情况下发生缓冲区溢出。
/// Limit the buffer size in the `fwrite` call to give an upper bound to buffer overrun in the case
/// of non-null terminated `msg`.
#define RMW_SAFE_FWRITE_TO_STDERR RCUTILS_SAFE_FWRITE_TO_STDERR

/// 如果在新创建的线程中调用，则强制初始化线程局部存储。
/// Forces initialization of thread-local storage if called in a newly created thread.
/**
 * 如果没有事先调用此函数，那么在首次设置错误状态或首次检索错误消息时，
 * 默认分配器将用于分配线程局部存储。
 *
 * 此函数可能会也可能不会分配内存。
 * 系统的线程局部存储实现可能需要分配内存，因为在不知道要创建多少线程的情况下，
 * 通常无法知道需要多少存储。大多数实现（如 C11、C++11 和 pthread）都没有办法指定
 * 如何分配这些内存，但是如果实现允许，给定的分配器将用于此函数，
 * 否则不使用。这只发生在创建和销毁线程时，通过重用线程池可以避免这种情况。
 *
 * 值得考虑的是，反复创建和销毁线程将导致反复分配内存，可能导致内存碎片化。
 * 通常通过使用线程池来避免这种情况。
 *
 * 如果返回代码表示错误，则不会设置错误消息。
 *
 * 如果在一个线程中多次调用，或者在隐式初始化后调用（通过设置错误状态），
 * 即使给定的分配器无效，也仍然返回 `RCUTILS_RET_OK`。
 * 本质上，如果已经调用了线程局部存储，那么此函数什么都不做。
 * 如果已经初始化，即使与最初用于初始化线程局部存储的分配器不匹配，也会忽略给定的分配器。
 *
 * \return `RCUTILS_RET_OK`，如果成功，或
 * \return `RCUTILS_RET_INVALID_ARGUMENT`，如果分配器无效，或
 * \return `RCUTILS_RET_BAD_ALLOC`，如果分配内存失败，或
 * \return `RCUTILS_RET_ERROR`，如果发生未指定的错误。
 */
#define rmw_initialize_error_handling_thread_local_storage \
  rcutils_initialize_error_handling_thread_local_storage

/// 设置错误消息，以及发生错误的文件和行。
/// Set the error message, as well as the file and line on which it occurred.
/**
 * 这不是直接使用，而是通过
 * RMW_SET_ERROR_MSG(msg) 宏来实现。
 *
 * error_msg 参数被复制到内部错误存储，必须为 null 结尾。
 * file 参数被复制到内部错误存储，必须为 null 结尾。
 *
 * \param[in] error_string 要设置的错误消息。
 * \param[in] file 发生错误的文件路径。
 * \param[in] line_number 发生错误的行号。
 */
#define rmw_set_error_state rcutils_set_error_state

/// 检查参数是否为空值。
/// Check an argument for a null value.
/**
 * 如果参数值为 `NULL`，则设置相应的错误消息并返回 `error_return_type`。
 *
 * \param[in] argument 要测试的参数。
 * \param[in] error_return_type 参数为 `NULL` 时要返回的类型。
 */
#define RMW_CHECK_ARGUMENT_FOR_NULL(argument, error_return_type) \
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(argument, error_return_type)

/// 检查一个值是否为 null，如果是，则设置错误消息并执行错误语句。
/// Check a value for null, with an error message and error statement.
/**
 * 如果 `value` 为 `NULL`，在设置错误消息后将评估错误语句。
 * If `value` is `NULL`, the error statement will be evaluated after
 * setting the error message.
 *
 * \param[in] value 要测试的值。The value to test.
 * \param[in] msg 当 `value` 为 `NULL` 时的错误消息。The error message if `value` is `NULL`.
 * \param[in] error_statement 如果 `value` 为 `NULL` 时要评估的语句。The statement to evaluate if
 * `value` is `NULL`.
 */
#define RMW_CHECK_FOR_NULL_WITH_MSG(value, msg, error_statement) \
  RCUTILS_CHECK_FOR_NULL_WITH_MSG(value, msg, error_statement)

/// 设置错误消息，并附加当前文件和行号。
/// Set the error message, as well as append the current file and line number.
/**
 * 如果先前设置了错误消息，并且此后没有调用 rmw_reset_error()，
 * 并且使用 RCUTILS_REPORT_ERROR_HANDLING_ERRORS 构建了此库，
 * 则先前设置的错误消息将打印到 stderr。
 * Error state storage is thread local and so all error related functions are
 * also thread local.
 *
 * \param[in] msg 要设置的错误消息。The error message to be set.
 */
#define RMW_SET_ERROR_MSG(msg) RCUTILS_SET_ERROR_MSG(msg)

/// 使用格式字符串和格式参数设置错误消息。
/// Set the error message using a format string and format arguments.
/**
 * 该函数使用给定的格式字符串设置错误消息。
 * 结果格式化字符串将在 RCUTILS_ERROR_MESSAGE_MAX_LENGTH 处静默截断。
 *
 * This function sets the error message using the given format string.
 * The resulting formatted string is silently truncated at
 * RCUTILS_ERROR_MESSAGE_MAX_LENGTH.
 *
 * \param[in] format_string 用作错误消息格式的字符串。The string to be used as the format of the
 * error message. \param[in] ... 格式字符串的参数。Arguments for the format string.
 */
#define RMW_SET_ERROR_MSG_WITH_FORMAT_STRING(format_string, ...) \
  RCUTILS_SET_ERROR_MSG_WITH_FORMAT_STRING(format_string, __VA_ARGS__)

/// 如果错误已设置，则返回 `true`，否则返回 `false`。
/// Return `true` if the error is set, otherwise `false`.
/**
 * 这当前定义为 `rcutils_error_is_set`
 */
#define rmw_error_is_set rcutils_error_is_set

/// 返回通过 rcutils_set_error_state() 设置的 rcutils_error_state_t。
/// Return an rcutils_error_state_t which was set with rcutils_set_error_state().
/**
 * 如果在此线程中没有设置错误，则返回的指针将为 NULL。
 * The returned pointer will be NULL if no error has been set in this thread.
 *
 * 在同一线程中调用 RCUTILS_SET_ERROR_MSG、rcutils_set_error_state 或 rcutils_reset_error 之前，
 * 返回的指针是有效的。
 * The returned pointer is valid until RCUTILS_SET_ERROR_MSG, rcutils_set_error_state,
 * or rcutils_reset_error are called in the same thread.
 *
 * \return 指向当前错误状态结构的指针。A pointer to the current error state struct.
 */
#define rmw_get_error_state rcutils_get_error_state

/// 如果设置，则返回错误消息，后跟 `, at <file>:<line>`，否则返回 "error not set"。
/// Return the error message followed by `, at <file>:<line>` if set, else "error not set".
/**
 * 该函数是“安全”的，因为它返回当前错误字符串的副本，
 * 或者如果没有设置错误，则包含字符串 "error not set" 的副本。
 * 这确保了副本由调用线程拥有，因此永远不会被其他错误处理调用无效，
 * 并且其中的 C 字符串始终有效且以空字符结尾。
 *
 * \return 当前错误字符串（带文件和行号）或 "error not set"（如果未设置）。The current error string,
 * with file and line number, or "error not set" if not set.
 */
#define rmw_get_error_string rcutils_get_error_string

/// 通过清除先前设置的任何错误状态来重置错误状态。
/// Reset the error state by clearing any previously set error state.
#define rmw_reset_error rcutils_reset_error

/// 使用 RCUTILS_SET_ERROR_MSG 设置错误消息并附加上一个错误。
/// Set the error message using RCUTILS_SET_ERROR_MSG and append the previous error.
/**
 * 如果没有先前的错误，与 RCUTILS_SET_ERROR_MSG 行为相同。
 * If there is no previous error, has same behavior as RCUTILS_SET_ERROR_MSG.
 * \param[in] msg 要设置的错误消息。The error message to be set.
 */
#define RMW_SET_ERROR_MSG_AND_APPEND_PREV_ERROR(msg) \
  RCUTILS_SET_ERROR_MSG_AND_APPEND_PREV_ERROR(msg)

/// 使用 RCUTILS_SET_ERROR_MSG_WITH_FORMAT_STRING 设置错误消息并附加上一个错误。
/// Set the error message with RCUTILS_SET_ERROR_MSG_WITH_FORMAT_STRING and append the previous
/// error.
/**
 * 该函数使用给定的格式字符串设置错误消息，并附加并重置最新的错误字符串。
 * 结果格式化字符串将在 RCUTILS_ERROR_MESSAGE_MAX_LENGTH 处静默截断。
 *
 * 如果没有先前的错误，与 RCUTILS_SET_ERROR_MSG_WITH_FORMAT_STRING 行为相同。
 *
 * This function sets the error message using the given format string, and appends and resets the
 * latest error string.
 * The resulting formatted string is silently truncated at RCUTILS_ERROR_MESSAGE_MAX_LENGTH.
 *
 * If there is no previous error, has same behavior as RCUTILS_SET_ERROR_MSG_WITH_FORMAT_STRING.
 *
 * \param[in] format_string 用作错误消息格式的字符串。The string to be used as the format of the
 * error message. \param[in] ... 格式字符串的参数。Arguments for the format string.
 */
#define RMW_SET_ERROR_MSG_WITH_FORMAT_STRING_AND_APPEND_PREV_ERROR(format_string, ...) \
  RCUTILS_SET_ERROR_MSG_WITH_FORMAT_STRING_AND_APPEND_PREV_ERROR(format_string, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif  // RMW__ERROR_HANDLING_H_
