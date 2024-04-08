#include <iostream>
#include <chrono>

#include <ros/ros.h>
#include <std_msgs/Int8.h>

#include "behaviortree_cpp/action_node.h"
#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/loggers/groot2_publisher.h"

using namespace std;

class Work : public BT::StatefulActionNode
{
public:
    Work(const std::string &name, const BT::NodeConfig &config)
        : StatefulActionNode(name, config), work_done(0)
    {
    }

    void init_ros(ros::NodeHandle &nh)
    {
        nh_ = nh;
        _state_sub = nh_.subscribe("state", 10, &Work::stateCallback, this);
    }

    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<int>("none")};
    }

    BT::NodeStatus onStart() override
    {
        cout << "[Work: ACTIVE]" << endl;
        return BT::NodeStatus::RUNNING;
    }

    BT::NodeStatus onRunning() override
    {
        int local_state = get_state();
        work_done++;

        cout << "[BT] work_done: " << work_done << endl;
        cout << "[BT] state: " << state << endl;
        cout << "[BT] get state: " << local_state << endl;

        if (work_done > 2)
        {
            work_done = 0;
            cout << "[Work: SUCCESS]" << endl;
            return BT::NodeStatus::SUCCESS;
        }
        else
        {
            cout << "[Work: RUNNING]" << endl;
            return BT::NodeStatus::RUNNING;
        }
    }

    void onHalted() override
    {
        cout << "[Work: ABORTED]" << endl;
    }

    BT::NodeStatus Switch()
    {
        std::cout << "[Switch: SUCCESS]" << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

    void stateCallback(const std_msgs::Int8::ConstPtr &State)
    {
        state = State->data;
        std::cout << "[ROS] work_done: " << work_done << std::endl;
        std::cout << "[ROS] state: " << state << std::endl;
    }

    int get_state()
    {
        return state;
    }

private:
    int work_done = 0;
    int state = 0;

    ros::NodeHandle nh_;
    ros::Subscriber _state_sub;
};

int main(int argc, char **argv)
{

    ros::init(argc, argv, "robot_behavior");
    ros::NodeHandle nh;

    string path = "/home/aditya/spark_ws/src/ros_vs_bt/config/ros_vs_bt.xml";

    BT::BehaviorTreeFactory factory;

    Work work("work", {});
    work.init_ros(nh);

    factory.registerNodeType<Work>("Work");
    factory.registerSimpleAction(
        "Switch",
        bind(&Work::Switch, &work));

    auto tree = factory.createTreeFromFile(path);

    BT::Groot2Publisher publisher(tree);

    std::chrono::milliseconds sleep_time = std::chrono::milliseconds(1000);

    BT::NodeStatus status = BT::NodeStatus::IDLE;

    while (true)
    {
        std::cout << "--- ticking\n";

        int local_state = work.get_state();

        cout << "[main] get state: " << local_state << endl;

        status = tree.tickOnce();
        ros::spinOnce();
        std::cout << "--- status: " << toStr(status) << "\n\n";
        tree.sleep(sleep_time);
    }
    return 0;
}