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

#include "rmw/subscription_content_filter_options.h"

#include <stddef.h>

#include "rcutils/strdup.h"
#include "rmw/error_handling.h"

/**
 * @brief 获取一个初始化为零的内容过滤器选项结构体 (Get a zero-initialized content filter options
 * structure)
 *
 * @return 返回一个初始化为零的内容过滤器选项结构体 (Return a zero-initialized content filter
 * options structure)
 */
rmw_subscription_content_filter_options_t rmw_get_zero_initialized_content_filter_options() {
  // 返回一个初始化为零的内容过滤器选项结构体，其中 filter_expression 为 NULL，expression_parameters
  // 为零初始化的字符串数组 (Return a zero-initialized content filter options structure with
  // filter_expression set to NULL and expression_parameters as a zero-initialized string array)
  return (const rmw_subscription_content_filter_options_t){
      .filter_expression = NULL,
      .expression_parameters = rcutils_get_zero_initialized_string_array()};  // NOLINT(readability/braces):
                                                                              // false positive
}

/**
 * @brief 初始化订阅内容过滤器选项 (Initialize subscription content filter options)
 *
 * @param[in] filter_expression 过滤表达式字符串 (Filter expression string)
 * @param[in] expression_parameters_argc 表达式参数数量 (Number of expression parameters)
 * @param[in] expression_parameter_argv 表达式参数字符串数组 (Expression parameter string array)
 * @param[in] allocator 分配器指针 (Pointer to the allocator)
 * @param[out] options 订阅内容过滤器选项结构体指针 (Pointer to the subscription content filter
 * options structure)
 * @return 返回 RMW_RET_OK 或错误代码 (Return RMW_RET_OK or an error code)
 */
rmw_ret_t rmw_subscription_content_filter_options_init(
    const char *filter_expression,
    size_t expression_parameters_argc,
    const char *expression_parameter_argv[],
    const rcutils_allocator_t *allocator,
    rmw_subscription_content_filter_options_t *options) {
  // 检查 filter_expression 是否为 NULL
  // (Check if filter_expression is NULL)
  RMW_CHECK_ARGUMENT_FOR_NULL(filter_expression, RMW_RET_INVALID_ARGUMENT);
  // 如果表达式参数数量大于0，检查 expression_parameter_argv 是否为 NULL
  // (If the number of expression parameters is greater than 0, check if expression_parameter_argv
  // is NULL)
  if (expression_parameters_argc > 0) {
    RMW_CHECK_ARGUMENT_FOR_NULL(expression_parameter_argv, RMW_RET_INVALID_ARGUMENT);
  }
  // 检查分配器是否有效
  // (Check if the allocator is valid)
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);
  // 检查 options 是否为 NULL
  // (Check if options is NULL)
  RMW_CHECK_ARGUMENT_FOR_NULL(options, RMW_RET_INVALID_ARGUMENT);

  rmw_ret_t ret = RMW_RET_OK;
  rcutils_ret_t rcutils_ret;
  char *new_filter_expression = NULL;
  size_t i;

  // 复制过滤表达式字符串
  // (Duplicate the filter expression string)
  new_filter_expression = rcutils_strdup(filter_expression, *allocator);
  if (!new_filter_expression) {
    RMW_SET_ERROR_MSG("failed to copy filter expression");
    ret = RMW_RET_BAD_ALLOC;
    goto failed;
  }

  if (expression_parameters_argc > 0) {
    // 初始化表达式参数字符串数组
    // (Initialize the expression parameter string array)
    rcutils_ret_t rcutils_ret = rcutils_string_array_init(&options->expression_parameters,
                                                          expression_parameters_argc, allocator);
    if (RCUTILS_RET_OK != rcutils_ret) {
      RMW_SET_ERROR_MSG("failed to init string array for expression parameters");
      ret = RMW_RET_BAD_ALLOC;
      goto failed;
    }

    // 复制表达式参数字符串
    // (Duplicate the expression parameter strings)
    for (i = 0; i < expression_parameters_argc; i++) {
      options->expression_parameters.data[i] =
          rcutils_strdup(expression_parameter_argv[i], *allocator);
      if (!options->expression_parameters.data[i]) {
        RMW_SET_ERROR_MSG("failed to copy expression parameter");
        ret = RMW_RET_BAD_ALLOC;
        goto clear_expression_parameters;
      }
    }
  }

  // 设置过滤表达式
  // (Set the filter expression)
  options->filter_expression = new_filter_expression;

  return RMW_RET_OK;

clear_expression_parameters:
  // 清理表达式参数字符串数组
  // (Clean up the expression parameter string array)
  rcutils_ret = rcutils_string_array_fini(&options->expression_parameters);
  if (RCUTILS_RET_OK != rcutils_ret) {
    RCUTILS_SAFE_FWRITE_TO_STDERR("Failed to fini string array.\n");
  }

failed:
  // 释放分配的内存
  // (Release allocated memory)
  allocator->deallocate(new_filter_expression, allocator->state);

  return ret;
}

/**
 * @brief 设置订阅内容过滤选项 (Set subscription content filter options)
 *
 * @param[in] filter_expression 过滤表达式 (Filter expression)
 * @param[in] expression_parameters_argc 表达式参数数量 (Number of expression parameters)
 * @param[in] expression_parameter_argv 表达式参数值数组 (Array of expression parameter values)
 * @param[in] allocator 分配器 (Allocator)
 * @param[out] options 订阅内容过滤选项指针 (Pointer to subscription content filter options)
 * @return rmw_ret_t 返回操作结果 (Return the operation result)
 */
rmw_ret_t rmw_subscription_content_filter_options_set(
    const char *filter_expression,
    size_t expression_parameters_argc,
    const char *expression_parameter_argv[],
    const rcutils_allocator_t *allocator,
    rmw_subscription_content_filter_options_t *options) {
  // 结束订阅内容过滤选项，释放资源 (Finalize subscription content filter options and release
  // resources)
  rmw_ret_t ret = rmw_subscription_content_filter_options_fini(options, allocator);
  if (ret != RMW_RET_OK) {
    return ret;
  }

  // 初始化订阅内容过滤选项 (Initialize subscription content filter options)
  return rmw_subscription_content_filter_options_init(
      filter_expression, expression_parameters_argc, expression_parameter_argv, allocator, options);
}

/**
 * @brief 复制订阅内容过滤选项 (Copy subscription content filter options)
 *
 * @param[in] src 源订阅内容过滤选项指针 (Pointer to source subscription content filter options)
 * @param[in] allocator 分配器 (Allocator)
 * @param[out] dst 目标订阅内容过滤选项指针 (Pointer to destination subscription content filter
 * options)
 * @return rmw_ret_t 返回操作结果 (Return the operation result)
 */
rmw_ret_t rmw_subscription_content_filter_options_copy(
    const rmw_subscription_content_filter_options_t *src,
    const rcutils_allocator_t *allocator,
    rmw_subscription_content_filter_options_t *dst) {
  // 检查源订阅内容过滤选项是否为空 (Check if source subscription content filter options is NULL)
  RMW_CHECK_ARGUMENT_FOR_NULL(src, RMW_RET_INVALID_ARGUMENT);
  // 检查分配器是否有效 (Check if allocator is valid)
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);
  // 检查目标订阅内容过滤选项是否为空 (Check if destination subscription content filter options is
  // NULL)
  RMW_CHECK_ARGUMENT_FOR_NULL(dst, RMW_RET_INVALID_ARGUMENT);

  // 设置目标订阅内容过滤选项 (Set destination subscription content filter options)
  return rmw_subscription_content_filter_options_set(
      src->filter_expression, src->expression_parameters.size,
      (const char **)src->expression_parameters.data, allocator, dst);
}

/**
 * @brief 结束订阅内容过滤选项，释放资源 (Finalize subscription content filter options and release
 * resources)
 *
 * @param[out] options 订阅内容过滤选项指针 (Pointer to subscription content filter options)
 * @param[in] allocator 分配器 (Allocator)
 * @return rmw_ret_t 返回操作结果 (Return the operation result)
 */
rmw_ret_t rmw_subscription_content_filter_options_fini(
    rmw_subscription_content_filter_options_t *options, const rcutils_allocator_t *allocator) {
  // 检查订阅内容过滤选项是否为空 (Check if subscription content filter options is NULL)
  RMW_CHECK_ARGUMENT_FOR_NULL(options, RMW_RET_INVALID_ARGUMENT);
  // 检查分配器是否有效 (Check if allocator is valid)
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);

  // 如果过滤表达式不为空，释放内存 (If filter expression is not NULL, deallocate memory)
  if (options->filter_expression) {
    allocator->deallocate(options->filter_expression, allocator->state);
    options->filter_expression = NULL;
  }

  // 结束字符串数组并释放资源 (Finalize string array and release resources)
  rcutils_ret_t ret = rcutils_string_array_fini(&options->expression_parameters);
  if (RCUTILS_RET_OK != ret) {
    RCUTILS_SAFE_FWRITE_TO_STDERR("Failed to fini string array.\n");
    return RMW_RET_ERROR;
  }

  return RMW_RET_OK;
}
