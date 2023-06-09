// Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#include "rmw/topic_endpoint_info.h"

#include "rcutils/macros.h"
#include "rcutils/strdup.h"
#include "rmw/error_handling.h"
#include "rmw/types.h"

/**
 * @brief 获取一个初始化为零的 rmw_topic_endpoint_info_t 结构体 (Get a zero-initialized
 * rmw_topic_endpoint_info_t structure)
 *
 * @return 返回一个初始化为零的 rmw_topic_endpoint_info_t 结构体 (Return a zero-initialized
 * rmw_topic_endpoint_info_t structure)
 */
rmw_topic_endpoint_info_t rmw_get_zero_initialized_topic_endpoint_info(void) {
  // 如果编译器是 Clang，则推入诊断设置 (If the compiler is Clang, push diagnostic settings)
#ifdef __clang__
#pragma clang diagnostic push

  // 忽略 "-Wmissing-field-initializers" 警告 (Ignore the "-Wmissing-field-initializers" warning)
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#endif

  // 创建一个 rmw_topic_endpoint_info_t 结构体变量，并将其所有字段初始化为零 (Create an
  // rmw_topic_endpoint_info_t structure variable and initialize all its fields to zero)
  rmw_topic_endpoint_info_t zero = {0};

  // 如果编译器是 Clang，则弹出诊断设置 (If the compiler is Clang, pop diagnostic settings)
#ifdef __clang__
#pragma clang diagnostic pop
#endif

  // 返回已初始化为零的 rmw_topic_endpoint_info_t 结构体 (Return the zero-initialized
  // rmw_topic_endpoint_info_t structure)
  return zero;
}

/**
 * @brief 释放话题端点信息字符串 (Release the topic endpoint information string)
 *
 * @param[in] topic_endpoint_info_str 话题端点信息字符串的指针 (Pointer to the topic endpoint
 * information string)
 * @param[in] allocator 分配器实例 (Allocator instance)
 * @return rmw_ret_t 结果状态 (Result status)
 */
rmw_ret_t _rmw_topic_endpoint_info_fini_str(
    const char **topic_endpoint_info_str, rcutils_allocator_t *allocator) {
  // 使用分配器释放字符串内存 (Use the allocator to release the string memory)
  allocator->deallocate((char *)*topic_endpoint_info_str, allocator->state);
  // 将字符串指针设置为 NULL (Set the string pointer to NULL)
  *topic_endpoint_info_str = NULL;
  // 返回成功状态 (Return success status)
  return RMW_RET_OK;
}

/**
 * @brief 释放节点名称字符串 (Release the node name string)
 *
 * @param[in] topic_endpoint_info 话题端点信息结构体实例 (Topic endpoint information structure
 * instance)
 * @param[in] allocator 分配器实例 (Allocator instance)
 * @return rmw_ret_t 结果状态 (Result status)
 */
rmw_ret_t _rmw_topic_endpoint_info_fini_node_name(
    rmw_topic_endpoint_info_t *topic_endpoint_info, rcutils_allocator_t *allocator) {
  // 调用 _rmw_topic_endpoint_info_fini_str 释放节点名称字符串 (Call
  // _rmw_topic_endpoint_info_fini_str to release the node name string)
  return _rmw_topic_endpoint_info_fini_str(&topic_endpoint_info->node_name, allocator);
}

/**
 * @brief 释放节点命名空间字符串 (Release the node namespace string)
 *
 * @param[in] topic_endpoint_info 话题端点信息结构体实例 (Topic endpoint information structure
 * instance)
 * @param[in] allocator 分配器实例 (Allocator instance)
 * @return rmw_ret_t 结果状态 (Result status)
 */
rmw_ret_t _rmw_topic_endpoint_info_fini_node_namespace(
    rmw_topic_endpoint_info_t *topic_endpoint_info, rcutils_allocator_t *allocator) {
  // 调用 _rmw_topic_endpoint_info_fini_str 释放节点命名空间字符串 (Call
  // _rmw_topic_endpoint_info_fini_str to release the node namespace string)
  return _rmw_topic_endpoint_info_fini_str(&topic_endpoint_info->node_namespace, allocator);
}

/**
 * @brief 释放话题类型字符串 (Release the topic type string)
 *
 * @param[in] topic_endpoint_info 话题端点信息结构体实例 (Topic endpoint information structure
 * instance)
 * @param[in] allocator 分配器实例 (Allocator instance)
 * @return rmw_ret_t 结果状态 (Result status)
 */
rmw_ret_t _rmw_topic_endpoint_info_fini_topic_type(
    rmw_topic_endpoint_info_t *topic_endpoint_info, rcutils_allocator_t *allocator) {
  // 调用 _rmw_topic_endpoint_info_fini_str 释放话题类型字符串 (Call
  // _rmw_topic_endpoint_info_fini_str to release the topic type string)
  return _rmw_topic_endpoint_info_fini_str(&topic_endpoint_info->topic_type, allocator);
}

/**
 * @brief 释放话题端点信息实例 (Release the topic endpoint information instance)
 *
 * @param[in] topic_endpoint_info 话题端点信息结构体实例 (Topic endpoint information structure
 * instance)
 * @param[in] allocator 分配器实例 (Allocator instance)
 * @return rmw_ret_t 结果状态 (Result status)
 */
rmw_ret_t rmw_topic_endpoint_info_fini(
    rmw_topic_endpoint_info_t *topic_endpoint_info, rcutils_allocator_t *allocator) {
  // 检查参数是否有效 (Check if the arguments are valid)
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  if (!topic_endpoint_info) {
    RMW_SET_ERROR_MSG("topic_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 释放节点名称、命名空间和话题类型字符串 (Release the node name, namespace, and topic type
  // strings)
  rmw_ret_t ret;
  ret = _rmw_topic_endpoint_info_fini_node_name(topic_endpoint_info, allocator);
  if (ret != RMW_RET_OK) {
    return ret;
  }
  ret = _rmw_topic_endpoint_info_fini_node_namespace(topic_endpoint_info, allocator);
  if (ret != RMW_RET_OK) {
    return ret;
  }
  ret = _rmw_topic_endpoint_info_fini_topic_type(topic_endpoint_info, allocator);
  if (ret != RMW_RET_OK) {
    return ret;
  }

  // 将话题端点信息实例重置为零初始化的状态 (Reset the topic endpoint information instance to a
  // zero-initialized state)
  *topic_endpoint_info = rmw_get_zero_initialized_topic_endpoint_info();

  // 返回成功状态 (Return success status)
  return RMW_RET_OK;
}

/**
 * @brief 复制字符串并分配内存
 * @param[out] topic_endpoint_info_str 输出字符串的指针
 * @param[in] str 输入字符串
 * @param[in,out] allocator 分配器
 * @return rmw_ret_t 返回操作结果
 *
 * @brief Copy string and allocate memory
 * @param[out] topic_endpoint_info_str Pointer to the output string
 * @param[in] str Input string
 * @param[in,out] allocator Allocator
 * @return rmw_ret_t Return operation result
 */
rmw_ret_t _rmw_topic_endpoint_info_copy_str(
    const char **topic_endpoint_info_str, const char *str, rcutils_allocator_t *allocator) {
  // 检查输入参数是否有效，如果无效则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if input arguments are valid, return RMW_RET_INVALID_ARGUMENT error if invalid
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_BAD_ALLOC);

  // 检查输入字符串是否为空，如果为空则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if input string is null, return RMW_RET_INVALID_ARGUMENT error if null
  if (!str) {
    RMW_SET_ERROR_MSG("str is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 检查输出字符串指针是否为空，如果为空则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if output string pointer is null, return RMW_RET_INVALID_ARGUMENT error if null
  if (!topic_endpoint_info_str) {
    RMW_SET_ERROR_MSG("topic_endpoint_info_str is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 检查分配器是否为空，如果为空则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if allocator is null, return RMW_RET_INVALID_ARGUMENT error if null
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 使用分配器为输出字符串分配内存并复制输入字符串的内容
  // Allocate memory for output string using allocator and copy the content of input string
  *topic_endpoint_info_str = rcutils_strdup(str, *allocator);
  if (NULL == *topic_endpoint_info_str) {
    return RMW_RET_BAD_ALLOC;
  }

  return RMW_RET_OK;
}

/**
 * @brief 设置话题端点信息的话题类型
 * @param[in,out] topic_endpoint_info 话题端点信息结构体指针
 * @param[in] topic_type 话题类型字符串
 * @param[in,out] allocator 分配器
 * @return rmw_ret_t 返回操作结果
 *
 * @brief Set topic type for topic endpoint info
 * @param[in,out] topic_endpoint_info Pointer to the topic endpoint info structure
 * @param[in] topic_type Topic type string
 * @param[in,out] allocator Allocator
 * @return rmw_ret_t Return operation result
 */
rmw_ret_t rmw_topic_endpoint_info_set_topic_type(
    rmw_topic_endpoint_info_t *topic_endpoint_info,
    const char *topic_type,
    rcutils_allocator_t *allocator) {
  // 检查输入参数是否有效，如果无效则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if input arguments are valid, return RMW_RET_INVALID_ARGUMENT error if invalid
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  // 检查话题端点信息结构体指针是否为空，如果为空则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if topic endpoint info structure pointer is null, return RMW_RET_INVALID_ARGUMENT error
  // if null
  if (!topic_endpoint_info) {
    RMW_SET_ERROR_MSG("topic_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_endpoint_info_copy_str(&topic_endpoint_info->topic_type, topic_type, allocator);
}

/**
 * @brief 设置话题端点信息的话题类型哈希值
 * @param[in,out] topic_endpoint_info 话题端点信息结构体指针
 * @param[in] type_hash 类型哈希值指针
 * @return rmw_ret_t 返回操作结果
 *
 * @brief Set topic type hash for topic endpoint info
 * @param[in,out] topic_endpoint_info Pointer to the topic endpoint info structure
 * @param[in] type_hash Pointer to the type hash value
 * @return rmw_ret_t Return operation result
 */
rmw_ret_t rmw_topic_endpoint_info_set_topic_type_hash(
    rmw_topic_endpoint_info_t *topic_endpoint_info, const rosidl_type_hash_t *type_hash) {
  // 检查输入参数是否有效，如果无效则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if input arguments are valid, return RMW_RET_INVALID_ARGUMENT error if invalid
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(topic_endpoint_info, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(type_hash, RMW_RET_INVALID_ARGUMENT);
  topic_endpoint_info->topic_type_hash = *type_hash;
  return RMW_RET_OK;
}

/**
 * @brief 设置话题端点信息的节点名称
 * @param[in,out] topic_endpoint_info 话题端点信息结构体指针
 * @param[in] node_name 节点名称字符串
 * @param[in,out] allocator 分配器
 * @return rmw_ret_t 返回操作结果
 *
 * @brief Set node name for topic endpoint info
 * @param[in,out] topic_endpoint_info Pointer to the topic endpoint info structure
 * @param[in] node_name Node name string
 * @param[in,out] allocator Allocator
 * @return rmw_ret_t Return operation result
 */
rmw_ret_t rmw_topic_endpoint_info_set_node_name(
    rmw_topic_endpoint_info_t *topic_endpoint_info,
    const char *node_name,
    rcutils_allocator_t *allocator) {
  // 检查输入参数是否有效，如果无效则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if input arguments are valid, return RMW_RET_INVALID_ARGUMENT error if invalid
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  // 检查话题端点信息结构体指针是否为空，如果为空则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if topic endpoint info structure pointer is null, return RMW_RET_INVALID_ARGUMENT error
  // if null
  if (!topic_endpoint_info) {
    RMW_SET_ERROR_MSG("topic_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_endpoint_info_copy_str(&topic_endpoint_info->node_name, node_name, allocator);
}

/**
 * @brief 设置话题端点信息的节点命名空间
 * @param[in,out] topic_endpoint_info 话题端点信息结构体指针
 * @param[in] node_namespace 节点命名空间字符串
 * @param[in,out] allocator 分配器
 * @return rmw_ret_t 返回操作结果
 *
 * @brief Set node namespace for topic endpoint info
 * @param[in,out] topic_endpoint_info Pointer to the topic endpoint info structure
 * @param[in] node_namespace Node namespace string
 * @param[in,out] allocator Allocator
 * @return rmw_ret_t Return operation result
 */
rmw_ret_t rmw_topic_endpoint_info_set_node_namespace(
    rmw_topic_endpoint_info_t *topic_endpoint_info,
    const char *node_namespace,
    rcutils_allocator_t *allocator) {
  // 检查输入参数是否有效，如果无效则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if input arguments are valid, return RMW_RET_INVALID_ARGUMENT error if invalid
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  // 检查话题端点信息结构体指针是否为空，如果为空则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if topic endpoint info structure pointer is null, return RMW_RET_INVALID_ARGUMENT error
  // if null
  if (!topic_endpoint_info) {
    RMW_SET_ERROR_MSG("topic_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_endpoint_info_copy_str(
      &topic_endpoint_info->node_namespace, node_namespace, allocator);
}

/**
 * @brief 设置话题端点信息的端点类型
 * @param[in,out] topic_endpoint_info 话题端点信息结构体指针
 * @param[in] type 端点类型
 * @return rmw_ret_t 返回操作结果
 *
 * @brief Set endpoint type for topic endpoint info
 * @param[in,out] topic_endpoint_info Pointer to the topic endpoint info structure
 * @param[in] type Endpoint type
 * @return rmw_ret_t Return operation result
 */
rmw_ret_t rmw_topic_endpoint_info_set_endpoint_type(
    rmw_topic_endpoint_info_t *topic_endpoint_info, rmw_endpoint_type_t type) {
  // 检查输入参数是否有效，如果无效则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if input arguments are valid, return RMW_RET_INVALID_ARGUMENT error if invalid
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  // 检查话题端点信息结构体指针是否为空，如果为空则返回 RMW_RET_INVALID_ARGUMENT 错误
  // Check if topic endpoint info structure pointer is null, return RMW_RET_INVALID_ARGUMENT error
  // if null
  if (!topic_endpoint_info) {
    RMW_SET_ERROR_MSG("topic_endpoint_info is null");

    return RMW_RET_INVALID_ARGUMENT;
  }

  topic_endpoint_info->endpoint_type = type;

  return RMW_RET_OK;
}

/**
 * @brief 设置主题端点信息的全局唯一标识符（GID）(Set the Global Unique Identifier (GID) for topic
 * endpoint info)
 *
 * @param[in, out] topic_endpoint_info 主题端点信息指针 (Pointer to topic endpoint info)
 * @param[in] gid 要设置的 GID (The GID to be set)
 * @param[in] size GID 的大小 (Size of the GID)
 * @return rmw_ret_t 返回操作结果 (Return operation result)
 */
rmw_ret_t rmw_topic_endpoint_info_set_gid(
    rmw_topic_endpoint_info_t *topic_endpoint_info, const uint8_t *gid, size_t size) {
  // 检查错误条件并返回错误 (Check error conditions and return error)
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  // 如果 topic_endpoint_info 为空，返回无效参数错误 (If topic_endpoint_info is null, return invalid
  // argument error)
  if (!topic_endpoint_info) {
    RMW_SET_ERROR_MSG("topic_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 如果 size 大于 RMW_GID_STORAGE_SIZE，返回无效参数错误 (If size is more than
  // RMW_GID_STORAGE_SIZE, return invalid argument error)
  if (size > RMW_GID_STORAGE_SIZE) {
    RMW_SET_ERROR_MSG("size is more than RMW_GID_STORAGE_SIZE");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 将 topic_endpoint_info->endpoint_gid 中的内容清零 (Zero the content in
  // topic_endpoint_info->endpoint_gid)
  memset(topic_endpoint_info->endpoint_gid, 0, RMW_GID_STORAGE_SIZE);

  // 将 gid 复制到 topic_endpoint_info->endpoint_gid (Copy gid to topic_endpoint_info->endpoint_gid)
  memcpy(topic_endpoint_info->endpoint_gid, gid, size);

  // 返回操作成功 (Return operation success)
  return RMW_RET_OK;
}

/**
 * @brief 设置主题端点信息的 QoS 配置文件 (Set the Quality of Service (QoS) profile for topic
 * endpoint info)
 *
 * @param[in, out] topic_endpoint_info 主题端点信息指针 (Pointer to topic endpoint info)
 * @param[in] qos_profile 要设置的 QoS 配置文件指针 (Pointer to the QoS profile to be set)
 * @return rmw_ret_t 返回操作结果 (Return operation result)
 */
rmw_ret_t rmw_topic_endpoint_info_set_qos_profile(
    rmw_topic_endpoint_info_t *topic_endpoint_info,  //
    const rmw_qos_profile_t *qos_profile) {
  // 检查错误条件并返回错误 (Check error conditions and return error)
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  // 如果 topic_endpoint_info 为空，返回无效参数错误 (If topic_endpoint_info is null, return invalid
  // argument error)
  if (!topic_endpoint_info) {
    RMW_SET_ERROR_MSG("topic_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 如果 qos_profile 为空，返回无效参数错误 (If qos_profile is null, return invalid argument error)
  if (!qos_profile) {
    RMW_SET_ERROR_MSG("qos_profile is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // 将 qos_profile 复制到 topic_endpoint_info->qos_profile (Copy qos_profile to
  // topic_endpoint_info->qos_profile)
  topic_endpoint_info->qos_profile = *qos_profile;

  // 返回操作成功 (Return operation success)
  return RMW_RET_OK;
}
