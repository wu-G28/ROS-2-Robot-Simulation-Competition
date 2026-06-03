/*
文件名: pnc地图服务器

编译类型: 节点（pnc_map_server_node）
依赖: ROS2内部库:
        rclcpp
    外部库:
        base_msgs
        config_reader
        pnc_map_creator

*/

#include "pnc_map_server.h"

namespace Planning
{
	PNCMapServer::PNCMapServer() : Node("pnc_map_server") //全局路径服务器
	{
		RCLCPP_INFO(this->get_logger(), "PNCMapServer created");
	}
}//namespace Planning

int main(int argc, char *argv[])
{
	rclcpp::init(argc, argv);
	auto node = std::make_shared<Planning::PNCMapServer>();
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}
