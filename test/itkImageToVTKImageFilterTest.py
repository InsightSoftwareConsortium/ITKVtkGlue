import itk

ImageType = itk.Image[itk.F, 2]

source = itk.RandomImageSource[ImageType].New()
size = itk.Size[2]()
size.Fill(20)
source.SetSize(size)

connector =itk.ImageToVTKImageFilter[ImageType].New()
connector.SetInput(source.GetOutput())

connector.UpdateLargestPossibleRegion();

print(connector)
print(connector.GetOutput())
