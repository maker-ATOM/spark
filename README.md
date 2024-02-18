# SPARK

## ToDo

**DESIGN**
- [x] Base for simulation
- [ ] Arm for simulation

**SOFTWARE STACK**

- [ ] Nav Stack
- [ ] Control
- [ ] Cartographer
- [ ] Camera Plugin
- [ ] Fused Odometry

**DOCUMENTATION**

- [ ] GIFs

## Launch Sequence

**MAP GENERATION**

```py
roslaunch spark_gazebo spark_playground.launch
# Robot State Publishers for the robot
# Spawns robot in gazebo
# LaserScan Merger
```

```py
roslaunch spark_slam mapping.launch
# Gmapping

OR 

roslaunch spark_gazebo slamtoolbox_mapping.launch
# SlamToolBox
```

```py
roslaunch spark_slam map_saver.launch map_name:=my_map
# Sae the Map
```

**NAVIGATION ON SAVED MAP**

```py
roslaunch spark_gazebo spark_playground.launch
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
roslaunch spark_gazebo spark_playground.launch
# Robot State Publishers for the robot
# Spawns robot in gazebo
# LaserScan Merger
```

```py
roslaunch spark_slam spark_slam.launch
# SLAM ToolBox Mapper
# MoveBase
```

## Packages

- Description
  - Launch
    - description ( Visualize tf and robot model )

- Laser Merger
  - Launch
    - laser_merger ( Merge LaserScan of both front and rear and produce a single LaserScan )
  - Node
    - ira_laser_merger

- Gazebo
  - Launch
    - empty_world ( Empty simulation env with robot )
    - turtlebot3_house ( Indoor simulation env with robot )

- Control
  - Node
    - spark_teleop.py ( Teleop )

- Slam
  - Launch
    - map_saver ( save map in the navigation package )
    - gmapping ( map generation using gmapping package )
    - cartographer ( map generation using cartographer )
    - slamtoolbox_mapping ( map generation using slamtoolbox )

- Navigation
  - Launch
   - spark_amcl ( Localization of the robot in the env )
   - spark_move_base ( Navigation entity )
   - spark_navigation ( To start map server, amcl and move_base )

## Issues

- [x] Robot State Publisher broadcasts tf for wheels as it should not because the joint types are continuous. Both gazebo and robot_state_publisher were broadcasting, robot_state_publisher receives data published by gazebo about joint states of wheels and broadcasts tf simultaneously gazebo also broadcasts tf about wheels state generating redundant data. So stopped gazebo from publishing
- [x] LaserScan data flickers - sensor name should be different as well
- [x] Scan Matching Failed, using odometry. Likelihood=-3887.64, when using merge laser scan and gmapping, using slam_toolbox for map generation

