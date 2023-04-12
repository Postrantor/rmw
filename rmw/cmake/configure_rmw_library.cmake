# Copyright 2014-2015 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

#
# Configures a library which implements the rmw interface with custom settings.
#
# This should be called on any library which is built to implement the rmw API.
# The custom settings are all related to library symbol visibility, see:
#
# https://gcc.gnu.org/wiki/Visibility
# http://www.ibm.com/developerworks/aix/library/au-aix-symbol-visibility/
#
# Thought about using:
#
# http://www.cmake.org/cmake/help/v2.8.8/cmake.html#module:GenerateExportHeader
#
# But it doesn't seem to set the compiler flags correctly on clang and also
# doesn't work very well when the headers and library are in different projects
# like this.
#
# Effectively, using this macro results in the RMW_BUILDING_DLL definition being
# set on Windows, and the -fvisibility* flags being passed to gcc and clang.
#
# :param library_target: the library target :type library_target: string :param
# LANGUAGE: Optional flag for the language of the library. Allowed values are
# "C" and "CXX". The default is "CXX". :type LANGUAGE: string
#
# @public
#
# @brief 配置 RMW 库的宏 (Macro for configuring RMW library)
#
# @param[in] library_target 目标库名称 (Target library name) @param[in] LANGUAGE
# 语言类型（默认为 "CXX"）(Language type (default is "CXX"))
macro(configure_rmw_library library_target)
  # 解析参数 (Parse arguments)
  cmake_parse_arguments(_ARG "" "LANGUAGE" "" ${ARGN})

  # 如果有未解析的参数，报错 (If there are unparsed arguments, report an error)
  if(_ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "configure_rmw_library() called with unused "
                        "arguments: ${_ARG_UNPARSED_ARGUMENTS}")
  endif()

  # 如果没有指定语言，则设置默认语言为 C++ (If no language is specified, set the default language
  # to C++)
  if(NOT _ARG_LANGUAGE)
    set(_ARG_LANGUAGE "CXX")
  endif()

  # 处理 C 语言情况 (Handling the C language case)
  if(_ARG_LANGUAGE STREQUAL "C")
    # 如果可能，默认将可见性设置为隐藏 (Set the visibility to hidden by default if possible)
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID MATCHES
                                             "Clang")
      # 对于 gcc 和 clang，默认将符号的可见性设置为隐藏（这已经是 Windows 上的默认值） (Set the visibility of
      # symbols to hidden by default for gcc and clang (this is already the
      # default on Windows))
      set_target_properties(${library_target} PROPERTIES COMPILE_FLAGS
                                                         "-fvisibility=hidden")
    endif()

    # 处理 C++ 语言情况 (Handling the C++ language case)
  elseif(_ARG_LANGUAGE STREQUAL "CXX")
    # 如果可能，默认将可见性设置为隐藏 (Set the visibility to hidden by default if possible)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES
                                               "Clang")
      # 对于 gcc 和 clang，默认将符号的可见性设置为隐藏（这已经是 Windows 上的默认值） (Set the visibility of
      # symbols to hidden by default for gcc and clang (this is already the
      # default on Windows))
      set_target_properties(
        ${library_target}
        PROPERTIES COMPILE_FLAGS
                   "-fvisibility=hidden -fvisibility-inlines-hidden")
    endif()

    # 不支持的语言类型，报错 (Unsupported language type, report an error)
  else()
    message(FATAL_ERROR "configure_rmw_library() called with unsupported "
                        "LANGUAGE: '${_ARG_LANGUAGE}'")
  endif()

  # 在 Windows 平台上处理可见性宏 (Handle visibility macros on Windows platform)
  if(WIN32)
    # 导致可见性宏使用 dllexport 而不是 dllimport，这在构建 dll 时是合适的，但在使用它时则不合适 (Causes the
    # visibility macros to use dllexport rather than dllimport, which is
    # appropriate when building the dll but not consuming it)
    target_compile_definitions(${library_target} PRIVATE "RMW_BUILDING_DLL")
  endif()
endmacro()
