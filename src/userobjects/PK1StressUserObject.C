//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// MOOSE includes
#include "PK1StressUserObject.h"

#include "libmesh/quadrature.h"

registerMooseObject("fe2App", PK1StressUserObject);

InputParameters
PK1StressUserObject::validParams()
{
  InputParameters params = UserObject::validParams();
  params.addClassDescription("Retrive deformation gradient at each quadrature point");
  return params;
}

PK1StressUserObject::PK1StressUserObject(const InputParameters & parameters)
  : UserObject(parameters)
{
}

void
PK1StressUserObject::initialize()
{
}

void
PK1StressUserObject::execute()
{
}

void
PK1StressUserObject::threadJoin(const UserObject & /*y*/)
{
}

void
PK1StressUserObject::finalize()
{
}

RankTwoTensor
PK1StressUserObject::getPK1Stress(const Point & p) const
{
  for (auto & [point, pk1_stress] : _qp_pk1_stress_map)
  {
    if (point.absolute_fuzzy_equals(p))
    {
      return pk1_stress;
    }
  }
  mooseError("No quadrature point found in pk1 stress user object. Point: ", p);
}

void
PK1StressUserObject::setPK1Stress(const Point & p, const RankTwoTensor & pk1)
{
  for (auto & [point, pk1_stress] : _qp_pk1_stress_map)
  {
    if (point.absolute_fuzzy_equals(p))
    {
      _qp_pk1_stress_map[point] = pk1;
      return;
    }
  }
  _qp_pk1_stress_map[p] = pk1;
}

void
PK1StressUserObject::print() const
{
  for (auto & [key, value] : _qp_pk1_stress_map)
  {
    _console << "Point: " << key << std::endl;
    _console << "PK1 stress: " << std::flush;
    value.print();
    _console << std::endl;
  }
}