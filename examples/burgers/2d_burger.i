Re = 1000
mu = 0.3
[GlobalParams]
  velocities = 'vel_x vel_y'
  sigma_BDF = 1
  nu = '${fparse 1/Re}'
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
  second_order = true
[]
[Variables]
  [vel_x]
    order = SECOND
  []
  [vel_y]
    order = SECOND
  []
[]
[ICs]
  [vel_x_ic]
    type = FunctionIC
    variable = vel_x
    function = 'if (x <= 0.5 & y <= 0.5, ${mu}*sin(2*pi*x)*sin(2*pi*y), 0)'
    # function = 'r:=sqrt(x*x+y*y); if (r > 1, 1, 1 + 0.5 * (sin(2 * pi * r - pi / 2) + 1))'
  []
  [vel_y_ic]
    type = FunctionIC
    variable = vel_y
    function = 'if (x <= 0.5 & y <= 0.5, ${mu}*sin(2*pi*x)*sin(2*pi*y), 0)'
    # function = 'r:=sqrt(x*x+y*y); if (r > 1, 1, 1 + 0.5 * (sin(2 * pi * r - pi / 2) + 1))'
  []
[]
[Kernels]
  [time_x]
    type = SUPGTimeDerivative
    variable = vel_x
    component = 0
  []
  [time_y]
    type = SUPGTimeDerivative
    variable = vel_y
    component = 1
  []
  [advection_x]
    type = SUPGAdvection
    variable = vel_x
    component = 0
  []
  [advection_y]
    type = SUPGAdvection
    variable = vel_y
    component = 1
  []
  [diffusion_x]
    type = SUPGDiffusion
    variable = vel_x
    component = 0
  []
  [diffusion_y]
    type = SUPGDiffusion
    variable = vel_y
    component = 1
  []
[]
[Executioner]
  # [TimeIntegrator]
  #   type = BDF2
  # []
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_type'
  petsc_options_value = 'lu gmres'
  reuse_preconditioner = true
  reuse_preconditioner_max_linear_its = 10
  # petsc_options_iname = '-pc_type -pc_hypre_type -ksp_type'
  # petsc_options_value = 'hypre boomeramg gmres'
  petsc_options = '-snes_converged_reason'
  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-8
  num_steps = 500
  end_time = 2
[]
[Outputs]
  exodus = true
[]
