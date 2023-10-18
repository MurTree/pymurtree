# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import os
import sys
import shutil

sys.path.insert(0, os.path.abspath('../src/pymurtree'))

shutil.copyfile('../README.md', './readme.md')

project = 'PyMurTree'
copyright = '2023, Yasel Quintero, Jose Urra, Emir Demirovic, Koos van der Linden'
author = 'Yasel Quintero, Jose Urra, Emir Demirovic, Koos van der Linden'
release = '0.0.1'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.napoleon',
    'sphinx.ext.coverage',
    'sphinx.ext.githubpages',
    'myst_parser',
    'sphinx_rtd_theme'
]

source_suffix = ['.rst', '.md']

# Specify autodoc settings
napoleon_numpy_docstring = True
napoleon_use_param = False

autosummary_generate = True  # Turn on sphinx.ext.autosummary

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
