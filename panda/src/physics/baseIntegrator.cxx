// Filename: baseIntegrator.cxx
// Created by:  charles (11Aug00)
// 
////////////////////////////////////////////////////////////////////

#include "baseIntegrator.h"
#include "physicalNode.h"
#include "forceNode.h"

#include <get_rel_pos.h>

////////////////////////////////////////////////////////////////////
//    Function : BaseIntegrator
//      Access : protected
// Description : constructor
////////////////////////////////////////////////////////////////////
BaseIntegrator::
BaseIntegrator(void) {
}

////////////////////////////////////////////////////////////////////
//    Function : ~BaseIntegrator
//      Access : public, virtual
// Description : destructor
////////////////////////////////////////////////////////////////////
BaseIntegrator::
~BaseIntegrator(void) {
}

////////////////////////////////////////////////////////////////////
//    Function : precompute_linear_matrices
//      Access : protected
// Description : effectively caches the xform matrices between
//               the physical's node and every force acting on it
//               so that each PhysicsObject in the set held by the
//               Physical doesn't have to wrt.
////////////////////////////////////////////////////////////////////
void BaseIntegrator::
precompute_linear_matrices(Physical *physical, 
                           const vector< PT(LinearForce) > &forces) {
  // make sure the physical's in the scene graph, somewhere.
  PhysicalNode *physical_node = physical->get_physical_node();
  nassertv(physical_node != NULL);

  // by global forces, we mean forces not contained in the physical
  int global_force_vec_size = forces.size();

  // by local forces, we mean members of the physical's force set.
  int local_force_vec_size = physical->get_linear_forces().size();
  int index = 0, i;

  LMatrix4f current_xform;
  ForceNode *force_node;

  // prepare the vector
  _precomputed_linear_matrices.clear();
  _precomputed_linear_matrices.reserve(global_force_vec_size + local_force_vec_size);

  // tally the global xforms
  for (i = 0; i < global_force_vec_size; i++) {
    force_node = forces[i]->get_force_node();
    nassertv(force_node != (ForceNode *) NULL);

    get_rel_mat(physical_node, force_node, current_xform);
    _precomputed_linear_matrices[index++] = current_xform;
  }

  const vector< PT(LinearForce) > &force_vector = 
    physical->get_linear_forces();

  // tally the local xforms
  for (i = 0; i < local_force_vec_size; i++) {
    force_node = force_vector[i]->get_force_node();
    nassertv(force_node != (ForceNode *) NULL);

    get_rel_mat(physical_node, force_node, current_xform);
    _precomputed_linear_matrices[index++] = current_xform;
  }
}

////////////////////////////////////////////////////////////////////
//    Function : precompute_angular_matrices
//      Access : protected
// Description : effectively caches the xform matrices between
//               the physical's node and every force acting on it
//               so that each PhysicsObject in the set held by the
//               Physical doesn't have to wrt.
////////////////////////////////////////////////////////////////////
void BaseIntegrator::
precompute_angular_matrices(Physical *physical, 
                            const vector< PT(AngularForce) > &forces) {
  // make sure the physical's in the scene graph, somewhere.
  PhysicalNode *physical_node = physical->get_physical_node();
  nassertv(physical_node != NULL);

  // by global forces, we mean forces not contained in the physical
  int global_force_vec_size = forces.size();

  // by local forces, we mean members of the physical's force set.
  int local_force_vec_size = physical->get_angular_forces().size();
  int index = 0, i;

  LMatrix4f current_xform;
  ForceNode *force_node;

  // prepare the vector
  _precomputed_angular_matrices.clear();
  _precomputed_angular_matrices.reserve(global_force_vec_size + local_force_vec_size);

  // tally the global xforms
  for (i = 0; i < global_force_vec_size; i++) {
    force_node = forces[i]->get_force_node();
    nassertv(force_node != (ForceNode *) NULL);

    get_rel_mat(physical_node, force_node, current_xform);
    _precomputed_angular_matrices[index++] = current_xform;
  }

  const vector< PT(AngularForce) > &force_vector = 
    physical->get_angular_forces();

  // tally the local xforms
  for (i = 0; i < local_force_vec_size; i++) {
    force_node = force_vector[i]->get_force_node();
    nassertv(force_node != (ForceNode *) NULL);

    get_rel_mat(physical_node, force_node, current_xform);
    _precomputed_angular_matrices[index++] = current_xform;
  }
}
