## ToDo

**NAVIGATION**

- [x] move_base_params
- [x] costmap_common_params
- [x] global_costmap_params
- [x] local_costmap_params
- [x] dwa_local_planner_params

Check performance in move_forward and in case of stuck and back off recover mode

**DESIGN**
- [x] Base for simulation
- [ ] Arm for simulation

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
- [ ] Behavior tree
- [ ] try other mapping agents
- [ ] Add param with in slam to choose mapping agent
- [ ] Camera Plugin
- [ ] Fused Odometry

**DOCUMENTATION**

- [ ] GIFs
  - [ ] Gmapping
  - [ ] Slamtoolbox
  - [ ] Cartographer
  - [ ] Navigation in the map
  - [ ] Prohibition Layer
  - [ ] Dynamic prohibition
  - [ ] Navigation with PoseArray