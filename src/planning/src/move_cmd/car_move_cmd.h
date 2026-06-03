#ifndef CAR_MOVE_CMD_H
#define CAR_MOVE_CMD_H

#include "rclcpp/rclcpp.hpp"
#include "base_msgs/msg/local_trajectory.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include <cmath>
#include "config_reader.h"
#include "main_car_info.h"

namespace Planning
{
	class CarMoveCmd : public rclcpp::Node
	{
	public:
		CarMoveCmd();

	private:
	};
}//namespace Planning

#endif CAR_MOVE_CMD_H
