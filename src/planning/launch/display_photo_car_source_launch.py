from pathlib import Path

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import Command, FindExecutable, LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    planning_dir = Path(__file__).resolve().parents[1]
    xacro_file = planning_dir / 'urdf' / 'main_car' / 'car.xacro'
    rviz_config = planning_dir / 'rviz' / 'photo_car.rviz'

    robot_description = ParameterValue(
        Command([FindExecutable(name='xacro'), ' ', str(xacro_file)]),
        value_type=str,
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'rviz',
            default_value='true',
            description='Start RViz2 with the robot model display.',
        ),
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            parameters=[{'robot_description': robot_description}],
            output='screen',
        ),
        Node(
            package='joint_state_publisher',
            executable='joint_state_publisher',
            output='screen',
        ),
        Node(
            package='rviz2',
            executable='rviz2',
            arguments=['-d', str(rviz_config)],
            condition=IfCondition(LaunchConfiguration('rviz')),
            output='screen',
        ),
    ])
