#include "SUPGBase.h"

InputParameters
SUPGBase::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addRequiredCoupledVar("velocities", "velocities");
  params.addRequiredParam<Real>("nu", "nu");
  params.addRequiredParam<Real>("sigma_BDF", "sigma_BDF");
  return params;
}

SUPGBase::SUPGBase(const InputParameters & params)
  : ADKernel(params),
    _velocities(adCoupledValues("velocities")),
    _velocity_gradients(adCoupledGradients("velocities")),
    _velocity_dots(adCoupledDots("velocities")),
    _nu(getParam<Real>("nu")),
    _sigma_BDF(getParam<Real>("sigma_BDF"))

{
  _velocities.resize(3, &_ad_zero);
  _velocity_gradients.resize(3, &_ad_grad_zero);
  _velocity_dots.resize(3, &_ad_zero);
}

void
SUPGBase::precalculateResidual()
{
  ADRealVectorValue u;
  Real S = 0;
  for (_qp = 0; _qp < _q_point.size(); _qp++)
  {
    ADRealVectorValue u_curr(
        (*_velocities[0])[_qp], (*_velocities[1])[_qp], (*_velocities[2])[_qp]);
    u += u_curr * _JxW[_qp];
    S += _JxW[_qp];
  }

  ADRealVectorValue u_avg = u / S;

  Real h = _current_elem->hmax();
  _tau_supg = 1 / std::sqrt(_sigma_BDF * _sigma_BDF / _dt / _dt + u_avg * u_avg / h / h +
                            60 * _nu * _nu / h / h / h / h);
}
