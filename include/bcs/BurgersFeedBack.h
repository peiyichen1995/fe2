#pragma once

#include "IntegratedBC.h"

class Function;

/**
 * Boundary condition of a Neumann style whose value is computed by a user-defined function
 */
class BurgersFeedBack : public IntegratedBC
{
public:
  static InputParameters validParams();

  BurgersFeedBack(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  /// The function being used for setting the value
  const Function & _func;
  Real _c0;
};
