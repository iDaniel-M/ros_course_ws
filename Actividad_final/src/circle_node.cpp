#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class CircleNode : public rclcpp::Node
{
public:
    CircleNode() : Node("circle_node")
    {
        mode_ = "manual";

        cmd_pub_ = create_publisher<geometry_msgs::msg::Twist>(
            "/turtle1/cmd_vel",10);

        mode_sub_ = create_subscription<std_msgs::msg::String>(
            "/turtle_mode",
            10,
            std::bind(&CircleNode::modeCallback,this,std::placeholders::_1));

        timer_ = create_wall_timer(
            100ms,
            std::bind(&CircleNode::controlLoop,this));

        RCLCPP_INFO(get_logger(),"Circle Node iniciado");
    }

private:

    void modeCallback(const std_msgs::msg::String::SharedPtr msg)
    {
        mode_=msg->data;
    }

    void controlLoop()
    {
        geometry_msgs::msg::Twist vel;

        if(mode_=="circle_ccw")
        {
            vel.linear.x=2.0;
            vel.angular.z=1.0;
        }
        else if(mode_=="circle_cw")
        {
            vel.linear.x=2.0;
            vel.angular.z=-1.0;
        }

        cmd_pub_->publish(vel);
    }

    std::string mode_;

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr mode_sub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc,char **argv)
{
    rclcpp::init(argc,argv);

    rclcpp::spin(std::make_shared<CircleNode>());

    rclcpp::shutdown();

    return 0;
}