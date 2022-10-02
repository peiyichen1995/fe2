//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TorchStress.h"

InputParameters
TorchStress::validParams()
{
  InputParameters params = ComputeLagrangianStressPK1::validParams();
  params.addRequiredParam<std::string>("script", "file name of the torch script");
  return params;
}

ComputeLagrangianStressPK2::ComputeLagrangianStressPK2(const InputParameters & parameters)
  : ComputeLagrangianStressPK1(parameters)
{
  std::string file_name = getParam<std::string>("script");
  module = torch::jit::load(file_name);
}

void
ComputeLagrangianStressPK2::computeQpPK1Stress()
{
  
}
