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
- [ ] Milestones
  - [ ] Ros param to set time and traveled distance
  - [ ] marker
- [ ] Behavior tree
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
  - [ ] Navigation with PoseArray