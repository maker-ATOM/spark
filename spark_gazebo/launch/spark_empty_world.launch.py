#!/usr/bin/env python3

"""
Spawns robot in an empty simulation env

Nodes launched:
Robot State Publisher
gazebo server and client
Spawn entity

"""
import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
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

    # ---------------------- Gazebo ---------------------- #

    gazebo_pkg = get_package_share_directory('gazebo_ros')
    spark_gazebo_pkg = get_package_share_directory('spark_gazebo')

    # Gazebo

    # world = os.path.join(
    #     spark_gazebo_pkg,
    #     'worlds',
    #     'empty.world'
    # )

    # gazebo_cmd = IncludeLaunchDescription(
    #     PythonLaunchDescriptionSource([
    #         os.path.join(get_package_share_directory('gazebo_ros'), 'launch', 'gazebo.launch.py')
    #     ]),
    # )

    gzserver_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(gazebo_pkg, 'launch', 'gzserver.launch.py')
        ),
        # launch_arguments={'world': world}.items()
    )

    gzclient_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(gazebo_pkg, 'launch', 'gzclient.launch.py')
        )
    )

    # ---------------------- Spawn Model ---------------------- #

    x_pose = LaunchConfiguration('x_pose', default=0.0)
    y_pose = LaunchConfiguration('y_pose', default=0.0)



    spawn_robot = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=[
            '-entity', 'spark',
            '-topic', 'robot_description',
            '-x', x_pose,
            '-y', y_pose,
            '-z', '0.01'
        ],
        output='screen',
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

    # Gazebo
    # ld.add_action(gazebo_cmd)
    ld.add_action(gzserver_cmd)
    ld.add_action(gzclient_cmd)
    ld.add_action(spawn_robot)


    # Rviz
    ld.add_action(rviz2_launcher)

    return ld
