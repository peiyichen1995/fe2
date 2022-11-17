//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "FE2PK1StressTransfer.h"

// MOOSE includes
#include "FEProblem.h"
#include "MooseTypes.h"
#include "MooseVariableScalar.h"
#include "MultiApp.h"
#include "SystemBase.h"

#include "libmesh/system.h"

#include "PK1StressUserObject.h"
#include "RankTwoTensor.h"

// Define the input parameters
registerMooseObject("fe2App", FE2PK1StressTransfer);

InputParameters
FE2PK1StressTransfer::validParams()
{
  InputParameters params = MultiAppTransfer::validParams();
  params.addClassDescription("Transfers data from a scalar variable to an auxiliary scalar "
                             "variable from different applications.");
  params.addRequiredParam<UserObjectName>("pk1_stress_uo", "The name of the pk1 stress userobject");
  params.addRequiredParam<std::vector<PostprocessorName>>(
      "pk1_stress_components",
      "The name of the postprocessors of the pk1 stress components to "
      "transfer the value from.");
  params.addParam<bool>(
      "verbose", false, "Print quadrature point and deformation gradient if true");
  return params;
}

FE2PK1StressTransfer::FE2PK1StressTransfer(const InputParameters & parameters)
  : MultiAppTransfer(parameters),
    _uo_name(getParam<UserObjectName>("pk1_stress_uo")),
    _pp_names(getParam<std::vector<PostprocessorName>>("pk1_stress_components")),
    _verbose(getParam<bool>("verbose"))
{
}

void
FE2PK1StressTransfer::execute()
{
  TIME_SECTION("FE2Transfer::execute()", 5, "Performing a scalar variable transfer");

  // Perform action based on the transfer direction
  switch (_current_direction)
  {
    case FROM_MULTIAPP:
    {
      FEProblemBase & to_problem = getFromMultiApp()->problemBase();

      const auto multi_app = getFromMultiApp();

      const PK1StressUserObject & const_userobject =
          dynamic_cast<const PK1StressUserObject &>(to_problem.getUserObjectBase(_uo_name, _tid));

      PK1StressUserObject & userobject = const_cast<PK1StressUserObject &>(const_userobject);

      for (unsigned int i = 0; i < multi_app->numGlobalApps(); i++)
      {
        if (multi_app->hasLocalApp(i))
        {
          RankTwoTensor pk1;
          for (int j = 0; j < _pp_names.size(); j++)
          {
            const Real & curr_pp_value =
                multi_app->appProblemBase(i).getPostprocessorValueByName(_pp_names[j]);
            pk1(j / 3, j % 3) = curr_pp_value;
          }
          const Point & q_point = multi_app->position(i);
          userobject.setPK1Stress(q_point, pk1);
        }
      }

      if (_verbose)
      {
        userobject.print();
      }

      break;
    }
  }
}
