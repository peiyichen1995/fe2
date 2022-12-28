#pragma once

// MOOSE includes
#include "SolutionUserObject.h"

class FE2SolutionUserObject : public SolutionUserObject
{
public:
  static InputParameters validParams();

  FE2SolutionUserObject(const InputParameters & parameters);

  Real pointValue(Real t,
                  const Point & p,
                  const std::string & var_name,
                  const std::set<subdomain_id_type> * subdomain_ids = nullptr) const;

  Real pointValue(Real t,
                  const Point & p,
                  const unsigned int local_var_index,
                  const std::set<subdomain_id_type> * subdomain_ids = nullptr) const;

protected:
  const VariableValue & _translation_scalar_var_x;
  const VariableValue & _translation_scalar_var_y;
  const VariableValue & _translation_scalar_var_z;
};
