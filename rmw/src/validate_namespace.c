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

#include "rmw/validate_namespace.h"

#include <ctype.h>
#include <rcutils/snprintf.h>
#include <stdio.h>
#include <string.h>

#include "rmw/error_handling.h"
#include "rmw/validate_full_topic_name.h"

/**
 * @brief 验证给定的命名空间是否有效 (Validate the given namespace for its validity)
 *
 * @param[in] namespace_ 要验证的命名空间 (The namespace to be validated)
 * @param[out] validation_result 验证结果，0 表示有效，其他值表示无效 (Validation result, 0 means
 * valid, other values mean invalid)
 * @param[out] invalid_index 如果命名空间无效，则返回无效字符的索引位置 (If the namespace is
 * invalid, returns the index position of the invalid character)
 * @return rmw_ret_t 返回 RMW_RET_INVALID_ARGUMENT 如果参数无效，否则调用
 * rmw_validate_namespace_with_size 函数并返回其结果 (Returns RMW_RET_INVALID_ARGUMENT if the
 * argument is invalid, otherwise calls rmw_validate_namespace_with_size function and returns its
 * result)
 */
rmw_ret_t rmw_validate_namespace(const char* namespace_,
                                 int* validation_result,
                                 size_t* invalid_index) {
  // 检查命名空间指针是否为空 (Check if the namespace pointer is NULL)
  if (!namespace_) {
    // 如果为空，返回 RMW_RET_INVALID_ARGUMENT (If it is NULL, return RMW_RET_INVALID_ARGUMENT)
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 如果命名空间指针不为空，调用 rmw_validate_namespace_with_size 函数进行验证，并将结果返回 (If
  // the namespace pointer is not NULL, call the rmw_validate_namespace_with_size function for
  // validation and return the result)
  return rmw_validate_namespace_with_size(namespace_, strlen(namespace_), validation_result,
                                          invalid_index);
}

/**
 * @brief 验证给定的命名空间是否有效并返回验证结果。 (Validate the given namespace and return the
 * validation result.)
 *
 * @param[in] namespace_ 要验证的命名空间。 (The namespace to be validated.)
 * @param[in] namespace_length 命名空间字符串的长度。 (The length of the namespace string.)
 * @param[out] validation_result 返回的命名空间验证结果。 (The returned namespace validation
 * result.)
 * @param[out] invalid_index 如果命名空间无效，此参数将包含导致无效的字符位置。 (If the namespace is
 * invalid, this parameter will contain the position of the character that caused it to be invalid.)
 * @return rmw_ret_t 返回操作的状态。 (Return the status of the operation.)
 */
rmw_ret_t rmw_validate_namespace_with_size(const char* namespace_,
                                           size_t namespace_length,
                                           int* validation_result,
                                           size_t* invalid_index) {
  // 检查传入的命名空间是否为空指针。 (Check if the passed namespace is a null pointer.)
  if (!namespace_) {
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 检查传入的验证结果是否为空指针。 (Check if the passed validation_result is a null pointer.)
  if (!validation_result) {
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 对根命名空间的特殊情况进行处理。 (Special case handling for root namespace.)
  if (namespace_length == 1 && namespace_[0] == '/') {
    // 命名空间有效且不会超过 RMW_NAMESPACE_MAX_LENGTH，可以直接返回。 (Namespace is valid and will
    // not exceed RMW_NAMESPACE_MAX_LENGTH, can return directly.)
    *validation_result = RMW_NAMESPACE_VALID;
    return RMW_RET_OK;
  }

  // 其他所有情况都应通过验证主题名称测试。 (All other cases should pass the validate topic name
  // test.)
  int t_validation_result;
  size_t t_invalid_index;
  rmw_ret_t ret = rmw_validate_full_topic_name(namespace_, &t_validation_result, &t_invalid_index);
  if (ret != RMW_RET_OK) {
    // 错误已设置。 (Error already set.)
    return ret;
  }

  if (t_validation_result != RMW_TOPIC_VALID && t_validation_result != RMW_TOPIC_INVALID_TOO_LONG) {
    switch (t_validation_result) {
      case RMW_TOPIC_INVALID_IS_EMPTY_STRING:
        *validation_result = RMW_NAMESPACE_INVALID_IS_EMPTY_STRING;
        break;
      case RMW_TOPIC_INVALID_NOT_ABSOLUTE:
        *validation_result = RMW_NAMESPACE_INVALID_NOT_ABSOLUTE;
        break;
      case RMW_TOPIC_INVALID_ENDS_WITH_FORWARD_SLASH:
        *validation_result = RMW_NAMESPACE_INVALID_ENDS_WITH_FORWARD_SLASH;
        break;
      case RMW_TOPIC_INVALID_CONTAINS_UNALLOWED_CHARACTERS:
        *validation_result = RMW_NAMESPACE_INVALID_CONTAINS_UNALLOWED_CHARACTERS;
        break;
      case RMW_TOPIC_INVALID_CONTAINS_REPEATED_FORWARD_SLASH:
        *validation_result = RMW_NAMESPACE_INVALID_CONTAINS_REPEATED_FORWARD_SLASH;
        break;
      case RMW_TOPIC_INVALID_NAME_TOKEN_STARTS_WITH_NUMBER:
        *validation_result = RMW_NAMESPACE_INVALID_NAME_TOKEN_STARTS_WITH_NUMBER;
        break;
      default: {
        char default_err_msg[256];
        // 显式不接受返回值，该返回值为写入的字节数。 (Explicitly not taking return value which is
        // the number of bytes written.)
        int ret = rcutils_snprintf(
            default_err_msg, sizeof(default_err_msg),
            "rmw_validate_namespace(): unknown rmw_validate_full_topic_name() result '%d'",
            t_validation_result);
        if (ret < 0) {
          RMW_SET_ERROR_MSG("rmw_validate_namespace(): rcutils_snprintf() failed");
        } else {
          RMW_SET_ERROR_MSG(default_err_msg);
        }
      }
        return RMW_RET_ERROR;
    }
    if (invalid_index) {
      *invalid_index = t_invalid_index;
    }
    return RMW_RET_OK;
  }

  // 最后检查命名空间是否太长，因为这可能是一个软无效。 (Check last if the namespace is too long, as
  // it might be a soft invalidation.)
  if (namespace_length > RMW_NAMESPACE_MAX_LENGTH) {
    *validation_result = RMW_NAMESPACE_INVALID_TOO_LONG;
    if (invalid_index) {
      *invalid_index = RMW_NAMESPACE_MAX_LENGTH - 1;
    }
    return RMW_RET_OK;
  }

  // 一切正常，将结果设置为有效的命名空间，避免设置无效索引，并返回。 (Everything was OK, set the
  // result to a valid namespace, avoid setting invalid_index and return.)
  *validation_result = RMW_NAMESPACE_VALID;
  return RMW_RET_OK;
}

/**
 * @brief 返回与命名空间验证结果对应的错误字符串 (Return the error string corresponding to the
 * namespace validation result)
 *
 * @param validation_result 命名空间验证结果 (Namespace validation result)
 * @return const char* 错误字符串，如果验证通过，则返回 NULL (Error string, return NULL if
 * validation passes)
 */
const char* rmw_namespace_validation_result_string(int validation_result) {
  // 使用 switch 语句处理不同的验证结果 (Handle different validation results using a switch
  // statement)
  switch (validation_result) {
    case RMW_NAMESPACE_VALID:
      // 验证通过，返回 NULL (Validation passed, return NULL)
      return NULL;
    case RMW_NAMESPACE_INVALID_IS_EMPTY_STRING:
      // 验证失败：命名空间不能为空 (Validation failed: namespace must not be empty)
      return "namespace must not be empty";
    case RMW_NAMESPACE_INVALID_NOT_ABSOLUTE:
      // 验证失败：命名空间必须是绝对的，以 '/' 开头 (Validation failed: namespace must be absolute,
      // it must lead with a '/')
      return "namespace must be absolute, it must lead with a '/'";
    case RMW_NAMESPACE_INVALID_ENDS_WITH_FORWARD_SLASH:
      // 验证失败：命名空间不能以 '/' 结尾，除非只有一个 '/' (Validation failed: namespace must not
      // end with a '/', unless only a '/')
      return "namespace must not end with a '/', unless only a '/'";
    case RMW_NAMESPACE_INVALID_CONTAINS_UNALLOWED_CHARACTERS:
      // 验证失败：命名空间不能包含除字母数字、'_' 和 '/' 之外的字符 (Validation failed: namespace
      // must not contain characters other than alphanumerics, '_', or '/')
      return "namespace must not contain characters other than alphanumerics, '_', or '/'";
    case RMW_NAMESPACE_INVALID_CONTAINS_REPEATED_FORWARD_SLASH:
      // 验证失败：命名空间不能包含重复的 '/' (Validation failed: namespace must not contain
      // repeated '/')
      return "namespace must not contain repeated '/'";
    case RMW_NAMESPACE_INVALID_NAME_TOKEN_STARTS_WITH_NUMBER:
      // 验证失败：命名空间中的标记不能以数字开头 (Validation failed: namespace must not have a
      // token that starts with a number)
      return "namespace must not have a token that starts with a number";
    case RMW_NAMESPACE_INVALID_TOO_LONG:
      // 验证失败：命名空间长度不应超过 RMW_NAMESPACE_MAX_NAME_LENGTH (Validation failed: namespace
      // should not exceed 'RMW_NAMESPACE_MAX_NAME_LENGTH')
      return "namespace should not exceed '" RMW_STRINGIFY(RMW_NAMESPACE_MAX_NAME_LENGTH) "'";
    default:
      // 未知的验证结果代码 (Unknown result code for rmw namespace validation)
      return "unknown result code for rmw namespace validation";
  }
}
