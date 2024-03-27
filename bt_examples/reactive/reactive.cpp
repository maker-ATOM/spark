#include <iostream>
#include <chrono>
#include "behaviortree_cpp/action_node.h"
#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/loggers/groot2_publisher.h"

using namespace std::chrono_literals;

/* --------------------------------------------------------------------------------- */

class MoveToA : public BT::StatefulActionNode
{
public:
    // Any TreeNode with ports must have a constructor with this signature
    MoveToA(const std::string &name, const BT::NodeConfig &config)
        : StatefulActionNode(name, config), DistanceToA(5)
    {
    }

    // It is mandatory to define this static method.
    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<int>("bruha")};
    }

    // this function is invoked once at the beginning.
    BT::NodeStatus onStart() override
    {
        // Get Coordinates to send and call send goal action service of movebase
        // validate send goal and update the status

        printf("[ MoveToA: ACTIVE ]\n");
        return BT::NodeStatus::RUNNING;
    }

    // If onStart() returned RUNNING, we will keep calling
    // this method until it return something different from RUNNING
    BT::NodeStatus onRunning() override
    {
        DistanceToA -= 1;
        if (DistanceToA > 0)
        {
            printf("Distance to goal: %d\n", DistanceToA);
            printf("[ MoveToA: RUNNING ]\n");
            std::this_thread::sleep_for(1s);
            return BT::NodeStatus::RUNNING;
        }
        else
        {
            printf("Distance to goal: %d\n", DistanceToA);
            printf("[ MoveToA: SUCCESS ]\n");
            DistanceToA = 5;
            return BT::NodeStatus::SUCCESS;
        }
    }

    // callback to execute if the action was aborted by another node
    void onHalted() override
    {
        printf("[ MoveToA: ABORTED ]\n");
    }

private:
    int DistanceToA;
};

/* --------------------------------------------------------------------------------- */

class MoveToB : public BT::StatefulActionNode
{
public:
    // Any TreeNode with ports must have a constructor with this signature
    MoveToB(const std::string &name, const BT::NodeConfig &config)
        : StatefulActionNode(name, config), DistanceToB(7)
    {
    }

    // It is mandatory to define this static method.
    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<int>("bruhb")};
    }

    // this function is invoked once at the beginning.
    BT::NodeStatus onStart() override
    {
        // Get Coordinates to send and call send goal action service of movebase
        // validate send goal and update the status

        printf("[ MoveToB: ACTIVE ]\n");
        return BT::NodeStatus::RUNNING;
    }

    // If onStart() returned RUNNING, we will keep calling
    // this method until it return something different from RUNNING
    BT::NodeStatus onRunning() override
    {
        DistanceToB -= 1;
        if (DistanceToB > 0)
        {
            printf("Distance to goal: %d\n", DistanceToB);
            printf("[ MoveToB: RUNNING ]\n");
            std::this_thread::sleep_for(1s);
            return BT::NodeStatus::RUNNING;
        }
        else
        {
            printf("Distance to goal: %d\n", DistanceToB);
            printf("[ MoveToB: SUCCESS ]\n");
            DistanceToB = 7;
            return BT::NodeStatus::SUCCESS;
        }
    }

    // callback to execute if the action was aborted by another node
    void onHalted() override
    {
        printf("[ MoveToB: ABORTED ]\n");
    }

private:
    int DistanceToB;
};

/* --------------------------------------------------------------------------------- */

class MoveToC : public BT::StatefulActionNode
{
public:
    // Any TreeNode with ports must have a constructor with this signature
    MoveToC(const std::string &name, const BT::NodeConfig &config)
        : StatefulActionNode(name, config), DistanceToC(3)
    {
    }

    // It is mandatory to define this static method.
    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<int>("bruhc")};
    }

    // this function is invoked once at the beginning.
    BT::NodeStatus onStart() override
    {
        // Get Coordinates to send and call send goal action service of movebase
        // validate send goal and update the status

        printf("[ MoveToC: ACTIVE ]\n");
        return BT::NodeStatus::RUNNING;
    }

    // If onStart() returned RUNNING, we will keep calling
    // this method until it return something different from RUNNING
    BT::NodeStatus onRunning() override
    {
        DistanceToC -= 1;
        if (DistanceToC > 0)
        {
            printf("Distance to goal: %d\n", DistanceToC);
            printf("[ MoveToC: RUNNING ]\n");
            std::this_thread::sleep_for(1s);
            return BT::NodeStatus::RUNNING;
        }
        else
        {
            printf("Distance to goal: %d\n", DistanceToC);
            printf("[ MoveToC: SUCCESS ]\n");
            DistanceToC = 3;
            return BT::NodeStatus::SUCCESS;
        }
    }

    // callback to execute if the action was aborted by another node
    void onHalted() override
    {
        printf("[ MoveToA: ABORTED ]\n");
    }

private:
    int DistanceToC;
};

/* --------------------------------------------------------------------------------- */

class MoveToDock : public BT::StatefulActionNode
{
public:
    // Any TreeNode with ports must have a constructor with this signature
    MoveToDock(const std::string &name, const BT::NodeConfig &config)
        : StatefulActionNode(name, config), DistanceToDock(5)
    {
    }

    // It is mandatory to define this static method.
    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<int>("bruhdock")};
    }

    // this function is invoked once at the beginning.
    BT::NodeStatus onStart() override
    {
        // Get Coordinates to send and call send goal action service of movebase
        // validate send goal and update the status

        printf("[ MoveToDock: ACTIVE ]\n");
        return BT::NodeStatus::RUNNING;
    }

    // If onStart() returned RUNNING, we will keep calling
    // this method until it return something different from RUNNING
    BT::NodeStatus onRunning() override
    {
        DistanceToDock -= 1;
        if (DistanceToDock > 0)
        {
            printf("Distance to goal: %d\n", DistanceToDock);
            printf("[ MoveToDock: RUNNING ]\n");
            std::this_thread::sleep_for(1s);
            return BT::NodeStatus::RUNNING;
        }
        else
        {
            printf("Distance to goal: %d\n", DistanceToDock);
            printf("[ MoveToDock: SUCCESS ]\n");
            DistanceToDock = 3;
            return BT::NodeStatus::SUCCESS;
        }
    }

    // callback to execute if the action was aborted by another node
    void onHalted() override
    {
        printf("[ MoveToA: ABORTED ]\n");
    }

private:
    int DistanceToDock;
};

/* --------------------------------------------------------------------------------- */

class RobotBehaviour
{
public:
    RobotBehaviour() : battery(3)
    {
    }

    BT::NodeStatus Battery()
    {
        battery -= 1;

        if (battery > 0)
        {
            printf("[ Battery: OK ] : %d\n", battery);
            return BT::NodeStatus::SUCCESS;
        }
        else
        {

            printf("[ Battery: EMPTY ] : %d\n", battery);
            battery = 10;
            return BT::NodeStatus::FAILURE;
        }
    }

    BT::NodeStatus Help()
    {
        std::cout << "Help..." << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

    BT::NodeStatus Charging()
    {
        std::cout << "Charging and Charged..." << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

private:
    int battery = 10;
};

/* --------------------------------------------------------------------------------- */

int main(int argc, char **argv)
{

    BT::BehaviorTreeFactory factory;

    RobotBehaviour robotbehaviour;

    factory.registerNodeType<MoveToA>("MoveToA");
    factory.registerNodeType<MoveToB>("MoveToB");
    factory.registerNodeType<MoveToC>("MoveToC");
    factory.registerNodeType<MoveToDock>("MoveToDock");
    factory.registerSimpleAction(
        "Help",
        std::bind(&RobotBehaviour::Help, &robotbehaviour));
    factory.registerSimpleCondition(
        "Battery",
        std::bind(&RobotBehaviour::Battery, &robotbehaviour));
    factory.registerSimpleAction(
        "Charging",
        std::bind(&RobotBehaviour::Charging, &robotbehaviour));

    auto tree = factory.createTreeFromFile("./../reactive.xml");

    BT::Groot2Publisher publisher(tree);

    std::chrono::milliseconds sleep_time = std::chrono::milliseconds(2000);

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