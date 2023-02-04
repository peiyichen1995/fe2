#pragma once

#include "ADKernel.h"

class Burgers : public ADKernel
{
public:
  static InputParameters validParams();
  Burgers(const InputParameters & params);

protected:
  virtual ADReal computeQpResidual() override;

  std::vector<const ADVariableValue *> _velocities;
  std::vector<const ADVariableGradient *> _velocity_gradients;
  const unsigned int _component;
  const Real _Re;
  RealVectorValue _w0;
};
