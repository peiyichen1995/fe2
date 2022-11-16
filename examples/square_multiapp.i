# Simple 2D test

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
  [strain]
    type = ParsedFunction
    value = 't'
  []
[]

[BCs]
  [leftx]
    type = DirichletBC
    preset = true
    boundary = left
    variable = disp_x
    value = 0.0
  []
  [boty]
    type = DirichletBC
    preset = true
    boundary = bottom
    variable = disp_y
    value = 0.0
  []
  [pull_x]
    type = FunctionDirichletBC
    boundary = right
    variable = disp_x
    function = strain
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
  [sxx]
    type = ElementIntegralMaterialProperty
    mat_prop = sxx
  []
[]

[Executioner]
  type = Transient

  solve_type = 'newton'
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
  # fixed_point_algorithm = picard
[]

[Outputs]
  exodus = true
[]

[UserObjects]
  [defF]
    type = DeformationGradientUserObject
    verbose = false
    execute_on = NONLINEAR
  []
[]

[MultiApps]
  [sub]
    type = QuadraturePointMultiApp
    input_files = 'square_homo_dbc.i'
    order = FIRST
    family = LAGRANGE
    qudrature_type = GAUSS
    qudrature_order = SECOND
    execute_on = TIMESTEP_END
  []
[]

[Transfers]
  [FE2]
    type = FE2Transfer
    def_grad_uo = defF
    def_grad_scalars = 'hvar_target_xx hvar_target_xy hvar_target_xz hvar_target_yx hvar_target_yy hvar_target_yz hvar_target_zx hvar_target_zy hvar_target_zz'
    to_multi_app = sub
  []
[]
