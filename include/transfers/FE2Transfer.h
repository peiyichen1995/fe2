#pragma once

// MOOSE includes
#include "MultiAppTransfer.h"
#include "TransientInterface.h"
#include "FE2Exodus.h"

class FE2Transfer : public MultiAppTransfer, TransientInterface
{
public:
  static InputParameters validParams();

  FE2Transfer(const InputParameters & parameters);

  /// Execute the transfer
  virtual void execute() override;

protected:
  /// The name of the scalar variable from which the values are being transfered
  UserObjectName _uo_name;

  /// The name of the auxiliary scalar variable to which the scalar values are being transfered
  std::vector<VariableName> _scalar_names;

  // The name of the auxiliary scalar variable to which the translation scalar values are being
  // transfered
  std::vector<VariableName> _translation_names;

  /// The name of the auxiliary scalar variable to which the scalar values are being transfered
  std::vector<PostprocessorName> _pp_names;

  std::string _exo_name;
};
