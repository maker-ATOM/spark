#include <iostream>
#include <chrono>
#include <std_msgs/Float32.h>
#include <Eigen/Geometry>

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

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
typedef move_base_msgs::MoveBaseResultConstPtr MoveBaseResultConstPtr;
typedef move_base_msgs::MoveBaseFeedbackConstPtr MoveBaseFeedbackConstPtr;

/* --------------------- Move To Goal Action Node defintion --------------------- */

class MoveToGoal : public BT::StatefulActionNode
{
public:
    MoveToGoal(const std::string &name, const BT::NodeConfig &config)
        : StatefulActionNode(name, config), ac("move_base", true)
    {
    }

    void ros_init(ros::NodeHandle &nh)
    {
        nh_ = nh;
        // while (!ac.waitForServer(ros::Duration(2.0)))
        // {
        //     ROS_INFO("Waiting for the move_base action server to come up...");
        // }

        status_sub_ = nh_.subscribe("move_base/status", 10, &MoveToGoal::statusCallback, this);
        battery_sub_ = nh_.subscribe("battery/per", 10, &MoveToGoal::batteryCallback, this);

        // Goal ID?
        goal.target_pose.pose.position.x = 0.0;
        goal.target_pose.pose.position.y = 0.0;
        goal.target_pose.pose.position.z = 0.0;
        goal.target_pose.pose.orientation.x = 0.0;
        goal.target_pose.pose.orientation.y = 0.0;
    }

    static BT::PortsList providedPorts()
    {
        return {
            BT::InputPort<std::string>("location"),
            BT::InputPort<std::map<std::string, Pose>>("pose")};
    }

    BT::NodeStatus onStart() override
    {
        BT::Expected<std::string> loc = getInput<std::string>("location");
        auto location_poses = getInput<std::map<std::string, Pose>>("pose");

        pose = location_poses.value().at(loc.value());
        location = loc.value();

        ROS_INFO("[ MoveToGoal: ACTIVE ]");

        ROS_INFO(" onStart: %f", battery_per);
        // goal.target_pose.header.frame_id = "map";
        // goal.target_pose.header.stamp = ros::Time::now();

        // goal.target_pose.pose.position.x = pose.x;
        // goal.target_pose.pose.position.y = pose.y;

        // Eigen::Quaterniond quat = Eigen::AngleAxisd(pose.yaw, Eigen::Vector3d::UnitZ()) *
        //                           Eigen::AngleAxisd(0, Eigen::Vector3d::UnitY()) *
        //                           Eigen::AngleAxisd(0, Eigen::Vector3d::UnitX());

        // goal.target_pose.pose.orientation.w = quat.w();
        // goal.target_pose.pose.orientation.z = quat.z();

        // send_goal(goal);

        return BT::NodeStatus::RUNNING;
    }

    BT::NodeStatus onRunning() override
    {
        // std::cout << "Going at x: " << pose.x << ", y: " << pose.x << ", yaw: " << pose.yaw << std::endl;

        ROS_INFO_THROTTLE(throttle, " Goal set to: %s", location.c_str());

        ROS_INFO(" onRunning: %f", battery_per);

        std::cout << "onRunning: " << battery_per << std::endl;

        // if (goal_status == 0)
        // {
        //     ROS_INFO_THROTTLE(throttle, "[ MoveToGoal: SUCCESS ]");
        //     return BT::NodeStatus::SUCCESS;
        // }
        // else
        // {

        //     ROS_INFO_THROTTLE(throttle, "[ MoveToGoal: RUNNING ]");
        //     return BT::NodeStatus::RUNNING;
        // }

        return BT::NodeStatus::RUNNING;
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
        ROS_INFO(" batteryCallback: %f", battery_per);
        battery_per = percentage->data;
    }

    void statusCallback(const actionlib_msgs::GoalStatusArray::ConstPtr &status)
    {

        ROS_INFO(" statusCallback: %f", battery_per);

        if (!status->status_list.empty())
        {

            const auto &last_status = status->status_list.back();
            goal_status = (int)last_status.status;
        }
        else
        {
            ROS_WARN("Goal status not made available by move_base yet...");
        }
    }

    void send_goal(move_base_msgs::MoveBaseGoal goal)
    {
        // Need boost::bind to pass in the 'this' pointer
        ac.sendGoal(goal,
                    boost::bind(&MoveToGoal::done_callback, this, _1, _2),
                    // MoveBaseClient::SimpleActiveCallback(),
                    boost::bind(&MoveToGoal::active_callback, this),
                    // MoveBaseClient::SimpleFeedbackCallback());
                    boost::bind(&MoveToGoal::feedback_callback, this, _1));
    }

    void done_callback(const actionlib::SimpleClientGoalState &state,
                       const MoveBaseResultConstPtr &result)
    {
        ROS_INFO("[Move Base: %s]", state.toString().c_str());
    }

    void active_callback()
    {
        ROS_DEBUG("Goal just went active");
    }

    void feedback_callback(const MoveBaseFeedbackConstPtr &feedback)
    {
        ROS_DEBUG("Got Feedback ...");
    }

    void temp_print(){
        std::cout << "temp print:" << battery_per << std::endl;
    }

private:
    Pose pose;

    ros::NodeHandle nh_;
    ros::Subscriber status_sub_;
    ros::Subscriber battery_sub_;

    MoveBaseClient ac;

    move_base_msgs::MoveBaseGoal goal;

    float battery_per = 100;
    int goal_status = 0;
    int throttle = 2;

    std::string location;
};

/* --------------------- Set Location Action Node defintion --------------------- */

class Set_Location : public BT::SyncActionNode
{
public:
    Set_Location(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
        Pose location_a = {1.5, 1.5, 0.79};
        Pose location_b = {4.0, 4.0, 1.57};
        Pose location_c = {0.5, 0.5, -1.57};

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

    std::chrono::milliseconds sleep_time = std::chrono::milliseconds(100);

    BT::NodeStatus status = BT::NodeStatus::IDLE;

    while (true)
    {
        movetogoal.temp_print();
        // std::cout << "--- ticking\n";
        status = tree.tickOnce();
        // std::cout << "--- status: " << toStr(status) << "\n\n";

        ros::spinOnce();

        tree.sleep(sleep_time);
    }

    ros::shutdown();

    return 0;
}