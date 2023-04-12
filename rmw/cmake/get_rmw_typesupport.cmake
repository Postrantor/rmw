# Copyright 2015-2016 Open Source Robotics Foundation, Inc.
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

# 获取特定 RMW 实现的类型支持包名称。
# Get the type support package names for a specific RMW implementation.
#
# :param var: 包含包名称的输出变量名
# :type var: 字符串列表
# :param rmw_implementation: RMW 实现的包名称
# :type rmw_implementation: 字符串
# :param LANGUAGE: 获取类型支持的语言的可选标志。如果省略了语言，则返回所有语言的类型支持。
# :type LANGUAGE: 字符串
#
# @public
#
function(get_rmw_typesupport var rmw_implementation)
  cmake_parse_arguments(ARG "" "LANGUAGE" "" ${ARGN})
  # 如果有未解析的参数，则报告致命错误
  # If there are unparsed arguments, report a fatal error
  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "get_rmw_typesupport() called with unused "
      "arguments: ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  # 设置资源类型为 "rmw_typesupport"
  # Set resource type as "rmw_typesupport"
  set(resource_type "rmw_typesupport")
  # 如果指定了 ARG_LANGUAGE 参数
  # If ARG_LANGUAGE is specified
  if(ARG_LANGUAGE)
    # 将 ARG_LANGUAGE 转换为小写
    # Convert ARG_LANGUAGE to lowercase
    string(TOLOWER "${ARG_LANGUAGE}" ARG_LANGUAGE)
    # 将资源类型设置为 "rmw_typesupport_${ARG_LANGUAGE}"
    # Set resource type as "rmw_typesupport_${ARG_LANGUAGE}"
    set(resource_type "${resource_type}_${ARG_LANGUAGE}")
  endif()
  # 检查是否具有指定的资源类型和 RMW 实现
  # Check if the specified resource type and RMW implementation are available
  ament_index_has_resource(has_resource "${resource_type}" "${rmw_implementation}")
  # 如果有资源
  # If there is a resource
  if(has_resource)
    # 获取资源
    # Get the resource
    ament_index_get_resource(resource "${resource_type}" "${rmw_implementation}")
  else()
    # 否则将资源设置为空
    # Otherwise, set the resource to empty
    set(resource "")
  endif()
  # 将结果设置为父级作用域的变量
  # Set the result as a variable in the parent scope
  set(${var} "${resource}" PARENT_SCOPE)
endfunction()
