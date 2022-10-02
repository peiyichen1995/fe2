#pragma once

#include <torch/script.h>
#include "ComputeLagrangianStressPK1.h"


/// Native interface for providing the 2nd Piola Kirchhoff stress
///
/// This class *implements* the 2nd PK stress update, providing:
///   1) The 2nd PK stress
///   2) The derivative of the 2nd PK stress wrt the Cauchy-Green strain
///
/// and wraps these to provide:
///   1) The 1st PK stress
///   2) d(PK1)/d(F)
///
class TorchStress : public ComputeLagrangianStressPK1
{
public:
  static InputParameters validParams();
  TorchStress(const InputParameters & parameters);

protected:
  /// Wrap PK2 -> PK1
  virtual void computeQpPK1Stress() override;
  torch::jit::script::Module module;
};
