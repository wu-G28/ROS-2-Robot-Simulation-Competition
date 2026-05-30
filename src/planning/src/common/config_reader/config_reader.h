#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include "rclcpp/rclcpp.hpp"
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace planning
{
    class ConfigReader  // 配置文件读取器
    {
    public:
        ConfigReader();

    private:
    };
} // namespace planning

#endif // CONFIG_READER_H_
