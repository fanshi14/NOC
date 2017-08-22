// -*- mode: c++ -*-
/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2016, JSK Lab
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/o2r other materials provided
 *     with the distribution.
 *   * Neither the name of the JSK Lab nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

#include <lqr_control/QuadrotorSimulator.h>
namespace quadrotor_simulator{
  QuadrotorSimulator::QuadrotorSimulator(ros::NodeHandle nh, ros::NodeHandle nhp): nh_(nh), nhp_(nhp){
    // controller_ptr_ = new LqrFiniteDiscreteControlQuadrotor(nh_, nhp_);
    controller_ptr_ = new SlqFiniteDiscreteControlQuadrotor(nh_, nhp_);

    pub_traj_path_ = nh_.advertise<nav_msgs::Path>("lqr_path", 1);
    pub_traj_end_points_ = nh_.advertise<visualization_msgs::MarkerArray>("end_points_markers", 1);
    sleep(1.0);
  }

  void QuadrotorSimulator::initQuadrotorSimulator(VectorXd *start_state_ptr, VectorXd *end_state_ptr, double period, double controller_freq){
    start_state_ptr_ = start_state_ptr;
    end_state_ptr_ = end_state_ptr;
    period_ = period;
    controller_freq_ = controller_freq;
    // controller_ptr_->initLQR(controller_freq_, period_, start_state_ptr_, end_state_ptr_);
    controller_ptr_->initSLQ(controller_freq_, period_, start_state_ptr_, end_state_ptr_);
    visualizeTrajectory();
    ROS_INFO("[QuadrotorSimulator] init finished.");
  }

  void QuadrotorSimulator::planOptimalTrajectory(){
    // lqr
    // controller_ptr_->updateAll();
    // std::cout << "[QuadrotorSimulator] updateAll finished\n";

    // slq
    while (1){
      std::cout << "[press 1 to continue, 2 to break]\n";
      int id;
      std::cin >> id;
      if (id == 2)
        break;
      controller_ptr_->iterativeOptimization();
      visualizeTrajectory();
    }
  }

  void QuadrotorSimulator::visualizeTrajectory(){
    traj_.poses.clear();
    visualization_msgs::MarkerArray end_points_markers;
    visualization_msgs::Marker point_marker;
    point_marker.ns = "end_points";
    point_marker.header.frame_id = std::string("/world");
    point_marker.header.stamp = ros::Time().now();
    point_marker.action = visualization_msgs::Marker::ADD;
    point_marker.type = visualization_msgs::Marker::SPHERE;

    point_marker.id = 0;
    point_marker.pose.position.x = (*start_state_ptr_)(0);
    point_marker.pose.position.y = (*start_state_ptr_)(1);
    point_marker.pose.position.z = (*start_state_ptr_)(2);
    point_marker.pose.orientation.x = 0.0;
    point_marker.pose.orientation.y = 0.0;
    point_marker.pose.orientation.z = 0.0;
    point_marker.pose.orientation.w = 1.0;
    point_marker.scale.x = 0.7;
    point_marker.scale.y = 0.7;
    point_marker.scale.z = 0.7;
    point_marker.color.a = 1;
    point_marker.color.r = 0.0f;
    point_marker.color.g = 1.0f;
    point_marker.color.b = 0.0f;
    end_points_markers.markers.push_back(point_marker);

    point_marker.id = 1;
    point_marker.pose.position.x = (*end_state_ptr_)(0);
    point_marker.pose.position.y = (*end_state_ptr_)(1);
    point_marker.pose.position.z = (*end_state_ptr_)(2);
    point_marker.color.r = 1.0f;
    point_marker.color.g = 0.0f;
    point_marker.color.b = 0.0f;
    end_points_markers.markers.push_back(point_marker);

    pub_traj_end_points_.publish(end_points_markers);

    traj_.header.frame_id = std::string("/world");
    traj_.header.stamp = ros::Time().now();
    geometry_msgs::PoseStamped pose_stamped;
    pose_stamped.header = traj_.header;
    pose_stamped.pose.orientation.x = 0.0f;
    pose_stamped.pose.orientation.y = 0.0f;
    pose_stamped.pose.orientation.z = 0.0f;
    pose_stamped.pose.orientation.w = 1.0f;
    for (int i = 0; i < controller_ptr_->x_vec_.size(); ++i){
      VectorXd result = controller_ptr_->getAbsoluteState(&(controller_ptr_->x_vec_[i]));
      pose_stamped.pose.position.x = (result)(0);
      pose_stamped.pose.position.y = (result)(1);
      pose_stamped.pose.position.z = (result)(2);
      traj_.poses.push_back(pose_stamped);
    }
    pub_traj_path_.publish(traj_);
  }
}


