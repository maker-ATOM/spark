#include <ros/ros.h>
#include <std_msgs/Int8.h>

class State
{
public:
    State(ros::NodeHandle &nh) : _nh(nh)
    {
        _state_pub = nh.advertise<std_msgs::Int8>("state", 10);
    }

    void publish()
    {
        std_msgs::Int8 data;
        data.data = state;
        state ++;
        _state_pub.publish(data);
    }

private:
    ros::NodeHandle _nh;
    ros::Publisher _state_pub;

    uint8_t state = 0;
};

int main(int argc, char **argv)
{

    ros::init(argc, argv, "switch_fsm");
    ros::NodeHandle nh;

    State state(nh);

    ros::Rate loopRate(1);

    while (ros::ok())
    {
        state.publish();

        ros::spinOnce();
        loopRate.sleep();
    }

    return 0;
}