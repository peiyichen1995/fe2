#pragma once

#include "FVFluxBC.h"

/**
 * Constant velocity scalar advection boundary conditions
 */
class FVBurgersOpenBC : public FVFluxBC
{
public:
  FVBurgersOpenBC(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual ADReal computeQpResidual() override;

  std::vector<const ADVariableValue *> _velocities;
  const unsigned int _component;
};
