# Simple 2D test

L = 1
n = 64
sample = 0

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
    xmax = ${L}
    ymax = ${L}
    nx = ${n}
    ny = ${n}
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

[BCs]
  [leftx]
    type = DirichletBC
    boundary = left
    variable = disp_x
    value = 0.0
  []
  [lefty]
    type = DirichletBC
    boundary = left
    variable = disp_y
    value = 0.0
  []
  [pull_x]
    type = FunctionDirichletBC
    boundary = right
    variable = disp_x
    function = 't'
    preset = false
  []
  [righty]
    type = DirichletBC
    boundary = right
    variable = disp_y
    value = 0.0
  []
[]

[AuxVariables]
  [gamma]
  []
[]

[AuxKernels]
  [gamma]
    type = FE2SolutionAux
    solution = stochastic_field
    variable = gamma
    from_variable = gamma
    execute_on = 'LINEAR'
    # scale_factor = 100
    # add_factor = 9000
  []
[]

[UserObjects]
  [stochastic_field]
    type = FE2SolutionUserObject
    mesh = 'gammas/gamma_${sample}.e'
    execute_on = 'INITIAL'
    timestep = 'LATEST'
    translation_scalar_vars = '0 0 0'
  []
[]

[Materials]
  [C]
    type = CustomIsotropicElasticityTensor
    lambda = 'gamma'
    shear_modulus = 6700
    # lambda = 40000
    # shear_modulus = 10000
  []
  [compute_stress]
    type = ComputeStVenantKirchhoffStress
  []
  [compute_strain]
    type = ComputeLagrangianStrain
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  line_search = none
  automatic_scaling = true
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  nl_max_its = 12
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  start_time = 0.0
  dt = 0.01
  end_time = 0.05

  [Predictor]
    type = SimplePredictor
    scale = 1
  []
[]

[Outputs]
  exodus = true
[]
