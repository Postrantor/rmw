// Copyright 2023 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__CHECK_TYPE_IDENTIFIERS_MATCH_H_
#define RMW__CHECK_TYPE_IDENTIFIERS_MATCH_H_

#include <string.h>

#include "rcutils/snprintf.h"
#include "rmw/allocators.h"
#include "rmw/error_handling.h"
#include "rmw/impl/config.h"  // For RMW_AVOID_MEMORY_ALLOCATION

/**
 * @brief 检查类型标识符是否匹配 (Check if type identifiers match)
 * @param ElementName 元素名称 (Element name)
 * @param ElementTypeID 当前元素类型ID (Current element type ID)
 * @param ExpectedTypeID 期望的元素类型ID (Expected element type ID)
 * @param OnFailure 匹配失败时执行的代码 (Code to execute on failure)
 */
#if RMW_AVOID_MEMORY_ALLOCATION
#define RMW_CHECK_TYPE_IDENTIFIERS_MATCH(ElementName, ElementTypeID, ExpectedTypeID, OnFailure) \
  do {                                                                                          \
// 如果当前元素类型ID与期望类型ID不匹配 (If the current element type ID does not match the expected
// type ID)
if (ElementTypeID !=
    ExpectedTypeID) {  // 定义一个长度为1024的字符数组 (Define a char array with a length of 1024)
  char __msg[1024];  // 尝试将错误消息格式化并存储在__msg中 (Attempt to format the error message and
                     // store it in __msg)
  int ret = rcutils_snprintf(__msg, sizeof(__msg),
#ElementName " implementation '%s'(%p) does not match rmw implementation '%s'(%p)",
                             ElementTypeID, (const void *)ElementTypeID, ExpectedTypeID,
                             (const void *)ExpectedTypeID);  // 如果格式化失败 (If formatting fails)
  if (ret < 0) {  // 设置静态错误消息 (Set static error message)
    static const char error_msg[] =
        "RMW_CHECK_TYPE_IDENTIFIERS_MATCH(): rcutils_snprintf() failed";  // 将错误消息复制到__msg
                                                                          // (Copy error message to
                                                                          // __msg)
    memmove(__msg, error_msg, sizeof(error_msg));
  }                          // 设置RMW错误消息 (Set RMW error message)
  RMW_SET_ERROR_MSG(__msg);  // 执行匹配失败时的代码 (Execute code on failure)
  OnFailure;
}
}
while (0)
#else  // RMW_AVOID_MEMORY_ALLOCATION
#define RMW_CHECK_TYPE_IDENTIFIERS_MATCH(ElementName, ElementTypeID, ExpectedTypeID, OnFailure) \
  do {                                                                                          \
  // 如果当前元素类型ID与期望类型ID不匹配 (If the current element type ID does not match the
  // expected type ID)
if (ElementTypeID !=
    ExpectedTypeID) {  // 计算将要写入的字节数 (Calculate the bytes that would have been written)
  int __bytes_that_would_have_been_written = rcutils_snprintf(
      NULL, 0, #ElementName " implementation '%s'(%p) does not match rmw implementation '%s'(%p)",
      ElementTypeID, (const void *)ElementTypeID, ExpectedTypeID,
      (const void *)ExpectedTypeID);               // 如果计算失败 (If calculation fails)
  if (__bytes_that_would_have_been_written < 0) {  // 设置RMW错误消息 (Set RMW error message)
    RMW_SET_ERROR_MSG(
        "RMW_CHECK_TYPE_IDENTIFIERS_MATCH(): rcutils_snprintf(NULL, 0, ...) failed");  // 执行匹配失败时的代码
                                                                                       // (Execute
                                                                                       // code on
                                                                                       // failure)
    OnFailure;
  } else {  // 为错误消息分配内存 (Allocate memory for the error message)
    char *__msg = (char *)rmw_allocate(__bytes_that_would_have_been_written +
                                       1);  // 如果内存分配失败 (If memory allocation fails)
    if (NULL == __msg) {                    // 设置RMW错误消息 (Set RMW error message)
      RMW_SET_ERROR_MSG("RMW_CHECK_TYPE_IDENTIFIERS_MATCH(): rmw_allocate() failed");
    } else {  // 尝试将错误消息格式化并存储在__msg中 (Attempt to format the error message and store
              // it in __msg)
      int ret =
          rcutils_snprintf(__msg, __bytes_that_would_have_been_written + 1,
#ElementName " implementation '%s'(%p) does not match rmw implementation '%s'(%p)",
                           ElementTypeID, (const void *)ElementTypeID, ExpectedTypeID,
                           (const void *)ExpectedTypeID);  // 如果格式化失败 (If formatting fails)
      if (ret < 0) {  // 设置RMW错误消息 (Set RMW error message)
        RMW_SET_ERROR_MSG("RMW_CHECK_TYPE_IDENTIFIERS_MATCH(): rcutils_snprintf() failed");
      } else {        // 设置RMW错误消息 (Set RMW error message)
        RMW_SET_ERROR_MSG(__msg);
      }
    }                 // 释放错误消息的内存 (Free the memory of the error message)
    rmw_free(__msg);  // 执行匹配失败时的代码 (Execute code on failure)
    OnFailure;
  }
}
}
while (0)
#endif  // RMW_AVOID_MEMORY_ALLOCATION

#endif  // RMW__CHECK_TYPE_IDENTIFIERS_MATCH_H_
