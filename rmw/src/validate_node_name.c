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
#include <rmw/validate_node_name.h>
#include <string.h>

#include "rcutils/isalnum_no_locale.h"

/**
 * @brief 验证节点名称的有效性 (Validate the validity of a node name)
 *
 * @param[in] node_name 要验证的节点名称 (The node name to be validated)
 * @param[out] validation_result 验证结果，将存储在这个指针所指向的变量中 (Validation result, will
 * be stored in the variable pointed by this pointer)
 * @param[out] invalid_index 无效字符的索引，如果有的话 (Index of the invalid character, if any)
 * @return rmw_ret_t 返回一个表示操作结果的枚举值 (Returns an enumeration value representing the
 * operation result)
 */
rmw_ret_t rmw_validate_node_name(const char* node_name,
                                 int* validation_result,
                                 size_t* invalid_index) {
  // 检查 node_name 是否为 NULL，如果是，则返回 RMW_RET_INVALID_ARGUMENT 错误
  // (Check if node_name is NULL, if it is, return RMW_RET_INVALID_ARGUMENT error)
  if (!node_name) {
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 使用给定的节点名称长度调用 rmw_validate_node_name_with_size 函数，并返回结果
  // (Call rmw_validate_node_name_with_size function with the given node name length and return the
  // result)
  return rmw_validate_node_name_with_size(node_name, strlen(node_name), validation_result,
                                          invalid_index);
}

/**
 * @brief 验证节点名称及其长度，并返回验证结果和无效索引（Validate the node name and its length, and
 * return the validation result and invalid index）
 *
 * @param[in] node_name 节点名称（Node name）
 * @param[in] node_name_length 节点名称的长度（Length of the node name）
 * @param[out] validation_result 验证结果指针，用于存储验证结果（Pointer to store the validation
 * result）
 * @param[out] invalid_index 无效索引指针，用于存储发现的第一个无效字符的索引（Pointer to store the
 * index of the first invalid character found）
 * @return rmw_ret_t 返回操作状态（Return operation status）
 */
rmw_ret_t rmw_validate_node_name_with_size(const char* node_name,
                                           size_t node_name_length,
                                           int* validation_result,
                                           size_t* invalid_index) {
  // 检查节点名称是否为空（Check if the node name is NULL）
  if (!node_name) {
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 检查验证结果指针是否为空（Check if the validation result pointer is NULL）
  if (!validation_result) {
    return RMW_RET_INVALID_ARGUMENT;
  }
  // 如果节点名称长度为0，则设置验证结果为无效且为空字符串（If the node name length is 0, set the
  // validation result as invalid and an empty string）
  if (node_name_length == 0) {
    *validation_result = RMW_NODE_NAME_INVALID_IS_EMPTY_STRING;
    if (invalid_index) {
      *invalid_index = 0;
    }
    return RMW_RET_OK;
  }
  // 检查不允许的字符（Check for unallowed characters）
  for (size_t i = 0; i < node_name_length; ++i) {
    if (rcutils_isalnum_no_locale(node_name[i])) {
      // 如果它是字母数字字符，即[0-9|A-Z|a-z]，则继续（If it is an alphanumeric character, i.e.,
      // [0-9|A-Z|a-z], continue）
      continue;
    } else if (node_name[i] == '_') {
      // 如果它是下划线，则继续（If it is an underscore, continue）
      continue;
    } else {
      // 如果它不是这些字符，则表示节点名称中存在不允许的字符（If it is none of these, then it is an
      // unallowed character in a node name）
      *validation_result = RMW_NODE_NAME_INVALID_CONTAINS_UNALLOWED_CHARACTERS;
      if (invalid_index) {
        *invalid_index = i;
      }
      return RMW_RET_OK;
    }
  }
  // 检查节点名称是否以数字开头，即[0-9]（Check if the node name starts with a number, i.e., [0-9]）
  if (isdigit(node_name[0]) != 0) {
    *validation_result = RMW_NODE_NAME_INVALID_STARTS_WITH_NUMBER;
    if (invalid_index) {
      *invalid_index = 0;
    }
    return RMW_RET_OK;
  }
  // 最后检查节点名称是否过长，因为它可能是软无效（Check if the node name is too long last, since it
  // might be a soft invalidation）
  if (node_name_length > RMW_NODE_NAME_MAX_NAME_LENGTH) {
    *validation_result = RMW_NODE_NAME_INVALID_TOO_LONG;
    if (invalid_index) {
      *invalid_index = RMW_NODE_NAME_MAX_NAME_LENGTH - 1;
    }
    return RMW_RET_OK;
  }
  // 一切正常，将结果设置为有效节点名称，避免设置无效索引，并返回（Everything was ok, set the result
  // to a valid node name, avoid setting invalid_index, and return）
  *validation_result = RMW_NODE_NAME_VALID;
  return RMW_RET_OK;
}

/**
 * @brief 返回节点名称验证结果的字符串表示
 *        Return the string representation of the node name validation result.
 *
 * @param[in] validation_result 验证结果代码，来自 rmw_node_name_validation_result_t 枚举
 *                               The validation result code, from the
 * rmw_node_name_validation_result_t enumeration.
 * @return 对应于验证结果的字符串指针，如果验证通过则返回 NULL
 *         A string pointer corresponding to the validation result, or NULL if the validation
 * passes.
 */
const char* rmw_node_name_validation_result_string(int validation_result) {
  // 使用 switch 语句根据不同的验证结果返回相应的错误信息
  // Use a switch statement to return the appropriate error message based on the validation result
  switch (validation_result) {
    case RMW_NODE_NAME_VALID:
      // 验证结果有效，返回 NULL
      // Validation result is valid, return NULL
      return NULL;
    case RMW_NODE_NAME_INVALID_IS_EMPTY_STRING:
      // 节点名称不能为空
      // Node name must not be empty
      return "node name must not be empty";
    case RMW_NODE_NAME_INVALID_CONTAINS_UNALLOWED_CHARACTERS:
      // 节点名称不能包含除字母数字和 '_' 以外的字符
      // Node name must not contain characters other than alphanumerics or '_'
      return "node name must not contain characters other than alphanumerics or '_'";
    case RMW_NODE_NAME_INVALID_STARTS_WITH_NUMBER:
      // 节点名称不能以数字开头
      // Node name must not start with a number
      return "node name must not start with a number";
    case RMW_NODE_NAME_INVALID_TOO_LONG:
      // 节点名称长度不能超过 RMW_NODE_NAME_MAX_NAME_LENGTH
      // Node name length should not exceed RMW_NODE_NAME_MAX_NAME_LENGTH
      return "node name length should not exceed '" RMW_STRINGIFY(
          RMW_NODE_NAME_MAX_NAME_LENGTH) "'";
    default:
      // 未知的验证结果代码
      // Unknown result code for rmw node name validation
      return "unknown result code for rmw node name validation";
  }
}
