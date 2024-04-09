#include <wait_recovery/wait_recovery.h>
#include <pluginlib/class_list_macros.hpp>
#include <nav_core/parameter_magic.h>
#include <tf2/utils.h>
#include <ros/ros.h>
#include <string>

PLUGINLIB_EXPORT_CLASS(wait_recovery::WaitRecovery, nav_core::RecoveryBehavior)

namespace wait_recovery
{
    WaitRecovery::WaitRecovery() : initialized_(false)
    {
    }

    void WaitRecovery::initialize(std::string name, tf2_ros::Buffer *,
                                  costmap_2d::Costmap2DROS *, costmap_2d::Costmap2DROS *local_costmap)
    {
        if (!initialized_)
        {

            ros::NodeHandle private_nh("~");

            private_nh.param("wait_duration", wait_duration_, 5.0);

            initialized_ = true;
        }
        else
        {
            ROS_ERROR("You should not call initialize twice on this object, doing nothing");
        }
    }

    WaitRecovery::~WaitRecovery()
    {
    }

    void WaitRecovery::runBehavior()
    {
        ROS_WARN("Waiting for %.2f second(s)", wait_duration_);
        ros::Duration(wait_duration_).sleep();
        ROS_WARN("Resuming Task...");
    }
}