/*
文件名: 局部路径平滑器

编译类型: 动态库
依赖: ROS2内部库:
      rclcpp
    外部库:
      base_msgs
      config_reader

*/

#include "local_path_smoother.h"

namespace Planning
{
    LocalPathSmoother::LocalPathSmoother() // 局部路径平滑器
    {
        RCLCPP_INFO(rclcpp::get_logger("local_path"), "local_path_smoother created");
    }

} // namespace Planning
