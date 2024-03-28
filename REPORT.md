
## Issues

- [x] Robot State Publisher broadcasts tf for wheels as it should not because the joint types are continuous. Both gazebo and robot_state_publisher were broadcasting, robot_state_publisher receives data published by gazebo about joint states of wheels and broadcasts tf simultaneously gazebo also broadcasts tf about wheels state generating redundant data. So stopped gazebo from publishing
- [x] LaserScan data flickers - sensor name should be different as well
- [x] Scan Matching Failed, using odometry. Likelihood=-3887.64, when using merge laser scan and gmapping, using slam_toolbox for map generation


## Mapping 

1. Slam_toolbox:
Bad performance, continuous origin shifts

2. Gmapping:
False true values

3. Cartographer:
Works good

4. Hector

5. Karto

6. frontier_exploration

## Dynamic Prohibition Layer

Map server has service which allows to change the map on the go, so we can simple call the service and provide it the url of the map that is to be updated. To call the serive we have to write a simple script.

**Logical Flow**

- Read the .yaml/.json file
- Add the polygon to the map file 
- call the service

**Features**

- Edits the same map, so ROS PARAM to get map file name
- ROS param to get package name where the map is stored
- Draws using polygon method as well as using lines using coordinate pairs

Do we really need to give the path of package as a param? No we can use the find method within launch file to give the full path of map file

Could not get the yaml-cpp link to source code so shifting to python

### Usage 

provide two arguments to the script first is the path to map file to be used and second is path to polygon.yaml file (refer prohibitor.launch in navigation package)

The node will read the polygon yaml and map.pgm edit the map based on .yaml file and save the map. `change_map` service by `map_server` will be called to update the modified map.

## Checkpoints

Checkpoints to store so that in case of robot power fails for some reason, the robot should not need to brought back to home instead just bring to closest checkpoint.

### Checkpoint_saver
At first we need to store robot poses as checkpoints and this will happen while mapping,
 - Service will be made available which when called will clear the yaml file if necessary.
 - Service to store the current position of the robot as milestone, gets name of the milestone
 - Topic which publishes marker in the form of poses.

**Usage**

- The Script only requires the path to checkpoint.yaml file through argument. (rosrun spark_slam checkpoint_saver.py /home/aditya/spark_ws/src/spark_navigation/config/checkpoints.yaml
)
- `/clear_checkpoint` service to clear the .yaml file (rosservice call /clear_checkpoints "{}")
- `/store_checkpoint` service to store the current robot pose as checkpoint, provide the name for checkpoint (rosservice call /store_checkpoint "name: 'checkpoint_a'"
)

### update_initpose

A script when launched along with gazebo/bringup takes the milestone from user in the form of service and pub the position on `initialpose` topic

- Publish markers as well for visualization in rviz.

## Battery

To simulate battery charge and discharge. A simple script which decrease voltage linearly and if the robot is in the vicinity of specified area increase the voltage linearly
Argument to provide the docking location.

## Navigation Goals

**A little preface**

<p align="center">
	<img src="images/movebase.png" width="900"/>
</p>


**Global costmap:** Produces cost map using static map given by the map server and laser scans by the robot.

**Global planner:** Produces a complete start to goal path using dijkstra's algorithm.

**Local Costmap:** Local cost map generated for the local planner using only sensor data.

**Local Planner:** produces cmd_vel given the global path and the local cost map

```py
        -->   /goal   -->
        -->  /cancel  -->
command <-- /feedback <-- move_base
        <--  /status  <-- (current status of the goal)
        <--  /results <-- (result of the goal after execution)
```

```py
rosmsg show move_base_msgs/MoveBaseAction

move_base_msgs/MoveBaseActionGoal action_goal
  std_msgs/Header header
    uint32 seq
    time stamp
    string frame_id
  actionlib_msgs/GoalID goal_id
    time stamp
    string id
  move_base_msgs/MoveBaseGoal goal
    geometry_msgs/PoseStamped target_pose
      std_msgs/Header header
        uint32 seq
        time stamp
        string frame_id
      geometry_msgs/Pose pose
        geometry_msgs/Point position
          float64 x
          float64 y
          float64 z
        geometry_msgs/Quaternion orientation
          float64 x
          float64 y
          float64 z
          float64 w
move_base_msgs/MoveBaseActionResult action_result
  std_msgs/Header header
    uint32 seq
    time stamp
    string frame_id
  actionlib_msgs/GoalStatus status
    uint8 PENDING=0
    uint8 ACTIVE=1
    uint8 PREEMPTED=2
    uint8 SUCCEEDED=3
    uint8 ABORTED=4
    uint8 REJECTED=5
    uint8 PREEMPTING=6
    uint8 RECALLING=7
    uint8 RECALLED=8
    uint8 LOST=9
    actionlib_msgs/GoalID goal_id
      time stamp
      string id
    uint8 status
    string text
  move_base_msgs/MoveBaseResult result
move_base_msgs/MoveBaseActionFeedback action_feedback
  std_msgs/Header header
    uint32 seq
    time stamp
    string frame_id
  actionlib_msgs/GoalStatus status
    uint8 PENDING=0
    uint8 ACTIVE=1
    uint8 PREEMPTED=2
    uint8 SUCCEEDED=3
    uint8 ABORTED=4
    uint8 REJECTED=5
    uint8 PREEMPTING=6
    uint8 RECALLING=7
    uint8 RECALLED=8
    uint8 LOST=9
    actionlib_msgs/GoalID goal_id
      time stamp
      string id
    uint8 status
    string text
  move_base_msgs/MoveBaseFeedback feedback
    geometry_msgs/PoseStamped base_position
      std_msgs/Header header
        uint32 seq
        time stamp
        string frame_id
      geometry_msgs/Pose pose
        geometry_msgs/Point position
          float64 x
          float64 y
          float64 z
        geometry_msgs/Quaternion orientation
          float64 x
          float64 y
          float64 z
          float64 w
```

### Sequential script

Three methods:

0. Raw Goal - direct topic pub without using ActionLib
1. Simple Goal - using actionlib
2. Threaded Simple Goal - A different thread to spin the node and different thread to wait for task to get complete
3. Callback Simple Goal - Do not wait for task to finish, creates a callback instead

ActionLib provides information related to health status of server as well like if the server is active or not, raw topic connection does not provide such. Similar to using ros services.

### Continuous

#### State Machines

- Switch case

incase of callback to check the status of the goal we should subscribe to `move_base/status` topic because feedback callback contains only position of the robot and done feedback is called when robot reaches the goal and after reaching one resets the status.

More over we need to add the while loop inside the class cause accessing private member using functions get cumbersome.

**Move base does not provide any feedback when robot has failed to produce a path and requires help :/**

- Function Pointer
- Struct Table

#### Behavior Trees

Ways to define leaf nodes:<br>
- Node Class which represents the node itself and inherits from ActionNode Class
- Function, register this function as a leaf node
- Custom Class Method, register methods within this node as leaf nodes.

To combine with move_base we define all the functionality of the node including actionlib client and other pub subs we use custom class so that data transfer between callback methods gets easier.

Moreover we also define action node for behavior tree in a class.

It is necessary for action node to inherit from SyncActionNode but that is node the case with move_base actionlib, so we can define move_base and other sub callbacks in the action node which inherits from SyncActionNode