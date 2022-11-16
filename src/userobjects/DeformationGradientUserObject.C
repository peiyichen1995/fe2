//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// MOOSE includes
#include "DeformationGradientUserObject.h"

#include "libmesh/quadrature.h"

registerMooseObject("fe2App", DeformationGradientUserObject);

InputParameters
DeformationGradientUserObject::validParams()
{
  InputParameters params = ElementUserObject::validParams();
  params.addClassDescription("Retrive deformation gradient at each quadrature point");
  params.addParam<bool>(
      "verbose", false, "Print quadrature point and deformation gradient if true");
  return params;
}

DeformationGradientUserObject::DeformationGradientUserObject(const InputParameters & parameters)
  : ElementUserObject(parameters),
    _F(getMaterialPropertyByName<RankTwoTensor>("deformation_gradient")),
    _verbose(getParam<bool>("verbose")),
    _ready(false)
{
}

void
DeformationGradientUserObject::initialize()
{
  _qp_deformation_gradient_map.clear();
}

void
DeformationGradientUserObject::execute()
{
  for (unsigned int i = 0; i < _q_point.size(); i++)
  {
    _qp_deformation_gradient_map[_q_point[i]] = _F[i];
  }
}

void
DeformationGradientUserObject::threadJoin(const UserObject & y)
{
  const DeformationGradientUserObject & pps = static_cast<const DeformationGradientUserObject &>(y);
  _qp_deformation_gradient_map.insert(pps._qp_deformation_gradient_map.begin(),
                                      pps._qp_deformation_gradient_map.end());
}

void
DeformationGradientUserObject::finalize()
{
  _ready = true;
  if (_verbose)
  {
    for (auto & [key, value] : _qp_deformation_gradient_map)
    {
      _console << "Point: " << key << std::endl;
      _console << "Deformation gradient: " << std::flush;
      value.print();
      _console << std::endl;
    }
  }
}

RankTwoTensor
DeformationGradientUserObject::getDeformationGradient(const Point & p) const
{
  if (!_ready)
  {
    return RankTwoTensor::Identity();
  }
  for (auto & [point, def_grad] : _qp_deformation_gradient_map)
  {
    if (point.absolute_fuzzy_equals(p))
    {
      return def_grad;
    }
  }
  mooseError("No quadrature point found in deformation gradient user object. Point: ", p);
}