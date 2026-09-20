cmake_minimum_required(VERSION 3.8)
project(turtle_controller)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)

add_executable(mode_manager src/mode_manager.cpp)
ament_target_dependencies(mode_manager rclcpp std_msgs)

add_executable(circle_node src/circle_node.cpp)
ament_target_dependencies(circle_node rclcpp std_msgs geometry_msgs)

add_executable(trajectory_node src/trajectory_node.cpp)
ament_target_dependencies(trajectory_node rclcpp std_msgs geometry_msgs turtlesim)

install(TARGETS
  mode_manager
  circle_node
  trajectory_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()