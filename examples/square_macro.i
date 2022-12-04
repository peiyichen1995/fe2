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
    nx = 25
    ny = 25
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
  [lefty]
    type = DirichletBC
    preset = true
    boundary = left
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
  nl_max_its = 50
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10
  start_time = 0.0
  dt = 0.01
  end_time = 0.01
[]
[Outputs]
  exodus = true
[]
