// Copyright 2014-2017 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__TYPES_H_
#define RMW__TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// map rcutils specific log levels to rmw speicfic type
#include <rcutils/logging.h>

#include "rmw/events_statuses/events_statuses.h"
#include "rmw/init.h"
#include "rmw/init_options.h"
#include "rmw/ret_types.h"
#include "rmw/security_options.h"
#include "rmw/serialized_message.h"
#include "rmw/subscription_content_filter_options.h"
#include "rmw/time.h"
#include "rmw/visibility_control.h"

// 我们将 GID 定义为 16 字节（128 位）。这应该足够确保系统中所有实体的唯一性。
// 如何填充这个值取决于各个 RMW 实现，可以从底层中间件获取，也可以直接从 RMW 层获取。
#define RMW_GID_STORAGE_SIZE 16u

/// \brief 封装了一个 rmw 节点的结构
typedef struct RMW_PUBLIC_TYPE rmw_node_s {
  /// RMW 实现的名称
  const char* implementation_identifier;
  /// 指向该节点数据的类型擦除指针
  void* data;
  /// 用于识别的简洁节点名称
  const char* name;
  /// 此 rmw 节点的命名空间
  const char* namespace_;
  /// 关于节点初始化特定信息的上下文信息
  rmw_context_t* context;
} rmw_node_t;

/// Endpoint enumeration type (端点枚举类型)
typedef enum RMW_PUBLIC_TYPE rmw_endpoint_type_e {
  /// Endpoint type has not yet been set (端点类型尚未设置)
  RMW_ENDPOINT_INVALID = 0,
  /// Creates and publishes messages to the ROS topic (创建并发布消息到ROS主题)
  RMW_ENDPOINT_PUBLISHER,
  /// Listens for and receives messages from a topic (从主题监听和接收消息)
  RMW_ENDPOINT_SUBSCRIPTION
} rmw_endpoint_type_t;

/// Unique network flow endpoints requirement enumeration (唯一网络流端点需求枚举)
typedef enum RMW_PUBLIC_TYPE rmw_unique_network_flow_endpoints_requirement_e {
  /// (不需要唯一的网络流端点)
  RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_NOT_REQUIRED = 0,
  /// (严格要求唯一的网络流端点。如果RMW实现没有提供，则出错。)
  RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_STRICTLY_REQUIRED,
  /// (可选地要求唯一的网络流端点。如果RMW实现没有提供，也不会出错。)
  RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_OPTIONALLY_REQUIRED,
  /// 系统决定唯一网络流端点的需求
  RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_SYSTEM_DEFAULT
} rmw_unique_network_flow_endpoints_requirement_t;

/// @brief 用于配置rmw中发布者创建的选项。
typedef struct RMW_PUBLIC_TYPE rmw_publisher_options_s {
  /// @brief 在发布者创建过程中传递rmw实现特定资源。
  /**
   * @details
   * 此字段是类型擦除的（而不是前向声明的），因为它通常是对语言特定对象的非拥有引用，例如，在C++中，它可能是只有rmw实现可以使用的多态类。
   * 此处指向的资源需要在此选项结构和使用它创建的任何rmw_publisher对象之外继续存在，因为它们会复制此结构并在其生命周期内可能使用此有效负载。
   */
  void* rmw_specific_publisher_payload;
  /// @brief 要求中间件生成唯一的网络流端点。
  /**
   * @details 唯一的网络流端点需要区分节点间通信的发布者和订阅者之间的网络提供的QoS。
   * 默认值为RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_NOT_REQUIRED。
   */
  /* gpt-4
    这句话的意思是，在分布式通信中，为了区分发布者和订阅者之间的网络流并提供相应的
    QoS（服务质量），我们需要唯一的`network_flow_endpoint`来实现这个目标。换句话说，`network_flow_endpoint`
    在通信节点中充当唯一标识符，使得我们能够准确地对发布者和订阅者间的网络流进行区分，并针对不同的网络流应用相应的
    QoS 策略。

    通过利用唯一的 `network_flow_endpoint`，DDS 系统中的通信节点可以针对该通信实体分配和管理 QoS
    策略。这样一来，DDS
    系统能够确保在复杂的分布式网络环境下，各个发布者和订阅者之间的通信能够满足特定的性能指标。

    简而言之，`network_flow_endpoint`是用来区分通信节点中发布者和订阅者之间的网络流，在 DDS
    系统中发挥着关键作用。通过为每个`network_flow_endpoint`提供适当的 QoS
    策略，我们可以实现在这些节点之间可靠、高效和安全地传输数据。
  */
  rmw_unique_network_flow_endpoints_requirement_t require_unique_network_flow_endpoints;
} rmw_publisher_options_t;

/// @brief 结构体，封装了一个rmw发布者
typedef struct RMW_PUBLIC_TYPE rmw_publisher_s {
  /// rmw实现的名称
  const char* implementation_identifier;
  /// 指向此发布者数据的类型擦除指针
  void* data;
  /// 此发布者发布到的ROS主题名称
  const char* topic_name;
  /// 发布者选项
  /**
   * @details 传递给rmw_create_publisher()的选项结构应由rmw实现分配给此字段。
   * 创建后不应修改字段，但选项结构的内容可能是const或非const，即浅层的const-ness。
   * 为避免在设置过程中进行任何const转换，此字段未标记为const。
   */
  rmw_publisher_options_t options;
  /// @brief 表示此发布者是否支持借用消息
  bool can_loan_messages;
} rmw_publisher_t;

/// @brief 可用于配置在 rmw 中创建订阅的选项。
typedef struct RMW_PUBLIC_TYPE rmw_subscription_options_s {
  /// @brief 在订阅创建期间用于传递 rmw 实现特定的资源。
  /**
   * 此字段在 rmw_publisher_options_t 中的所有相同细节和限制也适用于此结构体。
   * \sa rmw_publisher_options_t.rmw_specific_publisher_payload
   */
  void* rmw_specific_subscription_payload;
  /// @brief 如果为真，则中间件不应从本地发布者传递数据。
  /**
   * 当只应从远程节点接收数据时，尤其是在内部和跨进程通信同时进行时避免“双重传递”时，最常使用此设置。
   *
   * \todo(wjwwood): 当参与者映射问题解决时，确定这一点。
   *   参见：https://github.com/ros2/design/pull/250
   *
   * 目前，本地的定义有些模糊。
   * 现在它表示节点本地，这个定义效果最好，但是当参与者映射到上下文时，可能会变得更复杂。
   */
  bool ignore_local_publications;
  /// @brief 要求中间件生成唯一的网络流端点。
  /**
   * 需要唯一的网络流端点来区分通信节点中发布者和订阅者之间的流的网络提供的 QoS。
   * 默认值为 RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_NOT_REQUIRED。
   */
  rmw_unique_network_flow_endpoints_requirement_t require_unique_network_flow_endpoints;
  /// 在订阅创建期间用于创建内容过滤器选项。
  rmw_subscription_content_filter_options_t* content_filter_options;
} rmw_subscription_options_t;

/// 定义一个 ROS2 订阅者的结构体 (Defines a structure for a ROS2 subscription)
typedef struct RMW_PUBLIC_TYPE rmw_subscription_s {
  /// 实现的名称 (Name of the rmw implementation)
  const char* implementation_identifier;
  /// 指向此订阅者的类型擦除指针 (Type erased pointer to this subscription)
  void* data;
  /// 此订阅者侦听的 ROS 主题名称 (Name of the ros topic this subscription listens to)
  const char* topic_name;
  /// 订阅选项 (Subscription options)
  /**
   * 应将传递给 rmw_create_subscription() 的选项结构分配给此字段
   * 创建后不应修改字段，但是选项结构的内容可能是常量或非常量，即浅层常量
   * 为避免在设置过程中的任何常量转换，此字段未标记为常量
   */
  rmw_subscription_options_t options;
  /// 表示此订阅者是否可以借用消息
  bool can_loan_messages;
  /// 表示此订阅者的内容过滤主题是否已启用
  bool is_cft_enabled;
} rmw_subscription_t;

/// 定义一个 ROS2 服务的结构体 (Defines a handle to an rmw service)
typedef struct RMW_PUBLIC_TYPE rmw_service_s {
  /// 实现的名称 (The name of the rmw implementation)
  const char* implementation_identifier;
  /// 指向此服务的类型擦除指针 (Type erased pointer to this service)
  void* data;
  /// 此服务在 ROS 图中公开的名称 (The name of this service as exposed to the ros graph)
  const char* service_name;
} rmw_service_t;

/// 一个指向 rmw 服务客户端的句柄 (A handle to an rmw service client)
typedef struct RMW_PUBLIC_TYPE rmw_client_s {
  /// 实现的名称 (The name of the rmw implementation)
  const char* implementation_identifier;
  /// 指向此服务客户端的类型擦除指针 (Type erased pointer to this service client)
  void* data;
  /// 在 ros 图中暴露的此服务的名称 (The name of this service as exposed to the ros graph)
  const char* service_name;
} rmw_client_t;

/// 一个指向 rmw 守护条件的句柄 (Handle for an rmw guard condition)
typedef struct RMW_PUBLIC_TYPE rmw_guard_condition_s {
  /// 实现的名称 (The name of the rmw implementation)
  const char* implementation_identifier;
  /// 指向此守护条件的类型擦除指针 (Type erased pointer to this guard condition)
  void* data;
  /// 与此守护条件关联的 rmw 上下文 (rmw context associated with this guard condition)
  rmw_context_t* context;
} rmw_guard_condition_t;

/// 为 rmw 发布者分配内存 (Allocation of memory for an rmw publisher)
typedef struct RMW_PUBLIC_TYPE rmw_publisher_allocation_s {
  /// 实现的名称 (The name of the rmw implementation)
  const char* implementation_identifier;
  /// 指向此分配的类型擦除指针 (Type erased pointer to this allocation)
  void* data;
} rmw_publisher_allocation_t;

/// 为 rmw 订阅者分配内存 (Allocation of memory for an rmw subscription)
typedef struct RMW_PUBLIC_TYPE rmw_subscription_allocation_s {
  /// 实现的名称 (The name of the rmw implementation)
  const char* implementation_identifier;
  /// 指向此分配的类型擦除指针 (Type erased pointer to this allocation)
  void* data;
} rmw_subscription_allocation_t;

/// 订阅者句柄数组 (Array of subscriber handles).
/**
 * 一个 void * 指针数组，表示类型擦除的中间件特定订阅
 * 非空条目的数量可能小于分配的数组大小
 * 表示的订阅数量可能小于分配的数组大小
 * 此结构的创建者负责分配和释放数组
 */
typedef struct RMW_PUBLIC_TYPE rmw_subscriptions_s {
  /// 数组所代表的订阅者数量 (The number of subscribers represented by the array).
  size_t subscriber_count;
  /// 指向订阅者 void * 指针数组的指针 (Pointer to an array of void * pointers of subscriptions).
  void** subscribers;
} rmw_subscriptions_t;

/// 服务句柄数组 (Array of service handles).
/**
 * 一个 void * 指针数组，表示类型擦除的中间件特定服务
 * 非空条目的数量可能小于分配的数组大小
 * 表示的服务数量可能小于分配的数组大小
 * 此结构的创建者负责分配和释放数组
 */
typedef struct RMW_PUBLIC_TYPE rmw_services_s {
  /// 数组所代表的服务数量 (The number of services represented by the array).
  size_t service_count;
  /// 指向服务 void * 指针数组的指针 (Pointer to an array of void * pointers of services).
  void** services;
} rmw_services_t;

/// 客户端句柄数组 (Array of client handles).
/**
 * 一个 void * 指针数组，表示类型擦除的中间件特定客户端。非空条目的数量可能小于分配的数组大小
 * 表示的客户端数量可能小于分配的数组大小。此结构的创建者负责分配和释放数组
 */
typedef struct RMW_PUBLIC_TYPE rmw_clients_s {
  /// 数组所代表的客户端数量
  size_t client_count;
  /// 指向客户端 void * 指针数组的指针
  void** clients;
} rmw_clients_t;

// rmw_events_s 结构体定义
typedef struct RMW_PUBLIC_TYPE rmw_events_s {
  /// 事件数组中的事件数量（The number of events represented by the array）
  size_t event_count;
  /// 指向事件 void * 指针数组的指针（Pointer to an array of void * pointers of events）
  void** events;
} rmw_events_t;

// rmw_guard_conditions_s 结构体定义（Define the rmw_guard_conditions_s structure）
/**
 * 一个表示类型擦除的中间件特定守护条件的 void * 指针数组。非空条目的数量可能小于数组的分配大小
 * 表示的守护条件的数量可能小于数组的分配大小。此结构的创建者负责分配和释放数组
 */
typedef struct RMW_PUBLIC_TYPE rmw_guard_conditions_s {
  /// 数组所表示的守护条件数量（The number of guard conditions represented by the array）
  size_t guard_condition_count;
  /// 指向守护条件 void * 指针数组的指针
  void** guard_conditions;
} rmw_guard_conditions_t;

/// rmw_wait_set_s 结构体定义（Define the rmw_wait_set_s structure）
typedef struct RMW_PUBLIC_TYPE rmw_wait_set_s {
  /// rmw 实现的名称（The name of the rmw implementation）
  const char* implementation_identifier;
  /// 要等待的守护条件（The guard condition to be waited on）
  rmw_guard_conditions_t* guard_conditions;
  /// 指向此等待集数据的类型擦除指针（Type erased pointer to this wait set's data）
  void* data;
} rmw_wait_set_t;

/// rmw_request_id_s 结构体定义（Define the rmw_request_id_s structure）
typedef struct RMW_PUBLIC_TYPE rmw_request_id_s {
  /// 与此请求关联的写入者的 guid（The guid of the writer associated with this request）
  uint8_t writer_guid[RMW_GID_STORAGE_SIZE];
  /// 此服务的序列号（Sequence number of this service）
  int64_t sequence_number;
} rmw_request_id_t;

/// 服务相关接收的元数据 (Service-related take meta-data)
typedef struct RMW_PUBLIC_TYPE rmw_service_info_s {
  rmw_time_point_value_t
      source_timestamp;  ///< 源时间戳（发送方）(Source timestamp from the sender)
  rmw_time_point_value_t received_timestamp;  ///< 接收时间戳（本地）(Received timestamp at local)
  rmw_request_id_t request_id;                ///< 请求ID (Request ID)
} rmw_service_info_t;

/// QoS 可靠性策略枚举 (QoS reliability policy enumeration)
typedef enum RMW_PUBLIC_TYPE rmw_qos_reliability_policy_e {
  /// 实现特定的默认值
  RMW_QOS_POLICY_RELIABILITY_SYSTEM_DEFAULT,
  /// 保证样本传递，可能会重试多次
  RMW_QOS_POLICY_RELIABILITY_RELIABLE,
  /// 尝试传递样本，但如果网络不稳定，可能会丢失一些样本
  RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT,
  /// 可靠性策略尚未设置 (Reliability policy has not yet been set)
  RMW_QOS_POLICY_RELIABILITY_UNKNOWN,
  /// 将匹配大多数端点，并在可能的情况下使用可靠策略
  /**
   * 在创建订阅或发布者时选择策略。如果可靠策略与所有发现的端点匹配，则选择可靠策略，否则选择最佳努力策略。
   *
   * 由函数 `rmw_subscription_get_actual_qos` 或 `rmw_publisher_get_actual_qos` 报告的 QoS
   * 策略可能是最佳可用的、可靠的或最佳努力的。 (The QoS policy reported by functions like
   *
   * 不支持服务和客户端，默认为 `rmw_qos_profile_services_default` 中的可靠性值。
   *
   * 即使所选策略与新发现的端点不兼容，中间件也不会在创建订阅或发布者后更新策略。因此，由于与发现竞争，可能会出现不确定行为，应谨慎使用此策略。
   */
  RMW_QOS_POLICY_RELIABILITY_BEST_AVAILABLE
} rmw_qos_reliability_policy_t;

/// QoS 历史枚举, 描述样本如何持久化
typedef enum RMW_PUBLIC_TYPE rmw_qos_history_policy_e {
  /// 实现历史策略的默认值
  RMW_QOS_POLICY_HISTORY_SYSTEM_DEFAULT,
  /// 只存储最大数量的样本，超过最大值时删除最旧的样本
  RMW_QOS_POLICY_HISTORY_KEEP_LAST,
  /// 存储所有样本（受资源限制）
  RMW_QOS_POLICY_HISTORY_KEEP_ALL,
  /// 历史策略尚未设置
  RMW_QOS_POLICY_HISTORY_UNKNOWN
} rmw_qos_history_policy_t;

/// QoS 持久性枚举，描述样本如何持久化
typedef enum RMW_PUBLIC_TYPE rmw_qos_durability_policy_e {
  /// 特定实现的默认值
  RMW_QOS_POLICY_DURABILITY_SYSTEM_DEFAULT,
  /// rmw 发布者负责为“晚加入”的订阅者保留样本
  RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL,
  /// 样本不是持久的
  RMW_QOS_POLICY_DURABILITY_VOLATILE,
  /// 持久性策略尚未设置
  RMW_QOS_POLICY_DURABILITY_UNKNOWN,
  /// 将匹配大多数端点，并在可能的情况下使用临时本地策略
  /**
   * 在创建订阅或发布者时选择策略。
   * 如果与所有已发现的端点匹配，则选择临时本地策略，否则选择易失性策略。
   *
   * 在为订阅选择易失性策略的情况下，订阅创建之前由临时本地发布者发送的任何消息都不会被接收。
   *
   * 由函数 `rmw_subscription_get_actual_qos` 或 `rmw_publisher_get_actual_qos` 报告的 QoS
   * 策略可能是最佳可用的，临时本地的或易失性的。
   *
   * 不支持服务和客户端，默认使用 `rmw_qos_profile_services_default` 中的持久性值。
   *
   * 即使所选策略与新发现的端点不兼容，中间件也不应在创建订阅或发布者后更新策略。因此，由于与发现的竞争关系，使用此策略可能会导致非确定性行为。
   */
  RMW_QOS_POLICY_DURABILITY_BEST_AVAILABLE
} rmw_qos_durability_policy_t;

/**
 * @brief 定义RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_NODE_DEPRECATED_MSG，提示已弃用的警告消息。
 */
#define RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_NODE_DEPRECATED_MSG \
  "RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_NODE is deprecated. "    \
  "Use RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC if manually asserted liveliness is needed."

/**
 * @brief 如果不是在_WIN32平台下，定义RMW_DECLARE_DEPRECATED宏，用于声明一个已弃用的函数或变量。
 * @param name 已弃用的函数或变量的名称。The name of the deprecated function or variable.
 * @param msg 与已弃用的函数或变量相关的警告消息。The warning message associated with the deprecated
 * function or variable.
 */
#ifndef _WIN32
#define RMW_DECLARE_DEPRECATED(name, msg) name __attribute__((deprecated(msg)))
#else

/**
 * @brief 如果在_WIN32平台下，定义RMW_DECLARE_DEPRECATED宏，用于声明一个已弃用的函数或变量。
 * @param name 已弃用的函数或变量的名称。The name of the deprecated function or variable.
 * @param msg 与已弃用的函数或变量相关的警告消息。The warning message associated with the deprecated
 * function or variable.
 */
#define RMW_DECLARE_DEPRECATED(name, msg) name __pragma(deprecated(name))
#endif

// 用于描述发布者报告其活跃状态的QoS生命周期枚举。
// 对于订阅者，这些是它对主题发布者的要求。

// 抑制语法错误，因为cppcheck似乎无法处理枚举器属性。
// cppcheck-suppress syntaxError
typedef enum RMW_PUBLIC_TYPE rmw_qos_liveliness_policy_e {
  /// 实现特定的默认值
  RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT = 0,
  /// 建立主题存活的信号来自ROS rmw层。
  RMW_QOS_POLICY_LIVELINESS_AUTOMATIC = 1,
  /// 在这种情况下需要明确声明节点活跃。
  /// 此选项已弃用，如果您的应用程序需要手动声明活跃，请使用RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC。
  RMW_DECLARE_DEPRECATED(
      RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_NODE,
      RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_NODE_DEPRECATED_MSG) = 2,
  /// 建立主题存活的信号位于主题级别。只有在主题上发布消息或从应用程序发出明确信号以声明主题活跃，才会将主题标记为活跃。
  // 使用`3`以实现向后兼容。
  RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC = 3,
  /// 生命周期策略尚未设置
  RMW_QOS_POLICY_LIVELINESS_UNKNOWN = 4,
  /// 将匹配大多数端点，并在可能的情况下使用按主题手动策略
  /**
   * 在创建订阅或发布者时将选择一个策略。
   * 如果按主题手动策略与所有发现的端点匹配，则会选择按主题手动策略，否则将选择自动策略。
   *
   * 由`rmw_subscription_get_actual_qos`或`rmw_publisher_get_actual_qos`等函数报告的QoS策略可能是最佳可用、自动或按主题手动。
   *
   * 不支持服务和客户端，默认为`rmw_qos_profile_services_default`中的生命周期值。
   *
   * 因此，即使选择的策略与新发现的端点不兼容，在创建订阅或发布者后，也不希望中间件更新策略。
   * 因此，由于与发现的竞争关系，此策略可能会导致非确定性行为，因此应谨慎使用。
   */
  RMW_QOS_POLICY_LIVELINESS_BEST_AVAILABLE = 5
} rmw_qos_liveliness_policy_t;

/// QoS Deadline 默认值 (QoS Deadline default value).
#define RMW_QOS_DEADLINE_DEFAULT RMW_DURATION_UNSPECIFIED
// 将与大多数端点匹配，同时尽可能维持严格的策略
/**
 * 值为 RMW_DURATION_INFINITE - 1.
 *
 * 在创建订阅或发布器时选择策略。
 * 对于订阅，截止日期将是所有已发现发布者截止日期的最大值。
 * 对于发布者，截止日期将是所有已发现订阅截止日期的最小值。
 *
 * 函数 `rmw_subscription_get_actual_qos` 或
 * `rmw_publisher_get_actual_qos` 报告的 QoS 策略可能是最佳可用的或实际截止日期值。
 *
 * 不支持服务和客户端，默认使用 `rmw_qos_profile_services_default` 中的截止日期值。
 *
 * 即使选择的策略与新发现的端点不兼容，中间件也不会在创建订阅或发布器后更新策略。
 * 因此，由于与发现竞争，可能会出现非确定性行为，应谨慎使用此策略。
 */
#define RMW_QOS_DEADLINE_BEST_AVAILABLE \
  { 9223372036LL, 854775806LL }

/// QoS 生命周期默认值
#define RMW_QOS_LIFESPAN_DEFAULT RMW_DURATION_UNSPECIFIED

/// QoS 活跃度租期默认值。
#define RMW_QOS_LIVELINESS_LEASE_DURATION_DEFAULT RMW_DURATION_UNSPECIFIED

/// 尽可能保持严格策略，以匹配大多数端点
/**
 * 值为 RMW_DURATION_INFINITE - 1。
 *
 * 在创建订阅或发布者时，将选择一个策略。
 * 对于订阅，租期长度将是所有已发现发布者租期长度的最大值。
 * 对于发布者，租期长度将是所有已发现订阅租期长度的最小值。
 *
 * 由类似 `rmw_subscription_get_actual_qos` 或 `rmw_publisher_get_actual_qos` 这样的函数报告的 QoS
 * 策略可能是最佳可用的或实际租期持续时间值。
 *
 * 服务和客户端不受支持，并默认为 `rmw_qos_profile_services_default` 中的租期持续时间值。
 *
 * 即使所选策略与新发现的端点不兼容，中间件也不会在创建订阅或发布者后更新策略。
 * 因此，应谨慎使用此策略，因为由于与发现的竞争关系，可能会发生非确定性行为。
 */
#define RMW_QOS_LIVELINESS_LEASE_DURATION_BEST_AVAILABLE \
  { 9223372036LL, 854775806LL }

/// ROS 中间件质量服务配置 (ROS Middleware Quality of Service profile)
typedef struct RMW_PUBLIC_TYPE rmw_qos_profile_s {
  /// 历史消息策略 (History policy for messages)
  enum rmw_qos_history_policy_e history;
  /// 消息队列的大小 (Size of the message queue)
  size_t depth;
  /// 可靠性 QoS 策略设置 (Reliability QoS policy setting)
  enum rmw_qos_reliability_policy_e reliability;
  /// 持久性 QoS 策略设置 (Durability QoS policy setting)
  enum rmw_qos_durability_policy_e durability;
  /// 预期的消息发送/接收周期
  /**
   * RMW_DURATION_UNSPEFICIED 将使用 RMW 实现的默认值，可能是无限的。
   * RMW_DURATION_INFINITE 显式说明消息永远不会错过截止期望。
   */
  struct rmw_time_s deadline;
  /// 消息被认为过期且不再有效的年龄
  /**
   * RMW_DURATION_UNSPEFICIED 将使用 RMW 实现的默认值，可能是无限的。
   * RMW_DURATION_INFINITE 显式说明消息不会过期。
   */
  struct rmw_time_s lifespan;
  /// 活跃度 QoS 策略设置 (Liveliness QoS policy setting)
  enum rmw_qos_liveliness_policy_e liveliness;
  /// RMW 节点或发布者必须显示其活跃状态的时间
  /**
   * RMW_DURATION_UNSPEFICIED 将使用 RMW 实现的默认值，可能是无限的。
   * RMW_DURATION_INFINITE 显式说明活跃度未被执行。
   */
  struct rmw_time_s liveliness_lease_duration;
  /// 如果为 true，则将绕过任何 ROS 特定的命名空间约定。
  /**
   * 对于 DDS 和主题来说，例如，这意味着典型的 ROS 特定前缀 `rt` 不会像下面描述的那样应用：
   *   http://design.ros2.org/articles/topic_and_service_names.html#ros-specific-namespace-prefix
   * 当试图将本地 DDS 主题与 ROS 2 主题直接连接时，这可能非常有用。
   */
  bool avoid_ros_namespace_conventions;
} rmw_qos_profile_t;

/// \brief 全局唯一标识符，用于 ROS 图形实体 (Globally unique identifier for a ROS graph entity)
/**
 * \details 这在 ROS
 * 领域内应该是全局唯一的。无论是在本地（创建实体的地方）还是在远程主机或进程上报告时，标识符都应相同。
 */
typedef struct RMW_PUBLIC_TYPE rmw_gid_s {
  /// rmw 实现的名称 (Name of the rmw implementation)
  const char* implementation_identifier;
  /// 字节数据 GID 值 (Byte data GID value)
  uint8_t data[RMW_GID_STORAGE_SIZE];
} rmw_gid_t;

/// \brief 序列号不受支持的消息信息常量 (Message info constant for unsupported sequence numbers)
#define RMW_MESSAGE_INFO_SEQUENCE_NUMBER_UNSUPPORTED UINT64_MAX

/// 信息描述一个rmw消息 (Information describing an rmw message)
typedef struct RMW_PUBLIC_TYPE rmw_message_info_s {
  /// 发布者发布消息的时间。
  /**
   * 时间戳采取的确切点未指定，但每次都应在发布过程中的相同点一致地采取。
   */
  rmw_time_point_value_t source_timestamp;
  /// 订阅接收到消息的时间。
  /**
   * 时间戳采取的确切点未指定，但每次接收消息的过程中都应在相同点一致地采取。
   */
  rmw_time_point_value_t received_timestamp;
  /// 发布者设置的已接收消息的序列号。
  /**
   * 此序列号由发布者设置，因此与发布者GID结合时唯一标识消息。
   * 长时间运行的应用程序，序列号可能在某个时候环绕。
   *
   * 如果rmw实现不支持序列号，其值将为RMW_MESSAGE_INFO_SEQUENCE_NUMBER_UNSUPPORTED。
   *
   * 要求：
   *
   * 如果`psn1`和`psn2`是通过调用`rmw_take*()`获得的发布序列号，
   * 其中`psn1`在发生在`psn2`之前的调用中获得，且两个序列号都来自相同的发布者（即也是相同的发布者gid），那么：
   *
   * - psn2 > psn1（除非环绕）
   * - `psn2 - psn1 - 1`是发布者在两个接收消息之间发送的消息数。
   *   这些可能已经通过发生在中间的其他`rmw_take*()`调用获取或丢失。
   *   当且仅当发布者连续发送消息时，`psn2 - psn1 - 1 = 0`。
   */
  uint64_t publication_sequence_number;
  /// 订阅设置的已接收消息的序列号。
  /**
   * 此序列号由订阅设置，无论哪个发布者发送了消息。
   * 长时间运行的应用程序，序列号可能在某个时候环绕。
   *
   * 如果rmw实现不支持序列号，其值将为RMW_MESSAGE_INFO_SEQUENCE_NUMBER_UNSUPPORTED。
   *
   * 要求：
   *
   * 如果`rsn1`和`rsn2`是通过调用`rmw_take*()`获得的接收序列号，
   * 其中`rsn1`在发生在`rsn2`之前的调用中获得，那么：
   *
   * - rsn2 > rsn1（除非环绕）
   * - 当且仅当两个`rmw_take*()`调用连续发生时，`rsn2 = rsn1 + 1`。
   */
  uint64_t reception_sequence_number;
  /// 发送消息的发布者的全局唯一标识符。
  /**
   * 该标识符在本地上下文中唯一标识发布者，但对于相同的发布者，在其他上下文或进程中给出的标识符不一定相同。
   * 因此，标识符将在您的应用程序中唯一标识发布者，但与另一个应用程序进行比较时，可能会对该发布者的标识符产生分歧。
   * 即使存在这种限制，与发布者序列号结合也可以在本地上下文中唯一标识消息。
   * 由rmw实现生成的发布者GID可能在某个时候发生冲突，在这种情况下，无法确定哪个发布者发送了消息。
   * GID生成的详细信息取决于rmw实现。
   *
   * rmw实现可能需要重用发布者GID，因为耗尽唯一标识符或其他约束，在这种情况下，
   * rmw实现可能会记录在这种情况下发生的情况，但该行为在此未定义。
   * 但是，如果可能的话，rmw实现应尽量避免这种情况，并且在实践中不太可能发生。
   *
   * \todo 将来，我们希望这将在上下文、进程和机器之间全球范围内唯一地标识发布者。
   */
  rmw_gid_t publisher_gid;

  /// 此消息是否来自内部流程通信
  bool from_intra_process;
} rmw_message_info_t;

/// 获取零初始化的消息信息。 (Get zero initialized message info.)
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_message_info_t rmw_get_zero_initialized_message_info(void);

/// 当历史记录设置为 RMW_QOS_POLICY_HISTORY_KEEP_LAST 时，rmw 队列的默认大小，
/// 0 表示当前未设置。 (Default size of the rmw queue when history is set to
/// RMW_QOS_POLICY_HISTORY_KEEP_LAST, 0 indicates it is currently not set)
enum { RMW_QOS_POLICY_DEPTH_SYSTEM_DEFAULT = 0 };

/// rcutils 日志严重程度类型到 rmw 特定类型的类型映射。 (Type mapping of rcutils log severity types
/// to rmw specific types.)
typedef enum RMW_PUBLIC_TYPE {
  /// 调试日志严重程度，用于详细消息。 (Debug log severity, for pedantic messaging)
  RMW_LOG_SEVERITY_DEBUG = RCUTILS_LOG_SEVERITY_DEBUG,

  /// 信息日志严重程度，用于报告预期但不是压倒性的信息。 (Informational log severity, for reporting
  /// expected but not overwhelming information)
  RMW_LOG_SEVERITY_INFO = RCUTILS_LOG_SEVERITY_INFO,

  /// 警告日志严重程度，用于报告可恢复的问题。 (Warning log severity, for reporting recoverable
  /// issues)
  RMW_LOG_SEVERITY_WARN = RCUTILS_LOG_SEVERITY_WARN,

  /// 错误日志严重程度，用于报告无法解决的问题。 (Error log severity, for reporting uncoverable
  /// issues)
  RMW_LOG_SEVERITY_ERROR = RCUTILS_LOG_SEVERITY_ERROR,

  /// 致命日志严重程度，用于报告导致即将关闭的问题。 (Fatal log severity, for reporting issue
  /// causing imminent shutdown)
  RMW_LOG_SEVERITY_FATAL = RCUTILS_LOG_SEVERITY_FATAL
} rmw_log_severity_t;

#ifdef __cplusplus
}
#endif

#endif  // RMW__TYPES_H_
