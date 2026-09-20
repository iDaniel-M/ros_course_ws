from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():

    return LaunchDescription([

        Node(
            package='turtlesim',
            executable='turtlesim_node',
            name='turtlesim'
        ),

        Node(
            package='turtle_controller',
            executable='mode_manager',
            name='mode_manager'
        ),

        Node(
            package='turtle_controller',
            executable='circle_node',
            name='circle_node'
        ),

        Node(
            package='turtle_controller',
            executable='trajectory_node',
            name='trajectory_node'
        ),
    ])
