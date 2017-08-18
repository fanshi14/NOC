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

#ifndef LQR_FINITE_DISCRETE_CONTROLLER_QUADROTOR_H
#define LQR_FINITE_DISCRETE_CONTROLLER_QUADROTOR_H

#include <lqr_control/LqrDiscreteBase.h>
namespace lqr_discrete{
#define P_X 0
#define P_Y 1
#define P_Z 2
#define V_X 3
#define V_Y 4
#define V_Z 5
#define W_X 6
#define W_Y 7
#define W_Z 8
#define Q_W 9
#define Q_X 10
#define Q_Y 11
#define Q_Z 12
#define E_X 9
#define E_Y 10
#define E_Z 11
#define U_1 0
#define U_2 1
#define U_3 2
#define U_4 3
  class LqrFiniteDiscreteControlQuadrotor: public LqrDiscreteControlBase{
  public:
    LqrFiniteDiscreteControlQuadrotor(ros::NodeHandle nh, ros::NodeHandle nhp):
      LqrDiscreteControlBase(nh, nhp){};
    MatrixXd *I_ptr_;
    MatrixXd *M_para_ptr_;
    double uav_mass_;
    bool debug_;
    std::vector<MatrixXd> F_vec_;
    void initLQR(double freq, double period, VectorXd *x0, VectorXd *xn);
    void updateMatrixA();
    void updateMatrixB();
    void updateMatrixAB();
    void updateAll();
    void updateNewState(VectorXd *new_x_ptr);
    void normalizeQuaternion(VectorXd *new_x_ptr);
  };
}


#endif
