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
  type = FileMesh
  file = 'dogbone.msh'
  uniform_refine = 2
[]

[AuxVariables]
  [s11]
    family = MONOMIAL
    order = CONSTANT
  []
  [F11]
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
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [F11]
    type = RankTwoAux
    variable = F11
    rank_two_tensor = deformation_gradient
    index_i = 0
    index_j = 0
    execute_on = 'INITIAL TIMESTEP_END'
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
    type = FunctionDirichletBC
    boundary = left
    variable = disp_x
    function = '-t'
    preset = false
  []
  [lefty]
    type = DirichletBC
    boundary = left
    variable = disp_y
    value = 0.0
  []
  [rightx]
    type = DirichletBC
    boundary = right
    variable = disp_x
    value = 0.0
  []
  [topy]
    type = DirichletBC
    boundary = top
    variable = disp_y
    value = 0.0
  []
[]

[Materials]
  [C]
    type = CustomIsotropicElasticityTensor
    # type = ComputeIsotropicElasticityTensor
    lambda = 16000
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
  dt = 0.1
  end_time = 10.0
[]

[Outputs]
  [exodus]
    type = Exodus
  []
  [csv]
    type = CSV
  []
[]

[Postprocessors]
  [s11]
    type = PointValue
    variable = 's11'
    point = '50.0 5.0 0.0'
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [F11]
    type = PointValue
    variable = 'F11'
    point = '50.0 5.0 0.0'
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]
