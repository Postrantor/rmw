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

#ifndef RMW__IMPL__CPP__MACROS_HPP_
#define RMW__IMPL__CPP__MACROS_HPP_

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "rmw/check_type_identifiers_match.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/demangle.hpp"  // For demangle.

/*!
 * \brief 尝试在指定的缓冲区中构造一个新对象，并处理可能的 C++ 异常。
 *        Try to construct a new object in the specified buffer and handle any possible C++
 * exceptions. \param Destination 目标指针，用于存储新构造的对象。Destination pointer to store the
 * newly constructed object. \param BufferForNew 用于存放新对象的缓冲区。Buffer to hold the new
 * object. \param FailureAction 如果捕获到异常，则执行的操作。Action to perform if an exception is
 * caught. \param Type 要构造的对象的类型。Type of the object to be constructed. \param ...
 * 构造函数所需的参数。Arguments required for the constructor.
 */
#define RMW_TRY_PLACEMENT_NEW(Destination, BufferForNew, FailureAction, Type, ...)         \
  try {                                                                                    \
    /* 在指定缓冲区中构造新对象 */                                             \
    /* Construct the new object in the specified buffer */                                 \
    Destination = new (BufferForNew) Type(__VA_ARGS__);                                    \
  } catch (const std::exception& exception) {                                              \
    /* 设置错误信息并执行失败动作 */                                          \
    /* Set error message and perform failure action */                                     \
    RMW_SET_ERROR_MSG((std::string("caught C++ exception ") +                              \
                       rmw::impl::cpp::demangle(exception) + " constructing " #Type ": " + \
                       exception.what())                                                   \
                          .c_str());                                                       \
    FailureAction;                                                                         \
  } catch (...) {                                                                          \
    /* 设置错误信息并执行失败动作 */                                          \
    /* Set error message and perform failure action */                                     \
    RMW_SET_ERROR_MSG("caught unknown C++ exception constructing " #Type);                 \
    FailureAction;                                                                         \
  }

/*!
 * \brief 尝试执行析构语句并处理可能的 C++ 异常。
 *        Try to execute a destructor statement and handle any possible C++ exceptions.
 * \param Statement 要执行的析构语句。Destructor statement to be executed.
 * \param Type 对象的类型。Type of the object.
 * \param FailureAction 如果捕获到异常，则执行的操作。Action to perform if an exception is caught.
 */
#define RMW_TRY_DESTRUCTOR(Statement, Type, FailureAction)                                \
  try {                                                                                   \
    /* 执行析构语句 */                                                              \
    /* Execute the destructor statement */                                                \
    Statement;                                                                            \
  } catch (const std::exception& exception) {                                             \
    /* 设置错误信息并执行失败动作 */                                         \
    /* Set error message and perform failure action */                                    \
    RMW_SET_ERROR_MSG((std::string("caught C++ exception in destructor of " #Type ": ") + \
                       rmw::impl::cpp::demangle(exception) + ": " + exception.what())     \
                          .c_str());                                                      \
    FailureAction;                                                                        \
  } catch (...) {                                                                         \
    /* 设置错误信息并执行失败动作 */                                         \
    /* Set error message and perform failure action */                                    \
    RMW_SET_ERROR_MSG("caught unknown C++ exception in destructor of " #Type);            \
    FailureAction;                                                                        \
  }

/**
 * @brief 尝试在处理失败时调用析构函数。Attempt to call a destructor while handling a failure.
 * @param Statement 要执行的语句。The statement to execute.
 * @param Type 类型名称。The type name.
 */
#define RMW_TRY_DESTRUCTOR_FROM_WITHIN_FAILURE(Statement, Type)                                                   \
  try {                                                                                                           \
    /* 执行给定的语句。Execute the given statement. */                                                    \
    Statement;                                                                                                    \
  } catch (const std::exception& exception) {                                                                     \
    /* 创建一个字符串流对象。Create a stringstream object. */                                          \
    std::stringstream ss;                                                                                         \
    /* 将捕获到的异常信息添加到字符串流中。Add the captured exception information to the        \
     * stringstream. */                                                                                           \
    ss << "caught C++ exception in destructor of " #Type " while handling a failure: "                            \
       << rmw::impl::cpp::demangle(exception) << ": " << exception.what() << ", at: " << __FILE__                 \
       << ":" << __LINE__ << '\n';                                                                                \
    /* 输出字符串流内容并刷新缓冲区。Output the contents of the stringstream and flush the buffer. \
     */                                                                                                           \
    (std::cerr << ss.str()).flush();                                                                              \
  } catch (...) {                                                                                                 \
    /* 创建一个字符串流对象。Create a stringstream object. */                                          \
    std::stringstream ss;                                                                                         \
    /* 将未知异常信息添加到字符串流中。Add the unknown exception information to the stringstream. \
     */                                                                                                           \
    ss << "caught unknown C++ exception in destructor of " #Type                                                  \
       << " while handling a failure at: " << __FILE__ << ":" << __LINE__ << '\n';                                \
    /* 输出字符串流内容并刷新缓冲区。Output the contents of the stringstream and flush the buffer. \
     */                                                                                                           \
    (std::cerr << ss.str()).flush();                                                                              \
  }

#endif  // RMW__IMPL__CPP__MACROS_HPP_
