/*
文件名: 多项式曲线计算
编译类型: 动态库
依赖: ROS2内部库:
        rclcpp
    外部库:
        Eigen
*/

#include "decision_center.h"

namespace Planning
{
	DecisionCenter::DecisionCenter()
	{
		RCLCPP_INFO(rclcpp::get_logger("decision_center"), "decision_center created");
	}
}//namespace planning
