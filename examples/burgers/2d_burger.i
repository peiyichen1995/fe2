Re = 10000
mu = 0.9
[GlobalParams]
  velocities = 'vel_x vel_y'
[]
[Mesh]
  [gen_mesh]
    type = GeneratedMeshGenerator
    dim = 2
    xmin = 0
    xmax = 1
    ymin = 0
    ymax = 1
    nx = 60
    ny = 60
  []
[]
[Variables]
  [vel_x]
  []
  [vel_y]
  []
[]
[ICs]
  [vel_x_ic]
    type = FunctionIC
    variable = vel_x
    function = 'if (x <= 0.5 & y <= 0.5, ${mu}*sin(2*pi*x)*sin(2*pi*y), 0)'
  []
  [vel_y_ic]
    type = FunctionIC
    variable = vel_y
    function = 'if (x <= 0.5 & y <= 0.5, ${mu}*sin(2*pi*x)*sin(2*pi*y), 0)'
  []
[]
[Kernels]
  [advection_x]
    type = Burgers
    variable = vel_x
    component = 0
    Re = ${Re}
  []
  [advection_y]
    type = Burgers
    variable = vel_y
    component = 1
    Re = ${Re}
  []
  [time_x]
    type = TimeDerivative
    variable = vel_x
  []
  [time_y]
    type = TimeDerivative
    variable = vel_y
  []
  [diff_x]
    type = MatDiffusion
    variable = vel_x
    diffusivity = '${fparse 1/Re}'
  []
  [diff_y]
    type = MatDiffusion
    variable = vel_y
    diffusivity = '${fparse 1/Re}'
  []
[]
[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  petsc_options = '-snes_converged_reason'
  nl_abs_tol = 1e-7
  nl_rel_tol = 1e-8
  dt = 0.005
  end_time = 2
[]
[Outputs]
  exodus = true
[]
