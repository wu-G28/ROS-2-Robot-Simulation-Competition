"""
启动 Gazebo 厂区仿真环境 + 机器人模型

用法:
    ros2 launch planning gazebo_world.launch.py
    ros2 launch planning gazebo_world.launch.py rviz:=false   # 仅 Gazebo
    ros2 launch planning gazebo_world.launch.py world:=factory.world  # 指定世界文件
"""

import os
from pathlib import Path

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    IncludeLaunchDescription,
    SetEnvironmentVariable,
)
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # ==================== 路径 ====================
    pkg_planning = get_package_share_directory("planning")
    pkg_gazebo_ros = get_package_share_directory("gazebo_ros")

    # 模型路径: 让 Gazebo 能搜索到自定义模型
    models_dir = os.path.join(pkg_planning, "models")
    gazebo_models_path = os.environ.get("GAZEBO_MODEL_PATH", "")
    if gazebo_models_path:
        new_model_path = f"{models_dir}:{gazebo_models_path}"
    else:
        new_model_path = models_dir

    # ==================== Launch 参数 ====================
    world_arg = DeclareLaunchArgument(
        "world",
        default_value="factory.world",
        description="Gazebo 世界文件名 (位于 worlds/ 目录下)",
    )
    rviz_arg = DeclareLaunchArgument(
        "rviz",
        default_value="true",
        description="是否启动 RViz2",
    )
    use_sim_time_arg = DeclareLaunchArgument(
        "use_sim_time",
        default_value="true",
        description="使用 Gazebo 仿真时钟 /clock",
    )

    # ==================== 机器人 URDF 描述 ====================
    xacro_file = os.path.join(pkg_planning, "urdf", "main_car", "car.xacro")
    robot_description = ParameterValue(
        Command([FindExecutable(name="xacro"), " ", xacro_file]),
        value_type=str,
    )

    # ==================== Gazebo 启动 ====================
    world_path = PathJoinSubstitution([pkg_planning, "worlds", LaunchConfiguration("world")])

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [pkg_gazebo_ros, "/launch", "/gazebo.launch.py"]
        ),
        launch_arguments={
            "world": world_path,
            "verbose": "false",
            "pause": "false",
        }.items(),
    )

    # ==================== Spawn 机器人 ====================
    spawn_robot = Node(
        package="gazebo_ros",
        executable="spawn_entity.py",
        arguments=[
            "-entity", "main_car",
            "-topic", "robot_description",
            "-x", "7.0",
            "-y", "2.0",
            "-z", "0.1",
            "-Y", "1.5708",
        ],
        output="screen",
    )

    # ==================== Robot State Publisher ====================
    robot_state_pub = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[
            {
                "robot_description": robot_description,
                "use_sim_time": LaunchConfiguration("use_sim_time"),
            }
        ],
        output="screen",
    )

    # ==================== Joint State Publisher ====================
    joint_state_pub = Node(
        package="joint_state_publisher",
        executable="joint_state_publisher",
        parameters=[{"use_sim_time": LaunchConfiguration("use_sim_time")}],
        output="screen",
    )

    # ==================== RViz2 ====================
    rviz_config = os.path.join(pkg_planning, "rviz", "photo_car.rviz")
    rviz = Node(
        package="rviz2",
        executable="rviz2",
        arguments=["-d", rviz_config],
        condition=IfCondition(LaunchConfiguration("rviz")),
        parameters=[{"use_sim_time": LaunchConfiguration("use_sim_time")}],
        output="screen",
    )

    return LaunchDescription([
        # 环境变量
        SetEnvironmentVariable("GAZEBO_MODEL_PATH", new_model_path),
        # 参数
        world_arg,
        rviz_arg,
        use_sim_time_arg,
        # 启动
        gazebo,
        robot_state_pub,
        joint_state_pub,
        spawn_robot,
        rviz,
    ])
