#include "CustomIsotropicElasticityTensor.h"
#include "RankFourTensor.h"

registerMooseObject("fe2App", CustomIsotropicElasticityTensor);

InputParameters
CustomIsotropicElasticityTensor::validParams()
{
  InputParameters params = Material::validParams();
  params.addRequiredCoupledVar("lambda", "lambda");
  params.addRequiredCoupledVar("shear_modulus", "mu");
  return params;
}

CustomIsotropicElasticityTensor::CustomIsotropicElasticityTensor(const InputParameters & parameters)
  : Material(parameters),
    _lambda(coupledValue("lambda")),
    _mu(coupledValue("shear_modulus")),
    _C(declareProperty<RankFourTensor>("elasticity_tensor"))
{
}

void
CustomIsotropicElasticityTensor::computeQpProperties()
{
  Real lambda = _lambda[_qp];
  Real mu = _mu[_qp];
  Real E = (mu * (3 * lambda + 2 * mu)) / (lambda + mu);
  Real nu = lambda / (2 * (lambda + mu));
  _C[_qp].fillFromInputVector({E, nu}, RankFourTensor::symmetric_isotropic);
}
