/*
rosmsg show move_base_msgs/MoveBaseAction

move_base_msgs/MoveBaseActionGoal action_goal
  std_msgs/Header header
    uint32 seq
    time stamp
    string frame_id
  actionlib_msgs/GoalID goal_id
    time stamp
    string id
  move_base_msgs/MoveBaseGoal goal
    geometry_msgs/PoseStamped target_pose
      std_msgs/Header header
        uint32 seq
        time stamp
        string frame_id
      geometry_msgs/Pose pose
        geometry_msgs/Point position
          float64 x
          float64 y
          float64 z
        geometry_msgs/Quaternion orientation
          float64 x
          float64 y
          float64 z
          float64 w
move_base_msgs/MoveBaseActionResult action_result
  std_msgs/Header header
    uint32 seq
    time stamp
    string frame_id
  actionlib_msgs/GoalStatus status
    uint8 PENDING=0
    uint8 ACTIVE=1
    uint8 PREEMPTED=2
    uint8 SUCCEEDED=3
    uint8 ABORTED=4
    uint8 REJECTED=5
    uint8 PREEMPTING=6
    uint8 RECALLING=7
    uint8 RECALLED=8
    uint8 LOST=9
    actionlib_msgs/GoalID goal_id
      time stamp
      string id
    uint8 status
    string text
  move_base_msgs/MoveBaseResult result
move_base_msgs/MoveBaseActionFeedback action_feedback
  std_msgs/Header header
    uint32 seq
    time stamp
    string frame_id
  actionlib_msgs/GoalStatus status
    uint8 PENDING=0
    uint8 ACTIVE=1
    uint8 PREEMPTED=2
    uint8 SUCCEEDED=3
    uint8 ABORTED=4
    uint8 REJECTED=5
    uint8 PREEMPTING=6
    uint8 RECALLING=7
    uint8 RECALLED=8
    uint8 LOST=9
    actionlib_msgs/GoalID goal_id
      time stamp
      string id
    uint8 status
    string text
  move_base_msgs/MoveBaseFeedback feedback
    geometry_msgs/PoseStamped base_position
      std_msgs/Header header
        uint32 seq
        time stamp
        string frame_id
      geometry_msgs/Pose pose
        geometry_msgs/Point position
          float64 x
          float64 y
          float64 z
        geometry_msgs/Quaternion orientation
          float64 x
          float64 y
          float64 z
          float64 w
*/
#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <move_base_msgs/MoveBaseAction.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "simple_goal");

    // create the action client, connect to move_base server
    // true causes the client to spin its own thread
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);

    while (!ac.waitForServer(ros::Duration(2.0)))
    {
        ROS_INFO("Waiting for the move_base action server to come up...");
    }

    move_base_msgs::MoveBaseGoal goal;

    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();

    // Goal ID?

    goal.target_pose.pose.position.x = 1.0;
    goal.target_pose.pose.position.y = 1.0;
    goal.target_pose.pose.position.z = 0.0;
    goal.target_pose.pose.orientation.w = 1.0;
    goal.target_pose.pose.orientation.x = 0.0;
    goal.target_pose.pose.orientation.y = 0.0;
    goal.target_pose.pose.orientation.z = 0.0;

    ROS_INFO("Sending goal");
    ac.sendGoal(goal);

    // wait for the action to return before time out
    // bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

    // if (finished_before_timeout)
    // {
    //     actionlib::SimpleClientGoalState state = ac.getState();
    //     ROS_INFO("Action finished: %s", state.toString().c_str());
    // }
    // else
    // {
    //     ROS_INFO("Action did not finish before the time out.");
    //     ac.cancelGoal();
    // }

    // wait until result
    ac.waitForResult();

    actionlib::SimpleClientGoalState state = ac.getState();
    ROS_INFO("Action finished: %s", state.toString().c_str());

    // Print the result of the goal
    const auto &result = ac.getResult();
    ROS_INFO_STREAM("Result: " << result);

    return 0;
}