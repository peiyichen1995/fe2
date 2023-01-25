Re = 1000

[GlobalParams]
  velocities = 'v'
[]

[Mesh]
  [gen_mesh]
    type = GeneratedMeshGenerator
    dim = 1
    xmin = 0
    xmax = 2
    nx = 1000
  []
[]
[Variables]
  [v]
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
  [burgers]
    type = Burgers
    variable = v
    component = 0
    Re = ${Re}
  []
  [time]
    type = TimeDerivative
    variable = v
  []
  [diff]
    type = MatDiffusion
    diffusivity = '${fparse 1/Re}'
    variable = v
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
