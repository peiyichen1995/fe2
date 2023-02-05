Re = 1000

[GlobalParams]
  velocities = 'v'
  r = 2
  nu = '${fparse 1/Re}'
[]

[Mesh]
  [gen_mesh]
    type = GeneratedMeshGenerator
    dim = 1
    xmin = 0
    xmax = 2
    nx = 1000
  []
  second_order = true
[]
[Variables]
  [v]
    order = SECOND
  []
[]
[ICs]
  [v_ic]
    type = FunctionIC
    variable = v
    function = 'if (x > 1 & x <= 2, 1, 1 + 0.5 * (sin(2 * pi * x - pi / 2) + 1))'
  []
[]
[Kernels]
  [time]
    type = SUPGTimeDerivative
    variable = v
    component = 0
  []
  [advection]
    type = SUPGAdvection
    variable = v
    component = 0
  []
  [diffusion]
    type = SUPGDiffusion
    variable = v
    component = 0
  []
[]

[Executioner]
  type = Transient
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  petsc_options = '-snes_converged_reason'
  nl_abs_tol = 1e-7
  nl_rel_tol = 1e-8
  dt = 0.001
  end_time = 0.5
[]
[Outputs]
  exodus = true
[]
