
#include <nav_core/recovery_behavior.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <tf2_ros/buffer.h>
#include <base_local_planner/costmap_model.h>
#include <string>

namespace wait_recovery
{
    class WaitRecovery : public nav_core::RecoveryBehavior
    {
    public:
        WaitRecovery();

        void initialize(std::string name, tf2_ros::Buffer *,
                        costmap_2d::Costmap2DROS *, costmap_2d::Costmap2DROS *local_costmap);

        void runBehavior();

        ~WaitRecovery();

    private:
        bool initialized_;
        double wait_duration_;
    };
};