#!/usr/bin/env python3

# rosrun spark_navigation update_initpose.py /home/aditya/spark_ws/src/spark_navigation/config/checkpoints.yaml
# rosservice call /update_initpose "name: 'home'"

import sys
import yaml
import rospy
from geometry_msgs.msg import PoseWithCovarianceStamped
from spark_navigation.srv import Checkpoint  # Ignore
from visualization_msgs.msg import Marker, MarkerArray

class UpdateInitPose:
    
    def __init__(self, checkpoint_path):
        
        self.update_srv = rospy.Service('update_initpose', Checkpoint, self.srv_callback)
        rospy.loginfo("Update Initpose service available...")
        
        # Initialize a ROS publisher for visualization markers
        self.marker_pub = rospy.Publisher("/available_checkpoint_visualizer", MarkerArray, queue_size=10)
        
        self.init_pose_pub = rospy.Publisher("/initialpose", PoseWithCovarianceStamped, queue_size=10)
    
        self.Marker_Array = MarkerArray()
        self.Marker_Array.markers = []
        
        
        with open(checkpoint_path, 'r') as file:
            self.yaml_data = yaml.safe_load(file)

        # Iterate through all elements and append to the list
        for key, value in self.yaml_data.items():
            
            marker = Marker()
        
            marker.header.frame_id = "map"
            marker.header.stamp = rospy.Time.now()
            marker.ns = key
            marker.id = 0
            marker.type = Marker.ARROW
            marker.action = Marker.ADD
            marker.pose.position.x = value["pose"]["x"]
            marker.pose.position.y = value["pose"]["y"]
            marker.pose.position.z = 0.0
            marker.pose.orientation.x = value["pose"]["yaw"][0]
            marker.pose.orientation.y = value["pose"]["yaw"][1]
            marker.pose.orientation.z = value["pose"]["yaw"][2]
            marker.pose.orientation.w = value["pose"]["yaw"][3]
            marker.scale.x = 0.3
            marker.scale.y = 0.03
            marker.scale.z = 0.03
            marker.color.a = 1.0
            marker.color.r = 0.0
            marker.color.g = 1.0
            marker.color.b = 0.0
            
            # Add marker to the marker array
            self.Marker_Array.markers.append(marker)        
    
    def srv_callback(self,request):
        
        try:
            InitialPose = PoseWithCovarianceStamped()
            
            InitialPose.header.stamp = rospy.Time.now()
            InitialPose.header.frame_id = "map"
            
            InitialPose.pose.pose.position.x = self.yaml_data[request.name]["pose"]["x"] 
            InitialPose.pose.pose.position.y = self.yaml_data[request.name]["pose"]["y"] 
            InitialPose.pose.pose.position.z = 0.0
            
            InitialPose.pose.pose.orientation.x = self.yaml_data[request.name]["pose"]["yaw"][0]  
            InitialPose.pose.pose.orientation.y = self.yaml_data[request.name]["pose"]["yaw"][1] 
            InitialPose.pose.pose.orientation.z = self.yaml_data[request.name]["pose"]["yaw"][2] 
            InitialPose.pose.pose.orientation.w = self.yaml_data[request.name]["pose"]["yaw"][3] 
            
            InitialPose.pose.covariance = [0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.06853892326654787]

            self.init_pose_pub.publish(InitialPose)
            
            rospy.loginfo(f"Publishing checkpoint: {request.name}'s position as initial pose...")
            return 1
        except KeyError as e:
            rospy.logwarn("Checkpoint name invalid...") 
            return 0

if __name__ == "__main__":
    # Main function
    
    # Check for correct usage
    if len(sys.argv) < 2:
        print("Usage: rosrun spark_navigation update_initpose.py <checkpoint_path>")
        sys.exit(1)
        
    checkpoint_path = sys.argv[1]
    
    rospy.init_node('update_initpose')  # Initialize ROS node
    
    update_initpose = UpdateInitPose(checkpoint_path)  
    
    # Set the rate of loop
    rate = rospy.Rate(10)
    
    try:
        while not rospy.is_shutdown():
            
            # Publish the marker array
            update_initpose.marker_pub.publish(update_initpose.Marker_Array)
            rate.sleep()
            
    except rospy.ROSInterruptException:
        pass  # Ignore ROS interrupt exceptions