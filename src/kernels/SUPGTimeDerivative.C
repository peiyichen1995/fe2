#include "SUPGTimeDerivative.h"

registerMooseObject("fe2App", SUPGTimeDerivative);

InputParameters
SUPGTimeDerivative::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addRequiredParam<unsigned int>("component", "component");
  return params;
}

SUPGTimeDerivative::SUPGTimeDerivative(const InputParameters & params)
  : ADKernel(params), _component(getParam<unsigned int>("component"))
{
}

ADReal
SUPGTimeDerivative::computeQpResidual()
{
  ADRealVectorValue u((*_velocities[0])[_qp], (*_velocities[1])[_qp], (*_velocities[2])[_qp]);
  ADReal r = (*_velocity_dots[_component])[_qp] *
             (_test[_i][_qp] + _tau_supg[_i][_qp] * u * _grad_test[_i][_qp]);
  return r;
}
