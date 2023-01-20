#pragma once

#include "ComputeLagrangianStressPK2.h"
#include "GuaranteeConsumer.h"

#include "DeformationGradientPK1StressUserObject.h"
#include "fe2Types.h"

/// St. Venant-Kirchhoff hyperelasticity
///
/// St. Venant-Kirchhoff hyperelasticity derivative from the
/// strain energy function W = lambda / 2 tr(E)^2 + mu tr(E^2)i
///
/// This is basically "linear elasticity with the Green-Lagrange strain"
/// Therefore, we use the ComputeElasticityTensor system to get the
/// tensor but enforce it to be isotropic with the Guarantee system
/// as this is only a hyperelastic model for an isotropic tensor.
///
class CustomStVenantKirchhoffStress : public ComputeLagrangianStressPK2, public GuaranteeConsumer
{
public:
  static InputParameters validParams();
  CustomStVenantKirchhoffStress(const InputParameters & parameters);

protected:
  /// Setup function, used to check on isotropy
  virtual void initialSetup() override;

  /// Actual stress/Jacobian update
  virtual void computeQpPK2Stress() override;

protected:
  const MaterialPropertyName _elasticity_tensor_name;
  const MaterialProperty<RankFourTensor> & _elasticity_tensor;
  DeformationGradientPK1StressUserObject & _uo_F_pk1;
};
