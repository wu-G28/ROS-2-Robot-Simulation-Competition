/*
文件名: 普通全局路径规划器

编译类型: 动态库
依赖: ROS2内部库:
        rclcpp
        geometry_msgs
        nav_msgs
    外部库:
        base_msgs
        config_reader
*/

#include "global_planner_normal.h"

namespace Planning
{
	GlobalPlannerNormal::GlobalPlannerNormal() //普通全局路径规划器
	{
		RCLCPP_INFO(rclcpp::get_logger("global_path"), "global_planner_normal created");
	}
}//namespace Planning

