#pragma once

// MOOSE includes
#include "GeneralUserObject.h"

class DeformationGradientPK1StressUserObject : public GeneralUserObject
{
public:
  static InputParameters validParams();

  DeformationGradientPK1StressUserObject(const InputParameters & parameters);
  ~DeformationGradientPK1StressUserObject();

  virtual void initialize() override;
  virtual void execute() override;
  virtual void finalize() override;

  std::map<dof_id_type, std::vector<std::pair<RankTwoTensor, RankTwoTensor>>> F_P;
  // std::map<dof_id_type, std::vector<std::vector<double>>> coords;

protected:
  std::string _file_name;
  std::ofstream _file;
};
