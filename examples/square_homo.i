[GlobalParams]
  displacements = 'disp_x disp_y'
  large_kinematics = true
  macro_gradient = hvar
  homogenization_constraint = homogenization
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [hvar]
    family = SCALAR
    order = FOURTH
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

[UserObjects]
  [homogenization]
    type = HomogenizationConstraint
    constraint_types = 'strain strain none strain strain none none none none'
    targets = '0.04 -0.02 0.01 -0.02'
    execute_on = 'INITIAL LINEAR NONLINEAR'
  []
[]

[Kernels]
  [sdx]
    type = HomogenizedTotalLagrangianStressDivergence
    variable = disp_x
    component = 0
  []
  [sdy]
    type = HomogenizedTotalLagrangianStressDivergence
    variable = disp_y
    component = 1
  []
[]

[ScalarKernels]
  [enforce]
    type = HomogenizationConstraintScalarKernel
    variable = hvar
  []
[]

[BCs]
  [Periodic]
    [x]
      variable = disp_x
      auto_direction = 'x y'
    []
    [y]
      variable = disp_y
      auto_direction = 'x y'
    []
  []

  # [fix1_x]
  #   type = DirichletBC
  #   boundary = "fix1"
  #   variable = disp_x
  #   value = 0
  # []
  # [fix1_y]
  #   type = DirichletBC
  #   boundary = "fix1"
  #   variable = disp_y
  #   value = 0
  # []

  # [fix2_y]
  #   type = DirichletBC
  #   boundary = "fix2"
  #   variable = disp_y
  #   value = 0
  # []
[]

[AuxVariables]
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
    homogenization_gradient_names = 'homogenization_gradient'
  []
  [compute_homogenization_gradient]
    type = ComputeHomogenizedLagrangianStrain
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
  type = Transient

  solve_type = 'newton'
  # solve_type = FD
  line_search = none

  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  l_max_its = 2
  l_tol = 1e-14
  nl_max_its = 10
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10

  start_time = 0.0
  dt = 1.0
  dtmin = 1.0
  end_time = 1.0
[]

[Outputs]
  exodus = true
  csv = true
[]
