import math
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from std_srvs.srv import SetBool

class TurtleController(Node):
    def __init__(self):
        super().__init__('turtle_controller')
        
        # Publicador de velocidad
        self.cmd_vel_pub = self.create_publisher(Twist, '/turtle1/cmd_vel', 10)
        
        # Servicio para cambiar modo de operacion (True = Circulos, False = Manual)
        self.srv = self.create_service(SetBool, 'set_mode', self.change_mode_callback)
        
        # Modo por defecto: 0 = MANUAL, 1 = CIRCULOS
        self.mode = 0  
        self.clockwise = False
        
        # Timer de control (10 Hz)
        self.timer = self.create_timer(0.1, self.control_loop)
        self.get_logger().info('Nodo TurtleController iniciado. Modo inicial: MANUAL')

    def change_mode_callback(self, request, response):
        if request.data:
            self.mode = 1  # Modo Circulos
            response.success = True
            response.message = "Modo cambiado a: AUTONOMO CIRCULOS"
        else:
            self.mode = 0  # Modo Manual
            response.success = True
            response.message = "Modo cambiado a: MANUAL"
        
        self.get_logger().info(response.message)
        return response

    def control_loop(self):
        if self.mode == 1:
            # Modo Autónomo Círculos
            msg = Twist()
            msg.linear.x = 2.0
            msg.angular.z = -1.0 if self.clockwise else 1.0
            self.cmd_vel_pub.publish(msg)
        elif self.mode == 0:
            # Modo Manual: No publica en cmd_vel para permitir uso de teleop/rqt
            pass

def main(args=None):
    rclpy.init(args=args)
    node = TurtleController()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
