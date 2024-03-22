#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <actionlib_msgs/GoalStatusArray.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseActionResult.h>
#include <move_base_msgs/MoveBaseActionFeedback.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
typedef move_base_msgs::MoveBaseResultConstPtr MoveBaseResultConstPtr;
typedef move_base_msgs::MoveBaseFeedbackConstPtr MoveBaseFeedbackConstPtr;

class FSM
{
public:
    FSM(ros::NodeHandle &nh) : nh_(nh), ac("move_base", true)
    {
        while (!ac.waitForServer(ros::Duration(2.0)))
        {
            ROS_INFO("Waiting for the move_base action server to come up...");
        }

        status_sub_ = nh_.subscribe("move_base/status", 10, &FSM::statusCallback, this);
        battery_sub_ = nh_.subscribe("battery/per", 10, &FSM::batteryCallback, this);

        goal.target_pose.header.frame_id = "map";
        goal.target_pose.header.stamp = ros::Time::now();

        // Goal ID?
        goal.target_pose.pose.position.x = 0.0;
        goal.target_pose.pose.position.y = 0.0;
        goal.target_pose.pose.position.z = 0.0;
        goal.target_pose.pose.orientation.x = 0.0;
        goal.target_pose.pose.orientation.y = 0.0;
    }

    void send_goal(move_base_msgs::MoveBaseGoal goal)
    {
        // Need boost::bind to pass in the 'this' pointer
        ac.sendGoal(goal,
                    boost::bind(&FSM::done_callback, this, _1, _2),
                    // MoveBaseClient::SimpleActiveCallback(),
                    boost::bind(&FSM::active_callback, this),
                    // MoveBaseClient::SimpleFeedbackCallback());
                    boost::bind(&FSM::feedback_callback, this, _1));
    }

    void done_callback(const actionlib::SimpleClientGoalState &state,
                       const MoveBaseResultConstPtr &result)
    {
        ROS_INFO("Finished in state [%s]", state.toString().c_str());
    }

    void active_callback()
    {
        ROS_DEBUG("Goal just went active");
    }

    void feedback_callback(const MoveBaseFeedbackConstPtr &feedback)
    {
        ROS_DEBUG("Got Feedback ...");
    }

    void statusCallback(const actionlib_msgs::GoalStatusArray::ConstPtr &status)
    {
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

    void batteryCallback(const std_msgs::Float32::ConstPtr &percentage)
    {
        battery_per = percentage->data;
    }

    void loop()
    {
        ros::Rate loopRate(10);

        goal.target_pose.pose.position.x = 1.5;
        goal.target_pose.pose.position.y = 1.5;
        goal.target_pose.pose.orientation.w = 0.9229966;
        goal.target_pose.pose.orientation.z = 0.3848082;

        send_goal(goal);

        while (ros::ok())
        {
            switch (robot_state)
            {
            case State::MovingToA:
                ROS_INFO_THROTTLE(2, "Moving to A...");
                ROS_DEBUG_THROTTLE(2, "Battery Voltage: %f", battery_per);
                ROS_DEBUG("goal status:%d", goal_status);

                if (goal_status == 3)
                {
                    goal.target_pose.pose.position.x = 4.0;
                    goal.target_pose.pose.position.y = 4.0;
                    goal.target_pose.pose.orientation.w = 0.3809248;
                    goal.target_pose.pose.orientation.z = -0.924606;

                    robot_state = State::MovingToB;

                    send_goal(goal);
                }
                else if (goal_status == 4)
                {
                    robot_state = State::Fail;
                }

                if (battery_per < lower_battery_limit)
                {
                    previous_state = State::MovingToA;

                    goal.target_pose.pose.position.x = 0.5;
                    goal.target_pose.pose.position.y = 0.5;
                    goal.target_pose.pose.orientation.w = 1.0;
                    goal.target_pose.pose.orientation.z = 0.0;

                    robot_state = State::MovingToDock;

                    send_goal(goal);
                }

                ros::Duration(1.0).sleep();

                break;

            case State::MovingToB:
                ROS_INFO_THROTTLE(2, "Moving to B...");
                ROS_DEBUG_THROTTLE(2, "Battery Voltage: %f", battery_per);
                ROS_DEBUG("goal status:%d", goal_status);

                if (goal_status == 3)
                {
                    goal.target_pose.pose.position.y = -2.0;
                    goal.target_pose.pose.position.x = 3.5;
                    goal.target_pose.pose.orientation.w = 0.7073883;
                    goal.target_pose.pose.orientation.z = 0.7068252;

                    robot_state = State::MovingToC;

                    send_goal(goal);
                }
                else if (goal_status == 4)
                {
                    robot_state = State::Fail;
                }

                if (battery_per < lower_battery_limit)
                {
                    previous_state = State::MovingToB;

                    goal.target_pose.pose.position.x = 0.5;
                    goal.target_pose.pose.position.y = 0.5;
                    goal.target_pose.pose.orientation.w = 1.0;
                    goal.target_pose.pose.orientation.z = 0.0;

                    robot_state = State::MovingToDock;

                    send_goal(goal);
                }

                ros::Duration(1.0).sleep();

                break;

            case State::MovingToC:
                ROS_INFO_THROTTLE(2, "Moving to C...");
                ROS_DEBUG_THROTTLE(2, "Battery Voltage: %f", battery_per);
                ROS_DEBUG("goal status:%d", goal_status);

                if (goal_status == 3)
                {
                    goal.target_pose.pose.position.x = 1.5;
                    goal.target_pose.pose.position.y = 1.5;
                    goal.target_pose.pose.orientation.w = 0.9229966;
                    goal.target_pose.pose.orientation.z = 0.3848082;

                    robot_state = State::MovingToA;

                    send_goal(goal);
                }
                else if (goal_status == 4)
                {
                    robot_state = State::Fail;
                }

                if (battery_per < lower_battery_limit)
                {
                    previous_state = State::MovingToC;

                    goal.target_pose.pose.position.x = 0.5;
                    goal.target_pose.pose.position.y = 0.5;
                    goal.target_pose.pose.orientation.w = 1.0;
                    goal.target_pose.pose.orientation.z = 0.0;

                    robot_state = State::MovingToDock;

                    send_goal(goal);
                }

                ros::Duration(1.0).sleep();

                break;

            case State::MovingToDock:
                // ROS_INFO_THROTTLE(2, "Moving to Dock...");
                ROS_INFO_THROTTLE(2, "Moving to Dock...");
                ROS_DEBUG("goal status:%d", goal_status);

                if (goal_status == 3)
                {
                    ROS_INFO("Robot reached Dock...");
                    robot_state = State::Charging;
                }
                else if (goal_status == 4)
                {
                    robot_state = State::Fail;
                }

                break;

            case State::Charging:
                // ROS_INFO_THROTTLE(2, "Moving to Dock...");
                ROS_INFO_THROTTLE(2, "Charging...");
                ROS_DEBUG("goal status:%d", goal_status);

                if (battery_per > higher_battery_limit)
                {
                    if (previous_state == State::MovingToA)
                    {

                        goal.target_pose.pose.position.x = 1.5;
                        goal.target_pose.pose.position.y = 1.5;
                        goal.target_pose.pose.orientation.w = 0.9229966;
                        goal.target_pose.pose.orientation.z = 0.3848082;

                        robot_state = State::MovingToA;

                        send_goal(goal);
                    }

                    else if (previous_state == State::MovingToB)
                    {

                        goal.target_pose.pose.position.x = 4.0;
                        goal.target_pose.pose.position.y = 4.0;
                        goal.target_pose.pose.orientation.w = 0.3809248;
                        goal.target_pose.pose.orientation.z = -0.924606;

                        robot_state = State::MovingToB;

                        send_goal(goal);
                    }

                    else if (previous_state == State::MovingToC)
                    {
                        goal.target_pose.pose.position.y = -2.0;
                        goal.target_pose.pose.position.x = 3.5;
                        goal.target_pose.pose.orientation.w = 0.7073883;
                        goal.target_pose.pose.orientation.z = 0.7068252;

                        robot_state = State::MovingToC;

                        send_goal(goal);
                    }

                    ros::Duration(1.0).sleep();
                }

                break;

            case State::Fail:
                ROS_WARN("Robot could not reach the goal, aborted by move_base...");
                ros::shutdown();
                break;
            }

            ros::spinOnce();
            loopRate.sleep();
        }
    }

private:
    ros::NodeHandle nh_;
    MoveBaseClient ac;

    ros::Subscriber status_sub_;
    ros::Subscriber battery_sub_;

    int goal_status = 0;
    float battery_per = 100;
    move_base_msgs::MoveBaseGoal goal;

    int lower_battery_limit = 30;
    int higher_battery_limit = 40;

    enum class State
    {
        MovingToA,
        MovingToB,
        MovingToC,
        MovingToDock,
        Charging,
        Fail
    };

    State robot_state = State::MovingToA;
    State previous_state = State::MovingToA;
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "switch_fsm");
    ros::NodeHandle nh;

    // if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug))
    // {
    //     ros::console::notifyLoggerLevelsChanged();
    // }

    FSM fsm(nh);

    fsm.loop();

    return 0;
}