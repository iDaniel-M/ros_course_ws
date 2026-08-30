cmake_minimum_required(VERSION 3.8)
project(my_first_cpp_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(example_interfaces REQUIRED)

include_directories(include)

# Nodos
add_executable(hw_talker src/publisher_member_function.cpp)
ament_target_dependencies(hw_talker rclcpp std_msgs)

add_executable(hw_listener src/my_first_cpp_subs.cpp)
ament_target_dependencies(hw_listener rclcpp std_msgs)

# Acciones C++
add_executable(first_action_server src/first_action_server.cpp)
ament_target_dependencies(first_action_server rclcpp rclcpp_action example_interfaces)

add_executable(first_action_client src/first_action_client.cpp)
ament_target_dependencies(first_action_client rclcpp rclcpp_action example_interfaces)

install(TARGETS
  hw_talker
  hw_listener
  first_action_server
  first_action_client
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
  OPTIONAL
)

ament_package()
