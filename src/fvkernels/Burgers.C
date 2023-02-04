#include "Burgers.h"

registerMooseObject("fe2App", Burgers);

InputParameters
Burgers::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addRequiredCoupledVar("velocities", "velocities");
  params.addRequiredParam<unsigned int>("component", "component");
  params.addRequiredParam<Real>("Re", "Re");
  params.addRequiredParam<RealVectorValue>("w0", "w0");
  return params;
}

Burgers::Burgers(const InputParameters & params)
  : ADKernel(params),
    _velocities(adCoupledValues("velocities")),
    _velocity_gradients(adCoupledGradients("velocities")),
    _component(getParam<unsigned int>("component")),
    _Re(getParam<Real>("Re")),
    _w0(getParam<RealVectorValue>("w0"))
{
  _velocities.resize(3, &_ad_zero);
  _velocity_gradients.resize(3, &_ad_grad_zero);
}

ADReal
Burgers::computeQpResidual()
{
  ADRealVectorValue u((*_velocities[0])[_qp], (*_velocities[1])[_qp], (*_velocities[2])[_qp]);
  ADReal r = _test[_i][_qp] * (u + _w0) * (*_velocity_gradients[_component])[_qp];
  return r;
}
