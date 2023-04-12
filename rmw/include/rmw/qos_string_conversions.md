> [!NOTE]
> 应该就是这里，在 rclcpp 层的 qos_parameters.hpp 中直接跳转到了这里，前者是创建 pub/sub 时候会调用的函数
>
> ---
>
> 考虑增加 qos，应该增加 ownership 等

##

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

这些函数之间的关系是相互独立的，它们可以单独使用。主要目的是在处理 QoS 策略时，提供一种方便的方式来**在枚举值和字符串之间进行转换**。这样可以方便地在配置文件、命令行参数或其他需要以字符串形式表示 QoS 策略的场景中使用这些策略。

例如，当用户需要设置一个发布者的 QoS 策略时，他们可以在配置文件中使用字符串（如"reliable"）来表示所需的策略。然后，通过调用`rmw_qos_reliability_policy_from_str`函数，将字符串转换为对应的枚举值，以便在创建发布者时使用。
