# Simple 2D test

L = 1
# n = 2
# n_micro = 2
h = '${fparse L/n}'
# sample = 0

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

[Functions]
  [ux]
    type = ParsedFunction
    value = '((Fxx - 1) * x + Fxy * y) * t'
    vars = 'Fxx Fxy'
    vals = '${Fxx} ${Fxy}'
  []
  [uy]
    type = ParsedFunction
    value = '((Fyy - 1) * y + Fyx * x) * t'
    vars = 'Fyy Fyx'
    vals = '${Fyy} ${Fyx}'
  []
[]

[BCs]
  [x]
    type = FunctionDirichletBC
    boundary = 'left right top bottom'
    variable = disp_x
    function = ux
  []
  [y]
    type = FunctionDirichletBC
    boundary = 'left right top bottom'
    variable = disp_y
    function = uy
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
  end_time = 0.2

  [Predictor]
    type = SimplePredictor
    scale = 1
  []
[]

[Outputs]
  exodus = true
  file_base = './output/sample_${sample}'
[]

[UserObjects]
  [fe2]
    type = FE2UserObject
  []
  [fp]
    type = DeformationGradientPK1StressUserObject
    file_name = "data/FP_${sample}.csv"
    execute_on = TIMESTEP_END
  []
[]

[MultiApps]
  [sub]
    type = MicroScaleMultiApp
    input_files = 'square_micro.i'
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
