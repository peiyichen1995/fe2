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
  // https://www.tafsm.org/PUB_PRE/jALL/j106-JCAM-SP.pdf
  // std::vector<ADRealVectorValue> c(_test.size(), 0);
  // std::vector<ADRealVectorValue> ct(_test.size(), 0);
  // std::vector<ADRealVectorValue> kt(_test.size(), 0);
  // for (_i = 0; _i < _test.size(); _i++)
  // {
  //   for (_qp = 0; _qp < _q_point.size(); _qp++)
  //   {
  //     ADRealVectorValue u((*_velocities[0])[_qp], (*_velocities[1])[_qp],
  //     (*_velocities[2])[_qp]); ADRankTwoTensor grad_u =
  //     ADRankTwoTensor::initializeFromRows((*_velocity_gradients[0])[_qp],
  //                                                                  (*_velocity_gradients[1])[_qp],
  //                                                                  (*_velocity_gradients[2])[_qp]);
  //     ADRealVectorValue u_dot(
  //         (*_velocity_dots[0])[_qp], (*_velocity_dots[1])[_qp], (*_velocity_dots[2])[_qp]);
  //     c[_i] += _test[_i][_qp] * grad_u * u * _JxW[_qp];
  //     kt[_i] += (u * _grad_test[_i][_qp]) * (grad_u * u) * _JxW[_qp];
  //     ct[_i] += u * _grad_test[_i][_qp] * u_dot * _JxW[_qp];
  //   }
  // }

  _tau_supg.resize(_test.size(), std::vector<ADReal>(_q_point.size()));
  for (_i = 0; _i < _test.size(); _i++)
  {
    // ADReal tau_s1 = std::sqrt(c[_i] * c[_i] + 1e-12) / std::sqrt(kt[_i] * kt[_i] + 1e-12);
    // ADReal tau_s2 = std::sqrt(c[_i] * c[_i] + 1e-12) / std::sqrt(ct[_i] * ct[_i] + 1e-12) * _dt /
    // 2;
    for (_qp = 0; _qp < _q_point.size(); _qp++)
    {
      ADRealVectorValue u((*_velocities[0])[_qp], (*_velocities[1])[_qp], (*_velocities[2])[_qp]);
      // ADReal Re =
      //     u * u / _nu * std::sqrt(c[_i] * c[_i] + 1e-12) / std::sqrt(kt[_i] * kt[_i] + 1e-12);
      // ADReal tau_s3 = tau_s1 * Re;

      // _tau_supg[_i][_qp] = std::pow(
      //     std::pow(tau_s1, -_r) + std::pow(tau_s2, -_r) + std::pow(tau_s3, -_r), -1.0 / _r);
      Real h = _current_elem->hmax();
      _tau_supg[_i][_qp] = 1 / std::sqrt(_sigma_BDF * _sigma_BDF / _dt / _dt + u * u / h / h +
                                         15 * _nu * _nu / h / h / h / h);
    }
  }
}
