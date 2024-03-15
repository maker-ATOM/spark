
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