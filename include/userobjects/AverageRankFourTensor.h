#pragma once

// MOOSE includes
#include "ElementUserObject.h"
#include "RankFourTensorForward.h"

class AverageRankFourTensor : public ElementUserObject
{
public:
  static InputParameters validParams();

  AverageRankFourTensor(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void threadJoin(const UserObject & y) override;
  virtual void finalize() override {}

  /// Returns the integral value
  virtual RankFourTensor getValue();

protected:
  virtual RankFourTensor computeIntegral();

  unsigned int _qp;

  RankFourTensor _integral_value;

  const MaterialProperty<RankFourTensor> & _tensor;
};
