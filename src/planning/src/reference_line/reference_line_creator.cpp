/*
文件名: 参考线创建器

编译类型: 动态库
依赖: ROS2内部库:
        rclcpp
        nav_msgs
        geometry_msgs
    外部库:
        base_msgs
        config_reader
        mathlibs

*/
#include "reference_line_creator.h"

namespace Planning
{
	ReferenceLineCreator::ReferenceLineCreator()
	{
		RCLCPP_INFO(rclcpp::get_logger("reference_line"), "ReferenceLineCreator created");
	}
}//namespace Planning
