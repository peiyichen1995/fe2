#include "DeformationGradientPK1StressUserObject.h"

registerMooseObject("fe2App", DeformationGradientPK1StressUserObject);

InputParameters
DeformationGradientPK1StressUserObject::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
  params.addClassDescription("save deformation gradient and pk1 stress");
  params.addRequiredParam<std::string>("file_name", "file name to save the F and PK1");
  return params;
}

DeformationGradientPK1StressUserObject::DeformationGradientPK1StressUserObject(
    const InputParameters & parameters)
  : GeneralUserObject(parameters), _file_name(getParam<std::string>("file_name"))
{
  _file.open(_file_name);
}

DeformationGradientPK1StressUserObject::~DeformationGradientPK1StressUserObject() { _file.close(); }

void
DeformationGradientPK1StressUserObject::initialize()
{
}

void
DeformationGradientPK1StressUserObject::execute()
{
}

void
DeformationGradientPK1StressUserObject::finalize()
{
  for (auto & [key, value] : F_P)
  {
    for (int i = 0; i < value.size(); i++)
    {
      value[i].first.write_unformatted(_file, false);
      value[i].second.write_unformatted(_file, false);
      // _file << coords[key][i][0] << " " << coords[key][i][1] << " " << coords[key][i][2] << " ";
      _file << '\n';
    }
  }
}
