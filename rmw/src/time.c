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

#include "rmw/time.h"

#include "rcutils/time.h"

/**
 * @brief 检查两个 rmw_time_t 结构是否相等 (Check if two rmw_time_t structures are equal)
 *
 * @param left 第一个时间结构 (The first time structure)
 * @param right 第二个时间结构 (The second time structure)
 * @return 如果相等返回 true，否则返回 false (Returns true if equal, false otherwise)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
bool rmw_time_equal(const rmw_time_t left, const rmw_time_t right) {
  // 将输入的时间转换为纳秒并比较它们是否相等 (Convert the input times to nanoseconds and compare if
  // they are equal)
  return rmw_time_total_nsec(left) == rmw_time_total_nsec(right);
}

/**
 * @brief 计算 rmw_time_t 结构表示的总纳秒数 (Calculate the total nanoseconds represented by a
 * rmw_time_t structure)
 *
 * @param time 输入的时间结构 (The input time structure)
 * @return 总纳秒数 (Total nanoseconds)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_duration_t rmw_time_total_nsec(const rmw_time_t time) {
  // 最大可表示的秒数 (The maximum representable seconds)
  static const uint64_t max_sec = INT64_MAX / RCUTILS_S_TO_NS(1);

  // 如果秒数超过最大值，则返回 INT64_MAX (If the seconds are greater than the maximum value, return
  // INT64_MAX)
  if (time.sec > max_sec) {
    // 秒数无法用纳秒表示 (Seconds not representable in nanoseconds)
    return INT64_MAX;
  }

  // 将秒转换为纳秒 (Convert seconds to nanoseconds)
  const int64_t sec_as_nsec = RCUTILS_S_TO_NS(time.sec);

  // 检查溢出 (Check for overflow)
  if (time.nsec > (uint64_t)(INT64_MAX - sec_as_nsec)) {
    // 溢出 (overflow)
    return INT64_MAX;
  }

  // 返回总纳秒数 (Return the total nanoseconds)
  return sec_as_nsec + time.nsec;
}

/**
 * @brief 从纳秒数创建一个 rmw_time_t 结构 (Create a rmw_time_t structure from nanoseconds)
 *
 * @param nanoseconds 输入的纳秒数 (The input nanoseconds)
 * @return rmw_time_t 结构 (A rmw_time_t structure)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_time_t rmw_time_from_nsec(const rmw_duration_t nanoseconds) {
  // 如果纳秒数为负，则返回无限持续时间 (If the nanoseconds are negative, return an infinite
  // duration)
  if (nanoseconds < 0) {
    return (rmw_time_t)RMW_DURATION_INFINITE;
  }

  // 创建一个 rmw_time_t 结构并填充其值 (Create a rmw_time_t structure and populate its values)
  rmw_time_t time;
  time.sec = RCUTILS_NS_TO_S(nanoseconds);
  time.nsec = nanoseconds % RCUTILS_S_TO_NS(1);

  // 返回创建的结构 (Return the created structure)
  return time;
}

/**
 * @brief 标准化 rmw_time_t 结构，使其具有有效的秒和纳秒组合 (Normalize a rmw_time_t structure so
 * that it has a valid combination of seconds and nanoseconds)
 *
 * @param time 输入的时间结构 (The input time structure)
 * @return 标准化后的 rmw_time_t 结构 (A normalized rmw_time_t structure)
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_time_t rmw_time_normalize(const rmw_time_t time) {
  // 将输入的时间转换为纳秒，然后再将其转换回 rmw_time_t 结构 (Convert the input time to
  // nanoseconds, then convert it back to a rmw_time_t structure)
  return rmw_time_from_nsec(rmw_time_total_nsec(time));
}
