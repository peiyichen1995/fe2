//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "FE2Transfer.h"

// MOOSE includes
#include "FEProblem.h"
#include "MooseTypes.h"
#include "MooseVariableScalar.h"
#include "MultiApp.h"
#include "SystemBase.h"

#include "libmesh/system.h"

#include "DeformationGradientUserObject.h"
#include "RankTwoTensor.h"

// Define the input parameters
registerMooseObject("fe2App", FE2Transfer);

InputParameters
FE2Transfer::validParams()
{
  InputParameters params = MultiAppTransfer::validParams();
  params.addClassDescription("Transfers data from a scalar variable to an auxiliary scalar "
                             "variable from different applications.");
  params.addRequiredParam<UserObjectName>("def_grad_uo",
                                          "The name of the deformation gradient userobject");
  params.addRequiredParam<std::vector<VariableName>>("def_grad_scalars",
                                                     "The name of the scalar variables to "
                                                     "transfer the value to.");
  return params;
}

FE2Transfer::FE2Transfer(const InputParameters & parameters)
  : MultiAppTransfer(parameters),
    _uo_name(getParam<UserObjectName>("def_grad_uo")),
    _scalar_names(getParam<std::vector<VariableName>>("def_grad_scalars"))
{
}

void
FE2Transfer::execute()
{
  TIME_SECTION("FE2Transfer::execute()", 5, "Performing a scalar variable transfer");

  // Perform action based on the transfer direction
  switch (_current_direction)
  {
    // main app to multi_app
    case TO_MULTIAPP:
    {
      // Extract the scalar variable that is being transferred
      FEProblemBase & from_problem = getToMultiApp()->problemBase();
      const DeformationGradientUserObject & userobject =
          dynamic_cast<const DeformationGradientUserObject &>(
              from_problem.getUserObjectBase(_uo_name, _tid));

      // Loop through each of the sub apps
      for (unsigned int i = 0; i < getToMultiApp()->numGlobalApps(); i++)
        if (getToMultiApp()->hasLocalApp(i))
        {
          const Point & q_point = getToMultiApp()->position(i);
          RankTwoTensor def_grad = userobject.getDeformationGradient(q_point);

          for (unsigned int ii = 0; ii < 3; ii++)
          {
            for (unsigned int jj = 0; jj < 3; jj++)
            {
              // Get reference to the scalar variable that will be written
              MooseVariableScalar * variable =
                  &getToMultiApp()->appProblemBase(i).getScalarVariable(_tid,
                                                                        _scalar_names[ii * 3 + jj]);
              variable->reinit();

              // Determine number of DOFs that we're going to read and write
              auto && to_dof = variable->dofIndices();

              // Check that the DOF matches
              if (to_dof.size() != 1)
                mooseError("Order of SCALAR variables must be 1!");

              variable->sys().solution().set(to_dof[0], def_grad(ii, jj));
              variable->sys().solution().close();
            }
          }
        }
      break;
    }
  }
}
