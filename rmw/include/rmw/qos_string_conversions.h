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

#ifndef RMW__QOS_STRING_CONVERSIONS_H_
#define RMW__QOS_STRING_CONVERSIONS_H_

#include "rmw/types.h"
#include "rmw/visibility_control.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 这些函数之间的关系是相互独立的，它们可以单独使用。主要目的是在处理 QoS
 * 策略时，提供一种方便的方式来**在枚举值和字符串之间进行转换**。这样可以方便地**在配置文件、命令行参数或其他需要以字符串形式表示
 * QoS 策略的场景中使用这些策略**。
 *
 * QoS 策略的字符转换和反向转换。

- 这几个函数是独立的，它们各自处理**不同类型的 QoS 策略转换**。
  - `rmw_qos_policy_kind_to_str`、
  - `rmw_qos_durability_policy_to_str`、
  - `rmw_qos_history_policy_to_str`、
  - `rmw_qos_liveliness_policy_to_str` 和
  - `rmw_qos_reliability_policy_to_str`
- 这几个函数也是独立的，它们各自处理不同类型的 QoS 策略**反向转换**。
  - `rmw_qos_policy_kind_from_str`、
  - `rmw_qos_durability_policy_from_str`、
  - `rmw_qos_history_policy_from_str`、
  - `rmw_qos_liveliness_policy_from_str` 和
  - `rmw_qos_reliability_policy_from_str`
 */

/// 返回表示策略类型的字符串。
/**
 * 当 `kind` 为 `RMW_QOS_POLICY_INVALID` 或未定义类型时返回 `NULL`。
 *
 * 可以通过以下转换获得策略类型的字符串化版本：
 * RMW_QOS_POLICY_<POLICY_KIND> -> lower_case(<POLICY_KIND>)
 *
 * 例如，`RMW_QOS_POLICY_DURABILITY` 的字符串化版本是 "durability"，
 * 而 `RMW_QOS_POLICY_DEADLINE` 是 "deadline"。
 *
 * <hr>
 * 属性               | 符合性
 * ------------------ | -------------
 * 分配内存            | 否
 * 线程安全             | 是
 * 使用原子操作         | 否
 * 无锁                | 是
 *
 * \param[in] kind 要字符串化的 qos 策略类型。
 * \return 表示策略类型的空终止字符串，或
 * \return 如果 kind 为 `RMW_QOS_POLICY_INVALID` 或未定义类型，则返回 `NULL`。
 */
RMW_PUBLIC
const char* rmw_qos_policy_kind_to_str(rmw_qos_policy_kind_t kind);

/// 返回表示策略值的字符串。
/**
 * 当 `value` 为 `RMW_QOS_POLICY_*_UNKNOWN` 或未定义枚举值时返回 `NULL`。
 *
 * 可以通过以下转换获得策略值的字符串化版本：
 * RMW_QOS_POLICY_<POLICY_KIND>_<POLICY_VALUE> -> lower_case(<POLICY_VALUE>)
 *
 * 例如，`RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC` 的字符串化版本是 "manual_by_topic"，
 * "manual_by_topic", 而 `RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT` 是 "best_effort"。 and
 *
 * <hr>
 * 属性               | 符合性
 * ------------------ | -------------
 * 分配内存            | 否
 * 线程安全             | 是
 * 使用原子操作         | 否
 * 无锁                | 是
 *
 * \param[in] value 要字符串化的 qos 策略值。
 * \return 表示策略值的空终止字符串，或
 * \return 如果 value 为 `RMW_QOS_POLICY_*_UNKNOWN` 或未定义枚举值，则返回 `NULL`。
 */
RMW_PUBLIC
const char* rmw_qos_durability_policy_to_str(rmw_qos_durability_policy_t value);

/// 返回表示策略值的字符串。
/**
 * 有关更多详细信息，请参见 \ref rmw_qos_durability_policy_to_str()。
 */
RMW_PUBLIC
const char* rmw_qos_history_policy_to_str(rmw_qos_history_policy_t value);

/// 返回表示策略值的字符串。
/**
 * 有关更多详细信息，请参见 \ref rmw_qos_durability_policy_to_str()。
 */
RMW_PUBLIC
const char* rmw_qos_liveliness_policy_to_str(rmw_qos_liveliness_policy_t value);

/// 返回表示策略值的字符串。
/**
 * 有关更多详细信息，请参见 \ref rmw_qos_durability_policy_to_str()。
 */
RMW_PUBLIC
const char* rmw_qos_reliability_policy_to_str(rmw_qos_reliability_policy_t value);

/// 根据提供的字符串返回策略类型。
/**
 * 返回由提供的字符串表示的策略类型，或者当提供的字符串不表示任何策略类型时返回
 * `RMW_QOS_POLICY_INVALID`。
 *
 * 策略类型如何字符串化在 \ref rmw_qos_policy_kind_to_str 中有解释。
 *
 * <hr>
 * 属性          | 遵循
 * ------------------ | -------------
 * 分配内存 (Allocates Memory)   | 否 (No)
 * 线程安全 (Thread-Safe)        | 是 (Yes)
 * 使用原子操作 (Uses Atomics)       | 否 (No)
 * 无锁 (Lock-Free)          | 是 (Yes)
 *
 * \param[in] str 识别 qos 策略类型的字符串。
 * \return 由字符串表示的策略类型，或者
 * \return `RMW_QOS_POLICY_INVALID` 如果字符串不表示任何策略类型。
 */
RMW_PUBLIC
rmw_qos_policy_kind_t rmw_qos_policy_kind_from_str(const char* str);

/// 返回基于提供的字符串的枚举值。
/**
 * 根据提供的字符串返回枚举值，或者在提供的字符串意外时返回 `RMW_QOS_POLICY_*_UNKNOWN`。
 * `RMW_QOS_POLICY_*_UNKNOWN` when the provided string is unexpected.)
 *
 * 如何将策略值字符串化在 \ref rmw_qos_durability_policy_to_str 中有解释。
 *
 * <hr>
 * 属性                  | 遵循
 * ------------------   | -------------
 * 分配内存              | 否
 * 线程安全              | 是
 * 使用原子操作          | 否
 * 无锁                  | 是
 *
 * \param[in] str 用于标识 QoS 策略值的字符串。
 * \return 字符串表示的策略值，或者
 * \return 如果字符串不表示任何值，则返回 `RMW_QOS_POLICY_*_UNKNOWN`。
 */
RMW_PUBLIC
rmw_qos_durability_policy_t rmw_qos_durability_policy_from_str(const char* str);

/// 返回基于提供的字符串的枚举值。
/**
 * 更多详细信息请参见 \ref rmw_qos_durability_policy_from_str()。
 */
RMW_PUBLIC
rmw_qos_history_policy_t rmw_qos_history_policy_from_str(const char* str);

/// 返回基于提供的字符串的枚举值。
/**
 * 更多详细信息请参见 \ref rmw_qos_durability_policy_from_str()。
 */
RMW_PUBLIC
rmw_qos_liveliness_policy_t rmw_qos_liveliness_policy_from_str(const char* str);

/// 返回基于提供的字符串的枚举值。
/**
 * 更多详细信息请参见 \ref rmw_qos_durability_policy_from_str()。
 */
RMW_PUBLIC
rmw_qos_reliability_policy_t rmw_qos_reliability_policy_from_str(const char* str);

#ifdef __cplusplus
}
#endif

#endif  // RMW__QOS_STRING_CONVERSIONS_H_
