// MOOSE includes
#include "MicroScaleMultiApp.h"

registerMooseObject("MooseApp", MicroScaleMultiApp);

InputParameters
MicroScaleMultiApp::validParams()
{
  InputParameters params = FullSolveMultiApp::validParams();
  ExecFlagEnum & exec = params.set<ExecFlagEnum>("execute_on");
  exec.addAvailableFlags(FE2::EXEC_FE2);
  exec = {FE2::EXEC_FE2};
  params.suppressParameter<ExecFlagEnum>("execute_on");
  return params;
}

MicroScaleMultiApp::MicroScaleMultiApp(const InputParameters & parameters)
  : FullSolveMultiApp(parameters)
{
}
