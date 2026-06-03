/*
文件名: 局部路径规划器

编译类型: 动态库
依赖: ROS2内部库:
      rclcpp
      geometry_msgs
      nav_msgs
      tf2
    外部库:
      base_msgs
      config_reader
      mathlibs
      vehicle_info
      decision_center

*/

#include "local_path_planner.h"

namespace Planning
{
	LocalPathPlanner::LocalPathPlanner()//局部路径规划器
	{
		RCLCPP_INFO(rclcpp::get_logger("local_path"), "LocalPathPlanner created");
	}
}// namespace Planning
