"""A setuptools based setup module.

See:
https://packaging.python.org/en/latest/distributing.html
https://github.com/pypa/sampleproject
"""


# Always prefer setuptools over distutils
import glob
import sys
from setuptools import setup, find_packages
# To use a consistent encoding

setup(
    name='PyOpenVrInputEmulator',


    # Choose your license
    license='MIT',

    packages=find_packages(include=['pyopenvrinputemu'], exclude=['pyopenvrinputemu/OpenVR-InputEmulator']),

    #install_requires=['openvr==1.0.301', 'numpy'],
    install_requires=[ ],


    # If there are data files included in your packages that need to be
    # installed, specify them here.  If using Python 2.6 or less, then these
    # have to be included in MANIFEST.in as well.
    include_package_data=True,
    zip_safe=False,

    # To provide executable scripts, use entry points in preference to the
    # "scripts" keyword. Entry points provide cross-platform support and allow
    # pip to create the appropriate form of executable for the target platform.

)
