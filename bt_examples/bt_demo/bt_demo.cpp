#include <iostream>
#include <chrono>
#include "behaviortree_cpp/action_node.h"
#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/loggers/groot2_publisher.h"

using namespace std::chrono_literals;

/* -------------------------------------------------------------------------------- */
/* class to represent node which inherits from SyncActionNode */

class ApproachObject : public BT::SyncActionNode
{
public:
  explicit ApproachObject(const std::string &name) : BT::SyncActionNode(name, {})
  {
  }

  // Since the class inherits we use the tick method from Sync Action Node and override it
  BT::NodeStatus tick() override
  {
    std::cout << "Approach Object: " << this->name() << std::endl;

    std::this_thread::sleep_for(5s);
    return BT::NodeStatus::SUCCESS;
  }
};

/* -------------------------------------------------------------------------------- */
/* Function to register as leaf node */

BT::NodeStatus CheckBattery()
{
  std::cout << "Battery OK" << std::endl;
  return BT::NodeStatus::SUCCESS;
}

/* -------------------------------------------------------------------------------- */
/* Custom class whose methods are registered as leaf nodes */

class GripperInterface
{
public:
  GripperInterface() : _open(true) {}

  // Any method can be registered as a leaf node.
  BT::NodeStatus open()
  {
    _open = true;
    std::cout << "Gripper open" << std::endl;
    return BT::NodeStatus::SUCCESS;
  }

  BT::NodeStatus close()
  {
    _open = false;
    std::cout << "Gripper close" << std::endl;
    return BT::NodeStatus::SUCCESS;
  }

private:
  bool _open;
};

/* -------------------------------------------------------------------------------- */

int main()
{
  BT::BehaviorTreeFactory factory;

  factory.registerNodeType<ApproachObject>("ApproachObject");

  factory.registerSimpleCondition("CheckBattery", std::bind(CheckBattery));

  GripperInterface gripper;

  // register as SyncActionNode
  factory.registerSimpleAction(
      "OpenGripper",
      std::bind(&GripperInterface::open, &gripper));

  factory.registerSimpleAction(
      "CloseGripper",
      std::bind(&GripperInterface::close, &gripper));

  // Create Tree
  auto tree = factory.createTreeFromFile("./../bt_example.xml");

  BT::Groot2Publisher publisher(tree);

  std::this_thread::sleep_for(5s);

  // execute the tree
  tree.tickWhileRunning();

  return 0;
}