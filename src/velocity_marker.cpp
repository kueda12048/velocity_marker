/*!
 * \file velocity_marker.cpp
 * \brief Publishes visualization marker for velocity command in a visualization_msgs/Marker message.
 *
 * Publishes visualization marker in a visualization_msgs/Marker message based on the velocity command 
 * in a geometry_msgs/Twist.
 *
 * \author kueda
 * \date Jun 13, 2021
 */

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/TwistStamped.h>
#include <Eigen/Dense>

void sub_velocity_cb(const geometry_msgs::Twist msg);
void sub_velocity_stamped_cb(const geometry_msgs::TwistStamped msg);
void init_marker(visualization_msgs::Marker & marker, const std::string frame_id, const std::string ns);
void vector_to_marker(visualization_msgs::Marker & marker, const geometry_msgs::Vector3 vec3, const double scale);
geometry_msgs::Twist twist;

int main(int argc, char** argv)
{
	ros::init(argc, argv, "visualization_marker");
	ros::NodeHandle nh;
	ros::NodeHandle nh_priv("~");

	/// configuring parameters
	std::string base_frame;
	std::string twist_topic;
	bool is_stamped;
	double velocity_scale, anglular_velocity_scale;
	double publish_frequency;

	nh_priv.param<std::string>("base_frame", base_frame, "world");
	nh_priv.param<std::string>("twist_topic", twist_topic, "cmd_vel");
	nh_priv.param<bool>("is_stamped", is_stamped, false);
	nh_priv.param<double>("velocity_scale", velocity_scale, 1.0);
	nh_priv.param<double>("anglular_velocity_scale", anglular_velocity_scale, 1.0);
	nh_priv.param<double>("publish_frequency", publish_frequency, 10);
	
	/// publisher
	ros::Publisher pub_marker = nh.advertise<visualization_msgs::Marker>("velocity_twist_marker", 1);
	
	/// subscriber
	ros::Subscriber sub_velocity;
	if(is_stamped){
		sub_velocity = nh.subscribe(twist_topic, 1, sub_velocity_stamped_cb);
	}
	else{
		sub_velocity = nh.subscribe(twist_topic, 1, sub_velocity_cb);
	}

	/// setup markers
	visualization_msgs::Marker vel_marker, ang_vel_marker;
	init_marker(vel_marker, base_frame, "velocity");
	init_marker(ang_vel_marker, base_frame, "angular_velocity");

	ros::Rate rate(publish_frequency);
	while(nh.ok())
	{
		vector_to_marker(vel_marker, twist.linear, velocity_scale);
		pub_marker.publish(vel_marker);
		vector_to_marker(ang_vel_marker, twist.angular, anglular_velocity_scale);
		pub_marker.publish(ang_vel_marker);

		ros::spinOnce();
		rate.sleep();
	}
}

/**
 * callback for TwistStamped
 */
void sub_velocity_stamped_cb(const geometry_msgs::TwistStamped msg)
{
	twist = msg.twist;
}

/**
 * callback for Twist
 */
void sub_velocity_cb(const geometry_msgs::Twist msg)
{
	twist = msg;
}

/**
 * initialize marker
 */
void init_marker(visualization_msgs::Marker & marker, const std::string frame_id, const std::string ns)
{
		// Set the frame ID and timestamp.  See the TF tutorials for information on these.
		marker.header.frame_id = frame_id;		

		// Set the namespace and id for this marker.  This serves to create a unique ID
		// Any marker sent with the same namespace and id will overwrite the old one
		marker.ns = ns;
		marker.id = 0;

		// Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
		marker.type = visualization_msgs::Marker::ARROW;

		// Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
		marker.action = visualization_msgs::Marker::ADD;

		// Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
		marker.pose.position.x = 0;
		marker.pose.position.y = 0;
		marker.pose.position.z = 0;
		marker.pose.orientation.x = 0.0;
		marker.pose.orientation.y = 0.0;
		marker.pose.orientation.z = 0.0;
		marker.pose.orientation.w = 1.0;

		// Set the scale of the marker -- 1x1x1 here means 1m on a side
		marker.scale.x = 1.0;
		marker.scale.y = 0.1;
		marker.scale.z = 0.1;

		// Set the color -- be sure to set alpha to something non-zero!
		marker.color.r = 0.6f;
		marker.color.g = 0.6f;
		marker.color.b = 0.0f;
		marker.color.a = 1.0;

		marker.lifetime = ros::Duration();
}

/**
 * set marker parameter from trans or angular velocity vector.
 */
void vector_to_marker(visualization_msgs::Marker & marker, const geometry_msgs::Vector3 vec3, const double scale)
{
		Eigen::Vector3d	velocity_vector;
		velocity_vector << vec3.x, vec3.y, vec3.z;
		if(velocity_vector.norm() > 1e-6){
			marker.scale.x = velocity_vector.norm() * scale;
			marker.scale.y = 0.05;
			marker.scale.z = 0.05;
			/// https://eigen.tuxfamily.org/dox/classEigen_1_1Quaternion.html#acdb1eb44eb733b24749bc7892badde64
			Eigen::Quaterniond quat = Eigen::Quaterniond::FromTwoVectors(Eigen::Vector3d::UnitX(), velocity_vector);
			marker.pose.orientation.x = quat.x();
			marker.pose.orientation.y = quat.y();
			marker.pose.orientation.z = quat.z();
			marker.pose.orientation.w = quat.w();	
		}
		else{ /// velocity vector small.
			marker.scale.x = 0;
			marker.scale.y = 0;
			marker.scale.z = 0;
		}
		marker.header.stamp = ros::Time::now();
}
