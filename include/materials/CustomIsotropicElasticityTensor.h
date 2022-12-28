#pragma once

#include "Material.h"
#include "RankFourTensorForward.h"

/**
 * CustomIsotropicElasticityTensor defines an elasticity tensor material for
 * isotropic materials.
 */
class CustomIsotropicElasticityTensor : public Material
{
public:
  static InputParameters validParams();

  CustomIsotropicElasticityTensor(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  const VariableValue & _lambda;
  const VariableValue & _mu;

  MaterialProperty<RankFourTensor> & _C;
};
