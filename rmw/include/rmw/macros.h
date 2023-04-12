// Copyright 2015 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__MACROS_H_
#define RMW__MACROS_H_

#include "rcutils/macros.h"

/// 将参数扩展为其文本形式 (Expand the argument to its literal text)
/// \param[in] x 输入参数，将被转换为字符串形式 (The input parameter that will be converted to a
/// string form)
#define RMW_STRINGIFY(x) RCUTILS_STRINGIFY(x)

/// 表示方法的调用者必须检查返回值，否则编译器会发出警告。
/// (Indicate that the caller of a method must check the return value,
/// otherwise the compiler will issue a warning.)
///
/// 使用此宏可以确保在调用函数时检查返回值，从而提高代码质量和减少错误。
/// (Using this macro ensures that the return value is checked when calling functions,
/// which improves code quality and reduces errors.)
#define RMW_WARN_UNUSED RCUTILS_WARN_UNUSED

#endif  // RMW__MACROS_H_
