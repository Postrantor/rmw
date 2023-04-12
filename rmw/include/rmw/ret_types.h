// Copyright 2014-2018 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__RET_TYPES_H_
#define RMW__RET_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief 返回码定义，用于 rmw 函数的返回值
 * Return code definitions for rmw functions.
 */
typedef int32_t rmw_ret_t;

/**
 * @brief 操作正常运行，没有错误
 * The operation ran as expected without errors.
 */
#define RMW_RET_OK 0

/**
 * @brief 通用错误，表示操作无法成功完成
 * Generic error to indicate the operation could not complete successfully.
 */
#define RMW_RET_ERROR 1

/**
 * @brief 操作提前终止，因为超过了其超时条件
 * The operation was halted early because it exceeded its timeout criteria.
 */
#define RMW_RET_TIMEOUT 2

/**
 * @brief 不支持操作或事件处理
 * The operation or event handling is not supported.
 */
#define RMW_RET_UNSUPPORTED 3

/**
 * @brief 内存分配失败
 * Failed to allocate memory.
 */
#define RMW_RET_BAD_ALLOC 10

/**
 * @brief 函数参数无效
 * Argument to function was invalid.
 */
#define RMW_RET_INVALID_ARGUMENT 11

/**
 * @brief 不正确的 rmw 实现
 * Incorrect rmw implementation.
 */
#define RMW_RET_INCORRECT_RMW_IMPLEMENTATION 12

// rmw 节点特定返回代码在 2XX 中
// rmw node specific return codes in the 2XX range.

/**
 * @brief 未找到节点名称
 * Failed to find the node name.
 * 使用与 rcl 相同的返回代码
 * Using the same return code as in rcl.
 */
#define RMW_RET_NODE_NAME_NON_EXISTENT 203

#ifdef __cplusplus
}
#endif

#endif  // RMW__RET_TYPES_H_
