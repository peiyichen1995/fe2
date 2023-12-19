[Mesh]
  [msh]
    type = GeneratedMeshGenerator
    dim = 2
    xmin = 1
    ymin = 1
    xmax = 2
    ymax = 2
    nx = 5
    ny = 5
  []
[]
[UserObjects]
  [fe2]
    type = SolutionUserObject
    mesh = '../../../../../data0/pc178/fe2/outputs/scene2/sample_${sample}.e'
    execute_on = 'INITIAL'
    timestep = 'LATEST'
  []
  [macro]
    type = SolutionUserObject
    mesh = '../../../../../data0/pc178/fe2/BCs/scene2/sample_${sample}.e'
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
  [ref_sol]
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
  [ref_sol]
    type = ParsedAux
    variable = ref_sol
    args = 'macro_disp_x macro_disp_y'
    function = 'sqrt((macro_disp_x)^2 + (macro_disp_y)^2)'
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
  [ref_sol]
    type = ElementIntegralVariablePostprocessor
    variable = ref_sol
  []
  [normalized_error]
    type = ParsedPostprocessor
    pp_names = 'error ref_sol'
    function = 'error / ref_sol'
  []
[]
[Executioner]
  type = Steady
[]
[Problem]
  solve = false
[]

[Outputs]
  [csv]
    type = CSV
    execute_on = 'TIMESTEP_END'
    file_base = 'l2-norm/scene2/sample_${sample}'
  []
[]
