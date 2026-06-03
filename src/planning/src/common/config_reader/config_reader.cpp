/*
文件名: 配置文件读取器

编译类型: 动态库
依赖: ROS2内部库:
        rclcpp
        ament_index_cpp
    外部库:
        yaml-cpp
*/

#include "config_reader.h"

namespace Planning
{
    ConfigReader::ConfigReader() // 配置文件读取器
    {
        // 获取workspace/install/planning/share/planning/目录路径
        std::string planning_share_directory = ament_index_cpp::get_package_share_directory("planning");

        // 然后获取配置文件
        planning_config = YAML::LoadFile(planning_share_directory + "/config/planning_dynamic_obs_config.yaml");  //场景
    }

    /************vehicle*************/

    /************pnc_map*************/

    /************global_path*************/

    /************reference_line*************/

    /************local_path*************/

    /************local_speeds*************/

    /************decision*************/

    /************planning_process*************/

    /************move_cmd*************/
} // namespace planning
