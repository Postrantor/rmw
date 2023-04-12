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

#ifndef RMW__VALIDATE_NODE_NAME_H_
#define RMW__VALIDATE_NODE_NAME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rmw/macros.h"
#include "rmw/types.h"

// 定义了 ROS2 RMW 节点名称的有效性状态 (Define the valid state for ROS2 RMW node names)
#define RMW_NODE_NAME_VALID 0
// 当节点名称为空字符串时，表示无效状态 (Invalid state when the node name is an empty string)
#define RMW_NODE_NAME_INVALID_IS_EMPTY_STRING 1
// 当节点名称包含不允许的字符时，表示无效状态 (Invalid state when the node name contains disallowed
// characters)
#define RMW_NODE_NAME_INVALID_CONTAINS_UNALLOWED_CHARACTERS 2
// 当节点名称以数字开头时，表示无效状态 (Invalid state when the node name starts with a number)
#define RMW_NODE_NAME_INVALID_STARTS_WITH_NUMBER 3
// 当节点名称过长时，表示无效状态 (Invalid state when the node name is too long)
#define RMW_NODE_NAME_INVALID_TOO_LONG 4
// 定义了 RMW 节点名称的最大长度为 255 (Define the maximum length of RMW node names as 255)
#define RMW_NODE_NAME_MAX_NAME_LENGTH 255 /* arbitrary constraint */

/// 判断节点名称是否有效（Determine if a node name is valid）
/**
 * 节点名称必须遵循以下规则（Node names must follow these rules）：
 *
 * - 不能为空字符串（must not be an empty string）
 * - 只能包含字母数字字符和下划线（a-z|A-Z|0-9|_）（must only contain alphanumeric characters and
 * underscores (a-z|A-Z|0-9|_)）
 * - 不能以数字开头（must not start with a number）
 *
 * 如果节点名称 C 字符串或 validation_result 指针为 null，则返回 `RMW_RET_INVALID_ARGUMENT`。
 * node_name 应该是一个有效的，以空字符结尾的 C 字符串。
 * validation_result int 指针应指向有效内存，以便将结果作为输出变量存储在其中。
 * invalid_index size_t 指针应指向 NULL
 * 或有效内存，以便在验证错误时，可以将输入字符串中的位置存储在其中。 如果传递 NULL 给
 * invalid_index，它将不会被设置。
 *
 * 如果节点名称有效，invalid_index 将不会被赋值。
 *
 * validation_result 指向的整数将具有一些可能的结果值（使用宏定义）之一存储在其中：
 *
 * - RMW_NODE_NAME_VALID
 * - RMW_NODE_NAME_INVALID_IS_EMPTY_STRING
 * - RMW_NODE_NAME_INVALID_CONTAINS_UNALLOWED_CHARACTERS
 * - RMW_NODE_NAME_INVALID_STARTS_WITH_NUMBER
 * - RMW_NODE_NAME_INVALID_TOO_LONG
 *
 * 结果值可以使用 rmw_node_name_validation_result_string() 函数转换为描述。
 *
 * 保证最后检查
 * `RMW_NODE_NAME_INVALID_TOO_LONG`，这样如果您得到该结果，则可以假定所有其他检查都成功了。
 * 这样做是为了使长度限制可以视为警告而不是错误（if desired）。
 *
 * \param[in] node_name 要验证的节点名称（node name to be validated）
 * \param[out] validation_result 存储检查结果的 int（int in which the result of the check is
 * stored） \param[out] invalid_index 输入字符串中发生错误的 size_t 索引（size_t index of the input
 * string where an error occurred） \returns `RMW_RET_OK` 成功运行检查（on successfully running the
 * check），或 \returns `RMW_RET_INVALID_ARGUMENT` 非法参数（on invalid parameters），或 \returns
 * `RMW_RET_ERROR` 发生未指定错误（when an unspecified error occurs）。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_validate_node_name(const char* node_name,
                                 int* validation_result,
                                 size_t* invalid_index);

/// 确定给定的节点名称是否有效。 (Determine if a given node name is valid)
/**
 * 这是一个带有额外参数的重载，用于表示 node_name 的长度。
 * (This is an overload with an extra parameter for the length of node_name.)
 *
 * \sa rmw_validate_node_name(const char *, int *, size_t *)
 *
 * \param[in] node_name 需要验证的节点名称 (node name to be validated)
 * \param[in] node_name_length node_name 中的字符数量。 (The number of characters in node_name.)
 * \param[out] validation_result 用于存储检查结果的整数 (int in which the result of the check is
 * stored) \param[out] invalid_index 输入字符串中发生错误的索引 (size_t index of the input string
 * where an error occurred) \returns 如果成功运行检查，则返回 `RMW_RET_OK`，或者 (Returns
 * `RMW_RET_OK` on successfully running the check, or) \returns 如果参数无效，则返回
 * `RMW_RET_INVALID_ARGUMENT`，或者 (Returns `RMW_RET_INVALID_ARGUMENT` on invalid parameters, or)
 * \returns 当发生未指定的错误时，返回 `RMW_RET_ERROR`。 (Returns `RMW_RET_ERROR` when an
 * unspecified error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_validate_node_name_with_size(const char* node_name,
                                           size_t node_name_length,
                                           int* validation_result,
                                           size_t* invalid_index);

/// 返回验证结果描述，如果 RMW_NODE_NAME_VALID 则返回 NULL。
/// (Return a validation result description, or NULL if RMW_NODE_NAME_VALID.)
/**
 * \param[in] validation_result 验证结果 (the result of validation)
 * \returns 如果验证结果为 `RMW_NODE_NAME_VALID`，则返回 `NULL`，或者
 *          (Returns `NULL` if the validation result is `RMW_NODE_NAME_VALID`, or)
 * \returns 返回表示验证结果的字符串。 (Returns a string representing the validation result.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
const char* rmw_node_name_validation_result_string(int validation_result);

#ifdef __cplusplus
}
#endif

#endif  // RMW__VALIDATE_NODE_NAME_H_
