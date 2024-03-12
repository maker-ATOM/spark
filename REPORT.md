
## Issues

- [x] Robot State Publisher broadcasts tf for wheels as it should not because the joint types are continuous. Both gazebo and robot_state_publisher were broadcasting, robot_state_publisher receives data published by gazebo about joint states of wheels and broadcasts tf simultaneously gazebo also broadcasts tf about wheels state generating redundant data. So stopped gazebo from publishing
- [x] LaserScan data flickers - sensor name should be different as well
- [x] Scan Matching Failed, using odometry. Likelihood=-3887.64, when using merge laser scan and gmapping, using slam_toolbox for map generation


## Mapping 

1. Slam_toolbox
Bad performance, continuous origin shifts

2. Gmapping
False true values

3. Cartographer
Works good

4. Hector

5. Karto

6. frontier_exploration