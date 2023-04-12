// Copyright 2019 Open Source Robotics Foundation, Inc.
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

#include "rmw/event.h"

#include <stddef.h>

#include "rmw/error_handling.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获取一个零初始化的 rmw_event_t 结构体 (Get a zero-initialized rmw_event_t structure)
 *
 * @return 返回一个零初始化的 rmw_event_t 结构体 (Return a zero-initialized rmw_event_t structure)
 */
rmw_event_t rmw_get_zero_initialized_event(void) {
  // 定义一个常量 rmw_event_t 结构体，并将其各个成员变量初始化为 NULL 或无效值
  // (Define a constant rmw_event_t structure and initialize its member variables to NULL or invalid
  // values)
  const rmw_event_t event = {
      .implementation_identifier =
          NULL,      // 设置实现标识符为 NULL (Set the implementation identifier to NULL)
      .data = NULL,  // 设置数据指针为 NULL (Set the data pointer to NULL)
      .event_type = RMW_EVENT_INVALID  // 设置事件类型为无效 (Set the event type to invalid)
  };                                   // NOLINT(readability/braces): false positive
  return event;  // 返回初始化后的 event 结构体 (Return the initialized event structure)
}

/**
 * @brief 结束一个 rmw_event_t 结构体，释放其资源 (Finalize an rmw_event_t structure, releasing its
 * resources)
 *
 * @param[in,out] rmw_event 指向要结束的 rmw_event_t 结构体的指针 (Pointer to the rmw_event_t
 * structure to be finalized)
 * @return 返回 rmw_ret_t 类型的状态码，表示函数执行结果 (Return an rmw_ret_t type status code
 * indicating the result of the function execution)
 */
rmw_ret_t rmw_event_fini(rmw_event_t* rmw_event) {
  // 检查传入的 rmw_event 参数是否为 NULL，如果是，则返回 RMW_RET_INVALID_ARGUMENT 错误
  // (Check if the input rmw_event parameter is NULL, and if so, return the RMW_RET_INVALID_ARGUMENT
  // error)
  RMW_CHECK_ARGUMENT_FOR_NULL(rmw_event, RMW_RET_INVALID_ARGUMENT);

  // 调用 rmw_get_zero_initialized_event 函数，将传入的 rmw_event 结构体重置为零初始化状态
  // (Call the rmw_get_zero_initialized_event function to reset the input rmw_event structure to a
  // zero-initialized state)
  *rmw_event = rmw_get_zero_initialized_event();

  // 返回 RMW_RET_OK，表示函数执行成功 (Return RMW_RET_OK, indicating that the function executed
  // successfully)
  return RMW_RET_OK;
}

#ifdef __cplusplus
}
#endif
