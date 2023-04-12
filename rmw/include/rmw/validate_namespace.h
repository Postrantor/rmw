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

#ifndef RMW__VALIDATE_NAMESPACE_H_
#define RMW__VALIDATE_NAMESPACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rmw/macros.h"
#include "rmw/types.h"
#include "rmw/validate_full_topic_name.h"

// 命名空间有效 (Namespace is valid)
#define RMW_NAMESPACE_VALID 0
// 命名空间无效：为空字符串 (Namespace is invalid: empty string)
#define RMW_NAMESPACE_INVALID_IS_EMPTY_STRING 1
// 命名空间无效：不是绝对路径 (Namespace is invalid: not an absolute path)
#define RMW_NAMESPACE_INVALID_NOT_ABSOLUTE 2
// 命名空间无效：以正斜杠结尾 (Namespace is invalid: ends with a forward slash)
#define RMW_NAMESPACE_INVALID_ENDS_WITH_FORWARD_SLASH 3
// 命名空间无效：包含不允许的字符 (Namespace is invalid: contains disallowed characters)
#define RMW_NAMESPACE_INVALID_CONTAINS_UNALLOWED_CHARACTERS 4
// 命名空间无效：包含重复的正斜杠 (Namespace is invalid: contains repeated forward slashes)
#define RMW_NAMESPACE_INVALID_CONTAINS_REPEATED_FORWARD_SLASH 5
// 命名空间无效：名称标记以数字开头 (Namespace is invalid: name token starts with a number)
#define RMW_NAMESPACE_INVALID_NAME_TOKEN_STARTS_WITH_NUMBER 6
// 命名空间无效：太长 (Namespace is invalid: too long)
#define RMW_NAMESPACE_INVALID_TOO_LONG 7
// An additional 2 characters are reserved for the shortest possible topic, e.g. '/X'.
#define RMW_NAMESPACE_MAX_LENGTH (RMW_TOPIC_MAX_NAME_LENGTH - 2U)

/// 确定给定的命名空间是否有效。 (Determine if a given namespace is valid.)
/** 命名空间的有效性基于此处定义的主题规则：(Validity of a namespace is based on rules for a topic
 * defined here:)
 *
 *   http://design.ros2.org/articles/topic_and_service_names.html
 *
 * 请注意，此功能预计不会有上述文档中描述的可用于主题和服务的 URL 后缀。(Note that this function
 * expects that there are no URL suffixes as described in the above document which can be used for
 * topics and services.)
 *
 * 如果 C 字符串或 validation_result 指针为 null，则返回 `RMW_RET_INVALID_ARGUMENT`。
 * (If either the C string or validation_result pointer are null, then `RMW_RET_INVALID_ARGUMENT`
 * will be returned.) namespace_ 应该是一个有效的，以空字符结尾的 C 字符串。(The namespace_ should
 * be a valid, null-terminated C string.) validation_result int
 * 指针应指向有效内存，以便将结果作为输出变量存储在其中。(The validation_result int pointer should
 * point to valid memory so a result can be stored in it as an output variable.) invalid_index
 * size_t 指针应指向 NULL 或有效内存，以便在验证错误发生时，可以将输入字符串中的位置存储在其中。(The
 * invalid_index size_t pointer should either point NULL or to valid memory so in the event of a
 * validation error, the location in the input string can be stored therein.) 如果传入 NULL 作为
 * invalid_index，它将不会被设置。(If NULL is passed in for invalid_index, it will be not be set.)
 *
 * 如果命名空间有效，则不会为 invalid_index 分配值。(The invalid_index will not be assigned a value
 * if the namespace is valid.)
 *
 * validation_result 指向的 int 将有几个可能结果值（用宏定义）存储在其中：(The int which
 * validation_result points to will have a one of a few possible results values (defined with
 * macros) stored into it:)
 *
 * - RMW_NAMESPACE_VALID
 * - RMW_NAMESPACE_INVALID_IS_EMPTY_STRING
 * - RMW_NAMESPACE_INVALID_NOT_ABSOLUTE
 * - RMW_NAMESPACE_INVALID_ENDS_WITH_FORWARD_SLASH
 * - RMW_NAMESPACE_INVALID_CONTAINS_UNALLOWED_CHARACTERS
 * - RMW_NAMESPACE_INVALID_CONTAINS_REPEATED_FORWARD_SLASH
 * - RMW_NAMESPACE_INVALID_NAME_TOKEN_STARTS_WITH_NUMBER
 * - RMW_NAMESPACE_INVALID_TOO_LONG
 *
 * 可以使用 rmw_namespace_validation_result_string() 函数将结果值转换为描述。(The result value can
 * be converted to a description with the rmw_namespace_validation_result_string() function.)
 *
 * ``RMW_NAMESPACE_INVALID_ENDS_WITH_FORWARD_SLASH``
 * 验证结果不适用于``"/"``,这是一个有效的命名空间。(``RMW_NAMESPACE_INVALID_ENDS_WITH_FORWARD_SLASH``
 * validation result does not apply to ``"/"``, which is a valid namespace.)
 *
 * 保证最后检查``RMW_NAMESPACE_INVALID_TOO_LONG``，这样如果您得到该结果，那么您可以假设所有其他检查都成功了。(``RMW_NAMESPACE_INVALID_TOO_LONG``
 * is guaranteed to be checked last, such that if you get that result, then you can assume all other
 * checks succeeded.) 这样做是为了使长度限制可以被视为警告而不是错误（如果需要的话）。(This is done
 * so that the length limit can be treated as a warning rather than an error if desired.)
 *
 * 如果返回非 RMW_RET_OK 返回值，则将设置 RMW 错误消息。(If a non RMW_RET_OK return value is
 * returned, the RMW error message will be set)
 *
 * \param[in] namespace_ 要验证的命名空间 (namespace to be validated)
 * \param[out] validation_result 存储检查结果的 int (int in which the result of the check is stored)
 * \param[out] invalid_index 发生错误的输入字符串的索引 (index of the input string where an error
 * occurred) \returns `RMW_RET_OK` 成功运行检查，或 (on successfully running the check, or) \returns
 * `RMW_RET_INVALID_ARGUMENT` 无效参数，或 (on invalid parameters, or) \returns `RMW_RET_ERROR`
 * 发生未指定错误时。 (when an unspecified error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_validate_namespace(const char* namespace_,
                                 int* validation_result,
                                 size_t* invalid_index);

/// 判断给定的命名空间是否有效 (Determine if a given namespace is valid).
/**
 * 这是一个重载函数，它有一个额外的参数表示 namespace_ 的长度。
 * 如果返回非 RMW_RET_OK 的值，则会设置 RMW 错误消息。
 *
 * \sa rmw_validate_namespace(const char *, int *, size_t *)
 *
 * \param[in] namespace_ 需要验证的命名空间 (namespace to be validated)
 * \param[in] namespace_length namespace_ 中的字符数目 (The number of characters in namespace_)
 * \param[out] validation_result 存储检查结果的整数 (int in which the result of the check is stored)
 * \param[out] invalid_index 输入字符串中发生错误的索引 (index of the input string where an error
 * occurred) \returns 成功运行检查时返回 `RMW_RET_OK`，或 (on successfully running the check, or)
 * \returns 参数无效时返回 `RMW_RET_INVALID_ARGUMENT`，或 (on invalid parameters, or)
 * \returns 发生未指定错误时返回 `RMW_RET_ERROR`。(when an unspecified error occurs.)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_validate_namespace_with_size(const char* namespace_,
                                           size_t namespace_length,
                                           int* validation_result,
                                           size_t* invalid_index);

/// 返回验证结果描述，如果为 RMW_NAMESPACE_VALID 则返回 NULL。
/// (Return a validation result description, or NULL if RMW_NAMESPACE_VALID.)
/**
 * \param[in] validation_result 验证结果 (the result of validation)
 * \returns 如果验证结果为 `RMW_NAMESPACE_VALID`，则返回 `NULL`，或 (if the validation result is
 * `RMW_NAMESPACE_VALID`, or) \returns 表示验证结果的字符串 (a string representing the validation
 * result).
 */
RMW_PUBLIC
RMW_WARN_UNUSED
const char* rmw_namespace_validation_result_string(int validation_result);

#ifdef __cplusplus
}
#endif

#endif  // RMW__VALIDATE_NAMESPACE_H_
