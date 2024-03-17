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
#include <move_base_msgs/MoveBaseActionResult.h>
#include <move_base_msgs/MoveBaseActionFeedback.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
typedef move_base_msgs::MoveBaseResultConstPtr MoveBaseResultConstPtr;
typedef move_base_msgs::MoveBaseFeedbackConstPtr MoveBaseFeedbackConstPtr;

// Called once when the goal completes
void done_callback(const actionlib::SimpleClientGoalState &state,
                   const MoveBaseResultConstPtr &result)
{
  ROS_INFO("Finished in state [%s]", state.toString().c_str());
  ros::shutdown();
}

// Called once when the goal becomes active
void active_callback()
{
  ROS_INFO("Goal just went active");
}

// Called every time feedback is received for the goal
void feedback_callback(const MoveBaseFeedbackConstPtr &feedback)
{
  ROS_INFO("Got Feedback ...");
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "simple_goal");

  // create the action client, connect to move_base server
  // true causes the client to spin its own thread
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);

  ros::Rate loop_rate(10);

  while (!ac.waitForServer(ros::Duration(2.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up...");
  }

  move_base_msgs::MoveBaseGoal goal;

  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  // Goal ID?

  goal.target_pose.pose.position.x = 1.0;
  goal.target_pose.pose.position.y = 0.0;
  goal.target_pose.pose.position.z = 0.0;
  goal.target_pose.pose.orientation.w = 1.0;
  goal.target_pose.pose.orientation.x = 0.0;
  goal.target_pose.pose.orientation.y = 0.0;
  goal.target_pose.pose.orientation.z = 0.0;

  ROS_INFO("Sending goal");
  ac.sendGoal(goal, &done_callback, &active_callback, &feedback_callback);

  while (ros::ok())
  {
    ROS_INFO("fien");
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}