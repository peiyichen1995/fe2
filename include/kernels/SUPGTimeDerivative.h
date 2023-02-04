#pragma once

#include "SUPGBase.h"

class SUPGTimeDerivative : public SUPGBase
{
public:
  static InputParameters validParams();
  SUPGTimeDerivative(const InputParameters & params);

protected:
  virtual ADReal computeQpResidual() override;

  const unsigned int _component;
};
