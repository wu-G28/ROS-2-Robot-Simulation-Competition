cmake_minimum_required(VERSION 3.22.1)
project(planning)

#设置c和c++标准
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 设置预编译指令, 生成compile_commands.json
# 然后让c_cpp_properties.json读取这个文件, 防止预编译找不到cmake中定义的宏而报错
# c_cpp_properties.json中加上"compileCommands": "${workspaceFolder}/build/compile_commands.json"来读取这个文件
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置包名, 让子目录节点安装时使用
set(PLANNING_PACKAGE_NAME ${PROJECT_NAME})

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(planning_node src/planning_node.cpp)
target_include_directories(planning_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(planning_node PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
ament_target_dependencies(
  planning_node
  "rclcpp"
)

install(TARGETS planning_node
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
