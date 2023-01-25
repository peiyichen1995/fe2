#include "Burgers.h"

registerMooseObject("fe2App", Burgers);

InputParameters
Burgers::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addRequiredCoupledVar("velocities", "velocities");
  params.addRequiredParam<unsigned int>("component", "component");
  params.addRequiredParam<Real>("Re", "Re");
  return params;
}

Burgers::Burgers(const InputParameters & params)
  : ADKernel(params),
    _velocities(adCoupledValues("velocities")),
    _velocity_gradients(adCoupledGradients("velocities")),
    _u_dot(_var.adUDot()),
    _component(getParam<unsigned int>("component")),
    _Re(getParam<Real>("Re"))
{
  _velocities.resize(3, &_ad_zero);
  _velocity_gradients.resize(3, &_ad_grad_zero);
}

void
Burgers::precalculateResidual()
{
  ADRealVectorValue u_sum;
  Real volume = 0;

  for (unsigned int qp = 0; qp < _qrule->n_points(); qp++)
  {
    ADRealVectorValue u((*_velocities[0])[qp], (*_velocities[1])[qp], (*_velocities[2])[qp]);
    u_sum += u * _JxW[qp];
    volume += _JxW[qp];
  }
  _u_avg = u_sum / volume;
}

ADReal
Burgers::computeQpResidual()
{
  ADRealVectorValue u((*_velocities[0])[_qp], (*_velocities[1])[_qp], (*_velocities[2])[_qp]);
  Real h = _current_elem->hmin();
  ADReal u_norm = std::sqrt(_u_avg * _u_avg + 1e-12);
  ADReal tau =
      1 / std::sqrt((2 * u_norm / h) * (2 * u_norm / h) + 9 * std::pow((4 / h / h / _Re), 2.0));
  ADReal upwind = tau * _grad_test[_i][_qp] * u;
  ADReal r = (upwind + _test[_i][_qp]) * u * (*_velocity_gradients[_component])[_qp];
  r += upwind * _u_dot[_qp];
  return r;
}
