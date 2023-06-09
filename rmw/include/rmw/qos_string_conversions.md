> [!NOTE]
> 应该就是这里，在 rclcpp 层的 qos_parameters.hpp 中直接跳转到了这里，前者是创建 pub/sub 时候会调用的函数
>
> ---
>
> 考虑增加 qos，应该增加 ownership 等
> 应该清楚，这个文件中的功能是在哪里需要使用，如果只是增加这两个 qos 是否 有必要
> 根据下面的描述，是为了可以通过命令行实现 qos 的配置，作为字符串转换的
> 这样的话，前面考虑修改 rclcpp 文件就是为了能够兼容 beatles 中已经有的这个配置文件的方式
> 这里应该是进一步调用了 rmw 层中 该文件实现具体的字符串转换的能力

##

> [!NOTE]
> 在配置文件、命令行参数或其他需要以字符串形式表示 QoS 策略的场景中使用这些策略

这个文件包含以下功能：

1. **rmw_qos_policy_kind_to_str**：将`rmw_qos_policy_kind_t`**枚举值转换为相应的字符串表示**。
2. **rmw_qos_durability_policy_to_str**：将`rmw_qos_durability_policy_e`枚举值转换为相应的字符串表示。
3. **rmw_qos_history_policy_to_str**：将`rmw_qos_history_policy_e`枚举值转换为相应的字符串表示。
4. **rmw_qos_liveliness_policy_to_str**：将`rmw_qos_liveliness_policy_e`枚举值转换为相应的字符串表示。
5. **rmw_qos_reliability_policy_to_str**：将`rmw_qos_reliability_policy_e`枚举值转换为相应的字符串表示。
6. **rmw_qos_policy_kind_from_str**：将字符串转换为对应的`rmw_qos_policy_kind_t`枚举值。
7. **rmw_qos_durability_policy_from_str**：将字符串转换为对应的`rmw_qos_durability_policy_e`枚举值。
8. **rmw_qos_history_policy_from_str**：将字符串转换为对应的`rmw_qos_history_policy_e`枚举值。
9. **rmw_qos_liveliness_policy_from_str**：将字符串转换为对应的`rmw_qos_liveliness_policy_e`枚举值。
10. **rmw_qos_reliability_policy_from_str**：将字符串转换为对应的`rmw_qos_reliability_policy_e`枚举值。

这些函数之间的关系是相互独立的，它们可以单独使用。主要目的是在处理 QoS 策略时，提供一种方便的方式来**在枚举值和字符串之间进行转换**。这样可以方便地**在配置文件、命令行参数或其他需要以字符串形式表示 QoS 策略的场景中使用这些策略**。

例如，当用户需要设置一个发布者的 QoS 策略时，他们可以在配置文件中使用字符串（如"reliable"）来表示所需的策略。然后，通过调用`rmw_qos_reliability_policy_from_str`函数，将字符串转换为对应的枚举值，以便在创建发布者时使用。

## Trantor

这份代码是 ROS2 项目中的 rmw（ROS Middleware）模块的一部分，主要用于处理 QoS（Quality of Service，服务质量）相关的字符串转换。QoS 是在 ROS2 中用于控制和调整消息传输行为的一组策略。

文件中定义了以下几个函数：

1. `rmw_qos_policy_kind_to_str`：将 QoS 策略类型（`rmw_qos_policy_kind_t`）转换为对应的字符串表示。
2. `rmw_qos_durability_policy_to_str`：将 QoS 持久性策略（`rmw_qos_durability_policy_e`）转换为对应的字符串表示。
3. `rmw_qos_history_policy_to_str`：将 QoS 历史策略（`rmw_qos_history_policy_e`）转换为对应的字符串表示。
4. `rmw_qos_liveliness_policy_to_str`：将 QoS 活跃度策略（`rmw_qos_liveliness_policy_e`）转换为对应的字符串表示。
5. `rmw_qos_reliability_policy_to_str`：将 QoS 可靠性策略（`rmw_qos_reliability_policy_e`）转换为对应的字符串表示。
6. `rmw_qos_policy_kind_from_str`：将字符串表示转换为对应的 QoS 策略类型（`rmw_qos_policy_kind_t`）。
7. `rmw_qos_durability_policy_from_str`：将字符串表示转换为对应的 QoS 持久性策略（`rmw_qos_durability_policy_e`）。
8. `rmw_qos_history_policy_from_str`：将字符串表示转换为对应的 QoS 历史策略（`rmw_qos_history_policy_e`）。
9. `rmw_qos_liveliness_policy_from_str`：将字符串表示转换为对应的 QoS 活跃度策略（`rmw_qos_liveliness_policy_e`）。
10. `rmw_qos_reliability_policy_from_str`：将字符串表示转换为对应的 QoS 可靠性策略（`rmw_qos_reliability_policy_e`）。

函数之间的调用关系：

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

总结：

这份文件主要负责将 QoS 策略从枚举类型转换为字符串表示，以及从字符串表示转换回枚举类型。这些转换函数可以帮助开发者在处理 QoS 策略时，更方便地进行调试和输出。

## You

在 ros2 的项目中使用 dds 作为通信中间件，同时支持 dds 的一些 qos 策略，但是其中的 `ownership` 和 `ownership_strength` 策略并没有得到支持，如果希望增加这两种 qos 策略。对上面这个文件来说，以此为基础上增加 `ownership` 和 `ownerhsip_strength` 这两种 qos 策略该如何修改，请给出具体完善的修改代码，只给出修改部分即可。

## Trantor

> [!NOTE]
> 这几步很清楚！

为了增加 `ownership` 和 `ownership_strength` 这两种 QoS 策略，我们需要在上述代码中进行以下修改：

1. 在 `rmw_qos_policy_kind_t` 枚举中添加新的策略类型。
2. 修改 `rmw_qos_policy_kind_to_str` 函数以支持新的策略类型。
3. 添加新的转换函数 `rmw_qos_ownership_policy_to_str` 和 `rmw_qos_ownership_strength_policy_to_str`。
4. 修改 `rmw_qos_policy_kind_from_str` 函数以支持新的策略类型。
5. 添加新的反向转换函数 `rmw_qos_ownership_policy_from_str` 和 `rmw_qos_ownership_strength_policy_from_str`。

以下是具体的修改部分：

```c
// 1. 在 rmw_qos_policy_kind_t 枚举中添加新的策略类型
typedef enum RMW_PUBLIC_TYPE rmw_qos_policy_kind_t
{
  ...
  RMW_QOS_POLICY_OWNERSHIP,
  RMW_QOS_POLICY_OWNERSHIP_STRENGTH,
} rmw_qos_policy_kind_t;

// 2. 修改 rmw_qos_policy_kind_to_str 函数以支持新的策略类型
const char * rmw_qos_policy_kind_to_str(rmw_qos_policy_kind_t kind)
{
  switch (kind) {
    ...
    case RMW_QOS_POLICY_OWNERSHIP:
      return "ownership";
    case RMW_QOS_POLICY_OWNERSHIP_STRENGTH:
      return "ownership_strength";
    ...
  }
}

// 3. 添加新的转换函数 rmw_qos_ownership_policy_to_str 和 rmw_qos_ownership_strength_policy_to_str
const char * rmw_qos_ownership_policy_to_str(enum rmw_qos_ownership_policy_e value)
{
  switch (value) {
    case RMW_QOS_POLICY_OWNERSHIP_SYSTEM_DEFAULT:
      return "system_default";
    case RMW_QOS_POLICY_OWNERSHIP_SHARED:
      return "shared";
    case RMW_QOS_POLICY_OWNERSHIP_EXCLUSIVE:
      return "exclusive";
    case RMW_QOS_POLICY_OWNERSHIP_UNKNOWN:  // fallthrough
    default:
      return NULL;
  }
}

const char * rmw_qos_ownership_strength_policy_to_str(int32_t value)
{
  // 将整数值转换为字符串表示
  static char buffer[12];
  snprintf(buffer, sizeof(buffer), "%d", value);
  return buffer;
}

// 4. 修改 rmw_qos_policy_kind_from_str 函数以支持新的策略类型
rmw_qos_policy_kind_t rmw_qos_policy_kind_from_str(const char * str)
{
  ...
  if (RMW_QOS_STREQ_WITH_LITERAL("ownership", str)) {
    return RMW_QOS_POLICY_OWNERSHIP;
  }
  if (RMW_QOS_STREQ_WITH_LITERAL("ownership_strength", str)) {
    return RMW_QOS_POLICY_OWNERSHIP_STRENGTH;
  }
  ...
}

// 5. 添加新的反向转换函数 rmw_qos_ownership_policy_from_str 和 rmw_qos_ownership_strength_policy_from_str
enum rmw_qos_ownership_policy_e rmw_qos_ownership_policy_from_str(const char * str)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(str, RMW_QOS_POLICY_OWNERSHIP_UNKNOWN);
  if (RMW_QOS_STREQ_WITH_LITERAL("system_default", str)) {
    return RMW_QOS_POLICY_OWNERSHIP_SYSTEM_DEFAULT;
  }
  if (RMW_QOS_STREQ_WITH_LITERAL("shared", str)) {
    return RMW_QOS_POLICY_OWNERSHIP_SHARED;
  }
  if (RMW_QOS_STREQ_WITH_LITERAL("exclusive", str)) {
    return RMW_QOS_POLICY_OWNERSHIP_EXCLUSIVE;
  }
  return RMW_QOS_POLICY_OWNERSHIP_UNKNOWN;
}

int32_t rmw_qos_ownership_strength_policy_from_str(const char * str)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(str, -1);
  int32_t value = atoi(str);
  if (value < 0) {
    return -1;
  }
  return value;
}
```

请注意，这里的修改仅针对该文件。要完全支持新的 QoS 策略，还需要在其他相关文件和模块中进行相应的修改。

## You

你在上面补充了 `qos_string_conversions.c`，请帮我同时完善 `qos_string_conversions.h` 文件

## Trantor

当然可以。`qos_string_conversions.h` 文件应包含与 `qos_string_conversions.c` 中实现的函数相对应的函数声明。以下是一个完整的 `qos_string_conversions.h` 文件示例：

```cpp
#ifndef QOS_STRING_CONVERSIONS_H_
#define QOS_STRING_CONVERSIONS_H_

#include "rmw/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// 将字符串转换为 rmw_qos_reliability_policy_t 枚举值
/**
 * \param[in] str 输入字符串
 * \return 对应的 rmw_qos_reliability_policy_t 枚举值，如果输入无效，则返回 RMW_QOS_POLICY_RELIABILITY_UNKNOWN
 */
rmw_qos_reliability_policy_t
rmw_qos_reliability_policy_from_str(const char * str);

/// 将 rmw_qos_reliability_policy_t 枚举值转换为字符串
/**
 * \param[in] policy 输入的 rmw_qos_reliability_policy_t 枚举值
 * \return 对应的字符串表示，如果输入无效，则返回 NULL
 */
const char *
rmw_qos_reliability_policy_to_str(rmw_qos_reliability_policy_t policy);

/// 将字符串转换为 rmw_qos_durability_policy_t 枚举值
/**
 * \param[in] str 输入字符串
 * \return 对应的 rmw_qos_durability_policy_t 枚举值，如果输入无效，则返回 RMW_QOS_POLICY_DURABILITY_UNKNOWN
 */
rmw_qos_durability_policy_t
rmw_qos_durability_policy_from_str(const char * str);

/// 将 rmw_qos_durability_policy_t 枚举值转换为字符串
/**
 * \param[in] policy 输入的 rmw_qos_durability_policy_t 枚举值
 * \return 对应的字符串表示，如果输入无效，则返回 NULL
 */
const char *
rmw_qos_durability_policy_to_str(rmw_qos_durability_policy_t policy);

#ifdef __cplusplus
}
#endif

#endif  // QOS_STRING_CONVERSIONS_H_
```

这个头文件包含了 `qos_string_conversions.c` 中实现的四个函数的声明。请注意，为了支持 C 和 C++ 混合编程，我们使用了 `extern "C"` 来确保正确的链接。

## You

我在最上面为你提供的程序代码(qos_string_conversions.c)在 ros2 中是如何应用，可以给出一些示例吗

## Trantor

在 ROS2 中，`qos_string_conversions.c` 文件主要用于将 QoS 策略的字符串表示与其枚举值进行相互转换。这在解析参数文件、命令行参数以及处理用户输入时非常有用。

以下是一些示例：

### 示例 1: 从参数文件中加载 QoS 设置

假设我们有一个名为 `params.yaml` 的参数文件，其中包含了 QoS 设置：

```yaml
my_node:
  ros__parameters:
    topic_qos_profile:
      reliability: reliable
      durability: transient_local
```

在节点中，我们可以使用 `rclcpp::NodeOptions` 和 `rclcpp::SyncParametersClient` 来获取这些参数，并将它们应用到 QoS 配置上：

```cpp
#include "rclcpp/rclcpp.hpp"
#include "rmw/qos_string_conversions.h"

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("my_node");

  // 获取 QoS 参数
  auto parameters_client = std::make_shared<rclcpp::SyncParametersClient>(node);
  auto qos_params = parameters_client->get_parameters({"topic_qos_profile"});

  // 创建 QoS 配置并设置参数
  rclcpp::QoS qos_profile(10); // 假设我们使用 10 作为历史深度
  for (const auto & param : qos_params) {
    if (param.get_name() == "topic_qos_profile.reliability") {
      auto policy_str = param.as_string();
      auto policy = rmw_qos_reliability_policy_from_str(policy_str.c_str());
      qos_profile.reliability(policy);
    } else if (param.get_name() == "topic_qos_profile.durability") {
      auto policy_str = param.as_string();
      auto policy = rmw_qos_durability_policy_from_str(policy_str.c_str());
      qos_profile.durability(policy);
    }
  }

  // 使用 QoS 配置创建发布者或订阅者
  auto publisher = node->create_publisher<std_msgs::msg::String>("my_topic", qos_profile);

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
```

### 示例 2: 从命令行参数中加载 QoS 设置

假设我们希望通过命令行参数设置 QoS 策略，例如：

```sh
ros2 run my_package my_node --ros-args -p topic_qos_profile.reliability:=reliable -p topic_qos_profile.durability:=transient_local
```

在这种情况下，我们可以使用与示例 1 相同的方法来获取和应用这些参数。

### 示例 3: 将 QoS 策略转换为字符串表示

在某些情况下，我们可能需要将 QoS 策略转换为字符串表示以便于显示或记录。例如，当我们想要打印当前 QoS 配置时：

```cpp
#include "rclcpp/rclcpp.hpp"
#include "rmw/qos_string_conversions.h"

void print_qos_profile(const rclcpp::QoS & qos)
{
  const char * reliability_str = rmw_qos_reliability_policy_to_str(qos.get_rmw_qos_profile().reliability);
  const char * durability_str = rmw_qos_durability_policy_to_str(qos.get_rmw_qos_profile().durability);

  RCLCPP_INFO(rclcpp::get_logger("my_node"), "QoS profile:");
  RCLCPP_INFO(rclcpp::get_logger("my_node"), "  Reliability: %s", reliability_str);
  RCLCPP_INFO(rclcpp::get_logger("my_node"), "  Durability: %s", durability_str);
}

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("my_node");

  // 创建 QoS 配置
  rclcpp::QoS qos_profile(10);
  qos_profile.reliability(RMW_QOS_POLICY_RELIABILITY_RELIABLE);
  qos_profile.durability(RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL);

  // 打印 QoS 配置
  print_qos_profile(qos_profile);

  rclcpp::shutdown();
  return 0;
}
```

这些示例展示了如何在 ROS2 中使用 `qos_string_conversions.c` 文件中的函数来处理 QoS 策略的字符串表示。
