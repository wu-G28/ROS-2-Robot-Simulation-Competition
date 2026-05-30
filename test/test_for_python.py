#ROS2 Python接口库
import rclpy
#ROS2节点类
from rclpy.node import Node
#时间库
import time
#ROS2节点主入口函数
def main (args=None):
	#ROS2 python接口初始化
	rclpy.init(args=args)
	#创建ROS2节点对象并进行初始化
	node = Node('node_hello_world')
	#ROS2系统是否正常运行
	while rclpy.ok():
		#ROS2日志输出
		node.get_logger().info('Hello World')

		time.sleep(0.5)
	#销毁节点对象
	nod.destroy_node()
	#关闭ROS2 python 接口
	rclpy.shutdown()
