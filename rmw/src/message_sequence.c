// Copyright 2020 Open Source Robotics Foundation, Inc.
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

#include "rmw/message_sequence.h"

#include "rmw/types.h"

/**
 * @brief 获取零初始化的消息序列 (Get a zero-initialized message sequence)
 *
 * @return 返回一个零初始化的消息序列对象 (Return a zero-initialized message sequence object)
 */
rmw_message_sequence_t rmw_get_zero_initialized_message_sequence(void) {
  // 定义一个静态的消息序列对象，并初始化为零 (Define a static message sequence object and
  // initialize it to zero)
  static rmw_message_sequence_t message_sequence = {
      .data = NULL, .size = 0u, .capacity = 0u, .allocator = NULL};

  // 返回这个零初始化的消息序列对象 (Return this zero-initialized message sequence object)
  return message_sequence;
}

/**
 * @brief 初始化消息序列 (Initialize the message sequence)
 *
 * @param[out] sequence 指向要初始化的消息序列的指针 (Pointer to the message sequence to be
 * initialized)
 * @param[in] size 消息序列的容量 (Capacity of the message sequence)
 * @param[in] allocator 分配器用于分配内存 (Allocator for memory allocation)
 * @return 返回操作结果状态码 (Return the operation result status code)
 */
rmw_ret_t rmw_message_sequence_init(rmw_message_sequence_t *sequence,
                                    size_t size,
                                    rcutils_allocator_t *allocator) {
  // 检查输入参数是否为空 (Check if input arguments are NULL)
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(sequence, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);

  void *data = NULL;
  if (size > 0u) {
    // 为消息序列分配内存 (Allocate memory for the message sequence)
    data = allocator->allocate(sizeof(void *) * size, allocator->state);
    if (NULL == data) {
      return RMW_RET_BAD_ALLOC;
    }
  }

  // 初始化消息序列对象的属性 (Initialize the properties of the message sequence object)
  sequence->data = data;
  sequence->size = 0u;
  sequence->capacity = size;
  sequence->allocator = allocator;

  return RMW_RET_OK;
}

/**
 * @brief 终止消息序列 (Finalize the message sequence)
 *
 * @param[out] sequence 指向要终止的消息序列的指针 (Pointer to the message sequence to be finalized)
 * @return 返回操作结果状态码 (Return the operation result status code)
 */
rmw_ret_t rmw_message_sequence_fini(rmw_message_sequence_t *sequence) {
  // 检查输入参数是否为空 (Check if input arguments are NULL)
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(sequence, RMW_RET_INVALID_ARGUMENT);

  if (NULL != sequence->data) {
    assert(sequence->capacity > 0u);
    // 检查分配器是否有效 (Check if the allocator is valid)
    RCUTILS_CHECK_ALLOCATOR(sequence->allocator, return RMW_RET_INVALID_ARGUMENT);
    // 释放消息序列的内存 (Deallocate memory of the message sequence)
    sequence->allocator->deallocate(sequence->data, sequence->allocator->state);
  }

  // 将消息序列对象的属性重置为零 (Reset the properties of the message sequence object to zero)
  sequence->data = NULL;
  sequence->size = 0u;
  sequence->capacity = 0u;
  sequence->allocator = NULL;

  return RMW_RET_OK;
}

/**
 * @brief 获取零初始化的消息信息序列 (Get a zero-initialized message info sequence)
 *
 * @return 返回一个零初始化的消息信息序列对象 (Return a zero-initialized message info sequence
 * object)
 */
rmw_message_info_sequence_t rmw_get_zero_initialized_message_info_sequence(void) {
  // 定义一个静态的消息信息序列对象，并初始化为零 (Define a static message info sequence object and
  // initialize it to zero)
  static rmw_message_info_sequence_t message_info_sequence = {
      .data = NULL, .size = 0u, .capacity = 0u, .allocator = NULL};

  // 返回这个零初始化的消息信息序列对象 (Return this zero-initialized message info sequence object)
  return message_info_sequence;
}

/**
 * @brief 初始化消息信息序列 (Initialize the message info sequence)
 *
 * @param[out] sequence 指向要初始化的消息信息序列的指针 (Pointer to the message info sequence to be
 * initialized)
 * @param[in] size 消息信息序列的容量 (Capacity of the message info sequence)
 * @param[in] allocator 分配器用于分配内存 (Allocator for memory allocation)
 * @return 返回操作结果状态码 (Return the operation result status code)
 */
rmw_ret_t rmw_message_info_sequence_init(rmw_message_info_sequence_t *sequence,
                                         size_t size,
                                         rcutils_allocator_t *allocator) {
  // 检查输入参数是否为空 (Check if input arguments are NULL)
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(sequence, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);

  rmw_message_info_t *data = NULL;
  if (size > 0u) {
    // 为消息信息序列分配内存 (Allocate memory for the message info sequence)
    data = allocator->allocate(sizeof(rmw_message_info_t) * size, allocator->state);
    if (NULL == data) {
      return RMW_RET_BAD_ALLOC;
    }
  }

  // 初始化消息信息序列对象的属性 (Initialize the properties of the message info sequence object)
  sequence->data = data;
  sequence->size = 0u;
  sequence->capacity = size;
  sequence->allocator = allocator;

  return RMW_RET_OK;
}

/**
 * @brief 终止消息信息序列 (Finalize the message info sequence)
 *
 * @param[out] sequence 指向要终止的消息信息序列的指针 (Pointer to the message info sequence to be
 * finalized)
 * @return 返回操作结果状态码 (Return the operation result status code)
 */
rmw_ret_t rmw_message_info_sequence_fini(rmw_message_info_sequence_t *sequence) {
  // 检查输入参数是否为空 (Check if input arguments are NULL)
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(sequence, RMW_RET_INVALID_ARGUMENT);

  if (NULL != sequence->data) {
    assert(sequence->capacity > 0u);
    // 检查分配器是否有效 (Check if the allocator is valid)
    RCUTILS_CHECK_ALLOCATOR(sequence->allocator, return RMW_RET_INVALID_ARGUMENT);
    // 释放消息信息序列的内存 (Deallocate memory of the message info sequence)
    sequence->allocator->deallocate(sequence->data, sequence->allocator->state);
  }

  // 将消息信息序列对象的属性重置为零 (Reset the properties of the message info sequence object to
  // zero)
  sequence->data = NULL;
  sequence->size = 0u;
  sequence->capacity = 0u;
  sequence->allocator = NULL;

  return RMW_RET_OK;
}
