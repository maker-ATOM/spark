#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>

int main(int argc, char **argv)
{
    // Initialize the ROS node
    ros::init(argc, argv, "transform_listener");
    ros::NodeHandle node;

    // Create a TransformListener object
    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener tfListener(tfBuffer);

    // Specify the target frame and the source frame
    std::string targetFrame = "map";
    std::string sourceFrame = "base_link";

    // Set up a rate to control the loop frequency
    ros::Rate rate(2.0); // 1 Hz

    bool valid_tf = false;

    while (!valid_tf)
    {
        try
        {
            // Lookup the transform from source frame to target frame
            geometry_msgs::TransformStamped transformStamped;
            transformStamped = tfBuffer.lookupTransform(targetFrame, sourceFrame,
                                                        ros::Time(0.0));

            // Print the transform
            ROS_INFO("Translation: x=%.2f, y=%.2f, z=%.2f",
                     transformStamped.transform.translation.x,
                     transformStamped.transform.translation.y,
                     transformStamped.transform.translation.z);
            ROS_INFO("Rotation: x=%.2f, y=%.2f, z=%.2f, w=%.2f",
                     transformStamped.transform.rotation.x,
                     transformStamped.transform.rotation.y,
                     transformStamped.transform.rotation.z,
                     transformStamped.transform.rotation.w);

                     valid_tf = true;
        }
        catch (tf2::TransformException &ex)
        {
            // Handle any exceptions
            ROS_WARN("Transform lookup failed: %s", ex.what());
        }

        // Sleep to control the loop rate
        rate.sleep();
    }

    return 0;
}
