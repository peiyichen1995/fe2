#pragma once

#include "ADKernel.h"

class Burgers : public ADKernel
{
public:
  static InputParameters validParams();
  Burgers(const InputParameters & params);

protected:
  virtual ADReal computeQpResidual() override;
  virtual void precalculateResidual() override;

  std::vector<const ADVariableValue *> _velocities;
  std::vector<const ADVariableGradient *> _velocity_gradients;
  const ADVariableValue & _u_dot;
  const unsigned int _component;
  ADRealVectorValue _u_avg;
  const Real _Re;
};
