#pragma once

#include "ComputeLagrangianStressPK1.h"
#include "FE2UserObject.h"
#include "fe2Types.h"

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
class FE2PK1Stress : public ComputeLagrangianStressPK1
{
public:
  static InputParameters validParams();
  FE2PK1Stress(const InputParameters & parameters);

protected:
  /// Wrap PK2 -> PK1
  virtual void computeQpPK1Stress() override;
  FE2UserObject & _uo;
  const MaterialPropertyName _elasticity_tensor_name;
  const MaterialProperty<RankFourTensor> & _elasticity_tensor;
  const MaterialProperty<Real> & _mu1;
};
