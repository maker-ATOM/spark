#include <iostream>
#include <chrono>
#include "behaviortree_cpp/action_node.h"
#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/loggers/groot2_publisher.h"

using namespace std::chrono_literals;

class RobotBehaviour
{
    // Constructor defines subscription, publication and client to move_base
public:
    BT::NodeStatus MoveToA()
    {
        std::cout << "Moving to A ..." << std::endl;

        std::this_thread::sleep_for(1s);
        return BT::NodeStatus::SUCCESS;
    }

    BT::NodeStatus MoveToB()
    {
        std::cout << "Moving to B ..." << std::endl;
        std::this_thread::sleep_for(1s);
        return BT::NodeStatus::SUCCESS;
    }

    BT::NodeStatus MoveToC()
    {
        std::cout << "Moving to C ..." << std::endl;
        std::this_thread::sleep_for(1s);
        return BT::NodeStatus::SUCCESS;
    }

    BT::NodeStatus Help()
    {
        std::cout << "Help Me ..!" << std::endl;
        std::this_thread::sleep_for(1s);
        return BT::NodeStatus::SUCCESS;
    }
};

int main(int argc, char **argv)
{

    BT::BehaviorTreeFactory factory;

    RobotBehaviour robotbehaviour;

    factory.registerSimpleAction(
        "MoveToA",
        std::bind(&RobotBehaviour::MoveToA, &robotbehaviour));

    factory.registerSimpleAction(
        "MoveToB",
        std::bind(&RobotBehaviour::MoveToB, &robotbehaviour));

    factory.registerSimpleAction(
        "MoveToC",
        std::bind(&RobotBehaviour::MoveToC, &robotbehaviour));

    factory.registerSimpleAction(
        "Help",
        std::bind(&RobotBehaviour::Help, &robotbehaviour));

    auto tree = factory.createTreeFromFile("./../sync_loop.xml");

    BT::Groot2Publisher publisher(tree);

    std::chrono::milliseconds sleep_time = std::chrono::milliseconds(5000);

    BT::NodeStatus status = BT::NodeStatus::IDLE;

    while (true)
    {
        std::cout << "--- ticking\n";
        status = tree.tickOnce();
        std::cout << "--- status: " << toStr(status) << "\n\n";
        tree.sleep(sleep_time);
    }

    return 0;
}