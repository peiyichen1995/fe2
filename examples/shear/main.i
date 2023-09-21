# Simple 2D test

L = 1
n = 100

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

[UserObjects]
  [sol]
    type = SolutionUserObject
    mesh = 'shear_out.e'
    # translation = '-1 -1 0 '
  []
[]

[Functions]
  [ux]
    type = SolutionFunction
    solution = sol
    from_variable = disp_x
  []
  [uy]
    type = SolutionFunction
    solution = sol
    from_variable = disp_y
  []
[]

[BCs]
  [x]
    type = FunctionDirichletBC
    variable = disp_x
    function = ux
    boundary = 'top bottom left right'
    preset = false
  []
  [y]
    type = FunctionDirichletBC
    variable = disp_y
    function = uy
    boundary = 'top bottom left right'
    preset = false
  []
[]

[Materials]
  [C]
    type = ComputeIsotropicElasticityTensor
    lambda = 40000
    shear_modulus = 10000
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
