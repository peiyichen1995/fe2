#include "FE2Exodus.h"

#include "libmesh/exodusII_io.h"

registerMooseObject("fe2App", FE2Exodus);

InputParameters
FE2Exodus::validParams()
{
  InputParameters params = Exodus::validParams();
  return params;
}

FE2Exodus::FE2Exodus(const InputParameters & parameters) : Exodus(parameters) {}

void
FE2Exodus::output(const ExecFlagType & type)
{
  Exodus::output(type);
  clear();
}

std::string
FE2Exodus::filename()
{
  // Append the .e extension on the base file name
  std::ostringstream output;
  output << _file_base << "_step_" << time_step;
  output << "_x_" << translation(0);
  output << "_y_" << translation(1);
  output << "_z_" << translation(2);
  output << ".e";

  // Add the -s00x extension to the file
  if (_file_num > 1)
    output << "-s" << std::setw(_padding) << std::setprecision(0) << std::setfill('0') << std::right
           << _file_num;

  // Return the filename
  std::string fname = output.str();
  return fname;
}