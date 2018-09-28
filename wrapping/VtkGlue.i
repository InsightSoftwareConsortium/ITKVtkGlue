/* This file declares various VTK datatype to SWIG so they can be refered as*/
/* something else than SwigObject*/
/* Python bindings have been tested successfully.*/
/* TODO: Test TCL bindings*/

%include exception.i

%{
// VTK also includes a Py_hash_t typedef definition for Python 2 that clashes
// with SWIG's preprocessor macro
#if PY_VERSION_HEX < 0x3020000
#ifdef Py_hash_t
#undef Py_hash_t
#endif
#endif

#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "itkImageToImageFilter.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "vtkImageData.h"

%}


// We need to ensure that the VTK libraries are loaded, which are used by this
// module, but with weak linking. Since VTK does not use lazy loading yet,
// just 'import vtk' should do the trick.
// Then, dlopen the required libraries with RTLD_GLOBAL
%pythonbegin %{
import vtk
import ctypes
_vtk_major_version = str(vtk.vtkVersion.GetVTKMajorVersion())
_vtk_minor_version = str(vtk.vtkVersion.GetVTKMinorVersion())
_vtkIOlib = 'libvtkIO-' + _vtk_major_version + '.' + _vtk_minor_version + '.so.1'
ctypes.CDLL(_vtkIOlib, ctypes.RTLD_GLOBAL)
import sys
_vtkWrappinglib = 'libvtkWrappingPython' + str(sys.version_info[0]) + str(sys.version_info[1]) + 'Core-' + _vtk_major_version + '.'  + _vtk_minor_version + '.so.1'
ctypes.CDLL(_vtkWrappinglib, ctypes.RTLD_GLOBAL)
%}

#ifdef SWIGTCL
%{
#include "vtkTclUtil.h"
%}

%typemap(in) vtkImageExport* {
  int cerror=0;
  $1 = NULL;
  $1 = (vtkImageExport*) vtkTclGetPointerFromObject(Tcl_GetString($input),
                                                    "vtkImageExport", interp,
                                                    cerror );
  if ( cerror ) { SWIG_fail; }
}

%typemap(in) vtkImageImport* {
  int cerror=0;
  $1 = NULL;
  $1 = (vtkImageImport*) vtkTclGetPointerFromObject(Tcl_GetString($input),
                                                    "vtkImageImport", interp,
                                                    cerror );
  if ( cerror ) { SWIG_fail; }
}
#endif

#ifdef SWIGPYTHON
%module VtkGluePython

%typemap(out) vtkImageExport* {
  PyImport_ImportModule("vtk");
  $result = vtkPythonUtil::GetObjectFromPointer ( (vtkImageExport*)$1 );
}

%typemap(out) vtkImageImport* {
  PyImport_ImportModule("vtk");
  $result = vtkPythonUtil::GetObjectFromPointer ( (vtkImageImport*)$1 );
}

%typemap(out) vtkImageData* {
  PyImport_ImportModule("vtk");
  $result = vtkPythonUtil::GetObjectFromPointer ( (vtkImageData*)$1 );
}

%typemap(in) vtkImageData* {
  $1 = NULL;
  $1 = (vtkImageData*) vtkPythonUtil::GetPointerFromObject ( $input, "vtkImageData" );
  if ( $1 == NULL ) { SWIG_fail; }
}

%typemap(out) vtkPolyData* {
  PyImport_ImportModule("vtk");
  $result = vtkPythonUtil::GetObjectFromPointer ( (vtkPolyData*)$1 );
}

%typemap(in) vtkPolyData* {
  $1 = NULL;
  $1 = (vtkPolyData*) vtkPythonUtil::GetPointerFromObject ( $input, "vtkPolyData" );
  if ( $1 == NULL ) { SWIG_fail; }
}
#endif

#ifdef SWIGJAVA
%{
#include "vtkJavaUtil.h"
%}

/*// TODO: The java typemaps seem to only work in java.i, they are harmless in that file but misplaces.*/
#endif
