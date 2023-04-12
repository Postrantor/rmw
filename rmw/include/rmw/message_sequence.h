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

#ifndef RMW__MESSAGE_SEQUENCE_H_
#define RMW__MESSAGE_SEQUENCE_H_

#include <stddef.h>

#include "rmw/macros.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

#if __cplusplus
extern "C" {
#endif

/// 结构体用于存储ROS消息序列 (Structure to hold a sequence of ROS messages)
typedef struct RMW_PUBLIC_TYPE rmw_message_sequence_s {
  /// 指向ROS消息的指针数组 (Array of pointers to ROS messages)
  void** data;
  /// `data`中有效条目的数量 (The number of valid entries in `data`)
  size_t size;
  /// 数据数组的总分配容量 (The total allocated capacity of the data array)
  size_t capacity;
  /// 分配数据数组的分配器 (The allocator used to allocate the data array)
  rcutils_allocator_t* allocator;
} rmw_message_sequence_t;

/// 结构体用于存储消息信息序列 (Structure to hold a sequence of message infos)
typedef struct RMW_PUBLIC_TYPE rmw_message_info_sequence_s {
  /// 消息信息数组 (Array of message info)
  rmw_message_info_t* data;
  /// 数据中有效条目的数量 (The number of valid entries in data)
  size_t size;
  /// 数据数组的总分配容量 (The total allocated capacity of the data array)
  size_t capacity;
  /// 分配数据数组的分配器 (The allocator used to allocate the data array)
  rcutils_allocator_t* allocator;
} rmw_message_info_sequence_t;

/// 返回一个成员初始化为`NULL`的rmw_message_sequence_t结构体 (Return an rmw_message_sequence_t
/// struct with members initialized to `NULL`)
RMW_PUBLIC
rmw_message_sequence_t rmw_get_zero_initialized_message_sequence(void);

/// 初始化一个rmw_message_sequence_t对象 (Initialize an rmw_message_sequence_t object)
/**
 * \param[inout] sequence 要初始化的序列对象 (sequence object to be initialized)
 * \param[in] size 要分配的序列容量 (capacity of the sequence to be allocated)
 * \param[in] allocator 用于分配内存的分配器 (the allcator used to allocate memory)
 */
RMW_PUBLIC
rmw_ret_t rmw_message_sequence_init(rmw_message_sequence_t* sequence,
                                    size_t size,
                                    rcutils_allocator_t* allocator);

/// 结束一个rmw_message_sequence_t对象 (Finalize an rmw_message_sequence_t object)
/**
 * rmw_message_sequence_t结构体有需要在设置值之前为其分配内存的成员
 * (The rmw_message_sequence_t struct has members which require memory to be allocated to them
 * before setting values) 此函数回收对象中的任何分配资源并将所有其他成员归零 (This function reclaims
 * any allocated resources within the object and zeroes out all other members)
 *
 * 注意：这不会调用`fini`或释放底层消息结构
 * (Note: This will not call `fini` or deallocate the underlying message structures)
 *
 * \param[inout] sequence 要结束的序列对象 (sequence object to be finalized)
 */
RMW_PUBLIC
rmw_ret_t rmw_message_sequence_fini(rmw_message_sequence_t* sequence);

/// 返回一个成员初始化为`NULL`的rmw_message_info_sequence_t结构体 (Return an
/// rmw_message_info_sequence_t struct with members initialized to `NULL`)
RMW_PUBLIC
rmw_message_info_sequence_t rmw_get_zero_initialized_message_info_sequence(void);

/// 初始化一个rmw_message_info_sequence_t对象 (Initialize an rmw_message_info_sequence_t object)
/**
 * \param[inout] sequence 要初始化的序列对象 (sequence object to be initialized)
 * \param[in] size 要分配的序列容量 (capacity of the sequence to be allocated)
 * \param[in] allocator 用于分配内存的分配器 (the allcator used to allocate memory)
 */
RMW_PUBLIC
rmw_ret_t rmw_message_info_sequence_init(rmw_message_info_sequence_t* sequence,
                                         size_t size,
                                         rcutils_allocator_t* allocator);

/// 结束一个rmw_message_sequence_t对象 (Finalize an rmw_message_sequence_t object)
/**
 * rmw_message_sequence_t结构体有需要在设置值之前为其分配内存的成员
 * (The rmw_message_sequence_t struct has members which require memory to be allocated to them
 * before setting values) 此函数回收对象中的任何分配资源并将所有其他成员归零 (This function reclaims
 * any allocated resources within the object and zeroes out all other members)
 *
 * \param[inout] sequence 要结束的序列对象 (sequence object to be finalized)
 */
RMW_PUBLIC
rmw_ret_t rmw_message_info_sequence_fini(rmw_message_info_sequence_t* sequence);

#if __cplusplus
}
#endif

#endif  // RMW__MESSAGE_SEQUENCE_H_
