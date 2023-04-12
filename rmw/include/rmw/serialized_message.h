// Copyright 2018 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__SERIALIZED_MESSAGE_H_
#define RMW__SERIALIZED_MESSAGE_H_

#if __cplusplus
extern "C" {
#endif

#include "rcutils/types/uint8_array.h"

/**
 * \brief 序列化消息为字节字符串。
 * \brief Serialize message as a string of bytes.
 *
 * 包括以下成员（但不限于）：
 * It includes (but it is not limited to) the following members:
 *   \e \c buffer 指向内部存储的引用，作为指针
 *   \e \c buffer the reference to internal storage, as a pointer
 *   \e \c buffer_length 存储内容的大小，作为无符号整数
 *   \e \c buffer_length the size of stored contents, as an unsigned integer
 *   \e \c buffer_capacity 内部存储的容量，作为无符号整数
 *   \e \c buffer_capacity the capacity of internal storage, as an unsigned integer
 */
/* 目前这只是一个将序列化消息简单映射到 uint8 数组的别名。
 * 但是，在未来的开发中，这个序列化消息可能会变得更加复杂，因此进行了别名处理。
 */
/* For now this is a simple aliasing from a serialized message to a uint8 array.
 * However, in future developments this serialized message can become something
 * more complex and is therefore aliased.
 */
typedef rcutils_uint8_array_t rmw_serialized_message_t;

/// 返回一个零初始化的序列化消息结构体。
/// Return a zero initialized serialized message struct.
#define rmw_get_zero_initialized_serialized_message rcutils_get_zero_initialized_uint8_array

/// 初始化一个序列化消息，将其内容置零。
/// Initialize a serialized message, zero initializing its contents.
/**
 * 给定的序列化消息必须已经过零初始化。
 * Given serialized message must have been zero initialized.
 *
 * \param[inout] serialized_message 要初始化的序列化消息的指针
 * \param[inout] serialized_message a pointer to the serialized message to initialize
 * \param[in] message_capacity 要分配的内存大小
 * \param[in] message_capacity the size of the memory to allocate
 * \param[in] allocator 用于内存分配的分配器
 * \param[in] allocator the allocator to use for the memory allocation
 * \return `RMW_RET_OK` 如果成功，或者
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果任何参数无效，或者
 * \return `RMW_RET_INVALID_ARGUMENT` if any arguments are invalid, or
 * \return 'RMW_RET_BAD_ALLOC` 如果无法正确分配内存，或者
 * \return 'RMW_RET_BAD_ALLOC` if no memory could be allocated correctly, or
 * \return `RMW_RET_ERROR` 如果发生意外错误
 * \return `RMW_RET_ERROR` if an unexpected error occurs
 */
#define rmw_serialized_message_init(serialized_message, message_capacity, allocator) \
  rcutils_uint8_array_init(serialized_message, message_capacity, allocator)

/// 结束一个序列化消息。
/// Finalize a serialized message.
/**
 * 给定的序列化消息必须使用 `rmw_serialized_message_init()` 进行初始化。
 * Given serialized message must have been initialized with `rmw_serialized_message_init()`.
 *
 * \remarks 如果序列化消息是零初始化的，则返回 `RMW_RET_INVALID_ARGUMENT`。
 * \remarks If serialized message is zero initialized, then `RMW_RET_INVALID_ARGUMENT` is returned.
 *
 * \param[in] serialized_message 要清理的序列化消息的指针
 * \param[in] serialized_message pointer to the serialized message to be cleaned up
 * \return `RMW_RET_OK` 如果成功，或者
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` 如果序列化消息无效，或者
 * \return `RMW_RET_INVALID_ARGUMENT` if serialized_message is invalid, or
 * \return `RMW_RET_ERROR` 如果发生意外错误
 * \return `RMW_RET_ERROR` if an unexpected error occurs
 */
#define rmw_serialized_message_fini(serialized_message) rcutils_uint8_array_fini(serialized_message)

/// 调整序列化消息的内部缓冲区大小。
/// Resize the internal buffer of the serialized message.
/**
 * 如果需要，序列化消息的内部缓冲区可以动态调整大小。
 * If needed, the internal buffer of the serialized message can be resized dynamically.
 * 如果新大小小于当前容量，则内存将被截断。
 * If the new size is smaller than the current capacity, then the memory is truncated.
 *
 * \pre 给定的序列化消息必须使用 `rmw_serialized_message_init()` 进行初始化。
 * \pre Given serialized message must have been initialized with `rmw_serialized_message_init()`.
 *
 * \remarks 如果序列化消息为零初始化，则返回 `RMW_RET_INVALID_ARGUMENT`。
 * \remarks If serialized message is zero initialized, then `RMW_RET_INVALID_ARGUMENT` is returned.
 *
 * \warning 请注意，这可能会释放内存，因此使指向内部缓冲区的任何指针失效。
 * \warning Be aware that this might deallocate the memory and therefore invalidate any pointers to
 * the internal buffer.
 *
 * \param[inout] serialized_message 要调整大小的序列化消息的指针
 * \param[inout] serialized_message pointer to the serialized message to be resized
 * \param[in] new_size 内部缓冲区的新大小
 * \param[in] new_size the new size of the internal buffer
 * \return 如果成功，则返回 `RMW_RET_OK`，或
 * \return `RMW_RET_OK` if successful, or
 * \return 如果序列化消息无效或 new_size 设置为零，则返回 `RMW_RET_INVALID_ARGUMENT`，或
 * \return `RMW_RET_INVALID_ARGUMENT` if serialized_message is invalid or new_size is set to zero,
 * or \return 如果内存分配失败，则返回 `RMW_RET_BAD_ALLOC`，或 \return `RMW_RET_BAD_ALLOC` if memory
 * allocation failed, or \return 如果发生意外错误，则返回 `RMW_RET_ERROR` \return `RMW_RET_ERROR` if
 * an unexpected error occurs
 */
#define rmw_serialized_message_resize(serialized_message, new_size) \
  rcutils_uint8_array_resize(serialized_message, new_size)

#if __cplusplus
}
#endif

#endif  // RMW__SERIALIZED_MESSAGE_H_
