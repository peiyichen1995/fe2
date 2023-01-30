from paraview.simple import *

import glob

fnames = glob.glob("visualization/out_step_1_*.e")

    
outname = "stitched_random_field.png"
# lb = 9000
# ub = 10000
lb = 63000
ub = 26000
resolution = [1200, 800]
field_name = "gamma"

# get active view
renderView = GetActiveViewOrCreate('RenderView')

for fname in fnames:
    print(fname)

    tokens = fname.split("_")
    x = float(tokens[-5])
    y = float(tokens[-3])
    translation = [x, y, 0]

    file = IOSSReader(registrationName=fname, FileName=[fname])

    # get display properties
    fileDisplay = Show(file, renderView, 'UnstructuredGridRepresentation')

    # change representation type
    fileDisplay.SetRepresentationType('Surface With Edges')

    # set scalar coloring
    ColorBy(fileDisplay, ('POINTS', field_name))
    fileDisplay.SetScalarBarVisibility(renderView, False)
    gammaLUT = GetColorTransferFunction(field_name)
    gammaLUT.RescaleTransferFunction(lb, ub)

    # Properties modified on file
    file.ApplyDisplacements = 0

    fileDisplay.Position = translation
    fileDisplay.DataAxesGrid.Position = translation
    fileDisplay.PolarAxes.Translation = translation

renderView.InteractionMode = '2D'
renderView.CameraPosition = [0.5, 0.5, 0.5]
renderView.CameraFocalPoint = [0.5, 0.5, 0.0]
renderView.CameraParallelScale = 0.6
renderView.OrientationAxesVisibility = 0
renderView.UseColorPaletteForBackground = 0
renderView.Background = [1.0, 1.0, 1.0]

# update the view to ensure updated data information
renderView.Update()

SaveScreenshot(outname, renderView, ImageResolution=resolution)