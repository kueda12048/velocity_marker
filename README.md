# About
速度指令([geometry_msgs/Twist](http://docs.ros.org/en/melodic/api/geometry_msgs/html/msg/Twist.html)または[geometry_msgs/TwistStamped](http://docs.ros.org/en/melodic/api/geometry_msgs/html/msg/TwistStamped.html))をrvizで表示するため、visualization marker([visualization_msgs::Marker](http://docs.ros.org/en/melodic/api/visualization_msgs/html/msg/Marker.html))を発行する。

# Nodes

## velocity_marker
### Subscribed Topics
* `cmd_vel` (パラメータ `is_stamped`がfalseで[geometry_msgs/Twist](http://docs.ros.org/en/melodic/api/geometry_msgs/html/msg/Twist.html)、trueで [geometry_msgs/TwistStamped](http://docs.ros.org/en/melodic/api/geometry_msgs/html/msg/TwistStamped.html))
### Published Topics
* `velocity_twist_marker` ([visualization_msgs::Marker](http://docs.ros.org/en/melodic/api/visualization_msgs/html/msg/Marker.html))
### Parameters

* `~base_frame`  (string, default "world") 
	
	発行するvisualization markerのframe_id。

* `~is_stamped` (bool, default false) 
	
	falseで[geometry_msgs/Twist](http://docs.ros.org/en/melodic/api/geometry_msgs/html/msg/Twist.html)を、trueで [geometry_msgs/TwistStamped](http://docs.ros.org/en/melodic/api/geometry_msgs/html/msg/TwistStamped.html)を購読。

* `~velocity_scale` (double, default 1.0)

	visualization markerの矢印の長さ(m) = `~velocity_scale` * 速度(m/s)

* `~anglular_velocity_scale` (double, default 1.0)

	visualization markerの矢印の長さ(m) = `~anglular_velocity_scale` * 角速度(rad/s)

* `~publish_frequency` (double, default 10.0)

	発行周期(Hz)

# Usage

```
rosrun velocity_marker velocity_marker
```
