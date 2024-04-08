#include <iostream>
#include <chrono>
#include <std_msgs/Float32.h>

#include "behaviortree_cpp/action_node.h"
#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/loggers/groot2_publisher.h"

#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <actionlib_msgs/GoalStatusArray.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseActionResult.h>
#include <move_base_msgs/MoveBaseActionFeedback.h>

using namespace std::chrono_literals;

struct Pose
{
    double x;
    double y;
    double yaw;
};

/* --------------------- Move To Goal Action Node defintion --------------------- */

class MoveToGoal : public BT::StatefulActionNode
{
public:
    MoveToGoal(const std::string &name, const BT::NodeConfig &config)
        : StatefulActionNode(name, config)
    {
    }
    void ros_init(ros::NodeHandle &nh)
    {
        nh_ = nh;
        battery_sub_ = nh_.subscribe("battery/per", 10, &MoveToGoal::batteryCallback, this);
    }

    static BT::PortsList providedPorts()
    {
        return {
            BT::InputPort<std::string>("location"),
            BT::InputPort<std::map<std::string, Pose>>("pose")};
    }

    BT::NodeStatus onStart() override
    {
        // Get Coordinates to send and call send goal action service of movebase
        // validate send goal and update the status

        BT::Expected<std::string> loc = getInput<std::string>("location");
        auto location_poses = getInput<std::map<std::string, Pose>>("pose");
        pose = location_poses.value().at(loc.value());

        std::cout << "[ MoveToGoal: ACTIVE ]" << std::endl;
        std::cout << "Goal set to: " << loc.value() << std::endl;

        return BT::NodeStatus::RUNNING;
    }

    BT::NodeStatus onRunning() override
    {

        std::cout << "Going at x: " << pose.x << ", y: " << pose.x << ", yaw: " << pose.yaw << std::endl;
        std::cout << "[ MoveToGoal: SUCCESS ]" << std::endl;
        ROS_INFO(" onRunning: %f", battery_per);
        return BT::NodeStatus::SUCCESS;
    }

    void onHalted() override
    {
        std::cout << "[ MoveToGoal: ABORTED ]" << std::endl;
    }

    BT::NodeStatus Help()
    {
        std::cout << "Help..." << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

    void batteryCallback(const std_msgs::Float32::ConstPtr &percentage)
    {
        std::cout << "batteryCallback at x: " << pose.x << ", y: " << pose.x << ", yaw: " << pose.yaw << std::endl;
        ROS_INFO(" batteryCallback: %f", battery_per);
        battery_per = percentage->data;
    }

private:
    Pose pose;

    ros::NodeHandle nh_;
    ros::Subscriber battery_sub_;

    float battery_per = 100;
};

/* --------------------- Set Location Action Node defintion --------------------- */

class Set_Location : public BT::SyncActionNode
{
public:
    Set_Location(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
        Pose location_a = {1.0, 2.0, 0.5};
        Pose location_b = {3.0, 4.0, 1.0};
        Pose location_c = {5.0, 6.0, 1.5};

        location_pose["location_a"] = location_a;
        location_pose["location_b"] = location_b;
        location_pose["location_c"] = location_c;
    }

    static BT::PortsList providedPorts()
    {
        return {
            BT::OutputPort<std::map<std::string, Pose>>("pose")};
    }

    // This Action writes a value into the port "text"
    BT::NodeStatus tick() override
    {
        // Set Goal pose based on location
        std::cout << "[ Set_Location: SUCCESS ]" << std::endl;
        setOutput("pose", location_pose);
        return BT::NodeStatus::SUCCESS;
    }

private:
    std::map<std::string, Pose> location_pose;
};

/* ------------------------------------------------------------------------------ */

int main(int argc, char **argv)
{

    ros::init(argc, argv, "robot_behavior");
    ros::NodeHandle nh;

    // rosrun spark_nav_goals behavior ~/spark_ws/src/spark_nav_goals/params/behavior.xml
    std::string path = argv[1];

    BT::BehaviorTreeFactory factory;

    MoveToGoal movetogoal("movetogoal", {});
    movetogoal.ros_init(nh);

    factory.registerSimpleAction(
        "Help",
        std::bind(&MoveToGoal::Help, &movetogoal));
    factory.registerNodeType<Set_Location>("Set_Location");
    factory.registerNodeType<MoveToGoal>("MoveToGoal");

    auto tree = factory.createTreeFromFile(path);

    BT::Groot2Publisher publisher(tree);

    std::chrono::milliseconds sleep_time = std::chrono::milliseconds(500);

    BT::NodeStatus status = BT::NodeStatus::IDLE;

    while (true)
    {
        // std::cout << "--- ticking\n";
        status = tree.tickOnce();
        // std::cout << "--- status: " << toStr(status) << "\n\n";

        ros::spinOnce();
        tree.sleep(sleep_time);
    }

    return 0;
}