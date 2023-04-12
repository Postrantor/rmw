// Copyright 2014 Open Source Robotics Foundation, Inc.
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

#include "rmw/allocators.h"

#include <rcutils/allocator.h>
#include <stdlib.h>
#include <string.h>

#include "rmw/types.h"

/**
 * @brief 分配指定大小的内存空间 (Allocate memory of specified size)
 *
 * @param[in] size 要分配的内存大小，以字节为单位 (The size of memory to allocate, in bytes)
 * @return 返回分配的内存指针，若分配失败则返回NULL (Returns the allocated memory pointer, or NULL
 * if allocation fails)
 */
void *rmw_allocate(size_t size) {
  // 可以用通用内存分配器替换 (Could be overridden with a general purpose allocator)
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  // 使用分配器为指定大小的内存分配空间 (Use the allocator to allocate memory of specified size)
  void *ptr = allocator.allocate(size, allocator.state);

  // 如果内存分配成功，则将分配的内存初始化为零 (If memory allocation is successful, initialize the
  // allocated memory to zero)
  if (ptr) {
    memset(ptr, 0, size);
  }

  // 返回分配的内存指针 (Return the allocated memory pointer)
  return ptr;
}

/**
 * @brief 释放已分配的内存空间 (Free the allocated memory space)
 *
 * @param[in] pointer 要释放的内存指针 (The memory pointer to free)
 */
void rmw_free(void *pointer) {
  // 应该与 rmw_allocate 中的对应重写相匹配 (Should have a corresponding override with rmw_allocate)
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  // 使用分配器释放指定的内存空间 (Use the allocator to free the specified memory space)
  allocator.deallocate(pointer, allocator.state);
}

/**
 * @brief 分配一个新的 rmw_node_t 结构体实例 (Allocate a new rmw_node_t structure instance)
 *
 * @return 返回一个指向新分配的 rmw_node_t 结构体实例的指针 (Returns a pointer to the newly
 * allocated rmw_node_t structure instance)
 */
rmw_node_t *rmw_node_allocate() {
  // 可以用自定义（可能是静态的）节点结构分配器覆盖此函数 (Could be overridden with custom (maybe
  // static) node struct allocator)
  return (rmw_node_t *)rmw_allocate(sizeof(rmw_node_t));
}

/**
 * @brief 释放一个 rmw_node_t 结构体实例 (Free an rmw_node_t structure instance)
 *
 * @param[in] node 要释放的 rmw_node_t 结构体实例的指针 (Pointer to the rmw_node_t structure
 * instance to be freed)
 */
void rmw_node_free(rmw_node_t *node) {
  // 应与 rmw_node_allocate 函数具有匹配的重写 (Should have matching overide with rmw_node_allocate)
  rmw_free(node);
}

/**
 * @brief 分配一个新的 rmw_publisher_t 结构体实例 (Allocate a new rmw_publisher_t structure
 * instance)
 *
 * @return 返回一个指向新分配的 rmw_publisher_t 结构体实例的指针 (Returns a pointer to the newly
 * allocated rmw_publisher_t structure instance)
 */
rmw_publisher_t *rmw_publisher_allocate() {
  // 可以用自定义（可能是静态的）发布者结构分配器覆盖此函数 (Could be overridden with custom (maybe
  // static) publisher struct allocator)
  return (rmw_publisher_t *)rmw_allocate(sizeof(rmw_publisher_t));
}

/**
 * @brief 释放一个 rmw_publisher_t 结构体实例 (Free an rmw_publisher_t structure instance)
 *
 * @param[in] publisher 要释放的 rmw_publisher_t 结构体实例的指针 (Pointer to the rmw_publisher_t
 * structure instance to be freed)
 */
void rmw_publisher_free(rmw_publisher_t *publisher) {
  // 应与 rmw_publisher_allocate 函数具有匹配的重写 (Should have matching overide with
  // rmw_publisher_allocate)
  rmw_free(publisher);
}

/**
 * @brief 分配一个新的 rmw_subscription_t 结构体实例 (Allocate a new rmw_subscription_t structure
 * instance)
 *
 * @return 返回一个指向新分配的 rmw_subscription_t 结构体实例的指针 (Returns a pointer to the newly
 * allocated rmw_subscription_t structure instance)
 */
rmw_subscription_t *rmw_subscription_allocate() {
  // 可以用自定义（可能是静态的）订阅结构分配器覆盖此函数 (Could be overridden with custom (maybe
  // static) subscription struct allocator)
  return (rmw_subscription_t *)rmw_allocate(sizeof(rmw_subscription_t));
}

/**
 * @brief 释放一个 rmw_subscription_t 结构体实例 (Free an rmw_subscription_t structure instance)
 *
 * @param[in] subscription 要释放的 rmw_subscription_t 结构体实例的指针 (Pointer to the
 * rmw_subscription_t structure instance to be freed)
 */
void rmw_subscription_free(rmw_subscription_t *subscription) {
  // 应与 rmw_subscription_allocate 函数具有匹配的重写 (Should have matching overide with
  // rmw_subscription_allocate)
  rmw_free(subscription);
}

/**
 * @brief 为 rmw_guard_condition_t 结构体分配内存
 * @brief Allocate memory for rmw_guard_condition_t structure
 *
 * @return 返回一个指向 rmw_guard_condition_t 结构体的指针
 * @return A pointer to the rmw_guard_condition_t structure
 */
rmw_guard_condition_t *rmw_guard_condition_allocate() {
  // 可以用自定义（可能是静态的）guard_condition结构体分配器覆盖
  // Could be overridden with custom (maybe static) guard_condition struct allocator
  return (rmw_guard_condition_t *)rmw_allocate(sizeof(rmw_guard_condition_t));
}

/**
 * @brief 释放 rmw_guard_condition_t 结构体的内存
 * @brief Free the memory of rmw_guard_condition_t structure
 *
 * @param guard_condition 指向需要释放的 rmw_guard_condition_t 结构体的指针
 * @param guard_condition Pointer to the rmw_guard_condition_t structure to be freed
 */
void rmw_guard_condition_free(rmw_guard_condition_t *guard_condition) {
  // 应与 rmw_guard_condition_allocate 的覆盖相匹配
  // Should have matching override with rmw_guard_condition_allocate
  rmw_free(guard_condition);
}

/**
 * @brief 为 rmw_client_t 结构体分配内存
 * @brief Allocate memory for rmw_client_t structure
 *
 * @return 返回一个指向 rmw_client_t 结构体的指针
 * @return A pointer to the rmw_client_t structure
 */
rmw_client_t *rmw_client_allocate() {
  // 可以用自定义（可能是静态的）client结构体分配器覆盖
  // Could be overridden with custom (maybe static) client struct allocator
  return (rmw_client_t *)rmw_allocate(sizeof(rmw_client_t));
}

/**
 * @brief 释放 rmw_client_t 结构体的内存
 * @brief Free the memory of rmw_client_t structure
 *
 * @param client 指向需要释放的 rmw_client_t 结构体的指针
 * @param client Pointer to the rmw_client_t structure to be freed
 */
void rmw_client_free(rmw_client_t *client) {
  // 应与 rmw_client_allocate 的覆盖相匹配
  // Should have matching override with rmw_client_allocate
  rmw_free(client);
}

/**
 * @brief 为 rmw_service_t 结构体分配内存
 * @brief Allocate memory for rmw_service_t structure
 *
 * @return 返回一个指向 rmw_service_t 结构体的指针
 * @return A pointer to the rmw_service_t structure
 */
rmw_service_t *rmw_service_allocate() {
  // 可以用自定义（可能是静态的）client结构体分配器覆盖
  // Could be overridden with custom (maybe static) client struct allocator
  return (rmw_service_t *)rmw_allocate(sizeof(rmw_service_t));
}

/**
 * @brief 释放 rmw_service_t 结构体的内存
 * @brief Free the memory of rmw_service_t structure
 *
 * @param service 指向需要释放的 rmw_service_t 结构体的指针
 * @param service Pointer to the rmw_service_t structure to be freed
 */
void rmw_service_free(rmw_service_t *service) {
  // 应与 rmw_service_allocate 的覆盖相匹配
  // Should have matching override with rmw_service_allocate
  rmw_free(service);
}

/**
 * @brief 为 rmw_wait_set_t 结构体分配内存
 * @brief Allocate memory for rmw_wait_set_t structure
 *
 * @return 返回一个指向 rmw_wait_set_t 结构体的指针
 * @return A pointer to the rmw_wait_set_t structure
 */
rmw_wait_set_t *rmw_wait_set_allocate() {
  // 可以用自定义（可能是静态的）client结构体分配器覆盖
  // Could be overridden with custom (maybe static) client struct allocator
  return (rmw_wait_set_t *)rmw_allocate(sizeof(rmw_wait_set_t));
}
/**
 * @brief 释放给定的等待集 (Free the given wait set)
 *
 * @param[in] wait_set 要释放的等待集指针 (Pointer to the wait set to be freed)
 */
void rmw_wait_set_free(rmw_wait_set_t *wait_set) {
  // 应与 rmw_wait_set_allocate 具有匹配的重写 (Should have matching override with
  // rmw_wait_set_allocate) 调用 rmw_free 函数来释放传入的 wait_set 指针所指向的内存空间 (Call
  // rmw_free function to free the memory space pointed by the passed wait_set pointer)
  rmw_free(wait_set);
}
