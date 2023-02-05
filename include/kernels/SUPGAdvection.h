#pragma once

#include "SUPGBase.h"

class SUPGAdvection : public SUPGBase
{
public:
  static InputParameters validParams();
  SUPGAdvection(const InputParameters & params);

protected:
  virtual ADReal computeQpResidual() override;

  const unsigned int _component;
};
