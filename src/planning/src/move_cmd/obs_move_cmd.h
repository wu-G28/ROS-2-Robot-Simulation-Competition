#ifndef OBS_MOVE_CMD_H
#define OBS_MOVE_CMD_H

#include "rclcpp/rclcpp.hpp"
#include "config_reader.h"
#include "obs_info.h"

namespace Planning
{
	class ObsMoveCmd : public rclcpp::Node
	{
	public:
		ObsMoveCmd();

	private:
	};
}//namespace Planning
#endif // OBS_MOVE_CMD_H
