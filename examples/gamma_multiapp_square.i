# Simple 2D test

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
    type = FileMeshGenerator
    file = 'mesh/gamma.e'
    use_for_exodus_restart = true
  []
  [add_side_sets]
    type = SideSetsFromNormalsGenerator
    input = msh
    normals = ' -1  0  0
               1 0  0'
    new_boundary = 'left right'
    variance = 1e-3
    fixed_normal = true

  []
[]
[AuxVariables]
  [mu1]
    initial_from_file_var = 'gamma'
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
  [strain]
    type = ParsedFunction
    value = 't'
  []
[]
[BCs]
  [leftx]
    type = DirichletBC
    preset = true
    boundary = left
    variable = disp_x
    value = 0.0
  []
  [lefty]
    type = DirichletBC
    preset = true
    boundary = left
    variable = disp_y
    value = 0.0
  []
  [pull_x]
    type = FunctionDirichletBC
    boundary = right
    variable = disp_x
    function = strain
  []
[]
[Materials]
  [mu1]
    type = ParsedMaterial
    f_name = 'mu1'
    args = 'mu1'
    function = 'mu1'
  []
  [C]
    type = ComputeIsotropicElasticityTensor
    lambda = 4000
    shear_modulus = 6700
  []
  [stress]
    type = FE2PK1Stress
    fe2_uo = fe2
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
  solve_type = 'newton'
  line_search = none
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  nl_max_its = 50
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10
  start_time = 0.0
  dt = 0.01
  end_time = 0.01
[]
[Outputs]
  exodus = true
  perf_graph_live = false
[]
[UserObjects]
  [fe2]
    type = FE2UserObject
  []
[]
[MultiApps]
  [sub]
    type = MicroScaleMultiApp
    input_files = 'square_homo_dbc.i'
    max_procs_per_app = 1
  []
[]
[Transfers]
  [FE2_transfer]
    type = FE2Transfer
    fe2_uo = fe2
    def_grad_scalars = 'hvar_target_xx hvar_target_xy hvar_target_xz hvar_target_yx hvar_target_yy hvar_target_yz hvar_target_zx hvar_target_zy hvar_target_zz mu1'
    pk1_stress_components = 's11 s12 s13 s21 s22 s23 s31 s32 s33'
    to_multi_app = sub
    from_multi_app = sub
  []
[]
