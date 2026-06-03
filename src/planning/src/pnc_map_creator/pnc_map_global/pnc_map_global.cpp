/*
文件名: 直道地图创建器

编译类型: 动态库
依赖: ROS2内部库:
        rclcpp
        geometry_msgs
        visualization_msgs
    外部库:
        base_msgs
        config_reader

*/

#include "pnc_map_global.h"

namespace Planning
{
	PNCMapGlobal::PNCMapGlobal() //全局地图
	{
		RCLCPP_INFO(rclcpp::get_logger("pnc_map"), "PNCMapGlobal created");
	}
}//namespace Plannig
