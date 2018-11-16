import argparse
from subprocess import check_call
import os
import sys

SCRIPT_DIR = os.path.dirname(__file__)
ROOT_DIR = os.path.abspath(os.getcwd())
IPP_DIR = os.path.join('C:\\', 'P', 'IPP')
VPP_DIR = os.path.join('C:\\', 'P', 'VPP')


print("SCRIPT_DIR: %s" % SCRIPT_DIR)
print("ROOT_DIR: %s" % ROOT_DIR)

sys.path.insert(0, os.path.join(IPP_DIR, 'scripts', 'internal'))

from wheel_builder_utils import push_dir, push_env
from windows_build_common import DEFAULT_PY_ENVS, venv_paths

def build_wheels(py_envs=DEFAULT_PY_ENVS):
    for py_env in py_envs:
        (
            python_executable,
            python_include_dir,
            python_library,
            pip,
            ninja_executable,
            path
        ) = venv_paths(py_env)

        with push_env(PATH="%s%s%s" % (path, os.pathsep, os.environ["PATH"])):

            # Install dependencies
            requirements_file = os.path.join(ROOT_DIR, "requirements-dev.txt")
            if os.path.exists(requirements_file):
                check_call([pip, "install", "--upgrade", "-r", requirements_file])
            check_call([pip, "install", "cmake"])
            check_call([pip, "install", "scikit_build"])
            check_call([pip, "install", "ninja"])

            build_type = "Release"
            source_path = ROOT_DIR
            itk_build_path = os.path.abspath("%s/ITK-win_%s" % (IPP_DIR, py_env))
            print('ITKDIR: %s' % itk_build_path)
            vtk_build_path = os.path.abspath("%s/VTK-win_%s" % (VPP_DIR, py_env))
            print('VTKDIR: %s' % vtk_build_path)

            # Generate wheel
            check_call([
                python_executable,
                "setup.py", "bdist_wheel",
                "--build-type", build_type, "-G", "Ninja",
                "--",
                "-DCMAKE_MAKE_PROGRAM:FILEPATH=%s" % ninja_executable,
                "-DITK_DIR:PATH=%s" % itk_build_path,
                "-DVTK_DIR:PATH=%s" % vtk_build_path,
                "-DWRAP_ITK_INSTALL_COMPONENT_IDENTIFIER:STRING=PythonWheel",
                "-DSWIG_EXECUTABLE:FILEPATH=%s/Wrapping/Generators/SwigInterface/swig/bin/swig.exe" % itk_build_path,
                "-DBUILD_TESTING:BOOL=OFF",
                "-DPYTHON_EXECUTABLE:FILEPATH=%s" % python_executable,
                "-DPYTHON_INCLUDE_DIR:PATH=%s" % python_include_dir,
                "-DPYTHON_LIBRARY:FILEPATH=%s" % python_library
            ])
            # Cleanup
            # check_call([python_executable, "setup.py", "clean"])

def main():
    parser = argparse.ArgumentParser(description='Driver script to build ITKVtkGlue Python wheels.')
    parser.add_argument('--py-envs', nargs='+', default=DEFAULT_PY_ENVS,
            help='Target Python environment versions, e.g. "37-x64".')
    args = parser.parse_args()

    build_wheels(py_envs=args.py_envs)

if __name__ == '__main__':
    main()
