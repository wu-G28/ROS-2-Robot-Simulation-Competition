/*
文件名: 主车信息

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

#include "main_car_info.h"

namespace Planning
{
	MainCarInfo::MainCarInfo()
	{
		RCLCPP_INFO(rclcpp::get_logger("vehicle"), "MainCarInfo created");
	}
}//namespace Planning
