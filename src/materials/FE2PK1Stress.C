//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "FE2PK1Stress.h"

registerMooseObject("fe2App", FE2PK1Stress);

InputParameters
FE2PK1Stress::validParams()
{
  InputParameters params = ComputeLagrangianStressPK1::validParams();
  params.addRequiredParam<UserObjectName>("pk1_stress_uo", "The name of the pk1 stress userobject");
  params.addParam<MaterialPropertyName>(
      "elasticity_tensor", "elasticity_tensor", "The name of the elasticity tensor.");
  return params;
}

FE2PK1Stress::FE2PK1Stress(const InputParameters & parameters)
  : ComputeLagrangianStressPK1(parameters),
    _uo(getUserObject<PK1StressUserObject>("pk1_stress_uo")),
    _elasticity_tensor_name(getParam<MaterialPropertyName>("elasticity_tensor")),
    _elasticity_tensor(getMaterialProperty<RankFourTensor>(_elasticity_tensor_name))
{
}

void
FE2PK1Stress::computeQpPK1Stress()
{
  _pk1_stress[_qp] = _uo.getPK1Stress(_q_point[_qp]);

  // approximate jocobian
  usingTensorIndices(i_, j_, k_, l_);
  RankFourTensor dE = 0.5 * (RankTwoTensor::Identity().times<i_, l_, j_, k_>(_F[_qp].transpose()) +
                             _F[_qp].transpose().times<i_, k_, j_, l_>(RankTwoTensor::Identity()));

  _pk1_jacobian[_qp] = RankTwoTensor::Identity().times<i_, k_, j_, l_>(
                           (_F[_qp].inverse() * _pk1_stress[_qp]).transpose()) +
                       (_elasticity_tensor[_qp] * dE).singleProductI(_F[_qp]);
}
