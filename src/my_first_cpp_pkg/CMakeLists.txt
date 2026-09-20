cmake_minimum_required(VERSION 3.8)
project(my_first_cpp_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

include_directories(include)

# Publicador
add_executable(hw_talker src/publisher_member_function.cpp)
ament_target_dependencies(hw_talker rclcpp std_msgs)

# Suscriptor
add_executable(hw_listener src/my_first_cpp_subs.cpp)
ament_target_dependencies(hw_listener rclcpp std_msgs)

# Instalación
install(TARGETS
  hw_talker
  hw_listener
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch scripts
  DESTINATION share/${PROJECT_NAME}
  OPTIONAL
)

ament_package()
