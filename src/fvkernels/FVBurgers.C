#include "FVBurgers.h"

registerMooseObject("fe2App", FVBurgers);

InputParameters
FVBurgers::validParams()
{
  InputParameters params = FVFluxKernel::validParams();
  params.addRequiredCoupledVar("velocities", "velocities");
  params.addRequiredParam<unsigned int>("component", "component");
  return params;
}

FVBurgers::FVBurgers(const InputParameters & params)
  : FVFluxKernel(params),
    _velocities(adCoupledValues("velocities")),
    _velocities_neighbor(adCoupledNeighborValues("velocities")),
    _component(getParam<unsigned int>("component"))
{
  _velocities.resize(3, &_ad_zero);
  _velocities_neighbor.resize(3, &_ad_zero);
}

ADReal
FVBurgers::computeQpResidual()
{
  ADRealVectorValue u((*_velocities[0])[_qp], (*_velocities[1])[_qp], (*_velocities[2])[_qp]);
  ADRealVectorValue u_neighbor((*_velocities_neighbor[0])[_qp],
                               (*_velocities_neighbor[1])[_qp],
                               (*_velocities_neighbor[2])[_qp]);
  ADReal r = 0;
  // ADRealVectorValue u_av = 0.5 * (u + u_neighbor);
  // if (u_av * _normal > 0)
  r += 0.5 * u * u * _normal(_component);
  // else
  r += 0.5 * u_neighbor * u_neighbor * _normal(_component);
  return r;
}
