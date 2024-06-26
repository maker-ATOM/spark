# SPARK

[REPORT](REPORT.md), [TODO](TODO.md)

## Launch Sequence

**MAP GENERATION**

```py
roslaunch spark_gazebo spark_house.launch
# Robot State Publishers for the robot
# Spawns robot in gazebo
# LaserScan Merger
```

```py
roslaunch spark_slam gmapping.launch
# Gmapping

OR 

roslaunch spark_slam slamtoolbox_mapping.launch
# SlamToolBox

OR 

roslaunch spark_slam cartographer.launch
# Cartographer
```

```py
roslaunch spark_slam map_saver.launch map_name:=my_map
# Sae the Map
```

**NAVIGATION ON SAVED MAP**

```py
roslaunch spark_gazebo spark_house.launch
# Robot State Publishers for the robot
# Spawns robot in gazebo
# LaserScan Merger
```

```py
roslaunch spark_navigation spark_navigation.launch
# Map Server
# AMCL
# MoveBase
```

**SLAM**

```py
roslaunch spark_gazebo spark_house.launch
# Robot State Publishers for the robot
# Spawns robot in gazebo
# LaserScan Merger
```

```py
roslaunch spark_slam spark_slam.launch
# SLAM ToolBox Mapper
# MoveBase
```
> [!NOTE]
> For arena to load in gazebo copy `arena` directory within `spark_gazebo` package to `~/.gazebo/models `

**DYNAMIC PROHIBITED SECTOR**

```py
# Launch Gazebo and Navigation

roslaunch spark_navigation prohibitor.launch
# Verify path to map and .yaml file
```

**CHECKPOINT**

Save checkpoints.

```py
# Added in gazebo.launch (Bringup in real robot)
rosrun spark_slam checkpoint_saver.py /path_to/checkpoints.yaml
```

**SINGLE GOAL**

```py
# Launch Gazebo and Navigation

roslaunch spark_navigation callback_goal
```

## Packages

<br>Description
<br>└── Launch
<br>&emsp;&emsp;    └── description (Visualize tf and robot model)

<br>Laser Merger
<br>├── Launch
<br>│&emsp;&emsp;└── laser_merger (Merge LaserScan of both front and rear and produce a single LaserScan)
<br>└── Node
<br>&emsp;&emsp;    └── ira_laser_merger

<br>Gazebo
<br>├── Launch
<br>│&emsp;&emsp; ├── empty_world (Empty simulation env with robot)
<br>│&emsp;&emsp; ├── turtlebot3_house (Indoor simulation env with robot)
<br>│&emsp;&emsp; └── arena (Arena for Robocon 24)
<br>└── Node
<br>&emsp;&emsp;    └── battery (Simulation of linear battery discharge)

Control
<br>└── Node
<br>&emsp;&emsp;     └── spark_teleop.py (Teleop)

Slam
<br>└── Launch
<br>&emsp;&emsp;     ├── map_saver (save map in the navigation package)
<br>&emsp;&emsp;     ├── gmapping (map generation using gmapping package)

  
<p align="center">
	<img src="images/gmapping.png" width="700"/>
</p>

<br>&emsp;&emsp;     ├── cartographer (map generation using cartographer)

<p align="center">
	<img src="images/cartographer.png" width="700"/>
</p>

<br>&emsp;&emsp;     ├── slamtoolbox_mapping (map generation using slamtoolbox)

<p align="center">
	<img src="images/slam_toolbox.png" width="700"/>
</p>

<br>&emsp;&emsp;     ├── spark_slam (slam using any of the above mapping agent)
<br>&emsp;&emsp;     ├── checkpoint_saver.py (stores the current robot pose as checkpoint after calling the service) [Usage](REPORT.md#checkpoint_saver)
<br>&emsp;&emsp;     └── update_initpose.py (Gets milestone from user in form of service and pubs topic to /amcl/initialpose) [Usage](REPORT.md#update_initpose)

Navigation
<br>├── Launch
<br>│&emsp;&emsp; ├── spark_amcl (Localization of the robot in the environment)
<br>│&emsp;&emsp; ├── spark_move_base (Navigation entity)
<br>│&emsp;&emsp; └── spark_navigation (To start map server, amcl, and move_base)


<table>
  <tr>
    <td><img src="images/nav1.png" width="1000"/></td>
    <td><img src="images/nav2.png" width="1000"/></td>
  </tr>
</table>

<br>└──Node
<br>&emsp;&emsp;  ├── Dynamic prohibition layer: prohibitor.py (Adds removes sectors within map as prohibited layer after reading from YAML) [Usage](REPORT.md#dynamic-prohibition-layer)

<table>
  <tr>
    <td><img src="images/prohibit.png" width="1000"/></td>
    <td><img src="images/gz.png" width="1000"/></td>
  </tr>
  <tr>
    <td colspan="2"><img src="images/prohibit.gif" width="1000"/></td>
  </tr>
</table>

<br>Navigation_goals
<br>├── Launch
<br>│&emsp;&emsp; └── 
<br>└── Node
<br>&emsp;&emsp;    ├── raw_goal (Send Single Goal using direct topic publishment and not via Actionlib)
<br>&emsp;&emsp;    ├── simple_goal (Send Single Goal using Actionlib)
<br>&emsp;&emsp;    ├── callback_goal (Create callback and do not wait for result)
<br>&emsp;&emsp;    └── switch_fsm (fsm implementation independent of ros)

<p align="center">
	<img src="images/switch_fsm.png" width="700"/>
</p>

<br>BT Examples
<br>├── Sync_No_Loop
<br>├── Sync_Loop
<p align="center">
	<img src="images/sync.png" width="700"/>
</p>
<br>├── Reactive (Async behavior with Reactive sequence and memory)
<p align="center">
	<img src="images/reactive.png" width="700"/>
</p>
<br>└── port_blackboard (Same MoveToGoal SyncActionNode but to which goal to reach is defined by port name)

<br>ROS vs BT
<br>├── ros_vs_bt
<br>├── ros_and_bt
<br>└── thread (is multi-threading a problem)

<br>Wait Recovery
<p align="center">
	<img src="images/wait.png" width="700"/>
</p>

<br>Backoff Recovery
<p align="center">
	<img src="images/blind.png" width="700"/>
</p>