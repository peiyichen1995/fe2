//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

// MOOSE includes
#include "UserObject.h"
#include <unordered_map>

/**
 * This postprocessor computes a volume integral of the specified
 * variable.
 *
 * Note that specializations of this integral are possible by deriving
 * from this class and overriding computeQpIntegral().
 */
class PK1StressUserObject : public UserObject
{
public:
  static InputParameters validParams();

  PK1StressUserObject(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void threadJoin(const UserObject & y) override;
  virtual void finalize() override;
  RankTwoTensor getPK1Stress(const Point & p) const;
  void setPK1Stress(const Point & p, const RankTwoTensor & pk1);
  void print() const;

protected:
  std::unordered_map<Point, RankTwoTensor> _qp_pk1_stress_map;
};
