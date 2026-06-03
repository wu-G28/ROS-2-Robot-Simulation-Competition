/*
文件名: 规划总流程

编译类型: 节点(planning_process)

依赖: ROS2内部库:
      rclcpp
      nav_msgs
      tf2
      tf2_ros
    外部库:
      base_msgs
      config_reader
      vehicle_info
      decision_center
      reference_line
      local_planner

*/

#include "planning_process.h"

namespace Planning
{
  PlanningProcess::PlanningProcess() : Node("planning_process")//规划总流程
  {
      RCLCPP_INFO(this->get_logger(), "PlanningProcess created");

      //读取配置文件
      process_config_ = std::make_unique<ConfigReader>();
      process_config_->read_planning_process_config();
      auto obs_dis = process_config_->process().obs_dis_;
  }

  bool PlanningProcess::process() //总流程
  {
    return true;
  }
}//namespace Planning
