#include "FE2Transfer.h"

// MOOSE includes
#include "FEProblem.h"
#include "MooseTypes.h"
#include "MooseVariableScalar.h"
#include "MultiApp.h"
#include "SystemBase.h"

#include "libmesh/system.h"

#include "FE2UserObject.h"
#include "RankTwoTensor.h"

// Define the input parameters
registerMooseObject("fe2App", FE2Transfer);

InputParameters
FE2Transfer::validParams()
{
  InputParameters params = MultiAppTransfer::validParams();
  params.addClassDescription("Transfers data from a scalar variable to an auxiliary scalar "
                             "variable from different applications.");
  params.addRequiredParam<UserObjectName>("fe2_uo", "The name of the fe2 userobject");
  params.addRequiredParam<std::vector<VariableName>>("def_grad_scalars",
                                                     "The name of the scalar variables to "
                                                     "transfer the value to.");
  params.addRequiredParam<std::vector<PostprocessorName>>(
      "pk1_stress_components",
      "The name of the postprocessors of the pk1 stress components to "
      "transfer the value from.");
  return params;
}

FE2Transfer::FE2Transfer(const InputParameters & parameters)
  : MultiAppTransfer(parameters),
    _uo_name(getParam<UserObjectName>("fe2_uo")),
    _scalar_names(getParam<std::vector<VariableName>>("def_grad_scalars")),
    _pp_names(getParam<std::vector<PostprocessorName>>("pk1_stress_components"))
{
}

void
FE2Transfer::execute()
{
  // TIME_SECTION("FE2Transfer::execute()", 5, "Performing a scalar variable transfer");

  // Perform action based on the transfer direction
  switch (_current_direction)
  {
    // main app to multi_app
    case TO_MULTIAPP:
    {
      if (getToMultiApp()->hasLocalApp(0))
      {
        // Moose::ScopedCommSwapper swapper(getToMultiApp()->comm());
        // Extract the scalar variable that is being transferred
        FEProblemBase & from_problem = getToMultiApp()->problemBase();
        const FE2UserObject & userobject =
            dynamic_cast<const FE2UserObject &>(from_problem.getUserObjectBase(_uo_name, _tid));

        // swapper.forceSwap();
        RankTwoTensor F = userobject.F;
        // swapper.forceSwap();

        for (unsigned int ii = 0; ii < 3; ii++)
        {
          for (unsigned int jj = 0; jj < 3; jj++)
          {
            // Get reference to the scalar variable that will be written
            MooseVariableScalar * variable = &getToMultiApp()->appProblemBase(0).getScalarVariable(
                _tid, _scalar_names[ii * 3 + jj]);
            variable->reinit();

            // Determine number of DOFs that we're going to read and write
            auto && to_dof = variable->dofIndices();

            // Check that the DOF matches
            if (to_dof.size() != 1)
              mooseError("Order of SCALAR variables must be 1!");

            variable->sys().solution().set(to_dof[0], F(ii, jj));
            variable->sys().solution().close();
          }
        }
      }
      break;
    }
    case FROM_MULTIAPP:
    {
      if (getFromMultiApp()->hasLocalApp(0) && getFromMultiApp()->isRootProcessor())
      {
        FEProblemBase & to_problem = getFromMultiApp()->problemBase();

        const auto multi_app = getFromMultiApp();

        const FE2UserObject & const_userobject =
            dynamic_cast<const FE2UserObject &>(to_problem.getUserObjectBase(_uo_name, _tid));

        FE2UserObject & userobject = const_cast<FE2UserObject &>(const_userobject);

        RankTwoTensor pk1;
        for (size_t j = 0; j < _pp_names.size(); j++)
        {
          const Real & curr_pp_value =
              multi_app->appProblemBase(0).getPostprocessorValueByName(_pp_names[j]);
          pk1(j / 3, j % 3) = curr_pp_value;
        }
        userobject.P = pk1;
      }

      break;
    }
  }
}
