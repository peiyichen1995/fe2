//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "CustomStVenantKirchhoffStress.h"

registerMooseObject("fe2App", CustomStVenantKirchhoffStress);

InputParameters
CustomStVenantKirchhoffStress::validParams()
{
  InputParameters params = ComputeLagrangianStressPK2::validParams();

  params.addParam<MaterialPropertyName>(
      "elasticity_tensor", "elasticity_tensor", "The name of the elasticity tensor.");
  params.addRequiredParam<UserObjectName>(
      "f_pk1_uo", "The name of the deformation gradient and pk1 stress userobject");
  return params;
}

CustomStVenantKirchhoffStress::CustomStVenantKirchhoffStress(const InputParameters & parameters)
  : ComputeLagrangianStressPK2(parameters),
    GuaranteeConsumer(this),
    _elasticity_tensor_name(getParam<MaterialPropertyName>("elasticity_tensor")),
    _elasticity_tensor(getMaterialProperty<RankFourTensor>(_elasticity_tensor_name)),
    _uo_F_pk1(const_cast<DeformationGradientPK1StressUserObject &>(
        getUserObject<DeformationGradientPK1StressUserObject>("f_pk1_uo")))
{
}

void
CustomStVenantKirchhoffStress::initialSetup()
{
  // Enforce isotropic elastic tensor
  if (!hasGuaranteedMaterialProperty(_elasticity_tensor_name, Guarantee::ISOTROPIC))
    mooseError("CustomStVenantKirchhoffStress requires an isotropic elasticity "
               "tensor");
}

void
CustomStVenantKirchhoffStress::computeQpPK2Stress()
{
  // Hyperelasticity is weird, we need to branch on the type of update if we
  // want a truly linear model
  //
  // This is because we need to drop quadratic terms for the linear update to
  // use a linear strain measure

  // Jacobian is the same for both the small and Green-Lagrange strains
  _C[_qp] = _elasticity_tensor[_qp];

  // Get the right strain
  RankTwoTensor strain;
  if (_large_kinematics) // Large deformations = Green-Lagrange strain
    strain = _E[_qp];
  else // Small deformations = linear strain
    strain = 0.5 * (_F[_qp] + _F[_qp].transpose());

  // The stress update is linear with the correct strains/frame
  _S[_qp] = _C[_qp] * strain;

  // update F and PK1
  dof_id_type element_id = _current_elem->id();
  _uo_F_pk1.F_P[element_id].resize(_q_point.size());
  _uo_F_pk1.F_P[element_id][_qp] = {_F[_qp], _S[_qp]};
}
