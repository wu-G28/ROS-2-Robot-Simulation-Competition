#ifndef PLANNING_ROCESS_H_
#define PLANNING_ROCESS_H_

#include "rclcpp/rclcpp.hpp"
#include "base_msgs/msg/pnc_map.hpp"
#include "base_msgs/srv/global_path_server.hpp"
#include "base_msgs/srv/pnc_map_service.hpp"
#include "base_msgs/msg/local_trajectory.hpp"
#include "nav_msgs/msg/path.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "tf2_ros/static_transform_broadcaster.h"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "tf2/LinearMath/Quaternion.hpp"

#include "config_reader.h"
#include "main_car_info.h"
#include "obs_info.h"
#include "reference_line_creator.h"
#include "decision_center.h"
#include "local_path_planner.h"
#include "local_speeds_planner.h"
#include "local_trajectory_combiner.h"

namespace Planning
{
	class PlanningProcess :public rclcpp::Node //规划总流程
	{
	public:
		PlanningProcess();

	private:
	};
} //namespace Planning

#endif PLANNING_ROCESS_H_
