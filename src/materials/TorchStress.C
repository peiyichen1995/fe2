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
  params.addRequiredParam<std::string>("script", "file name of the torch script");
  return params;
}

TorchStress::TorchStress(const InputParameters & parameters)
  : ComputeLagrangianStressPK1(parameters)
{
  std::string file_name = getParam<std::string>("script");
  module = torch::jit::load(file_name);
}

void
TorchStress::computeQpPK1Stress()
{
  double F_vals[9] = {_F[_qp](0, 0),
                      _F[_qp](0, 1),
                      _F[_qp](0, 2),
                      _F[_qp](1, 0),
                      _F[_qp](1, 1),
                      _F[_qp](1, 2),
                      _F[_qp](2, 0),
                      _F[_qp](2, 1),
                      _F[_qp](2, 2)};
  torch::Tensor F = torch::from_blob(F_vals, {3, 3}, {torch::kFloat64});
  torch::Tensor P_vals = module({F}).toTensor();
  _pk1_stress[_qp] = RankTwoTensor(P_vals.index({0, 0}).item<double>(),
                                   P_vals.index({1, 0}).item<double>(),
                                   P_vals.index({2, 0}).item<double>(),
                                   P_vals.index({0, 1}).item<double>(),
                                   P_vals.index({1, 1}).item<double>(),
                                   P_vals.index({2, 1}).item<double>(),
                                   P_vals.index({0, 2}).item<double>(),
                                   P_vals.index({1, 2}).item<double>(),
                                   P_vals.index({2, 2}).item<double>());
}
#endif