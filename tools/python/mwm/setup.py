from distutils.core import setup

setup(
    name='mwm',
    version='1.0.0',
    author='Ilya Zverev',
    author_email='ilya@zverev.info',
    packages=['mwm'],
    url='http://pypi.python.org/pypi/mwm/',
    license='LICENSE.txt',
    description='Library to read binary MAPS.ME map files.',
    long_description=open('README.txt').read(),
)
