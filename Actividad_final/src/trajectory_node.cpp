#include <chrono>
#include <cmath>
#include <vector>
#include <utility>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/string.hpp"
#include "turtlesim/msg/pose.hpp"

using namespace std::chrono_literals;

class TrajectoryNode : public rclcpp::Node
{
public:
    TrajectoryNode() : Node("trajectory_node")
    {
        mode_ = "manual";
        cancel_requested_ = false;
        current_point_ = 0;
        pose_received_ = false;

        // Puntos de la trayectoria
        waypoints_ = {
            {2.0, 2.0},
            {9.0, 2.0},
            {5.5, 8.0}
        };

        cmd_pub_ = create_publisher<geometry_msgs::msg::Twist>(
            "/turtle1/cmd_vel", 10);

        set_mode_pub_ = create_publisher<std_msgs::msg::String>(
            "/set_mode", 10);

        mode_sub_ = create_subscription<std_msgs::msg::String>(
            "/turtle_mode",
            10,
            std::bind(&TrajectoryNode::modeCallback, this, std::placeholders::_1));

        cancel_sub_ = create_subscription<std_msgs::msg::String>(
            "/trajectory_cancel",
            10,
            std::bind(&TrajectoryNode::cancelCallback, this, std::placeholders::_1));

        pose_sub_ = create_subscription<turtlesim::msg::Pose>(
            "/turtle1/pose",
            10,
            std::bind(&TrajectoryNode::poseCallback, this, std::placeholders::_1));

        timer_ = create_wall_timer(
            50ms,
            std::bind(&TrajectoryNode::controlLoop, this));

        RCLCPP_INFO(get_logger(), "Trajectory Node iniciado");
    }

private:

    void poseCallback(const turtlesim::msg::Pose::SharedPtr msg)
    {
        pose_ = *msg;
        pose_received_ = true;
    }

    void modeCallback(const std_msgs::msg::String::SharedPtr msg)
    {
        if(msg->data=="trajectory" && mode_!="trajectory")
        {
            current_point_=0;
            cancel_requested_=false;
            RCLCPP_INFO(get_logger(),"Trayectoria iniciada");
        }

        mode_=msg->data;
    }

    void cancelCallback(const std_msgs::msg::String::SharedPtr msg)
    {
        if(msg->data=="cancel")
        {
            cancel_requested_=true;
        }
    }

    void returnManual()
    {
        std_msgs::msg::String msg;
        msg.data="manual";
        set_mode_pub_->publish(msg);
        mode_="manual";
    }

    void stopRobot()
    {
        geometry_msgs::msg::Twist vel;
        cmd_pub_->publish(vel);
    }

    void controlLoop()
    {
        if(!pose_received_)
            return;

        if(mode_!="trajectory")
            return;

        if(cancel_requested_)
        {
            stopRobot();
            returnManual();
            RCLCPP_INFO(get_logger(),"Trayectoria cancelada");
            return;
        }

        auto goal=waypoints_[current_point_];

        double dx=goal.first-pose_.x;
        double dy=goal.second-pose_.y;

        double distance=sqrt(dx*dx+dy*dy);

        if(distance<0.20)
        {
            RCLCPP_INFO(get_logger(),
                "Punto %d alcanzado",
                current_point_+1);

            current_point_++;

            if(current_point_>=waypoints_.size())
            {
                stopRobot();
                returnManual();

                RCLCPP_INFO(get_logger(),
                    "Trayectoria finalizada");

                return;
            }

            return;
        }

        double desired_angle=atan2(dy,dx);
        double error=desired_angle-pose_.theta;

        while(error>M_PI) error-=2*M_PI;
        while(error<-M_PI) error+=2*M_PI;

        geometry_msgs::msg::Twist vel;

        vel.linear.x=std::min(2.0,distance);
        vel.angular.z=4.0*error;

        cmd_pub_->publish(vel);
    }

    std::string mode_;
    bool cancel_requested_;
    bool pose_received_;

    turtlesim::msg::Pose pose_;

    size_t current_point_;

    std::vector<std::pair<double,double>> waypoints_;

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr set_mode_pub_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr mode_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr cancel_sub_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;

    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc,char **argv)
{
    rclcpp::init(argc,argv);

    rclcpp::spin(std::make_shared<TrajectoryNode>());

    rclcpp::shutdown();

    return 0;
}