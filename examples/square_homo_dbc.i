# Simple 3D test

[GlobalParams]
  displacements = 'disp_x disp_y'
  large_kinematics = true
[]
[Variables]
  [disp_x]
  []
  [disp_y]
  []
[]
[Mesh]
  [msh]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 1
    ny = 1
  []
[]

[AuxVariables]
  [hvar_target_xx]
    family = SCALAR
    order = FIRST
  []
  [hvar_target_xy]
    family = SCALAR
    order = FIRST
  []
  [hvar_target_xz]
    family = SCALAR
    order = FIRST
  []
  [hvar_target_yx]
    family = SCALAR
    order = FIRST
  []
  [hvar_target_yy]
    family = SCALAR
    order = FIRST
  []
  [hvar_target_yz]
    family = SCALAR
    order = FIRST
  []
  [hvar_target_zx]
    family = SCALAR
    order = FIRST
  []
  [hvar_target_zy]
    family = SCALAR
    order = FIRST
  []
  [hvar_target_zz]
    family = SCALAR
    order = FIRST
  []
  [s11]
    family = MONOMIAL
    order = CONSTANT
  []
  [s21]
    family = MONOMIAL
    order = CONSTANT
  []
  [s31]
    family = MONOMIAL
    order = CONSTANT
  []
  [s12]
    family = MONOMIAL
    order = CONSTANT
  []
  [s22]
    family = MONOMIAL
    order = CONSTANT
  []
  [s32]
    family = MONOMIAL
    order = CONSTANT
  []
  [s13]
    family = MONOMIAL
    order = CONSTANT
  []
  [s23]
    family = MONOMIAL
    order = CONSTANT
  []
  [s33]
    family = MONOMIAL
    order = CONSTANT
  []

  [F11]
    family = MONOMIAL
    order = CONSTANT
  []
  [F21]
    family = MONOMIAL
    order = CONSTANT
  []
  [F31]
    family = MONOMIAL
    order = CONSTANT
  []
  [F12]
    family = MONOMIAL
    order = CONSTANT
  []
  [F22]
    family = MONOMIAL
    order = CONSTANT
  []
  [F32]
    family = MONOMIAL
    order = CONSTANT
  []
  [F13]
    family = MONOMIAL
    order = CONSTANT
  []
  [F23]
    family = MONOMIAL
    order = CONSTANT
  []
  [F33]
    family = MONOMIAL
    order = CONSTANT
  []
[]

[AuxKernels]
  [s11]
    type = RankTwoAux
    variable = s11
    rank_two_tensor = pk1_stress
    index_i = 0
    index_j = 0
  []
  [s21]
    type = RankTwoAux
    variable = s21
    rank_two_tensor = pk1_stress
    index_i = 1
    index_j = 0
  []
  [s31]
    type = RankTwoAux
    variable = s31
    rank_two_tensor = pk1_stress
    index_i = 2
    index_j = 0
  []
  [s12]
    type = RankTwoAux
    variable = s12
    rank_two_tensor = pk1_stress
    index_i = 0
    index_j = 1
  []
  [s22]
    type = RankTwoAux
    variable = s22
    rank_two_tensor = pk1_stress
    index_i = 1
    index_j = 1
  []
  [s32]
    type = RankTwoAux
    variable = s32
    rank_two_tensor = pk1_stress
    index_i = 2
    index_j = 1
  []
  [s13]
    type = RankTwoAux
    variable = s13
    rank_two_tensor = pk1_stress
    index_i = 0
    index_j = 2
  []
  [s23]
    type = RankTwoAux
    variable = s23
    rank_two_tensor = pk1_stress
    index_i = 1
    index_j = 2
  []
  [s33]
    type = RankTwoAux
    variable = s33
    rank_two_tensor = pk1_stress
    index_i = 2
    index_j = 2
  []

  [F11]
    type = RankTwoAux
    variable = F11
    rank_two_tensor = deformation_gradient
    index_i = 0
    index_j = 0
  []
  [F21]
    type = RankTwoAux
    variable = F21
    rank_two_tensor = deformation_gradient
    index_i = 1
    index_j = 0
  []
  [F31]
    type = RankTwoAux
    variable = F31
    rank_two_tensor = deformation_gradient
    index_i = 2
    index_j = 0
  []
  [F12]
    type = RankTwoAux
    variable = F12
    rank_two_tensor = deformation_gradient
    index_i = 0
    index_j = 1
  []
  [F22]
    type = RankTwoAux
    variable = F22
    rank_two_tensor = deformation_gradient
    index_i = 1
    index_j = 1
  []
  [F32]
    type = RankTwoAux
    variable = F32
    rank_two_tensor = deformation_gradient
    index_i = 2
    index_j = 1
  []
  [F13]
    type = RankTwoAux
    variable = F13
    rank_two_tensor = deformation_gradient
    index_i = 0
    index_j = 2
  []
  [F23]
    type = RankTwoAux
    variable = F23
    rank_two_tensor = deformation_gradient
    index_i = 1
    index_j = 2
  []
  [F33]
    type = RankTwoAux
    variable = F33
    rank_two_tensor = deformation_gradient
    index_i = 2
    index_j = 2
  []
[]
[Kernels]
  [sdx]
    type = TotalLagrangianStressDivergence
    variable = disp_x
    component = 0
  []
  [sdy]
    type = TotalLagrangianStressDivergence
    variable = disp_y
    component = 1
  []
[]

[Functions]
  [left_x_bc]
    type = ParsedFunction
    value = 'Fxy * y'
    vars = 'Fxy'
    vals = 'hvar_target_xy'
  []
  [left_y_bc]
    type = ParsedFunction
    value = '(Fyy - 1) * y'
    vars = 'Fyy'
    vals = 'hvar_target_yy'
  []
  [right_x_bc]
    type = ParsedFunction
    value = '(Fxx - 1) + Fxy * y'
    vars = 'Fxx Fxy'
    vals = 'hvar_target_xx hvar_target_xy'
  []
  [right_y_bc]
    type = ParsedFunction
    value = 'Fyx + (Fyy - 1) * y'
    vars = 'Fyx Fyy'
    vals = 'hvar_target_yx hvar_target_yy'
  []
  [top_x_bc]
    type = ParsedFunction
    value = '(Fxx - 1) * x + Fxy'
    vars = 'Fxx Fxy'
    vals = 'hvar_target_xx hvar_target_xy'
  []
  [top_y_bc]
    type = ParsedFunction
    value = 'Fyx * x + (Fyy - 1)'
    vars = 'Fyx Fyy'
    vals = 'hvar_target_yx hvar_target_yy'
  []

  [bottom_x_bc]
    type = ParsedFunction
    value = '(Fxx - 1) * x'
    vars = 'Fxx'
    vals = 'hvar_target_xx'
  []
  [bottom_y_bc]
    type = ParsedFunction
    value = 'Fyx * x'
    vars = 'Fyx'
    vals = 'hvar_target_yx'
  []
[]

[BCs]
  [leftx]
    type = FunctionDirichletBC
    boundary = left
    variable = disp_x
    # function = '0.01 * y'
    function = left_x_bc
  []
  [lefty]
    type = FunctionDirichletBC
    boundary = left
    variable = disp_y
    # function = '-0.02 * y'
    function = left_y_bc
  []
  [rightx]
    type = FunctionDirichletBC
    boundary = right
    variable = disp_x
    # function = '0.01 * y + 0.04'
    function = right_x_bc
  []
  [righty]
    type = FunctionDirichletBC
    boundary = right
    variable = disp_y
    # function = '-0.02 * y - 0.02'
    function = right_y_bc
  []
  [topx]
    type = FunctionDirichletBC
    boundary = top
    variable = disp_x
    # function = '0.04 * x + 0.01'
    function = top_x_bc
  []
  [topy]
    type = FunctionDirichletBC
    boundary = top
    variable = disp_y
    # function = '-0.02 * x - 0.02'
    function = top_y_bc
  []
  [botx]
    type = FunctionDirichletBC
    boundary = bottom
    variable = disp_x
    # function = '0.04 * x'
    function = bottom_x_bc
  []
  [boty]
    type = FunctionDirichletBC
    boundary = bottom
    variable = disp_y
    # function = '-0.02 * x'
    function = bottom_y_bc
  []
[]
[Materials]
  [C]
    type = ComputeIsotropicElasticityTensor
    lambda = 4000
    shear_modulus = 6700
  []
  [compute_stress]
    type = ComputeStVenantKirchhoffStress
  []
  [compute_strain]
    type = ComputeLagrangianStrain
  []
  [stress_xx]
    type = RankTwoCartesianComponent
    property_name = sxx
    rank_two_tensor = pk1_stress
    index_i = 0
    index_j = 0
  []
[]
[Postprocessors]
  [s11]
    type = ElementAverageValue
    variable = s11
    execute_on = 'initial timestep_end'
  []
  [s21]
    type = ElementAverageValue
    variable = s21
    execute_on = 'initial timestep_end'
  []
  [s31]
    type = ElementAverageValue
    variable = s31
    execute_on = 'initial timestep_end'
  []
  [s12]
    type = ElementAverageValue
    variable = s12
    execute_on = 'initial timestep_end'
  []
  [s22]
    type = ElementAverageValue
    variable = s22
    execute_on = 'initial timestep_end'
  []
  [s32]
    type = ElementAverageValue
    variable = s32
    execute_on = 'initial timestep_end'
  []
  [s13]
    type = ElementAverageValue
    variable = s13
    execute_on = 'initial timestep_end'
  []
  [s23]
    type = ElementAverageValue
    variable = s23
    execute_on = 'initial timestep_end'
  []
  [s33]
    type = ElementAverageValue
    variable = s33
    execute_on = 'initial timestep_end'
  []

  [F11]
    type = ElementAverageValue
    variable = F11
    execute_on = 'initial timestep_end'
  []
  [F21]
    type = ElementAverageValue
    variable = F21
    execute_on = 'initial timestep_end'
  []
  [F31]
    type = ElementAverageValue
    variable = F31
    execute_on = 'initial timestep_end'
  []
  [F12]
    type = ElementAverageValue
    variable = F12
    execute_on = 'initial timestep_end'
  []
  [F22]
    type = ElementAverageValue
    variable = F22
    execute_on = 'initial timestep_end'
  []
  [F32]
    type = ElementAverageValue
    variable = F32
    execute_on = 'initial timestep_end'
  []
  [F13]
    type = ElementAverageValue
    variable = F13
    execute_on = 'initial timestep_end'
  []
  [F23]
    type = ElementAverageValue
    variable = F23
    execute_on = 'initial timestep_end'
  []
  [F33]
    type = ElementAverageValue
    variable = F33
    execute_on = 'initial timestep_end'
  []
[]
[Executioner]
  # type = Transient
  type = Steady
  solve_type = 'newton'
  #   solve_type = FD
  line_search = none
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  l_max_its = 2
  l_tol = 1e-14
  nl_max_its = 10
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10
[]
[Outputs]
  exodus = true
  csv = true
  console = false
[]
