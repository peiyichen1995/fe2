#pragma once

// MOOSE includes
#include "Exodus.h"

// libMesh forward declarations
namespace libMesh
{
class ExodusII_IO;
}

class FE2Exodus : public Exodus
{
public:
  static InputParameters validParams();

  FE2Exodus(const InputParameters & parameters);

  virtual void output(const ExecFlagType & type) override;

  size_t time_step;

  Point translation;

protected:
  virtual std::string filename() override;
};
