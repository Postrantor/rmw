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

#ifndef RMW__VALIDATE_FULL_TOPIC_NAME_H_
#define RMW__VALIDATE_FULL_TOPIC_NAME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rmw/macros.h"
#include "rmw/types.h"

#define RMW_TOPIC_VALID 0
#define RMW_TOPIC_INVALID_IS_EMPTY_STRING 1
#define RMW_TOPIC_INVALID_NOT_ABSOLUTE 2
#define RMW_TOPIC_INVALID_ENDS_WITH_FORWARD_SLASH 3
#define RMW_TOPIC_INVALID_CONTAINS_UNALLOWED_CHARACTERS 4
#define RMW_TOPIC_INVALID_CONTAINS_REPEATED_FORWARD_SLASH 5
#define RMW_TOPIC_INVALID_NAME_TOKEN_STARTS_WITH_NUMBER 6
#define RMW_TOPIC_INVALID_TOO_LONG 7

#define RMW_TOPIC_MAX_NAME_LENGTH 255U /* impl constraint */ - 8U /* reserved for prefixes */

/// 确定给定的完全限定主题名称是否有效。
/// Determine if a given fully qualified topic name is valid.
/** 主题的 FQN 有效性是基于以下规则确定的：
 *   http://design.ros2.org/articles/topic_and_service_names.html
 *
 * 请注意，此功能期望上述文档中描述的任何 URL 后缀已被删除。
 * Note that this function expects any URL suffixes as described in the above
 * document to have already been removed.
 *
 * 如果 C 字符串或 validation_result 指针为空，则返回 `RMW_RET_INVALID_ARGUMENT`。
 * The topic_name should be a valid, null-terminated C string.
 * The validation_result int pointer should point to valid memory so a result
 * can be stored in it as an output variable.
 * The invalid_index size_t pointer should either point NULL or to valid memory
 * so in the event of a validation error, the location in the input string can
 * be stored therein.
 * If NULL is passed in for invalid_index, it will be not be set.
 *
 * 如果主题有效，则不会为 invalid_index 分配值。
 * The invalid_index will not be assigned a value if the topic is valid.
 *
 * validation_result 指向的 int 将存储几个可能的结果值之一（用宏定义）：
 * The int which validation_result points to will have a one of a few possible
 * results values (defined with macros) stored into it:
 *
 * - RMW_TOPIC_VALID
 * - RMW_TOPIC_INVALID_IS_EMPTY_STRING
 * - RMW_TOPIC_INVALID_NOT_ABSOLUTE
 * - RMW_TOPIC_INVALID_ENDS_WITH_FORWARD_SLASH
 * - RMW_TOPIC_INVALID_CONTAINS_UNALLOWED_CHARACTERS
 * - RMW_TOPIC_INVALID_CONTAINS_REPEATED_FORWARD_SLASH
 * - RMW_TOPIC_INVALID_NAME_TOKEN_STARTS_WITH_NUMBER
 * - RMW_TOPIC_INVALID_TOO_LONG
 *
 * 可以使用 rmw_full_topic_name_validation_result_string() 函数将结果值转换为描述。
 * The result value can be converted to a description with the
 * rmw_full_topic_name_validation_result_string() function.
 *
 * 保证最后检查 `RMW_TOPIC_INVALID_TOO_LONG`，这样如果您得到该结果，
 * 则可以假设所有其他检查都成功。这样做是为了使长度限制可以视为警告而不是错误（如果需要）。
 * The `RMW_TOPIC_INVALID_TOO_LONG` is guaranteed to be checked last, such
 * that if you get that result, then you can assume all other checks succeeded.
 * This is done so that the length limit can be treated as a warning rather
 * than an error if desired.
 *
 * \param[in] topic_name 要验证的主题名称
 * \param[out] validation_result 存储检查结果的 int
 * \param[out] invalid_index 输入字符串中发生错误的 size_t 索引
 * \returns `RMW_RET_OK` 成功运行检查，或
 * \returns `RMW_RET_INVALID_ARGUMENT` 参数无效，或
 * \returns `RMW_RET_ERROR` 发生未指定错误。
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_validate_full_topic_name(const char* topic_name,
                                       int* validation_result,
                                       size_t* invalid_index);

/// 判断给定的主题名称是否有效。 (Determine if a given topic name is valid.)
/**
 * 这是一个带有额外参数（主题名称长度）的重载函数。
 * (This is an overload with an extra parameter for the length of topic_name.)
 *
 * \sa rmw_validate_full_topic_name(const char *, int *, size_t *)
 *
 * \param[in] topic_name 需要验证的主题名称 (topic name to be validated)
 * \param[in] topic_name_length 主题名称中的字符数量 (The number of characters in topic_name.)
 * \param[out] validation_result 用于存储检查结果的整数 (int in which the result of the check is
 * stored) \param[out] invalid_index 输入字符串中发生错误的索引 (size_t index of the input string
 * where an error occurred) \returns 如果成功运行检查, 返回 `RMW_RET_OK`, 或者 (on successfully
 * running the check, or) \returns 如果参数无效, 返回 `RMW_RET_INVALID_ARGUMENT`, 或者 (on invalid
 * parameters, or) \returns 当发生未指定错误时, 返回 `RMW_RET_ERROR`. (when an unspecified error
 * occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_validate_full_topic_name_with_size(const char* topic_name,
                                                 size_t topic_name_length,
                                                 int* validation_result,
                                                 size_t* invalid_index);

/// 返回验证结果描述，如果为 RMW_TOPIC_VALID，则返回 NULL。
/// (Return a validation result description, or NULL if RMW_TOPIC_VALID.)
/**
 * \param[in] validation_result 验证结果 (the result of validation)
 * \returns 如果验证结果为 `RMW_TOPIC_VALID`, 返回 `NULL`, 或者 (if the validation result is
 * `RMW_TOPIC_VALID`, or) \returns 返回表示验证结果的字符串。 (a string representing the validation
 * result.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
const char* rmw_full_topic_name_validation_result_string(int validation_result);

#ifdef __cplusplus
}
#endif

#endif  // RMW__VALIDATE_FULL_TOPIC_NAME_H_
