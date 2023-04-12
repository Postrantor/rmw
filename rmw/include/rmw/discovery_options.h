// Copyright 2022 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__DISCOVERY_OPTIONS_H_
#define RMW__DISCOVERY_OPTIONS_H_

#include "rcutils/allocator.h"
#include "rmw/macros.h"
#include "rmw/ret_types.h"
#include "rmw/visibility_control.h"

#ifdef __cplusplus
extern "C" {
#endif

/// 用于控制节点发现的范围 (Used to control the range that nodes will be discovered)
typedef enum RMW_PUBLIC_TYPE rmw_automatic_discovery_range_e {
  /// 发现范围尚未设置 (The discovery range has not been set)
  RMW_AUTOMATIC_DISCOVERY_RANGE_NOT_SET = 0,
  /// 强制关闭发现 (Force discovery off)
  RMW_AUTOMATIC_DISCOVERY_RANGE_OFF = 1,
  /// 允许发现同一主机上的节点 (Allows discovering nodes on the same host)
  RMW_AUTOMATIC_DISCOVERY_RANGE_LOCALHOST = 2,
  /// 允许发现同一子网上的节点 (Allows discovering nodes on the same subnet)
  RMW_AUTOMATIC_DISCOVERY_RANGE_SUBNET = 3,
  /// 使用与中间件直接配置的发现设置 (Use discovery settings configured directly with the
  /// middleware)
  RMW_AUTOMATIC_DISCOVERY_RANGE_SYSTEM_DEFAULT = 4,
} rmw_automatic_discovery_range_t;

/// 对等主机名或IP地址的最大长度 (Maximum length of a peer hostname or IP address)
#define RMW_DISCOVERY_OPTIONS_STATIC_PEERS_MAX_LENGTH 256

/// 结构体类型定义一些对等地址 (Struct to typedef some of the peer addresses)
typedef struct rmw_peer_address_s {
  char peer_address[RMW_DISCOVERY_OPTIONS_STATIC_PEERS_MAX_LENGTH];
} rmw_peer_address_t;

/// 用于指定控制发现方式的选项 (Used to specify the options that control how discovery is performed)
typedef struct RMW_PUBLIC_TYPE rmw_discovery_options_s {
  /// 允许发现节点的范围有多远 (How far to allow discovering nodes)
  /**
   * 这需要设置为不是 RMW_AUTOMATIC_DISCOVERY_RANGE_NOT_SET 的值，
   * 因为这只是一个哨兵值，用于查看是否设置了此值，但是如果在上下文初始化期间给出了这个值，
   * rmw 实现应该报错。
   * (This needs to be set to something other than RMW_AUTOMATIC_DISCOVERY_RANGE_NOT_SET,
   * because that is just a sentinel value to see if this was set, but rmw
   * implementations should error if this is what is given during context init.)
   */
  rmw_automatic_discovery_range_t automatic_discovery_range;

  /// 与之执行静态发现的手动指定的对等体列表 (The list of manually-specified peers to perform static
  /// discovery with)
  /**
   * 每个对等体都以主机名或 IP 地址（IPv4 和 IPv6 都可以接受）或子网（例如
   * 192.168.0.0/24）的形式指定。 (Each peer is specified as a hostname or an IP address (IPv4 and
   * IPv6 are both acceptable), or a subnet, e.g. 192.168.0.0/24.)
   */
  rmw_peer_address_t* static_peers;

  /// 手动指定的对等体数量 (The number of manually-specified peers)
  size_t static_peers_count;

  /// 用于分配 static_peers 的分配器 (The allocator used to allocate static_peers)
  rcutils_allocator_t allocator;
} rmw_discovery_options_t;

/// 返回一个零初始化的发现选项结构。 (Return a zero-initialized discovery options structure.)
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_discovery_options_t rmw_get_zero_initialized_discovery_options(void);

/// 使用一定数量的静态对等体初始化发现选项结构。 (Initialize a discovery options structure with a
/// set number of static peers.)
/**
 * 此函数使用一组静态对等体的空间初始化 rmw_discovery_options_t。
 * (This function initializes rmw_discovery_options_t with space for a set number of static peers.)
 *
 * \param[in] discovery_options 成功时要初始化的零初始化选项结构的指针，但在失败时保持不变。
 *               (Pointer to a zero initialized option structure to be initialized on
 *                success, but left unchanged on failure.)
 * \param[in] size 要为其分配空间的静态对等体数量。 (Number of static peers to allocate space for.)
 * \param[in] allocator 用于分配内存的分配器。 (Allocator to be used to allocate memory.)
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `discovery_options` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `discovery_options` is not
 *   zero initialized, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `allocator` is invalid,
 *   by rcutils_allocator_is_valid() definition, or
 * \returns `RMW_BAD_ALLOC` if memory allocation fails, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_discovery_options_init(rmw_discovery_options_t* discovery_options,
                                     size_t size,
                                     rcutils_allocator_t* allocator);

/// 比较两个discovery参数实例是否相等。
/// Compare two discovery parameter instances for equality.
/**
 * 相等意味着automatic_discovery_range值相等，它们具有相同的static_peers_count值，
 * 以及使用strncmp评估static_peers中的每个条目作为相等。
 * Equality means the automatic_discovery_range values are equal, they have the same
 * static_peers_count value, and each entry in static_peers is evaluated as
 * equal using strncmp.
 *
 * 注意：如果两个参数结构以不同顺序列出静态对等体，则此结果将被视为不相等。
 * NOTE: If the two parameter structs list the static peers in different orders
 * then this will evaluate as NOT equal.
 *
 * \param[in] left - 要比较的第一组选项
 * \param[in] right - 要比较的第二组选项
 * \param[out] result - 计算结果
 * \param[in] left - The first set of options to compare
 * \param[in] right - The second set of options to compare
 * \param[out] result - The result of the calculation.
 *
 * \return 当输入参数有效时返回RMW_RET_OK。
 * \return RMW_RET_OK when the input arguments are valid.
 * \return
 * 当任何输入为nullptr或者两个结构中的某些内容格式错误（如static_peers为nullptr而static_peers_count非零）时，返回RMW_RET_INVALID_ARGUMENT。
 * \return RMW_RET_INVALID_ARGUMENT will be returned when any input is a nullptr,
 * or if something in either struct was malformed, such as static_peers being
 * a nullptr while static_peers_count is non-zero.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_discovery_options_equal(const rmw_discovery_options_t* const left,
                                      const rmw_discovery_options_t* const right,
                                      bool* result);

/// 使用给定的分配器将discovery选项从src深拷贝到dst。
/// Perform a deep copy of the discovery options from src into dst using the
/// given allocator.
/**
 * dst将保留一个static_peers数组的拥有权，其字符串值与src匹配。
 * The dst will be left with an owned copy of the static peers array whose
 * string values match the src.
 * 如果成功，则使用rmw_discovery_options_equal评估src和dst相等。
 * If successful, src and dst will evaluate as equal using
 * rmw_discovery_options_equal.
 *
 * \param[in] src 要复制的discovery选项
 * \param[in] allocator 要使用的分配器
 * \param[out] dst 要使用的目标选项
 * \param[in] src discovery options to be copied.
 * \param[in] allocator to use.
 * \param[out] dst Destination options to use.
 * \return 成功时返回RMW_RET_OK。
 * \return RMW_RET_OK if success.
 * \return 如果src、allocator或dst为null，或者src和dst是同一个对象，则返回RMW_RET_INVALID_ARGUMENT。
 * \return RMW_RET_INVALID_ARGUMENT if either the src, allocator or dst is null, or
 * \return RMW_RET_INVALID_ARUGMENT if src and dst are the same object.
 * \return 如果分配失败，则返回RMW_RET_BAD_ALLOC。
 * \return RMW_RET_BAD_ALLOC if allocation fails.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_discovery_options_copy(const rmw_discovery_options_t* src,
                                     rcutils_allocator_t* allocator,
                                     rmw_discovery_options_t* dst);

/// rmw_discovery_options_t的析构函数
/// Destructor for rmw_discovery_options_t
/**
 * \param[in] discovery_options 要销毁的discovery选项
 * \param[in] allocator 用于销毁的分配器
 * \param[in] discovery_options to destroy
 * \param[in] allocator to be used for destruction.
 * \return 成功时返回RMW_RET_OK。
 * \return RMW_RET_OK if success.
 * \return 如果分配器无效或discovery_options为null，则返回RMW_RET_INVALID_ARGUMENT。
 * \return RMW_RET_INVALID_ARGUMENT if allocator is invalid
 * or discovery_options is null.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t rmw_discovery_options_fini(rmw_discovery_options_t* discovery_options);

#ifdef __cplusplus
}
#endif

#endif  // RMW__DISCOVERY_OPTIONS_H_
