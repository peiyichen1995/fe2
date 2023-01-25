//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
#pragma once

#include "FVFluxKernel.h"

class FVBurgers : public FVFluxKernel
{
public:
  static InputParameters validParams();
  FVBurgers(const InputParameters & params);

protected:
  virtual ADReal computeQpResidual() override;

  std::vector<const ADVariableValue *> _velocities;
  std::vector<const ADVariableValue *> _velocities_neighbor;
  const unsigned int _component;
};
