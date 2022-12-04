[Mesh]
  [msh]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 25
    ny = 25
  []
[]

[UserObjects]
  [fe2]
    type = SolutionUserObject
    mesh = 'square_multiapp_out.e'
    execute_on = 'INITIAL'
    timestep = 'LATEST'
  []
  [macro]
    type = SolutionUserObject
    mesh = 'square_macro_out.e'
    execute_on = 'INITIAL'
    timestep = 'LATEST'
  []
[]

[AuxVariables]
  [fe2_disp_x]
  []
  [fe2_disp_y]
  []
  [macro_disp_x]
  []
  [macro_disp_y]
  []
  [error]
  []
[]

[AuxKernels]
  [fe2_disp_x]
    type = SolutionAux
    solution = fe2
    variable = fe2_disp_x
    from_variable = disp_x
    execute_on = 'INITIAL'
  []
  [fe2_disp_y]
    type = SolutionAux
    solution = fe2
    variable = fe2_disp_y
    from_variable = disp_y
    execute_on = 'INITIAL'
  []
  [macro_disp_x]
    type = SolutionAux
    solution = macro
    variable = macro_disp_x
    from_variable = disp_x
    execute_on = 'INITIAL'
  []
  [macro_disp_y]
    type = SolutionAux
    solution = macro
    variable = macro_disp_y
    from_variable = disp_y
    execute_on = 'INITIAL'
  []
  [error]
    type = ParsedAux
    variable = error
    args = 'fe2_disp_x fe2_disp_y macro_disp_x macro_disp_y'
    function = '(fe2_disp_x - macro_disp_x)^2 + (fe2_disp_y - macro_disp_y)^2'
    execute_on = 'INITIAL'
  []
[]

[Postprocessors]
  [error_squared]
    type = ElementIntegralVariablePostprocessor
    variable = error
  []
  [error]
    type = ParsedPostprocessor
    pp_names = error_squared
    function = 'sqrt(error_squared)'
  []
[]

[Executioner]
  type = Steady
[]

[Problem]
  solve = false
[]

[Outputs]
  exodus = true
[]
