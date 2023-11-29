//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifdef LIBTORCH_ENABLED
#include "TorchStress.h"

registerMooseObject("fe2App", TorchStress);

InputParameters
TorchStress::validParams()
{
  InputParameters params = ComputeLagrangianStressPK1::validParams();
  params.addRequiredParam<std::string>(
      "PK1_script", "file name of the torch script that defines PK1 as a function of F");
  params.addRequiredParam<std::string>(
      "dPK1_script", "file name of the torch script that defines dPK1/dF as a function of F");
  return params;
}

TorchStress::TorchStress(const InputParameters & parameters)
  : ComputeLagrangianStressPK1(parameters)
{
  _pk1 = torch::jit::load(getParam<std::string>("PK1_script"));
  _dpk1 = torch::jit::load(getParam<std::string>("dPK1_script"));
}

void
TorchStress::computeQpPK1Stress()
{
  torch::Tensor F = torch::tensor({{_F[_qp](0, 0), _F[_qp](0, 1), _F[_qp](0, 2)},
                                   {_F[_qp](1, 0), _F[_qp](1, 1), _F[_qp](1, 2)},
                                   {_F[_qp](2, 0), _F[_qp](2, 1), _F[_qp](2, 2)}},
                                  torch::kFloat64);
  torch::Tensor P_vals = _pk1({F}).toTensor();
  torch::Tensor dP_dF_vals = _dpk1({F}).toTensor();

  for (auto i : make_range(3))
    for (auto j : make_range(3))
      _pk1_stress[_qp](i, j) = P_vals.index({i, j}).item<double>();

  for (auto i : make_range(3))
    for (auto j : make_range(3))
      for (auto k : make_range(3))
        for (auto l : make_range(3))
          _pk1_jacobian[_qp](i, j, k, l) = dP_dF_vals.index({i, j, k, l}).item<double>();
}
#endif