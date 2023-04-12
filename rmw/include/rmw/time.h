// Copyright 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#ifndef RMW__TIME_H_
#define RMW__TIME_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>

#include "rcutils/time.h"

#include "rmw/macros.h"
#include "rmw/visibility_control.h"

/// 表示 RMW 中持续时间或相对时间的结构体，不编码起点。
/// A struct representing a duration or relative time in RMW - does not encode an origin.
typedef struct RMW_PUBLIC_TYPE rmw_time_s
{
  /// 秒数部分
  /// Seconds component
  uint64_t sec;

  /// 纳秒部分
  /// Nanoseconds component
  uint64_t nsec;
} rmw_time_t;

// 定义 rmw_time_point_value_t 为 rcutils_time_point_value_t 类型
// Define rmw_time_point_value_t as rcutils_time_point_value_t type
typedef rcutils_time_point_value_t rmw_time_point_value_t;

// 定义 rmw_duration_t 为 rcutils_duration_value_t 类型
// Define rmw_duration_t as rcutils_duration_value_t type
typedef rcutils_duration_value_t rmw_duration_t;

/// 代表无限持续时间的常量。用 rmw_time_equal 进行比较。
/**
  * 不同的 RMW 实现具有不同的无限持续时间表示。
  * 对于未指定的 QoS 策略持续时间，将报告此值。
  * 不要直接比较 `sec == sec && nsec == nsec`，因为我们不想对非规范化值（nsec > 1 秒）敏感 - 改用 rmw_time_equal。
  * 此值为 INT64_MAX 纳秒 = 0x7FFF FFFF FFFF FFFF = d 9 223 372 036 854 775 807
  *
  * 注意：这些常量不能是 `static const rmw_time_t`，因为在 C 中它们不能用作编译时初始化器
  */
/// Constant representing an infinite duration. Use rmw_time_equal for comparisons.
/**
  * Different RMW implementations have different representations for infinite durations.
  * This value is reported for QoS policy durations that are left unspecified.
  * Do not directly compare `sec == sec && nsec == nsec`, because we don't want to be sensitive
  * to non-normalized values (nsec > 1 second) - use rmw_time_equal instead.
  * This value is INT64_MAX nanoseconds = 0x7FFF FFFF FFFF FFFF = d 9 223 372 036 854 775 807
  *
  * Note: these constants cannot be `static const rmw_time_t` because in C that can't be used
  * as a compile-time initializer
  */
#define RMW_DURATION_INFINITE                                                                      \
  {                                                                                                \
    9223372036LL, 854775807LL                                                                      \
  }
#define RMW_DURATION_UNSPECIFIED                                                                   \
  {                                                                                                \
    0LL, 0LL                                                                                       \
  }

/// 检查两个 rmw_time_t 是否表示相同的时间。
/// Check whether two rmw_time_t represent the same time.
RMW_PUBLIC
RMW_WARN_UNUSED
bool rmw_time_equal(const rmw_time_t left, const rmw_time_t right);

/// 返回一个时间的总纳秒表示。
/// Return the total nanosecond representation of a time.
/**
  * \return 若输入值太大而无法存储在64位中，则返回 INT64_MAX
  * \return INT64_MAX if input is too large to store in 64 bits
  */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_duration_t rmw_time_total_nsec(const rmw_time_t time);

/// 使用总纳秒表示构建 rmw_time_t。
/// Construct rmw_time_t from a total nanoseconds representation.
/**
  * rmw_time_t 只指定相对时间，因此该计算中原点不相关。
  * rmw_time_t only specifies relative time, so the origin is not relevant for this calculation.
  * \return 如果输入为负数，则返回 RMW_DURATION_INFINITE，它不能在 rmw_time_t 中表示。
  * \return RMW_DURATION_INFINITE if input is negative, which is not representable in rmw_time_t
  */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_time_t rmw_time_from_nsec(const rmw_duration_t nanoseconds);

/// 确保 rmw_time_t 的纳秒数不超过1秒。
/// Ensure that an rmw_time_t does not have nanoseconds > 1 second.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_time_t rmw_time_normalize(const rmw_time_t time);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // RMW__TIME_H_
