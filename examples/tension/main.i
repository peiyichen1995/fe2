# Simple 2D test

L = 1
h = '${fparse L/n}'

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
    xmin = 1
    ymin = 1
    xmax = 2
    ymax = 2
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
    mesh = '../../../../../data0/pc178/fe2/BCs/scene2_nov/sample_${sample}.e'
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
  [stress]
    type = FE2PK1Stress
    fe2_uo = fe2
    f_pk1_uo = fp
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
  end_time = 0.1

  [Predictor]
    type = SimplePredictor
    scale = 1
  []
[]

[Outputs]
  exodus = false
  # file_base = '../../../../../data0/pc178/fe2/outputs/scene2_nov/sample_${sample}'
[]

[UserObjects]
  [fe2]
    type = FE2UserObject
  []
  [fp]
    type = DeformationGradientPK1StressUserObject
    file_name = "../../../../../data0/pc178/fe2/datas/scene2_nov/FP_${sample}.csv"
    execute_on = TIMESTEP_END
  []
[]

[MultiApps]
  [sub]
    type = MicroScaleMultiApp
    input_files = 'micro.i'
    max_procs_per_app = 1
    cli_args = 'h=${h};n=${n_micro};sample=${sample}'
  []
[]

[Transfers]
  [FE2_transfer]
    type = FE2Transfer
    fe2_uo = fe2
    def_grad_scalars = 'hvar_target_xx hvar_target_xy hvar_target_xz hvar_target_yx hvar_target_yy hvar_target_yz hvar_target_zx hvar_target_zy hvar_target_zz'
    translation_scalars = 'transl_x transl_y'
    pk1_stress_components = 's11 s12 s13 s21 s22 s23 s31 s32 s33'
    FE2Exodus_name = 'exo'
    to_multi_app = sub
    from_multi_app = sub
  []
[]
