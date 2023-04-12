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

/// 返回表示策略类型的字符串。
/// Return a string representing the policy kind.
/**
 * 当 `kind` 为 `RMW_QOS_POLICY_INVALID` 或未定义类型时返回 `NULL`。
 * Returns `NULL` when `kind` is `RMW_QOS_POLICY_INVALID` or an undefined kind.
 *
 * 可以通过以下转换获得策略类型的字符串化版本：
 * The stringified version of the policy kind can be obtained doing the following conversion:
 * RMW_QOS_POLICY_<POLICY_KIND> -> lower_case(<POLICY_KIND>)
 *
 * 例如，`RMW_QOS_POLICY_DURABILITY` 的字符串化版本是 "durability"，
 * For example, the stringified version of `RMW_QOS_POLICY_DURABILITY` is "durability",
 * 而 `RMW_QOS_POLICY_DEADLINE` 是 "deadline"。
 * and `RMW_QOS_POLICY_DEADLINE` is "deadline".
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
 * \param[in] kind qos policy kind to be stringified.
 * \return 表示策略类型的空终止字符串，或
 * \return a null terminated string representing the policy kind, or
 * \return 如果 kind 为 `RMW_QOS_POLICY_INVALID` 或未定义类型，则返回 `NULL`。
 * \return `NULL` if kind is `RMW_QOS_POLICY_INVALID` or an undefined kind.
 */
RMW_PUBLIC
const char* rmw_qos_policy_kind_to_str(rmw_qos_policy_kind_t kind);

/// 返回表示策略值的字符串。
/// Return a string representing the policy value.
/**
 * 当 `value` 为 `RMW_QOS_POLICY_*_UNKNOWN` 或未定义枚举值时返回 `NULL`。
 * Returns `NULL` when `value` is `RMW_QOS_POLICY_*_UNKNOWN` or an undefined enum value.
 *
 * 可以通过以下转换获得策略值的字符串化版本：
 * The stringified version of the policy value can be obtained doing the following conversion:
 * RMW_QOS_POLICY_<POLICY_KIND>_<POLICY_VALUE> -> lower_case(<POLICY_VALUE>)
 *
 * 例如，`RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC` 的字符串化版本是 "manual_by_topic"，
 * For example, the stringified version of `RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC` is
 * "manual_by_topic", 而 `RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT` 是 "best_effort"。 and
 * `RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT` is "best_effort".
 *
 * <hr>
 * 属性               | 符合性
 * Attribute          | Adherence
 * ------------------ | -------------
 * 分配内存            | 否
 * Allocates Memory   | No
 * 线程安全             | 是
 * Thread-Safe        | Yes
 * 使用原子操作         | 否
 * Uses Atomics       | No
 * 无锁                | 是
 * Lock-Free          | Yes
 *
 * \param[in] value 要字符串化的 qos 策略值。
 * \param[in] value qos policy value to be stringified.
 * \return 表示策略值的空终止字符串，或
 * \return a null terminated string representing the policy value, or
 * \return 如果 value 为 `RMW_QOS_POLICY_*_UNKNOWN` 或未定义枚举值，则返回 `NULL`。
 * \return `NULL` if value is `RMW_QOS_POLICY_*_UNKNOWN` or an undefined enum value.
 */
RMW_PUBLIC
const char* rmw_qos_durability_policy_to_str(rmw_qos_durability_policy_t value);

/// 返回表示策略值的字符串。 (Return a string representing the policy value.)
/**
 * 有关更多详细信息，请参见 \ref rmw_qos_durability_policy_to_str()。 (See \ref
 * rmw_qos_durability_policy_to_str() for more details.)
 */
RMW_PUBLIC
const char* rmw_qos_history_policy_to_str(rmw_qos_history_policy_t value);

/// 返回表示策略值的字符串。 (Return a string representing the policy value.)
/**
 * 有关更多详细信息，请参见 \ref rmw_qos_durability_policy_to_str()。 (See \ref
 * rmw_qos_durability_policy_to_str() for more details.)
 */
RMW_PUBLIC
const char* rmw_qos_liveliness_policy_to_str(rmw_qos_liveliness_policy_t value);

/// 返回表示策略值的字符串。 (Return a string representing the policy value.)
/**
 * 有关更多详细信息，请参见 \ref rmw_qos_durability_policy_to_str()。 (See \ref
 * rmw_qos_durability_policy_to_str() for more details.)
 */
RMW_PUBLIC
const char* rmw_qos_reliability_policy_to_str(rmw_qos_reliability_policy_t value);

/// 根据提供的字符串返回策略类型。 (Return a policy kind based on the provided string.)
/**
 * 返回由提供的字符串表示的策略类型，或者当提供的字符串不表示任何策略类型时返回
 * `RMW_QOS_POLICY_INVALID`。 (Returns the policy kind represented by the provided string, or
 * `RMW_QOS_POLICY_INVALID` when the provided string doesn't represent any policy kind.)
 *
 * 策略类型如何字符串化在 \ref rmw_qos_policy_kind_to_str 中有解释。 (How policy kinds are
 * stringified is explained in \ref rmw_qos_policy_kind_to_str.)
 *
 * <hr>
 * 属性 (Attribute)          | 遵循 (Adherence)
 * ------------------ | -------------
 * 分配内存 (Allocates Memory)   | 否 (No)
 * 线程安全 (Thread-Safe)        | 是 (Yes)
 * 使用原子操作 (Uses Atomics)       | 否 (No)
 * 无锁 (Lock-Free)          | 是 (Yes)
 *
 * \param[in] str 识别 qos 策略类型的字符串。 (string identifying a qos policy kind.)
 * \return 由字符串表示的策略类型，或者 (the policy kind represented by the string, or)
 * \return `RMW_QOS_POLICY_INVALID` 如果字符串不表示任何策略类型。 (if the string doesn't represent
 * any policy kind.)
 */
RMW_PUBLIC
rmw_qos_policy_kind_t rmw_qos_policy_kind_from_str(const char* str);

/// 返回基于提供的字符串的枚举值。 (Return a enum value based on the provided string.)
/**
 * 根据提供的字符串返回枚举值，或者在提供的字符串意外时返回 `RMW_QOS_POLICY_*_UNKNOWN`。
 * (Returns the enum value based on the provided string, or
 * `RMW_QOS_POLICY_*_UNKNOWN` when the provided string is unexpected.)
 *
 * 如何将策略值字符串化在 \ref rmw_qos_durability_policy_to_str 中有解释。
 * (How policy values are stringified is explained in \ref rmw_qos_durability_policy_to_str.)
 *
 * <hr>
 * 属性                  | 遵循
 * ------------------   | -------------
 * 分配内存              | 否
 * 线程安全              | 是
 * 使用原子操作          | 否
 * 无锁                  | 是
 *
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \param[in] str 用于标识 QoS 策略值的字符串。(string identifying a qos policy value.)
 * \return 字符串表示的策略值，或者
 * (the policy value represented by the string, or)
 * \return 如果字符串不表示任何值，则返回 `RMW_QOS_POLICY_*_UNKNOWN`。
 * (`RMW_QOS_POLICY_*_UNKNOWN` if the string doesn't represent any value.)
 */
RMW_PUBLIC
rmw_qos_durability_policy_t rmw_qos_durability_policy_from_str(const char* str);

/// 返回基于提供的字符串的枚举值。 (Return a enum value based on the provided string.)
/**
 * 更多详细信息请参见 \ref rmw_qos_durability_policy_from_str()。
 * (See \ref rmw_qos_durability_policy_from_str() for more details.)
 */
RMW_PUBLIC
rmw_qos_history_policy_t rmw_qos_history_policy_from_str(const char* str);

/// 返回基于提供的字符串的枚举值。 (Return a enum value based on the provided string.)
/**
 * 更多详细信息请参见 \ref rmw_qos_durability_policy_from_str()。
 * (See \ref rmw_qos_durability_policy_from_str() for more details.)
 */
RMW_PUBLIC
rmw_qos_liveliness_policy_t rmw_qos_liveliness_policy_from_str(const char* str);

/// 返回基于提供的字符串的枚举值。 (Return a enum value based on the provided string.)
/**
 * 更多详细信息请参见 \ref rmw_qos_durability_policy_from_str()。
 * (See \ref rmw_qos_durability_policy_from_str() for more details.)
 */
RMW_PUBLIC
rmw_qos_reliability_policy_t rmw_qos_reliability_policy_from_str(const char* str);

#ifdef __cplusplus
}
#endif

#endif  // RMW__QOS_STRING_CONVERSIONS_H_
