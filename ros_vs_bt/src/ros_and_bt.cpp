#include <iostream>
#include <chrono>

#include <ros/ros.h>
#include <std_msgs/Int8.h>

#include "behaviortree_cpp/action_node.h"
#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/loggers/groot2_publisher.h"

using namespace std;

class RobotState
{
public:
    RobotState(ros::NodeHandle &nh) : _nh(nh), state(0)
    {
        _state_sub = _nh.subscribe("state", 10, &RobotState::stateCallback, this);
    }

    void stateCallback(const std_msgs::Int8::ConstPtr &data)
    {
        cout << "StateCallback: " << state << std::endl;
        state = data->data;
    }

    int get_state()
    {
        return state;
    }

private:
    ros::NodeHandle _nh;
    ros::Subscriber _state_sub;

    int state;
};

class Work : public BT::StatefulActionNode
{
public:
    Work(const std::string &name, const BT::NodeConfig &config)
        : StatefulActionNode(name, config), work_done(0)
    {
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
        work_done++;

        cout << "[BT] work_done: " << work_done << endl;

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
        cout << "[Switch: SUCCESS]" << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

private:
    int work_done;
};

int main(int argc, char **argv)
{

    ros::init(argc, argv, "robot_behavior");
    ros::NodeHandle nh;

    RobotState robotstate(nh);

    string path = "/home/aditya/spark_ws/src/ros_vs_bt/config/ros_vs_bt.xml";

    BT::BehaviorTreeFactory factory;

    Work work("work", {});

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

        cout << "Main: " << robotstate.get_state() << std::endl;
        status = tree.tickOnce();
        ros::spinOnce();
        std::cout << "--- status: " << toStr(status) << "\n\n";
        tree.sleep(sleep_time);
    }
    
    return 0;
}