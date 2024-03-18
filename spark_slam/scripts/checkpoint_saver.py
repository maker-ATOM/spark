#!/usr/bin/env python3

# rosrun spark_slam checkpoint_saver.py /home/aditya/spark_ws/src/spark_navigation/config/checkpoints.yaml
# rosservice call /clear_checkpoints "{}"
# rosservice call /store_checkpoint "name: 'home'"

import sys
import yaml
import rospy
import tf
from std_srvs.srv import Empty, EmptyResponse
from spark_navigation.srv import Checkpoint  # Ignore
from visualization_msgs.msg import Marker, MarkerArray

class CheckpointSaver:
    
    def __init__(self, checkpoint_path):
        # Initialize the CheckpointSaver class with the provided checkpoint path
        self.checkpoint_path = checkpoint_path
        
        # Initialize ROS services for clearing and storing checkpoints
        self.cp_srv = rospy.Service('clear_checkpoints', Empty, self.clear_checkpoint)
        rospy.loginfo("Clear checkpoint service available...")

        self.store_srv = rospy.Service('store_checkpoint', Checkpoint, self.store_checkpoint)
        rospy.loginfo("Store checkpoint service available...")
        
        # Initialize a ROS publisher for visualization markers
        self.marker_pub = rospy.Publisher("/checkpoint_visualizer", MarkerArray, queue_size=10)
        self.MarkerArray = MarkerArray()
        
        # Initialize a TF listener
        self.tf_listener = tf.TransformListener()

    def clear_checkpoint(self, request):
        # Callback function to clear the checkpoints
        self.MarkerArray.markers = []  # Clear the marker array
        
        try:
            with open(self.checkpoint_path, 'r') as f:
                data = yaml.safe_load(f)
                if data is None or not bool(data):
                    rospy.loginfo("checkpoints.yaml is empty...")
                else:
                    rospy.loginfo("checkpoints.yaml is not empty, clearing the file...")
                    
                    with open(self.checkpoint_path, 'w') as f:
                        f.truncate(0)  # Clear the content of the file
        except FileNotFoundError:
            rospy.loginfo(f"Provided path '{self.checkpoint_path}' does not exist, please verify once...")
            rospy.signal_shutdown("Invalid path provided.")
            
        request
        return EmptyResponse()  # Return an empty response
    
    def store_checkpoint(self, request):
        # Callback function to store a checkpoint
        self.checkpoint_name = request.name  # Get the checkpoint name from the request
        
        try:
            (trans, rot) = self.tf_listener.lookupTransform('/map', '/base_link', rospy.Time(0))  # Get transform data
        
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            rospy.logwarn("TF lookup exception occurred")
            
        # Prepare data for storing
        data = {
            self.checkpoint_name: {
                "pose": {
                    "x": trans[0],
                    "y": trans[1],
                    "yaw": rot  # Note: 'rot' is already a quaternion
                }
            }
        }
        
        try:
            with open(self.checkpoint_path, 'a') as f:
                yaml.dump(data, f, default_flow_style=False)  # Write data to the YAML file
                status = 1  # Indicate success
        except Exception as e:
            status = 0  # Indicate failure
            
        rospy.loginfo("Stored checkpoint...")
        
        # Create a marker for visualization
        marker = Marker()
        
        marker.header.frame_id = "map"
        marker.header.stamp = rospy.Time.now()
        marker.ns = self.checkpoint_name
        marker.id = 0
        marker.type = Marker.ARROW
        marker.action = Marker.ADD
        marker.pose.position.x = trans[0]
        marker.pose.position.y = trans[1]
        marker.pose.position.z = 0.0
        marker.pose.orientation.x = rot[0]
        marker.pose.orientation.y = rot[1]
        marker.pose.orientation.z = rot[2]
        marker.pose.orientation.w = rot[3]
        marker.scale.x = 0.3
        marker.scale.y = 0.03
        marker.scale.z = 0.03
        marker.color.a = 1.0
        marker.color.r = 0.0
        marker.color.g = 1.0
        marker.color.b = 0.0
        
        # Add marker to the marker array
        self.MarkerArray.markers.append(marker)
        
        return status  # Return the status of storing operation

if __name__ == "__main__":
    # Main function
    
    # Check for correct usage
    if len(sys.argv) < 2:
        print("Usage: rosrun spark_slam checkpoint_saver.py <checkpoint_path>")
        sys.exit(1)
        
    rospy.init_node('checkpoint_saver')  # Initialize ROS node
    checkpoint_saver = CheckpointSaver(sys.argv[1])  # Initialize CheckpointSaver object
    
    # Set the rate of loop
    rate = rospy.Rate(10)
    
    try:
        while not rospy.is_shutdown():
            # Publish the marker array
            checkpoint_saver.marker_pub.publish(checkpoint_saver.MarkerArray)
            rate.sleep()
    except rospy.ROSInterruptException:
        pass  # Ignore ROS interrupt exceptions