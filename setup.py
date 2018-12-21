# -*- coding: utf-8 -*-
from __future__ import print_function
from os import sys

try:
    from skbuild import setup
except ImportError:
    print('scikit-build is required to build from source.', file=sys.stderr)
    print('Please run:', file=sys.stderr)
    print('', file=sys.stderr)
    print('  python -m pip install scikit-build')
    sys.exit(1)

setup(
    name='lesionsizingtoolkit',
    version='0.0.1',
    author='Xiao Xiao Liu',
    author_email='itk+community@discourse.itk.org',
    packages=['itk'],
    package_dir={'itk': 'itk'},
    download_url=r'https://github.com/InsightSoftwareConsortium/LesionSizingToolkit',
    description=r'A generic and extensible ITK module for lesion segmentation',
    long_description='lesionsizingtoolkit provides a generic, modular, and '
                     'extensible architecture for lesion sizing algorithms '
                     'in medical images as well as a reference algorithm for '
                     'lung solid lesion segmentation in CT images.\n'
                     'Please refer to:\n'
                     'Liu X., Helba B., Krishnan K., Reynolds P., McCormick M., Turner W., Ibáñez L., Yankelevitz D., Avila R., '
                     '"Fostering Open Science in Lung Cancer Lesion Sizing with ITK module LSTK", '
                     'Insight Journal, January-December 2012, http://hdl.handle.net/10380/3369.',
    classifiers=[
        "License :: OSI Approved :: BSD 2-Clause \"Simplified\" License",
        "Programming Language :: Python",
        "Programming Language :: C++",
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Intended Audience :: Education",
        "Intended Audience :: Healthcare Industry",
        "Intended Audience :: Science/Research",
        "Topic :: Scientific/Engineering",
        "Topic :: Scientific/Engineering :: Medical Science Apps.",
        "Topic :: Scientific/Engineering :: Information Analysis",
        "Topic :: Software Development :: Libraries",
        "Operating System :: Android",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: POSIX",
        "Operating System :: Unix",
        "Operating System :: MacOS"
        ],
    license='BSD',
    keywords='ITK InsightToolkit',
    url=r'https://github.com/InsightSoftwareConsortium/LesionSizingToolkit',
    install_requires=[
        r'itk'
    ]
    )
