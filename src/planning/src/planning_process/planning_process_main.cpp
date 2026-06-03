/*
文件名: 规划总流程入口

编译类型: 节点(planning_process)

流程: 为节点planning_process提供入口函数main()

*/
#include "planning_process.h"

int main(int argc, char *argv[])
{
	rclcpp::init(argc, argv);
	RCLCPP_INFO(rclcpp::get_logger("planning_process_main"), "planning start");
}
