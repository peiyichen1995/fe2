#pragma once

#include "AuxKernel.h"

class FE2SolutionUserObject;

class FE2SolutionAux : public AuxKernel
{
public:
  static InputParameters validParams();

  FE2SolutionAux(const InputParameters & parameters);

  /**
   * Sets up the variable name for extraction from the SolutionUserObject
   */
  virtual void initialSetup() override;

protected:
  /**
   * Computes a value for a node or element depending on the type of kernel,
   * it also uses the 'direct' flag to extract values based on the dof if the
   * flag is set to true.
   * @ return The desired value of the solution for the current node or element
   */
  virtual Real computeValue() override;

  /// Reference to the SolutionUserObject storing the solution
  const FE2SolutionUserObject & _solution_object;

  /// The variable name of interest
  std::string _var_name;

  /// Multiplier for the solution, the a of ax+b
  const Real _scale_factor;

  /// Additional factor added to the solution, the b of ax+b
  const Real _add_factor;
};
