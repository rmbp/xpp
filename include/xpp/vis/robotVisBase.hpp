/**
 @file    robotVisBase.hpp
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Jun 24, 2015
 @brief   Base class for robot visualization using rviz

          Based of the code provided by : Diego Pardo
 */

#ifndef ROBOT_VIS_BASE_H_
#define ROBOT_VIS_BASE_H_

#include <cstdlib>
#include <iostream>
#include <string>
#include <memory>

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <ros/package.h>
#include <urdf/model.h>

#include <sensor_msgs/JointState.h>
#include <robot_state_publisher/robot_state_publisher.h>
#include <kdl_parser/kdl_parser.hpp>

#include <xpp_msgs/HyqStateTrajectory.h>
#include <xpp_msgs/CurrentInfo.h>

namespace xpp {
namespace vis {

template <size_t NJOINTS>
class robotVisBase {
public:
  using TrajectoryMsg     = xpp_msgs::HyqStateTrajectory;
  using CurrentInfoMsg    = xpp_msgs::CurrentInfo;
  using NameJointAngleMap = std::map<std::string, double>;

  // this has to be built for publishing in rviz with the correct name of each joint
  NameJointAngleMap model_joint_positions_;

  robotVisBase(std::string my_robot_name, const std::array<std::string, NJOINTS>& my_robot_joints);
  virtual ~robotVisBase();

  void init();
  void setRobotJointNames(const std::array<std::string, NJOINTS>& my_robot_joints,size_t array_size);


protected:
  std::array<std::string, NJOINTS> robot_joint_names;


private:
  static const size_t base_dof = 6;
  double playbackSpeed_;

  ros::Subscriber state_sub_; /// gets joint states, floating base and stance estimation
  ros::Subscriber traj_sub_; /// gets joint states, floating base and stance estimation
  tf::TransformBroadcaster broadcaster;
  std::shared_ptr<robot_state_publisher::RobotStatePublisher> robot_state_publisher;

  void stateCallback(const CurrentInfoMsg::ConstPtr& msg);
  void trajectoryCallback(const TrajectoryMsg::ConstPtr& msg);
  void visualizeState(const ros::Time& stamp, const geometry_msgs::Pose& baseState, const sensor_msgs::JointState& jointState);
  virtual void setRobotJointsFromMessage(const sensor_msgs::JointState &msg, NameJointAngleMap& model_joint_positions) = 0;
  void setRobotBaseStateFromMessage(const geometry_msgs::Pose &msg, geometry_msgs::TransformStamped& W_X_B_message);
  void setZeroState();
};

} // namespace vis
} // namespace xpp

#include "implementation/robotVisBase.hpp"

#endif /* ROBOT_VIS_BASE_H_ */
