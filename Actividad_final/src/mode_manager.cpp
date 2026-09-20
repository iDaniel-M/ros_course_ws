#include <chrono>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class ModeManager : public rclcpp::Node
{
public:
    ModeManager() : Node("mode_manager")
    {
        mode_ = "manual";

        mode_pub_ = create_publisher<std_msgs::msg::String>(
            "/turtle_mode", 10);

        cancel_pub_ = create_publisher<std_msgs::msg::String>(
            "/trajectory_cancel", 10);

        mode_sub_ = create_subscription<std_msgs::msg::String>(
            "/set_mode",
            10,
            std::bind(&ModeManager::modeCallback, this, std::placeholders::_1));

        timer_ = create_wall_timer(
            500ms,
            std::bind(&ModeManager::publishMode, this));

        RCLCPP_INFO(get_logger(), "Mode Manager iniciado");
        RCLCPP_INFO(get_logger(), "Modo inicial: manual");
    }

private:

    void publishMode()
    {
        std_msgs::msg::String msg;
        msg.data = mode_;
        mode_pub_->publish(msg);
    }

    void modeCallback(const std_msgs::msg::String::SharedPtr msg)
    {
        if(msg->data == "cancel")
        {
            std_msgs::msg::String cancel;
            cancel.data = "cancel";
            cancel_pub_->publish(cancel);

            RCLCPP_INFO(get_logger(),"Cancelación enviada");
            return;
        }

        if(msg->data=="manual" ||
           msg->data=="circle_cw" ||
           msg->data=="circle_ccw" ||
           msg->data=="trajectory")
        {
            mode_=msg->data;
            RCLCPP_INFO(get_logger(),"Modo cambiado a %s",mode_.c_str());
        }
        else
        {
            RCLCPP_WARN(get_logger(),"Modo inválido");
        }
    }

    std::string mode_;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr mode_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr cancel_pub_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr mode_sub_;

    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc,char **argv)
{
    rclcpp::init(argc,argv);

    rclcpp::spin(std::make_shared<ModeManager>());

    rclcpp::shutdown();

    return 0;
}