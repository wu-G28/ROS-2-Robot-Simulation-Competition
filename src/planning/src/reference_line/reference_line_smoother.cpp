/*
文件名: 参考线平滑器

编译类型: 动态库
依赖: ROS2内部库:
        rclcpp
        geometry_msgs
    外部库:
        base_msgs
        config_reader
        Eigen
        OsqpEigen
*/

#include "reference_line_smoother.h"

namespace Planning
{
	ReferenceLineSmoother::ReferenceLineSmoother()
	{
		RCLCPP_INFO(rclcpp::get_logger("reference_line"), "ReferenceLineSmoother created");
	}
}//namespace Planning
