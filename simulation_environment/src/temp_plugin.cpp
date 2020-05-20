#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ros/ros.h>
#include "sensor_msgs/Temperature.h"
#include <string.h>

namespace gazebo
{
  class TempSensor : public ModelPlugin
  {
	  
	// Pointer to the model
    private: physics::ModelPtr model;
    
    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
    
    // Handle for the gazebo ros node
    private: std::unique_ptr<ros::NodeHandle> rosNode;
    			
    // ROS publisher
    ros::Publisher rosPub;
	  
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
    {			
      // Store the pointer to the model
      this->model = _parent;
      
	  // Make sure the ROS node for Gazebo has already been initialized  
	  if (!ros::isInitialized())
      {
		ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
			<< "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
		return;
      }
      
      // Reset the ros node name and initialize subscriber and publisher
      this->rosNode.reset(new ros::NodeHandle(""));
      rosPub = this->rosNode->advertise<sensor_msgs::Temperature>("temperature", 10);
      
      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          std::bind(&TempSensor::OnUpdate, this));
    }

    // Called by the world update start event
    public: void OnUpdate()
    {
	  // Get pose
	  ignition::math::Pose3d pose = model->WorldPose();
      // Publish to rostopic temperature
      sensor_msgs::Temperature msg;
      msg.temperature = 20;
      msg.variance = 0;
      this->rosPub.publish(msg);   
    }
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(TempSensor)
}
