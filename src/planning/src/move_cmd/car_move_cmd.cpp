/*
文件名: 主车运动指令

编译类型: 节点(move_cmd_node)
依赖: ROS2内部库:
      rclcpp
      tf2_ros
      geometry_msgs
    外部库:
      base_msgs
      config_reader
      vehicle_info
*/

#include "car_move_cmd.h"

namespace Planning
{
	CarMoveCmd::CarMoveCmd() : Node("car_move_cmd_node") //主车运动指令
	{

	}
}//namespace Planning

int main(int argc, char *argv[])
{
	rclcpp::init(argc, argv);
	auto node = std::make_shared<Planning::CarMoveCmd>();
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}
