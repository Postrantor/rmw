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

#ifndef RMW__IMPL__CPP__DEMANGLE_HPP_
#define RMW__IMPL__CPP__DEMANGLE_HPP_

#ifndef _WIN32
// Includes for abi::__cxa_demangle.
#include <cxxabi.h>

#include <cstdlib>
#include <memory>
#endif
#include <iostream>
#include <string>

#include "rmw/impl/config.h"

namespace rmw {
namespace impl {
namespace cpp {

/**
 * @brief 返回类型 T 实例的解析名字 (Return the demangled name of the instance of type T)
 *
 * @tparam T 类型参数 (Type parameter)
 * @param[in] instance 类型 T 的实例 (Instance of type T)
 * @return std::string 解析后的名字 (Demangled name)
 */
template <typename T>
std::string demangle(const T &instance) {
  // 忽略 instance 参数，避免未使用警告 (Ignore the instance parameter to avoid unused warning)
  (void)instance;

  // 如果在 Windows 上或者希望避免内存分配，则无法进行解析 (Cannot do demangling if on Windows or if
  // we want to avoid memory allocation)
#if !defined(_WIN32) || RMW_AVOID_MEMORY_ALLOCATION
  // 定义一个整数 status，用于存储解析状态 (Define an integer status for storing the demangling
  // status)
  int status = 0;

  // 获取类型 T 的 typeid 名称 (Get the typeid name of type T)
  std::string mangled_typeid_name = typeid(T).name();

  // 使用 abi::__cxa_demangle 函数进行解析，并将结果存储在 res 中 (Use the abi::__cxa_demangle
  // function to perform demangling and store the result in res)
  std::unique_ptr<char, void (*)(void *)> res{
      abi::__cxa_demangle(mangled_typeid_name.c_str(), NULL, NULL, &status), std::free};

  // 如果 status 为 0，则返回解析后的名字，否则返回原始的 typeid 名称 (Return the demangled name if
  // status is 0, otherwise return the original typeid name)
  return (status == 0) ? res.get() : mangled_typeid_name;
#else
  // 在 Windows 上或者希望避免内存分配时，直接返回 typeid 名称 (Return the typeid name directly if
  // on Windows or if we want to avoid memory allocation)
  return typeid(T).name();
#endif
}

}  // namespace cpp
}  // namespace impl
}  // namespace rmw

#endif  // RMW__IMPL__CPP__DEMANGLE_HPP_
