#include "SUPGAdvection.h"

registerMooseObject("fe2App", SUPGAdvection);

InputParameters
SUPGAdvection::validParams()
{
  InputParameters params = SUPGBase::validParams();
  params.addRequiredParam<unsigned int>("component", "component");
  return params;
}

SUPGAdvection::SUPGAdvection(const InputParameters & params)
  : SUPGBase(params), _component(getParam<unsigned int>("component"))
{
}

ADReal
SUPGAdvection::computeQpResidual()
{
  ADRealVectorValue u((*_velocities[0])[_qp], (*_velocities[1])[_qp], (*_velocities[2])[_qp]);
  ADReal r = (*_velocity_gradients[_component])[_qp] * u *
             (_test[_i][_qp] + _tau_supg[_i][_qp] * u * _grad_test[_i][_qp]);
  return r;
}
