#include "SUPGBase.h"

InputParameters
SUPGBase::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addRequiredCoupledVar("velocities", "velocities");
  params.addRequiredParam<Real>("nu", "nu");
  params.addRequiredParam<Real>("r", "r");
  return params;
}

SUPGBase::SUPGBase(const InputParameters & params)
  : ADKernel(params),
    _velocities(adCoupledValues("velocities")),
    _velocity_gradients(adCoupledGradients("velocities")),
    _velocity_dots(adCoupledDots("velocities")),
    _nu(getParam<Real>("nu")),
    _r(getParam<Real>("r"))

{
  _velocities.resize(3, &_ad_zero);
  _velocity_gradients.resize(3, &_ad_grad_zero);
  _velocity_dots.resize(3, &_ad_zero);
}

void
SUPGBase::precalculateResidual()
{
  // https://www.tafsm.org/PUB_PRE/jALL/j106-JCAM-SP.pdf
  std::vector<ADRealVectorValue> c(_test.size(), 0);
  std::vector<ADRealVectorValue> ct(_test.size(), 0);
  std::vector<ADRealVectorValue> kt(_test.size(), 0);
  for (_i = 0; _i < _test.size(); _i++)
  {
    for (_qp = 0; _qp < _q_point.size(); _qp++)
    {
      ADRealVectorValue u((*_velocities[0])[_qp], (*_velocities[1])[_qp], (*_velocities[2])[_qp]);
      ADRankTwoTensor grad_u = ADRankTwoTensor::initializeFromRows((*_velocity_gradients[0])[_qp],
                                                                   (*_velocity_gradients[1])[_qp],
                                                                   (*_velocity_gradients[2])[_qp]);
      ADRealVectorValue u_dot(
          (*_velocity_dots[0])[_qp], (*_velocity_dots[1])[_qp], (*_velocity_dots[2])[_qp]);
      c[_i] += _test[_i][_qp] * grad_u * u * _JxW[_qp];
      kt[_i] += (u * _grad_test[_i][_qp]) * (grad_u * u) * _JxW[_qp];
      ct[_i] += u * _grad_test[_i][_qp] * u_dot * _JxW[_qp];
    }
  }

  _tau_supg.resize(_test.size(), std::vector<ADReal>(_q_point.size()));
  for (_i = 0; _i < _test.size(); _i++)
  {
    ADReal tau_s1 = c[_i].norm() / kt[_i].norm();
    ADReal tau_s2 = c[_i].norm() / ct[_i].norm() * _dt / 2;
    for (_qp = 0; _qp < _q_point.size(); _qp++)
    {
      ADRealVectorValue u((*_velocities[0])[_qp], (*_velocities[1])[_qp], (*_velocities[2])[_qp]);
      ADReal Re = u * u / _nu * c[_i].norm() / kt[_i].norm();
      ADReal tau_s3 = tau_s1 * Re;

      _tau_supg[_i][_qp] = std::pow(
          std::pow(tau_s1, -_r) + std::pow(tau_s2, -_r) + std::pow(tau_s3, -_r), -1.0 / _r);
    }
  }
}
