import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, SetEnvironmentVariable, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    # 路径设置
    planning_path = get_package_share_directory("planning")

    # gazebo world 路径
    gazebo_world_path = os.path.join(planning_path, "world", "factory.world")

    # gazebo 模型路径（让 Gazebo 能找到 零件1 等自定义模型）
    gazebo_model_path = os.path.join(planning_path, "gazebo")

    # 设置 GAZEBO_MODEL_PATH 环境变量
    set_model_path = SetEnvironmentVariable(
        "GAZEBO_MODEL_PATH", gazebo_model_path
    )

    # 启动 Gazebo server + client
    start_gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            os.path.join(
                get_package_share_directory("gazebo_ros"),
                "launch", "gazebo.launch.py"
            )
        ]),
        launch_arguments={
            "world": gazebo_world_path,
        }.items(),
    )

    return LaunchDescription([
        set_model_path,
        start_gazebo,
    ])
