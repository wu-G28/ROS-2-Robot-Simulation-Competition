/*
文件名: 障碍物运动指令

编译类型: 节点(move_cmd_node)
依赖: ROS2内部库:
      rclcpp
      tf2
      tf2_ros
      geometry_msgs
    外部库:
      config_reader
      vehical_info

*/

#include "obs_move_cmd.h"

namespace Planning
{
	ObsMoveCmd::ObsMoveCmd() : Node("obs_move_cmd")
	{
		RCLCPP_INFO(this->get_logger(), "obs_move_cmd created");
	}
}//namespace Planning
