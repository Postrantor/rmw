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

#include <ctype.h>
#include <rmw/validate_full_topic_name.h>
#include <string.h>

#include "rcutils/isalnum_no_locale.h"

/**
 * @brief 验证完整的主题名称 (Validate the full topic name)
 *
 * @param[in] topic_name 要验证的主题名称 (The topic name to be validated)
 * @param[out] validation_result 验证结果，指向一个整数，表示验证状态 (Validation result, pointing
 * to an integer representing the validation status)
 * @param[out] invalid_index 无效索引，指向一个 size_t 类型的变量，存储第一个无效字符的位置 (Invalid
 * index, pointing to a variable of type size_t, storing the position of the first invalid
 * character)
 * @return rmw_ret_t 返回 RMW_RET_INVALID_ARGUMENT 如果 topic_name 为空，否则返回验证结果 (Return
 * RMW_RET_INVALID_ARGUMENT if topic_name is NULL, otherwise return the validation result)
 */
rmw_ret_t rmw_validate_full_topic_name(const char* topic_name,
                                       int* validation_result,
                                       size_t* invalid_index) {
  // 如果 topic_name 为空，返回 RMW_RET_INVALID_ARGUMENT
  // If topic_name is NULL, return RMW_RET_INVALID_ARGUMENT
  if (!topic_name) {
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 调用 rmw_validate_full_topic_name_with_size 函数进行验证，并传入 topic_name 的长度
  // Call the rmw_validate_full_topic_name_with_size function for validation and pass in the length
  // of topic_name
  return rmw_validate_full_topic_name_with_size(topic_name, strlen(topic_name), validation_result,
                                                invalid_index);
}

/**
 * @brief 验证主题名的有效性和长度（Validate the full topic name with its size）
 *
 * @param[in] topic_name 主题名称（Topic name）
 * @param[in] topic_name_length 主题名称的长度（Length of the topic name）
 * @param[out] validation_result 验证结果（Validation result）
 * @param[out] invalid_index 非法索引（Invalid index）
 * @return rmw_ret_t 返回验证状态（Return the validation status）
 */
rmw_ret_t rmw_validate_full_topic_name_with_size(const char* topic_name,
                                                 size_t topic_name_length,
                                                 int* validation_result,
                                                 size_t* invalid_index) {
  // 检查 topic_name 是否为空（Check if topic_name is NULL）
  if (!topic_name) {
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 检查 validation_result 是否为空（Check if validation_result is NULL）
  if (!validation_result) {
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 如果 topic_name_length 为0，设置验证结果为 RMW_TOPIC_INVALID_IS_EMPTY_STRING（If
  // topic_name_length is 0, set validation_result to RMW_TOPIC_INVALID_IS_EMPTY_STRING）
  if (topic_name_length == 0) {
    *validation_result = RMW_TOPIC_INVALID_IS_EMPTY_STRING;
    if (invalid_index) {
      *invalid_index = 0;
    }
    return RMW_RET_OK;
  }
  // 检查主题名称是否以'/'开头（Check if topic_name starts with '/'）
  if (topic_name[0] != '/') {
    *validation_result = RMW_TOPIC_INVALID_NOT_ABSOLUTE;
    if (invalid_index) {
      *invalid_index = 0;
    }
    return RMW_RET_OK;
  }
  // 在此点上，topic_name_length 大于等于1（At this point, topic_name_length is >= 1）
  if (topic_name[topic_name_length - 1] == '/') {
    // 捕获以'/'结尾的情况（Catches cases ending with '/'）
    *validation_result = RMW_TOPIC_INVALID_ENDS_WITH_FORWARD_SLASH;
    if (invalid_index) {
      *invalid_index = topic_name_length - 1;
    }
    return RMW_RET_OK;
  }
  // 检查不允许的字符（Check for unallowed characters）
  for (size_t i = 0; i < topic_name_length; ++i) {
    if (rcutils_isalnum_no_locale(topic_name[i])) {
      // 如果是字母数字字符，即 [0-9|A-Z|a-z]，继续（If it is an alphanumeric character, i.e.
      // [0-9|A-Z|a-z], continue）
      continue;
    } else if (topic_name[i] == '_') {
      // 如果是下划线，继续（If it is an underscore, continue）
      continue;
    } else if (topic_name[i] == '/') {
      // 如果是正斜杠，继续（If it is a forward slash, continue）
      continue;
    } else {
      // 如果都不是，则表示在 FQN 主题名称中存在不允许的字符（If none of these, then it is an
      // unallowed character in a FQN topic name）
      *validation_result = RMW_TOPIC_INVALID_CONTAINS_UNALLOWED_CHARACTERS;
      if (invalid_index) {
        *invalid_index = i;
      }
      return RMW_RET_OK;
    }
  }
  // 检查双'/'和以数字开头的标记（Check for double '/' and tokens that start with a number）
  for (size_t i = 0; i < topic_name_length; ++i) {
    if (i == topic_name_length - 1) {
      // 如果这是最后一个字符，那么没有什么可以检查的（If this is the last character, then nothing
      // to check）
      continue;
    }
    // 过了这一点，假设 i+1 是一个有效索引（Past this point, assuming i+1 is a valid index）
    if (topic_name[i] == '/') {
      if (topic_name[i + 1] == '/') {
        *validation_result = RMW_TOPIC_INVALID_CONTAINS_REPEATED_FORWARD_SLASH;
        if (invalid_index) {
          *invalid_index = i + 1;
        }
        return RMW_RET_OK;
      }
      if (isdigit(topic_name[i + 1]) != 0) {
        // 这是'/'后面跟着数字的情况，即 [0-9]（This is the case where a '/' is followed by a
        // number, i.e. [0-9]）
        *validation_result = RMW_TOPIC_INVALID_NAME_TOKEN_STARTS_WITH_NUMBER;
        if (invalid_index) {
          *invalid_index = i + 1;
        }
        return RMW_RET_OK;
      }
    }
  }
  // 最后检查主题名称是否过长，因为它可能是软无效（Check if the topic name is too long last, since
  // it might be a soft invalidation）
  if (topic_name_length > RMW_TOPIC_MAX_NAME_LENGTH) {
    *validation_result = RMW_TOPIC_INVALID_TOO_LONG;
    if (invalid_index) {
      *invalid_index = RMW_TOPIC_MAX_NAME_LENGTH - 1;
    }
    return RMW_RET_OK;
  }
  // 一切正常，将结果设置为有效主题，避免设置 invalid_index，并返回（Everything was ok, set result
  // to valid topic, avoid setting invalid_index, and return）
  *validation_result = RMW_TOPIC_VALID;
  return RMW_RET_OK;
}

/**
 * @brief 根据验证结果返回相应的错误信息 (Return the corresponding error message based on the
 * validation result)
 *
 * @param[in] validation_result 验证结果 (Validation result)
 * @return const char* 错误信息字符串指针 (Pointer to the error message string)
 */
const char* rmw_full_topic_name_validation_result_string(int validation_result) {
  // 使用 switch 语句根据不同的验证结果返回对应的错误信息
  // (Use switch statement to return the corresponding error message based on different validation
  // results)
  switch (validation_result) {
    case RMW_TOPIC_VALID:
      // 主题名有效，返回 NULL (Topic name is valid, return NULL)
      return NULL;
    case RMW_TOPIC_INVALID_IS_EMPTY_STRING:
      // 主题名不能为空 (Topic name must not be empty)
      return "topic name must not be empty";
    case RMW_TOPIC_INVALID_NOT_ABSOLUTE:
      // 主题名必须是绝对路径，以 '/' 开头 (Topic name must be absolute, it must lead with a '/')
      return "topic name must be absolute, it must lead with a '/'";
    case RMW_TOPIC_INVALID_ENDS_WITH_FORWARD_SLASH:
      // 主题名不能以 '/' 结尾 (Topic name must not end with a '/')
      return "topic name must not end with a '/'";
    case RMW_TOPIC_INVALID_CONTAINS_UNALLOWED_CHARACTERS:
      // 主题名只能包含字母数字、'_' 或 '/' (Topic name must not contain characters other than
      // alphanumerics, '_', or '/')
      return "topic name must not contain characters other than alphanumerics, '_', or '/'";
    case RMW_TOPIC_INVALID_CONTAINS_REPEATED_FORWARD_SLASH:
      // 主题名不能包含重复的 '/' (Topic name must not contain repeated '/')
      return "topic name must not contain repeated '/'";
    case RMW_TOPIC_INVALID_NAME_TOKEN_STARTS_WITH_NUMBER:
      // 主题名的标记不能以数字开头 (Topic name must not have a token that starts with a number)
      return "topic name must not have a token that starts with a number";
    case RMW_TOPIC_INVALID_TOO_LONG:
      // 主题名长度不应超过 RMW_TOPIC_MAX_NAME_LENGTH (Topic length should not exceed
      // 'RMW_TOPIC_MAX_NAME_LENGTH')
      return "topic length should not exceed '" RMW_STRINGIFY(RMW_TOPIC_MAX_NAME_LENGTH) "'";
    default:
      // 未知的验证结果代码 (Unknown result code for rmw topic name validation)
      return "unknown result code for rmw topic name validation";
  }
}
