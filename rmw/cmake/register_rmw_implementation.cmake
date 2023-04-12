# Copyright 2014 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# 注册当前包作为一个 ROS 中间件实现。
# Register the current package as a ROS middleware implementation.
#
# 该函数的一个有效用法是
# A valid usage of this function is
# register_rmw_implementation("c:rosidl_typesupport_introspection_c"
#                             "cpp:rosidl_typesupport_introspection_cpp")
# 如果一个包中有多个有效的 typesupport，可以用冒号隔开，例如：
# If there were multiple valid typesupports for one language in a package,
# they would be separated by colons, e.g.
# register_rmw_implementation(
#   "cpp:rosidl_typesupport_a_cpp:rosidl_typesupport_b_cpp")
# 如果有多个具有相同语言标志（第一个条目）的输入，函数将出错。
# If there are multiple inputs with the same language flag (first entry) the
# function will error.
# :param ARGN: 一个 <language:typesupport> 元组列表，其中 language 是
# typesupport 包的语言，typesupport 是包的名称。
# :param ARGN: a list of <language:typesupport> tuples where language is the
# language of the typesupport package and typesupport is the name of the
# package.
# :type ARGN: 字符串列表
# :type ARGN: a list of strings
#
# @public
#
function(register_rmw_implementation)
  set(all_typesupports "")
  set(language_labels "")

  foreach(arg_raw ${ARGN})
    # 将冒号替换为分号，以转换为列表
    # replace colon with semicolon to turn into a list
    string(REPLACE ":" ";" arg "${arg_raw}")
    list(LENGTH arg arg_length)
    if(arg_length LESS 1)
      message(FATAL_ERROR
        "register_rmw_implementation() called with invalid input: ${arg_raw}")
    endif()
    list(GET arg 0 language_label)
    list(REMOVE_AT arg 0)
    string(TOLOWER "${language_label}" language_label)
    # 缓存找到的现有语言标签（这样我们就不必每个循环都检查两次 ament 索引），
    # 如果我们已经遇到了该语言，则报错。
    # Cache the existing languages labels found (so that we don't have to
    # check the ament index twice every loop) and error if we've already
    # encountered the language
    list(FIND language_labels "${language_label}" label_found)
    if(NOT ${label_found} EQUAL -1)
      message(FATAL_ERROR
        "register_rmw_implementation() got language '${language_label}' "
        "multiple times")
    endif()
    list(APPEND language_labels "${language_label}")
    ament_index_register_resource(
      "rmw_typesupport_${language_label}" CONTENT "${arg}"
    )
    list_append_unique(all_typesupports "${arg}")
  endforeach()

  ament_index_register_resource("rmw_typesupport" CONTENT "${all_typesupports}")
endfunction()
