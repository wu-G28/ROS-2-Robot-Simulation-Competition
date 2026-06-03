/*
文件名: 速度规划器

编译类型: 动态库
依赖: ROS2内部库:
        rclcpp
      外部库:
        base_msgs
        config_reader
        mathlibs
        decision_center

*/

#include "local_speeds_planner.h"

namespace Planning
{
    LocalSpeedsPlanner::LocalSpeedsPlanner() // 速度规划器
    {
        RCLCPP_INFO(rclcpp::get_logger("local_speed"), "local_speeds_planner created");
    }

} // namespace Planning
