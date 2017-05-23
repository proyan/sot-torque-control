/*
 * Copyright 2017, A. Del Prete, T. Flayols, O. Stasse, LAAS-CNRS
 *
 * This file is part of sot-torque-control.
 * sot-torque-control is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * sot-torque-control is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with sot-torque-control.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef __sot_torque_control_common_H__
#define __sot_torque_control_common_H__

/* --------------------------------------------------------------------- */
/* --- API ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if defined (WIN32)
#  if defined (hrp2_common_EXPORTS)
#    define HRP2COMMON_EXPORT __declspec(dllexport)
#  else
#    define HRP2COMMON_EXPORT __declspec(dllimport)
#  endif
#else
#  define HRP2COMMON_EXPORT
#endif


/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */
#include <dynamic-graph/linear-algebra.h>
#include <sot/torque_control/signal-helper.hh>
#include <sot/torque_control/utils/vector-conversions.hh>
#include <map>
#include <initializer_list>
#include "boost/assign.hpp"
#include <sot/torque_control/utils/logger.hh>
namespace dg = ::dynamicgraph;
using namespace dg;


namespace dynamicgraph {
  namespace sot {
    namespace torque_control {

      struct JointLimits
      {
        double upper;
        double lower;

        JointLimits():
          upper(0.0),
          lower(0.0)
        {}

        JointLimits(double l, double u):
          upper(u),
          lower(l)
        {}
      };
      
      struct FromURDFToSoT
      {
	typedef Eigen::VectorXd::Index Index;
      protected:

	/// Map from the urdf index to the SoT index.
	std::vector<Index> m_urdf_to_sot;
	
	/// Nb of Dofs for the robot.
        long unsigned int m_nbJoints;

	/// Map from the name to the id.
	std::map<std::string,Index> m_name_to_id;

	/// The map between id and name
	std::map<Index,std::string> m_id_to_name;

	/// The joint limits map.
	std::map<Index,JointLimits> m_limits_map;

	/// This method creates the map between id and name.
	/// It is called each time a new link between id and name is inserted 
	/// (i.e. when set_name_to_id is called).
	void create_id_to_name_map();


      public:
	dynamicgraph::Vector m_dgv_urdf_to_sot;

        /** Given a joint name it finds the associated joint id.
         * If the specified joint name is not found it returns -1;
         * @param name Name of the joint to find.
         * @return The id of the specified joint, -1 if not found. */
	const Index get_id_from_name(const std::string &name);

	/** Given a joint id it finds the associated joint name.
         * If the specified joint is not found it returns "Joint name not found";
         * @param id Id of the joint to find.
         * @return The name of the specified joint, "Joint name not found" if not found. */
	
	/// Get the joint name from its index
        const std::string & get_name_from_id(Index id);

	/// Set relation between the name and the SoT id
	void set_name_to_id(const std::string &jointName,
			    const double & jointId);

	/// Set the map between urdf index and sot index
	void set_urdf_to_sot(const std::vector<Index> &urdf_to_sot);
	void set_urdf_to_sot(const dg::Vector &urdf_to_sot);
	
	/// Set the limits (lq,uq) for joint idx
	void set_joint_limits_for_id(const double &idx,
				     const double &lq,
				     const double &uq);

	bool joints_urdf_to_sot(Eigen::ConstRefVector q_urdf, Eigen::RefVector q_sot);

	bool joints_sot_to_urdf(Eigen::ConstRefVector q_sot, Eigen::RefVector q_urdf);
	
	/** Given a joint id it finds the associated joint limits.
         * If the specified joint is not found it returns JointLimits(0,0).
         * @param id Id of the joint to find.
         * @return The limits of the specified joint, JointLimits(0,0) if not found. */
        const JointLimits & get_limits_from_id(Index id);

	void sendMsg(const std::string& msg, 
		     MsgType t=MSG_TYPE_INFO, 
		     const char* file="", int line=0)
	{
          getLogger().sendMsg("[FromURDFToSoT] "+msg, t, file, line);
	}

      }; // struct FromURDFToSoT


      bool base_se3_to_sot(Eigen::ConstRefVector pos,
                           Eigen::ConstRefMatrix R,
                           Eigen::RefVector q_sot);
      bool base_urdf_to_sot(Eigen::ConstRefVector q_urdf, Eigen::RefVector q_sot);
      bool base_sot_to_urdf(Eigen::ConstRefVector q_sot, Eigen::RefVector q_urdf);
      bool config_urdf_to_sot(Eigen::ConstRefVector q_urdf, Eigen::RefVector q_sot);
      bool config_sot_to_urdf(Eigen::ConstRefVector q_sot, Eigen::RefVector q_urdf);
      bool velocity_urdf_to_sot(Eigen::ConstRefVector v_urdf, Eigen::RefVector v_sot);
      bool velocity_sot_to_urdf(Eigen::ConstRefVector v_sot, Eigen::RefVector v_urdf);
      bool joints_urdf_to_sot(Eigen::ConstRefVector q_urdf, Eigen::RefVector q_sot);
      bool joints_sot_to_urdf(Eigen::ConstRefVector q_sot, Eigen::RefVector q_urdf);

    }    // namespace torque_control
  }      // namespace sot
}        // namespace dynamicgraph

#endif // sot_torque_control_common_h_