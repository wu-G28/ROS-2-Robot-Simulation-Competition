/*
文件名: 障碍物信息

编译类型: 动态库
依赖: ROS2内部库:
        rclcpp
        nav_msgs
        geometry_msgs
        tf2
    外部库:
        base_msgs
        config_reader
        mathlibs

*/

#include "obs_info.h"

namespace Planning
{
	ObsInfo::ObsInfo()
	{
		RCLCPP_INFO(rclcpp::get_logger("vehicle"), "obs_info created");
	}
}//namespace Planning
