#include <ros/ros.h>
#include <move_base_msgs/MoveBaseActionGoal.h>

int main(int argc, char **argv)
{
    ROS_INFO("Start");
    ros::init(argc, argv, "raw_goal");
    ros::NodeHandle nh;

    ros::Publisher raw_pub_ = nh.advertise<move_base_msgs::MoveBaseActionGoal>("/move_base/goal", 10);

    ros::Duration(2.0).sleep();

    move_base_msgs::MoveBaseActionGoal goal;

    // goal.header
    goal.header.frame_id = "map";
    goal.header.stamp = ros::Time::now();

    // goal.goal_id
    goal.goal_id.stamp = ros::Time::now();
    goal.goal_id.id = "/fein";

    goal.goal.target_pose.header.frame_id = "map";
    goal.goal.target_pose.header.stamp = ros::Time::now();
    goal.goal.target_pose.pose.position.x = 1.0;
    goal.goal.target_pose.pose.position.y = 1.0;
    goal.goal.target_pose.pose.position.z = 0.0;
    goal.goal.target_pose.pose.orientation.w = 1.0;
    goal.goal.target_pose.pose.orientation.x = 0.0;
    goal.goal.target_pose.pose.orientation.y = 0.0;
    goal.goal.target_pose.pose.orientation.z = 0.0;

    raw_pub_.publish(goal);

    return 0;
}