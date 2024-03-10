#!/usr/bin/env python3

"""
Visualize tf and robot model ( Joint state publisher is required to publish the joint data which are not fixed )

Nodes launched:
Robot State Publisher
Joint State Publisher
Rviz
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

import xacro


def generate_launch_description():

    urdf_file = 'spark_description.xacro'
    spark_description_pkg = get_package_share_directory('spark_description')
    use_sim_time = LaunchConfiguration('use_sim_time', default='true')

    print("Fetching URDF ==>")
    print('urdf_file_name : {}'.format(urdf_file))

    xacro_file = os.path.join(spark_description_pkg, 'urdf', urdf_file)
    robot_description_config = xacro.process_file(xacro_file)
    robot_urdf = robot_description_config.toxml()

    # ---------------------- State publisher ---------------------- #

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[{
                'use_sim_time': use_sim_time,
                'robot_description': robot_urdf
        }],
    )

    joint_state_publisher = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        name='joint_state_publisher_gui'
    )

    # ---------------------- Rviz ---------------------- #

    rviz_config = os.path.join(spark_description_pkg,
                               'rviz', 'display.rviz')

    rviz2_launcher = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', rviz_config]
    )

    ld = LaunchDescription()

    # State Publisher
    ld.add_action(robot_state_publisher)
    ld.add_action(joint_state_publisher)

    # Rviz
    ld.add_action(rviz2_launcher)

    return ld