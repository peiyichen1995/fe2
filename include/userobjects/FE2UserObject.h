#pragma once

// MOOSE includes
#include "UserObject.h"

class FE2UserObject : public UserObject
{
public:
  static InputParameters validParams();

  FE2UserObject(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void threadJoin(const UserObject & y) override;
  virtual void finalize() override;

  RankTwoTensor F;
  RankTwoTensor P;
  Point translation;
  RankFourTensor dPdF;
};
