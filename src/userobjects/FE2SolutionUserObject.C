#include "FE2SolutionUserObject.h"

registerMooseObject("fe2App", FE2SolutionUserObject);

InputParameters
FE2SolutionUserObject::validParams()
{
  // Get the input parameters from the parent class
  InputParameters params = SolutionUserObject::validParams();
  params.addRequiredCoupledVar("translation_scalar_vars", "3 translation scalar variables");
  return params;
}

FE2SolutionUserObject::FE2SolutionUserObject(const InputParameters & parameters)
  : SolutionUserObject(parameters),
    _translation_scalar_var_x(coupledScalarValue("translation_scalar_vars", 0)),
    _translation_scalar_var_y(coupledScalarValue("translation_scalar_vars", 1)),
    _translation_scalar_var_z(coupledScalarValue("translation_scalar_vars", 2))
{
}

Real
FE2SolutionUserObject::pointValue(Real t,
                                  const Point & p,
                                  const std::string & var_name,
                                  const std::set<subdomain_id_type> * subdomain_ids) const
{
  const unsigned int local_var_index = getLocalVarIndex(var_name);
  return pointValue(t, p, local_var_index, subdomain_ids);
}

Real
FE2SolutionUserObject::pointValue(Real libmesh_dbg_var(t),
                                  const Point & p,
                                  const unsigned int local_var_index,
                                  const std::set<subdomain_id_type> * subdomain_ids) const
{
  // Create copy of point
  Point pt(p);

  // do the transformations
  for (unsigned int trans_num = 0; trans_num < _transformation_order.size(); ++trans_num)
  {
    if (_transformation_order[trans_num] == "rotation0")
      pt = _r0 * pt;
    else if (_transformation_order[trans_num] == "translation")
    {
      for (const auto i : make_range(Moose::dim))
        pt(i) -= _translation[i];
      pt(0) += _translation_scalar_var_x[0];
      pt(1) += _translation_scalar_var_y[0];
      pt(2) += _translation_scalar_var_z[0];
    }
    else if (_transformation_order[trans_num] == "scale")
      for (const auto i : make_range(Moose::dim))
        pt(i) /= _scale[i];
    else if (_transformation_order[trans_num] == "scale_multiplier")
      for (const auto i : make_range(Moose::dim))
        pt(i) *= _scale_multiplier[i];
    else if (_transformation_order[trans_num] == "rotation1")
      pt = _r1 * pt;
  }

  // Extract the value at the current point
  Real val = evalMeshFunction(pt, local_var_index, 1, subdomain_ids);

  // Interpolate
  if (_file_type == 1 && _interpolate_times)
  {
    mooseAssert(t == _interpolation_time,
                "Time passed into value() must match time at last call to timestepSetup()");
    Real val2 = evalMeshFunction(pt, local_var_index, 2, subdomain_ids);
    val = val + (val2 - val) * _interpolation_factor;
  }

  return val;
}