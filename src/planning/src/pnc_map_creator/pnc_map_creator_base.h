#ifndef PNC_MAP_CREATOR_BASE_H
#define PNC_MAP_CREATOR_BASE_H

#include "rclcpp/rclcpp.hpp"
#include "config_reader.h"
#include <cmath>

#include "base_msgs/msg/pnc_map.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "visualization_msgs/msg/marker.hpp"
#include "visualization_msgs/msg/marker_array.hpp"

namespace Planning
{
	using base_msgs::msg::PNCMap;
	using geometry_msgs::msg::Point;
	using visualization_msgs::msg::Marker;
	using visualization_msgs::msg::MarkerArray;

	class PNCMapCreatorBase // pnc_map创建器基类
	{
	public:
		virtual PNCMap create_pnc_map() = 0; //生成地图
		virtual ~PNCMapCreatorBase() {};

	private: //保护类型，子类可以继承并访问但对象不行
		std::unique_ptr<ConfigReader> pnc_map_config_ ; //配置
		PNCMap pnc_map_; //地图
		MarkerArray pnc_map_markerarry_; //rviz用的地图

		Point p_mid, pl_, pr_; //生成地位坐标点
		double len_step_ = 0.0; //步长
		double theta_current_ = 0.0;  //当前角度
		double theta_step_ = 0.0; //步进角度
	};
}//namespace Planning
#endif //PNC_MAP_CREATOR_BASE_H
