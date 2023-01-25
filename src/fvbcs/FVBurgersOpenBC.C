//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
#include "FVBurgersOpenBC.h"

registerMooseObject("fe2App", FVBurgersOpenBC);

InputParameters
FVBurgersOpenBC::validParams()
{
  InputParameters params = FVFluxBC::validParams();
  params.addRequiredCoupledVar("velocities", "velocities");
  params.addRequiredParam<unsigned int>("component", "component");
  return params;
}

FVBurgersOpenBC::FVBurgersOpenBC(const InputParameters & parameters)
  : FVFluxBC(parameters),
    _velocities(adCoupledValues("velocities")),
    _component(getParam<unsigned int>("component"))
{
  _velocities.resize(3, &_ad_zero);
}

ADReal
FVBurgersOpenBC::computeQpResidual()
{
  ADRealVectorValue u((*_velocities[0])[_qp], (*_velocities[1])[_qp], (*_velocities[2])[_qp]);

  return 0.5 * u * u * _normal(_component);
}
