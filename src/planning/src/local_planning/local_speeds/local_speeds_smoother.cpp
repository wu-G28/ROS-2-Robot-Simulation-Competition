/*
文件名: 速度平滑器

编译类型: 动态库
依赖: ROS2内部库:
      rclcpp
    外部库:
      base_msgs
      config_reader

*/

#include "local_speeds_smoother.h"

namespace Planning
{
    LocalSpeedsSmoother::LocalSpeedsSmoother() // 速度平滑器
    {
        RCLCPP_INFO(rclcpp::get_logger("local_speed"), "local_speeds_smoother created");
    }

} // namespace Planning
