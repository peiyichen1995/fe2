# Simple 3D test

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  large_kinematics = true
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [disp_z]
  []
[]

[Mesh]
  [msh]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 1
    ny = 1
    nz = 1
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
  [sdz]
    type = TotalLagrangianStressDivergence
    variable = disp_z
    component = 2
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
    boundary = left
    variable = disp_x
    value = 0.0
  []
  [boty]
    type = DirichletBC
    boundary = bottom
    variable = disp_y
    value = 0.0
  []
  [backz]
    type = DirichletBC
    boundary = back
    variable = disp_z
    value = 0.0
  []

  [pull_x]
    type = FunctionDirichletBC
    boundary = right
    variable = disp_x
    function = strain
    preset = false
  []
[]

[Materials]
  # [C]
  #   type = ComputeIsotropicElasticityTensor
  #   lambda = 4000
  #   shear_modulus = 6700
  # []
  # [compute_stress]
  #   type = ComputeStVenantKirchhoffStress
  # []
  [stress]
    type = TorchStress
    PK1_script = 'PK1.pt'
    dPK1_script = 'dPK1.pt'
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

  solve_type = NEWTON
  line_search = none

  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  nl_max_its = 50
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10

  dt = 0.01
  end_time = 0.1
  num_steps = 2
[]

[Outputs]
  exodus = true
[]
