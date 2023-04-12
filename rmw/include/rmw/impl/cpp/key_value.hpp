// Copyright 2017 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__IMPL__CPP__KEY_VALUE_HPP_
#define RMW__IMPL__CPP__KEY_VALUE_HPP_

#include <map>
#include <string>
#include <vector>

namespace rmw {
namespace impl {
namespace cpp {

/**
 * @brief 解析键值对 (Parse key-value pairs)
 *
 * @param[in] kv 一个包含键值对的 uint8_t 类型的向量 (A vector of uint8_t containing key-value
 * pairs)
 * @return std::map<std::string, std::vector<uint8_t>> 返回解析后的键值对映射 (Returns the parsed
 * key-value pair mapping)
 */
static std::map<std::string, std::vector<uint8_t>> parse_key_value(const std::vector<uint8_t>& kv) {
  // 创建一个存储键值对的映射 (Create a map to store the key-value pairs)
  std::map<std::string, std::vector<uint8_t>> m;

  // 标记是否找到键 (Flag for whether a key has been found)
  bool keyfound = false;

  // 存储键和值的变量 (Variables to store the key and value)
  std::string key;
  std::vector<uint8_t> value;
  uint8_t prev = '\0';

  // 如果向量为空，则跳转到 not_valid (If the vector is empty, jump to not_valid)
  if (kv.size() == 0) {
    goto not_valid;
  }

  // 遍历向量中的每个元素 (Iterate through each element in the vector)
  for (uint8_t u8 : kv) {
    // 如果已经找到键 (If a key has already been found)
    if (keyfound) {
      // 处理键值分隔符 ';' (Handle the key-value delimiter ';')
      if ((u8 == ';') && (prev != ';')) {
        prev = u8;
        continue;
      } else if ((u8 != ';') && (prev == ';')) {
        // 如果值为空，则跳转到 not_valid (If the value is empty, jump to not_valid)
        if (value.size() == 0) {
          goto not_valid;
        }
        // 将键值对添加到映射中 (Add the key-value pair to the map)
        m[key] = value;

        // 清除键和值，准备下一次迭代 (Clear the key and value for the next iteration)
        key.clear();
        value.clear();
        keyfound = false;
      } else {
        // 否则，将当前字符添加到值向量中 (Otherwise, add the current character to the value vector)
        value.push_back(u8);
      }
    }
    // 如果没有找到键 (If a key has not been found)
    if (!keyfound) {
      // 处理键值分隔符 '=' (Handle the key-value delimiter '=')
      if (u8 == '=') {
        // 如果键为空，则跳转到 not_valid (If the key is empty, jump to not_valid)
        if (key.size() == 0) {
          goto not_valid;
        }
        keyfound = true;
      } else if (isalnum(u8)) {
        // 如果当前字符是字母或数字，将其添加到键字符串中 (If the current character is alphanumeric,
        // add it to the key string)
        key.push_back(u8);
      } else if ((u8 == '\0') && (key.size() == 0) && (m.size() > 0)) {
        // 接受尾随的 '\0' 字符 (Accept trailing '\0' characters)
        break;
      } else if ((prev != ';') || (key.size() > 0)) {
        // 其他情况，跳转到 not_valid (In other cases, jump to not_valid)
        goto not_valid;
      }
    }
    prev = u8;
  }
  // 如果找到键 (If a key has been found)
  if (keyfound) {
    // 如果值为空，则跳转到 not_valid (If the value is empty, jump to not_valid)
    if (value.size() == 0) {
      goto not_valid;
    }
    // 将键值对添加到映射中 (Add the key-value pair to the map)
    m[key] = value;
  } else if (key.size() > 0) {
    // 如果键不为空，但没有找到键，则跳转到 not_valid (If the key is not empty but no key was found,
    // jump to not_valid)
    goto not_valid;
  }
  // 返回解析后的映射 (Return the parsed map)
  return m;

not_valid:
  // 这不是失败，因为 participant_qos 的 userData 可能被用于其他目的
  // (This is not a failure as the participant_qos userData might be used for other purposes)
  // 系统中未由 rmw 创建的其他参与者可能会将 userData 用于其他事项
  // (Other participants in the system not created by rmw could use userData for something else)
  return std::map<std::string, std::vector<uint8_t>>();
}

}  // namespace cpp
}  // namespace impl
}  // namespace rmw

#endif  // RMW__IMPL__CPP__KEY_VALUE_HPP_
