// Copyright 2021 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__SUBSCRIPTION_CONTENT_FILTER_OPTIONS_H_
#define RMW__SUBSCRIPTION_CONTENT_FILTER_OPTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rcutils/allocator.h"
#include "rcutils/types.h"
#include "rmw/macros.h"
#include "rmw/ret_types.h"
#include "rmw/visibility_control.h"

// 定义一个结构体类型，表示订阅器内容过滤选项
// Define a struct type representing subscription content filter options
typedef struct RMW_PUBLIC_TYPE rmw_subscription_content_filter_options_s {
  /**
   * 指定用于选择感兴趣的数据样本的条件。
   * 类似于 SQL 子句中的 WHERE 部分。
   *
   * Specify the criteria to select the data samples of interest.
   * It is similar to the WHERE part of an SQL clause.
   */
  char* filter_expression;

  /**
   * 给过滤表达式中的占位符‘parameters’（即，“%n”标记从0开始）提供值。提供的参数数量必须符合请求的值。
   * 如果在过滤表达式中没有“%n”占位符，它可以是 NULL。
   * 最大索引号必须小于100。
   *
   * Give values to the tokens placeholder ‘parameters’ (i.e., "%n" tokens begin from 0) in the
   * filter_expression. The number of supplied parameters must fit with the requested values.
   * It can be NULL if there is no "%n" tokens placeholder in filter_expression.
   * The maximum index number must be smaller than 100.
   */
  rcutils_string_array_t expression_parameters;
} rmw_subscription_content_filter_options_t;

/**
 * @brief 获取零初始化的内容过滤器选项
 * @return 返回一个零初始化的内容过滤器选项结构体实例
 *
 * @brief Get zero initialized content filter options.
 * @return Returns a zero-initialized content filter options struct instance.
 */
RMW_PUBLIC
rmw_subscription_content_filter_options_t rmw_get_zero_initialized_content_filter_options();

/// 初始化给定的内容过滤器选项。 (Initialize the given content filter options.)
/**
 * \param[in] filter_expression 过滤表达式。 (The filter expression.)
 * \param[in] expression_parameters_argc 表达式参数的 argc。 (The expression parameters argc.)
 * \param[in] expression_parameter_argv 表达式参数的 argv。 (The expression parameters argv.)
 * \param[in] allocator 当复制数据到内容过滤器选项时使用的分配器。 (The allocator used when copying
 * data to the content filter options.) \param[out] options 要设置的内容过滤器选项。 (The content
 * filter options to be set.) \returns RMW_RET_INVALID_ARGUMENT, or \returns RMW_RET_BAD_ALLOC, or
 * \returns RMW_RET_OK
 */
RMW_PUBLIC
rmw_ret_t rmw_subscription_content_filter_options_init(
    const char* filter_expression,      // 过滤表达式 (Filter expression)
    size_t expression_parameters_argc,  // 表达式参数的 argc (Expression parameters argc)
    const char* expression_parameter_argv[],  // 表达式参数的 argv (Expression parameters argv)
    const rcutils_allocator_t* allocator,     // 分配器 (Allocator)
    rmw_subscription_content_filter_options_t* options);  // 内容过滤器选项 (Content filter options)

/// 设置给定的内容过滤器选项。 (Set the given content filter options.)
/**
 * \param[in] filter_expression 过滤表达式。 (The filter expression.)
 * \param[in] expression_parameters_argc 表达式参数的 argc。 (The expression parameters argc.)
 * \param[in] expression_parameter_argv 表达式参数的 argv。 (The expression parameters argv.)
 * \param[in] allocator 当复制数据到内容过滤器选项时使用的分配器。 (The allocator used when copying
 * data to the content filter options.) \param[out] options 要设置的内容过滤器选项。 (The content
 * filter options to be set.) \returns RMW_RET_INVALID_ARGUMENT, or \returns RMW_RET_BAD_ALLOC, or
 * \returns RMW_RET_OK
 */
RMW_PUBLIC
rmw_ret_t rmw_subscription_content_filter_options_set(
    const char* filter_expression,      // 过滤表达式 (Filter expression)
    size_t expression_parameters_argc,  // 表达式参数的 argc (Expression parameters argc)
    const char* expression_parameter_argv[],  // 表达式参数的 argv (Expression parameters argv)
    const rcutils_allocator_t* allocator,     // 分配器 (Allocator)
    rmw_subscription_content_filter_options_t* options);  // 内容过滤器选项 (Content filter options)

/// 复制给定的内容过滤器选项。
/// Copy the given content filter options.
/**
 * \param[in] src 需要复制的内容过滤器选项。
 * \param[in] allocator 在将数据复制到新的内容过滤器选项时使用的分配器。
 * \param[out] dst 要设置的内容过滤器选项。
 * \returns RMW_RET_INVALID_ARGUMENT, 或
 * \returns RMW_RET_BAD_ALLOC, 或
 * \returns RMW_RET_OK
 */
RMW_PUBLIC
rmw_ret_t rmw_subscription_content_filter_options_copy(
    const rmw_subscription_content_filter_options_t* src,  // 需要复制的源内容过滤器选项
    const rcutils_allocator_t* allocator,                  // 用于分配内存的分配器
    rmw_subscription_content_filter_options_t* dst);  // 目标内容过滤器选项，将被设置为源选项的副本

/// 结束内容过滤器选项。
/// Finalize the content filter options.
/**
 * \param[in] options 需要结束的内容过滤器选项。
 * \param[in] allocator 用于释放内容过滤器选项的分配器。
 * \returns RMW_RET_INVALID_ARGUMENT, 或
 * \returns RMW_RET_ERROR, 或
 * \returns RMW_RET_OK
 */
RMW_PUBLIC
rmw_ret_t rmw_subscription_content_filter_options_fini(
    rmw_subscription_content_filter_options_t* options,  // 需要结束的内容过滤器选项
    const rcutils_allocator_t* allocator);               // 用于释放内存的分配器

#ifdef __cplusplus
}
#endif

#endif  // RMW__SUBSCRIPTION_CONTENT_FILTER_OPTIONS_H_
