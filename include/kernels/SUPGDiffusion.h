#pragma once

#include "SUPGBase.h"

class SUPGDiffusion : public SUPGBase
{
public:
  static InputParameters validParams();
  SUPGDiffusion(const InputParameters & params);

protected:
  virtual ADReal computeQpResidual() override;

  const unsigned int _component;
  const ADVariableSecond & _velocity_second;
};
