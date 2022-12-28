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
  params.addRequiredParam<UserObjectName>("fe2_uo", "The name of the fe2 userobject");
  return params;
}

FE2PK1Stress::FE2PK1Stress(const InputParameters & parameters)
  : ComputeLagrangianStressPK1(parameters),
    _uo(const_cast<FE2UserObject &>(getUserObject<FE2UserObject>("fe2_uo")))
{
}

void
FE2PK1Stress::computeQpPK1Stress()
{
  _console << "-----------------------------------\n";
  _console << "At point " << _q_point[_qp] << std::endl;

  computeQpMicroscaleTranslation();

  const RankTwoTensor F = _F[_qp];
  _pk1_stress[_qp] = microscalePK1Stress(F);

  if (_fe_problem.currentlyComputingJacobian())
  {

    // Finite difference jacobian
    // Assume 2D, that is, only F(0, 0), F(0, 1), F(1, 0), F(1, 1) != 0
    // But we only need to do symmetric perturbation

    // machine eps = 10^-12
    Real eps = 1e-6;
    RankTwoTensor F_eps;

    // Perturb Fxx
    F_eps = F;
    Real Fxx_eps = std::max(eps * F(0, 0), eps);
    // Real Fxx_eps = eps;
    F_eps(0, 0) += Fxx_eps;
    auto dPxx = microscalePK1Stress(F_eps) - _pk1_stress[_qp];

    // Perturb Fxy
    F_eps = F;
    Real Fxy_eps = std::max(eps * F(0, 1), eps);
    // Real Fxy_eps = eps;
    F_eps(0, 1) += Fxy_eps;
    auto dPxy = microscalePK1Stress(F_eps) - _pk1_stress[_qp];

    // Perturb Fyx
    F_eps = F;
    Real Fyx_eps = std::max(eps * F(1, 0), eps);
    // Real Fyx_eps = eps;
    F_eps(1, 0) += Fyx_eps;
    auto dPyx = microscalePK1Stress(F_eps) - _pk1_stress[_qp];

    // Perturb Fyy
    F_eps = F;
    Real Fyy_eps = std::max(eps * F(1, 1), eps);
    // Real Fyy_eps = eps;
    F_eps(1, 1) += Fyy_eps;
    auto dPyy = microscalePK1Stress(F_eps) - _pk1_stress[_qp];

    // Approximate jacobian
    _pk1_jacobian[_qp].zero();
    for (auto i : make_range(RankFourTensor::N))
      for (auto j : make_range(RankFourTensor::N))
      {
        _pk1_jacobian[_qp](i, j, 0, 0) = dPxx(i, j) / Fxx_eps;
        _pk1_jacobian[_qp](i, j, 0, 1) = dPxy(i, j) / Fxy_eps;
        _pk1_jacobian[_qp](i, j, 1, 0) = dPyx(i, j) / Fyx_eps;
        _pk1_jacobian[_qp](i, j, 1, 1) = dPyy(i, j) / Fyy_eps;
      }
  }
}

void
FE2PK1Stress::computeQpMicroscaleTranslation()
{
  const auto & ref_q_point = qRule().get_points()[_qp];

  Point lower_left = _current_elem->node_ref(0);
  Point elem_range = _current_elem->node_ref(2) - _current_elem->node_ref(0);
  Real h = elem_range(0);
  _uo.translation = lower_left;

  if (ref_q_point(0) < 0 && ref_q_point(1) < 0)
    _uo.translation += Point(h / 4, h / 4);
  if (ref_q_point(0) > 0 && ref_q_point(1) < 0)
    _uo.translation += Point(3 * h / 4, h / 4, 0);
  if (ref_q_point(0) < 0 && ref_q_point(1) > 0)
    _uo.translation += Point(h / 4, 3 * h / 4, 0);
  if (ref_q_point(0) > 0 && ref_q_point(1) > 0)
    _uo.translation += Point(3 * h / 4, 3 * h / 4, 0);
}

RankTwoTensor
FE2PK1Stress::microscalePK1Stress(const RankTwoTensor & F)
{
  _uo.F = F;

  _fe_problem.execMultiAppTransfers(FE2::EXEC_FE2, MultiAppTransfer::TO_MULTIAPP);

  bool success = _fe_problem.execMultiApps(FE2::EXEC_FE2);
  if (!success)
    mooseError("Microscale solve failed");

  _fe_problem.execMultiAppTransfers(FE2::EXEC_FE2, MultiAppTransfer::FROM_MULTIAPP);

  return _uo.P;
}