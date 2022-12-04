#pragma once

#include "FullSolveMultiApp.h"

#include "fe2Types.h"

/**
 * Automatically generates Sub-App positions from centroids of elements in the master mesh.
 */
class MicroScaleMultiApp : public FullSolveMultiApp
{
public:
  static InputParameters validParams();

  MicroScaleMultiApp(const InputParameters & parameters);

protected:
};
