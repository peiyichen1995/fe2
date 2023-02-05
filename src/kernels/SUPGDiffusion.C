#include "SUPGDiffusion.h"

registerMooseObject("fe2App", SUPGDiffusion);

InputParameters
SUPGDiffusion::validParams()
{
  InputParameters params = SUPGBase::validParams();
  params.addRequiredParam<unsigned int>("component", "component");
  return params;
}

SUPGDiffusion::SUPGDiffusion(const InputParameters & params)
  : SUPGBase(params),
    _component(getParam<unsigned int>("component")),
    _velocity_second(_var.adSecondSln())
{
}

ADReal
SUPGDiffusion::computeQpResidual()
{
  ADRealVectorValue u((*_velocities[0])[_qp], (*_velocities[1])[_qp], (*_velocities[2])[_qp]);
  ADReal r = _grad_test[_i][_qp] * _nu * (*_velocity_gradients[_component])[_qp] -
             _tau_supg * u * _grad_test[_i][_qp] * _nu * _velocity_second[_qp].tr();
  return r;
}
