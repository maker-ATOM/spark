#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <tf/transform_listener.h>
#include <cmath>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "battery_status");
    ros::NodeHandle nh;
    ros::Rate loop_rate(20);

    ros::Publisher battery_per_pub = nh.advertise<std_msgs::Float32>("/battery/per", 10);

    tf::TransformListener listener;
    tf::StampedTransform transform;

    float dock_x = std::stof(argv[1]);
    float dock_y = std::stof(argv[2]);

    ROS_INFO("x: %f, y%f", dock_x, dock_y);

    float battery_per = 99.9;
    float battery_consumption = 0.01;
    float tolerance = 0.1;

    std_msgs::Float32 battery;

    while (ros::ok())
    {
        try
        {
            listener.lookupTransform("/odom", "/base_link",
                                     ros::Time(0), transform);

            ROS_DEBUG("Translation: x=%f, y=%f", transform.getOrigin().x(), transform.getOrigin().y());
            ROS_DEBUG("Goal: x=%f, y=%f", dock_x, dock_y);

            float linear_distance = sqrt(pow((transform.getOrigin().x() - dock_x), 2) + pow((transform.getOrigin().y() - dock_y), 2));
            ROS_DEBUG("l: %f", linear_distance);

            if (linear_distance < tolerance)
            {
                if (battery_per < 100)
                    battery_per += battery_consumption;
            }
            else
            {
                if (battery_per > 0)
                    battery_per -= battery_consumption;
            }
        }
        catch (tf::TransformException ex)
        {
            ROS_ERROR("%s", ex.what());
            ros::Duration(1.0).sleep();
        }

        if (battery_per > 0 && battery_per < 100)
        {
        }

        battery.data = battery_per;

        battery_per_pub.publish(battery);

        loop_rate.sleep();
    }

    return 0;
}