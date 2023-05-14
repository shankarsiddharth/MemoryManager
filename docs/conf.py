# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#

from textwrap import dedent
from typing import Dict, Any

# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'MemoryManager'
project_copyright = '2021-2023'
author = 'shankarsiddharth'
author_link = 'https://github.com/shankarsiddharth'

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'breathe',  # Breathe extension
    'exhale',  # Exhale extension
    'sphinxcontrib.youtube',
]

# Setup the breathe extension
breathe_projects = {
    "MemoryManager": "./doxygen/xml"
}
breathe_default_project = "MemoryManager"

# Setup the exhale extension
exhale_args = {
    ############################################################################
    # These arguments are required.                                            #
    ############################################################################
    "containmentFolder": "./api",
    "rootFileName": "library_root.rst",
    "doxygenStripFromPath": "../src",
    ############################################################################
    # Optional arguments.                                                      #
    ############################################################################
    "rootFileTitle": "C++ API Reference",
    "createTreeView": True,
    # If using the sphinx-bootstrap-theme, treeViewIsBootstrap is required
    # "treeViewIsBootstrap": True,
    ############################################################################
    # HTML Theme specific configurations.                                      #
    ############################################################################
    # Fix broken Sphinx RTD Theme 'Edit on GitHub' links
    "pageLevelConfigMeta": ":github_url: https://github.com/shankarsiddharth/MemoryManager",
    ############################################################################
    # Main library page layout example configuration.                          #
    ############################################################################
    # Skip the Namespace section from the sidebar
    "unabridgedOrphanKinds": {"namespace"},
    "afterTitleDescription": dedent(u'''
        The following sections present the C++ API Reference.       
    '''),
    "fullApiSubSectionTitle": "C++ API (Complete)",
    ############################################################################
    # Individual page layout example configuration.                            #
    ############################################################################
    # Fix furo theme content definition warning in the docs
    "contentsDirectives": False,
    ############################################################################
    # Doxygen arguments.                                                       #
    ############################################################################
    "exhaleExecutesDoxygen": True,
    # "exhaleUseDoxyfile": True,
    "exhaleDoxygenStdin": dedent('''
PROJECT_NAME = "MemoryManager"
EXTRACT_ALL = YES
WARN_IF_UNDOCUMENTED = NO
INPUT = ../src
RECURSIVE = YES
EXCLUDE = \
../.git/ \
../.idea/ \
../.vs/ \
../docs/ \
../.gitignore \
../venv/ \
../README.txt \
../README.md \
../src/.idea/ \
../src/.vs/ \
../src/.gitignore \
../src/x64/ \
../src/Debug/ \
../src/Release/ \
../src/BitArray/ \
../src/FixedSizeAllocator/
# EXCLUDE_SYMBOLS = 
GENERATE_HTML = YES
GENERATE_LATEX = NO
GENERATE_XML = YES
ENABLE_PREPROCESSING = YES
# PREDEFINED += DOXYGEN
# WARN_AS_ERROR = YES
''')

}

# Tell sphinx what the primary language being documented is.
primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'cpp'

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#

html_title = "Memory Manager"

# html_theme = 'sphinx_rtd_theme'
html_theme = 'furo'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']
html_css_files = ["custom.css"]

html_logo = "_static/logo.svg"
# html_show_sphinx = False

html_theme_options: Dict[str, Any] = {
    "footer_icons": [
        {
            "name": "GitHub",
            "url": "https://github.com/shankarsiddharth/MemoryManager",
            "html": """
                <svg stroke="currentColor" fill="currentColor" stroke-width="0" viewBox="0 0 16 16">
                    <path fill-rule="evenodd" d="M8 0C3.58 0 0 3.58 0 8c0 3.54 2.29 6.53 5.47 7.59.4.07.55-.17.55-.38 0-.19-.01-.82-.01-1.49-2.01.37-2.53-.49-2.69-.94-.09-.23-.48-.94-.82-1.13-.28-.15-.68-.52-.01-.53.63-.01 1.08.58 1.23.82.72 1.21 1.87.87 2.33.66.07-.52.28-.87.51-1.07-1.78-.2-3.64-.89-3.64-3.95 0-.87.31-1.59.82-2.15-.08-.2-.36-1.02.08-2.12 0 0 .67-.21 2.2.82.64-.18 1.32-.27 2-.27.68 0 1.36.09 2 .27 1.53-1.04 2.2-.82 2.2-.82.44 1.1.16 1.92.08 2.12.51.56.82 1.27.82 2.15 0 3.07-1.87 3.75-3.65 3.95.29.25.54.73.54 1.48 0 1.07-.01 1.93-.01 2.2 0 .21.15.46.55.38A8.013 8.013 0 0 0 16 8c0-4.42-3.58-8-8-8z"></path>
                </svg>
            """,
            "class": "",
        },
    ],
    "source_repository": "https://github.com/shankarsiddharth/MemoryManager/",
    "source_branch": "main",
    "source_directory": "docs/",
}

# Variables used in the page.html template
html_context = {
    "author_link": author_link,
    "author": author
}
