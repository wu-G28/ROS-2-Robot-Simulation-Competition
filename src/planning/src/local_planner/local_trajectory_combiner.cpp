/*
文件名: 轨迹合成器

编译类型: 动态库
依赖: ROS2内部库:
        rclcpp
    外部库:
        base_msgs
*/
#include "local_trajectory_combiner.h"

namespace Planning
{
	LocalTrajectoryCombiner::LocalTrajectoryCombiner() //轨迹合成器
	{
			RCLCPP_INFO(rclcpp::get_logger("local_trajectory"), "LocalTrajectoryCombiner created");
	}
}//namespace Planning
