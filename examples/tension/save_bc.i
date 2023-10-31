# Simple 2D test

[Problem]
  solve = false
[]

[AuxVariables]
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

[UserObjects]
  [sol]
    type = SolutionUserObject
    mesh = 'BCs/scene2/sample_${sample}.e'
    timestep = 'LATEST'
    execute_on = 'INITIAL'
  []
[]

[AuxKernels]
  [bc_x]
    type = SolutionAux
    solution = sol
    variable = disp_x
    from_variable = disp_x
    execute_on = 'INITIAL'
  []
  [bc_y]
    type = SolutionAux
    solution = sol
    variable = disp_y
    from_variable = disp_y
    execute_on = 'INITIAL'
  []
[]

[Executioner]
  type = Steady
[]

[Outputs]
  [csv]
    type = CSV
    execute_on = 'INITIAL'
    file_base = 'BCs_csv/scene2/sample_${sample}_'
  []
[]

[VectorPostprocessors]
  [top_x]
    type = LineValueSampler
    start_point = '1 2 0'
    end_point = '2 2 0'
    num_points = 6
    sort_by = x
    variable = disp_x
    execute_on = 'INITIAL'
  []
  [top_y]
    type = LineValueSampler
    start_point = '1 2 0'
    end_point = '2 2 0'
    num_points = 6
    sort_by = x
    variable = disp_y
    execute_on = 'INITIAL'
  []
  [bottom_x]
    type = LineValueSampler
    start_point = '1 1 0'
    end_point = '2 1 0'
    num_points = 6
    sort_by = x
    variable = disp_x
    execute_on = 'INITIAL'
  []
  [bottom_y]
    type = LineValueSampler
    start_point = '1 1 0'
    end_point = '2 1 0'
    num_points = 6
    sort_by = x
    variable = disp_y
    execute_on = 'INITIAL'
  []
  [left_x]
    type = LineValueSampler
    start_point = '1 1 0'
    end_point = '1 2 0'
    num_points = 6
    sort_by = y
    variable = disp_x
    execute_on = 'INITIAL'
  []
  [left_y]
    type = LineValueSampler
    start_point = '1 1 0'
    end_point = '1 2 0'
    num_points = 6
    sort_by = y
    variable = disp_y
    execute_on = 'INITIAL'
  []
  [right_x]
    type = LineValueSampler
    start_point = '2 1 0'
    end_point = '2 2 0'
    num_points = 6
    sort_by = y
    variable = disp_x
    execute_on = 'INITIAL'
  []
  [right_y]
    type = LineValueSampler
    start_point = '2 1 0'
    end_point = '2 2 0'
    num_points = 6
    sort_by = y
    variable = disp_y
    execute_on = 'INITIAL'
  []
[]
