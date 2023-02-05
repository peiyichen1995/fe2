#pragma once

#include "ADKernel.h"

class SUPGBase : public ADKernel
{
public:
  static InputParameters validParams();
  SUPGBase(const InputParameters & params);

protected:
  virtual void precalculateResidual() override;

  std::vector<const ADVariableValue *> _velocities;
  std::vector<const ADVariableGradient *> _velocity_gradients;
  std::vector<const ADVariableValue *> _velocity_dots;
  const Real _nu;
  // std::vector<std::vector<ADReal>> _tau_supg;
  ADReal _tau_supg;
  const Real _sigma_BDF;
};
