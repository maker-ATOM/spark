## ToDo

**NAVIGATION**

- [x] move_base_params
- [x] costmap_common_params
- [x] global_costmap_params
- [x] local_costmap_params
- [x] dwa_local_planner_params

Check performance in move_forward and in case of stuck and back off recover mode


**SOFTWARE STACK**

- [x] Control
- [x] Cartographer
- [x] Friction of wheels, bot slips
- [x] Prohibition Layer
  - [x] ROS arg for map_file name
  - [x] Read the .yaml/.json file
  - [x] Include true or false to remove/add the polygon from map
  - [x] Include multi shape filled polygon  (Use other method to draw the shape instead of line)
  - [x] Add the polygon to the map file 
  - [x] call the service
  - [x] Roslog not working in launch file
- [x] Store Checkpoints
  - [x] service to clear the yaml file
  - [x] custom srv msg to get checkpoint name
  - [x] service to store the pose
  - [x] sub to odom
  - [x] marker for visualization
  - [x] include in bringup(gazebo) launch file
- [x] Use Checkpoints
  - [x] Pub all checkpoints in the .yaml file
  - [x] pub to initialpose based to service call by user
- [x] Battery Plugin
  - [x] Linear model
  - [x] charging dock (if robot at doc increase the voltage)
  - [x] Argument for docking location.
- [ ] Nav Goal
  - [x] Sequential goals
    - [x] raw topic pub 
    - [x] simple goals
    - [x] threaded simple
    - [x] callback simple
  - [x] State machine
    - [x] switch fsm
      - [x] Get feedback of status from topic
      - [x] while loop inside class?
  - [ ] Behavior tree
    - [x] Example
      - [x] sync no Loop
      - [x] Sync loop
      - [x] Reactive
        - [ ] ~~Base Class definition for send move to goal, this class will inherit from StatefulActionNode~~
        - [ ] ~~Sub tree for battery~~
      - [x] Port and blackboard with same action node.
    - [ ] Only go to goal (inherit from ports and blackboard)
- [ ] try other mapping agents
- [ ] Add param with in slam to choose mapping agent
- [ ] Camera Plugin
- [ ] Fused Odometry

**DESIGN**
- [x] Base for simulation
- [ ] Arm for simulation

**DOCUMENTATION**

- [ ] GIFs
  - [x] Gmapping
  - [x] Slamtoolbox
  - [x] Cartographer
  - [x] Navigation in the map
  - [x] Prohibition Layer
  - [x] Dynamic prohibition
  - [ ] Store checkpoints
  - [ ] Load checkpoint
  - [ ] Navigation with PoseArray