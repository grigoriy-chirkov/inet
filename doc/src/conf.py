# -*- coding: utf-8 -*-
#
# Configuration file for the Sphinx documentation builder.
#
# This file does only contain a selection of the most common options. For a
# full list see the documentation:
# http://www.sphinx-doc.org/en/master/config

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
sys.path.insert(0, os.path.abspath('.'))
sys.path.insert(0, os.path.abspath('_themes'))


# -- Project information -----------------------------------------------------

project = 'INET'
copyright = 'INET community'
author = 'INET community'

# The short X.Y version
version = '4.2'
# The full version, including alpha/beta/rc tags
release = '4.2'

# -- General configuration ---------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#
needs_sphinx = '2.2'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.mathjax',
    'sphinx.ext.extlinks',
    'sphinx.ext.ifconfig',
    'sphinx.ext.todo',
    'sphinx.ext.githubpages',
    'sphinx.ext.graphviz',
#    'sphinxcontrib.images',  ## needed, but not yet compatible with sphinx 2.x
    'tools.doxylink',
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
#
source_suffix = [ '.rst',
# '.md',
]

# Source parsers
source_parsers = {
#   '.md': 'recommonmark.parser.CommonMarkParser',
}


# The master toctree document.
master_doc = 'index'

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = None

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path .
exclude_patterns = ['_build', '_deploy', 'Thumbs.db', '.DS_Store', '**/_docs', 'global.rst',
#  'users-guide/**',
#  'developers-guide/**',
#  'showcases/**',
#  'tutorials/**',
]

# graphviz options
graphviz_output_format = 'svg'

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
# https://github.com/omnetpp/sphinx_opp_theme
html_theme = 'opptheme'
html_theme_path = ['_themes']

# material theme config
html_theme_options = {
    # Specify a list of menu in Header.
    # Tuples forms:
    #  ('Name', 'external url or path of pages in the document', boolean, 'icon name')
    #
    # Third argument:
    # True indicates an external link.
    # False indicates path of pages in the document.
    #
    # Fourth argument:
    # Specify the icon name.
    # For details see link.
    # https://material.io/icons/
    'header_links' : [
#        ("INET Website", "https://inet.omnetpp.org", True, 'language'),
#        ("Users Guide", "users-guide", False, ''),
#        ("Developers Guide", "developers-guide", False, ''),
    ],

    # Customize css colors.
    # For details see link.
    # https://getmdl.io/customize/index.html
    #
    # Values: amber, blue, brown, cyan deep_orange, deep_purple, green, grey, indigo, light_blue,
    #         light_green, lime, orange, pink, purple, red, teal, yellow(Default: indigo)
    'primary_color': 'blue',
    # Values: Same as primary_color. (Default: pink)
    'accent_color': 'light_blue',

    # Customize layout.
    # For details see link.
    # https://getmdl.io/components/index.html#layout-section
    'fixed_drawer': True,
    'fixed_header': True,
    'header_waterfall': False,
    'header_scroll': False,

    # Render title in header.
    # Values: True, False (Default: False)
    'show_header_title': False,
    # Render title in drawer.
    # Values: True, False (Default: True)
    'show_drawer_title': False,
    # Render footer.
    # Values: True, False (Default: True)
    'show_footer': False
}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

#html_extra_path = ['.']

# Custom sidebar templates, must be a dictionary that maps document names
# to template names.
#
# The default sidebars (for documents that don't match any pattern) are
# defined by theme itself.  Builtin themes are using these templates by
# default: ``['localtoc.html', 'relations.html', 'sourcelink.html',
# 'searchbox.html']``.
#
# html_sidebars = {}


# -- Options for HTMLHelp output ---------------------------------------------

# Output file base name for HTML help builder.
htmlhelp_basename = 'INETFrameworkdoc'


# -- Options for LaTeX output ------------------------------------------------

latex_elements = {
    # The paper size ('letterpaper' or 'a4paper').
    #
    'papersize': 'a4paper',

    # The font size ('10pt', '11pt' or '12pt').
    #
    'pointsize': '10pt',

    # Additional stuff for the LaTeX preamble.
    #
    # 'preamble': '',

    # Latex figure (float) alignment
    #
    # 'figure_align': 'htbp',
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    ('users-guide/index', 'users-guide.tex', "INET Framework User's Guide", '', 'manual', False),
    ('developers-guide/index', 'developers-guide.tex', "INET Framework Developer's Guide", '', 'manual', False),
]


# -- Options for manual page output ------------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    (master_doc, 'inetframework', 'INET Framework Documentation',
     [author], 1)
]


# -- Options for Texinfo output ----------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
    (master_doc, 'INETFramework', 'INET Framework Documentation',
     author, 'INETFramework', 'One line description of project.',
     'Miscellaneous'),
]

# external link configuration
extlinks = {
  'wiki': ('https://en.wikipedia.org/wiki/%s', '')
}

# image extension config
images_config = {
    'override_image_directive': False,
#    'backend': 'LightBox2',
#    'default_image_width': '100%',
#    'default_image_height': 'auto',
#    'default_group': 'None',
#    'download': True,
    'default_show_title': False
}

# -- Doxylink config ---------------------------------------------------------

doxylink = {
#        'cpp' : ('doxytags.xml', 'https://omnetpp.org/doc/inet/api-current/doxy/'),
        'ned' : ('nedtags.xml', '/home/user/Integration/inet/doc/neddoc/'),
        'msg' : ('msgtags.xml', 'https://omnetpp.org/doc/inet/api-current/neddoc/'),
}

# -- Extension configuration -------------------------------------------------
rst_prolog = open('global.rst', 'r').read()

# whether to show TODO items
todo_include_todos = False
todo_emit_warnings = False

def opp_preprocess(app, docname, source):
    """
    Render our pages as a jinja template for fancy templating goodness.
    """
    # Make sure we're outputting HTML
    if app.builder.format != 'html':
        return
    src = source[0]

    # implicitly import global macros in into the opp namespace at the beginning of each source file
    src = "{% import 'global-macros.inc' as opp %}\n" + src

    # run the templating engine on the source file
    rendered = app.builder.templates.render_string(
        src, app.config.html_context
    )
    source[0] = rendered

#################################x
# inline highlight extension

# Use defaults provided by highlight directive for code role.
# inline_highlight_respect_highlight = False

# Highlight also normal literals like :code:`literal`
# inline_highlight_literals = False

# ###########################################################################
# The name of the Pygments (syntax highlighting) style to use.
pygments_style = "default"

from pygments.lexers.c_cpp import CLexer, CppLexer
from pygments.lexer import RegexLexer, include, bygroups, using, this, inherit, default, words
from pygments.token import Name, Keyword, Comment, Text, Operator, String, Number, Punctuation, Error
from sphinx.highlighting import lexers

#####
class NedLexer(RegexLexer):
    name = 'ned'
    filenames = ['*.ned']

    #: optional Comment or Whitespace
    _ws = r'(?:\s|//.*?\n|/[*].*?[*]/)+'

    # The trailing ?, rather than *, avoids a geometric performance drop here.
    #: only one /* */ style comment
    _ws1 = r'\s*(?:/[*].*?[*]/\s*)?'

    tokens = {
        'whitespace': [
            (r'\n', Text),
            (r'\s+', Text),
            (r'\\\n', Text),  # line continuation
            (r'//(\n|[\w\W]*?[^\\]\n)', Comment.Single),
            (r'/(\\\n)?[*][\w\W]*?[*](\\\n)?/', Comment.Multiline),
            # Open until EOF, so no ending delimeter
            (r'/(\\\n)?[*][\w\W]*', Comment.Multiline),
        ],
        'statements': [
            (r'(L?)(")', bygroups(String.Affix, String), 'string'),
            (r"(L?)(')(\\.|\\[0-7]{1,3}|\\x[a-fA-F0-9]{1,2}|[^\\\'\n])(')",
             bygroups(String.Affix, String.Char, String.Char, String.Char)),
            (r'(true|false)\b', Name.Builtin),
            (r'(<-->|-->|<--|\.\.)', Keyword),
            (r'(bool|double|int|xml)\b', Keyword.Type),
            (r'(inout|input|output)\b', Keyword.Type),
            (r'(\d+\.\d*|\.\d+|\d+)[eE][+-]?\d+[LlUu]*', Number.Float),
            (r'(\d+\.\d*|\.\d+|\d+[fF])[fF]?', Number.Float),
            (r'0x[0-9a-fA-F]+[LlUu]*', Number.Hex),
            (r'#[0-9a-fA-F]+[LlUu]*', Number.Hex),
            (r'0[0-7]+[LlUu]*', Number.Oct),
            (r'\d+[LlUu]*', Number.Integer),
            (r'\*/', Error),
            (r'[~!%^&*+=|?:<>/-]', Operator),
            (r'[()\[\],.]', Punctuation),
            (words(("channel", "channelinterface", "simple", "module", "network", "moduleinterface"), suffix=r'\b'), Keyword),
            (words(("parameters", "gates", "types", "submodules", "connections"), suffix=r'\b'), Keyword),
            (words(("volatile", "allowunconnected", "extends", "for", "if", "import", "like", "package", "property"), suffix=r'\b'), Keyword),
            (words(("sizeof", "const", "default", "ask", "this", "index", "typename", "xmldoc"), suffix=r'\b'), Keyword),
            (words(("acos", "asin", "atan", "atan2", "bernoulli","beta", "binomial", "cauchy", "ceil", "chi_square", "cos", "erlang_k", "exp","exponential", "fabs", "floor", "fmod", "gamma_d", "genk_exponential","genk_intuniform", "genk_normal", "genk_truncnormal", "genk_uniform", "geometric","hypergeometric", "hypot", "intuniform", "log", "log10", "lognormal", "max", "min","negbinomial", "normal", "pareto_shifted", "poisson", "pow", "simTime", "sin", "sqrt","student_t", "tan", "triang", "truncnormal", "uniform", "weibull", "xml", "xmldoc"), suffix=r'\b'), Name.Builtin),
            ('@[a-zA-Z_]\w*', Name.Builtin),
            ('[a-zA-Z_]\w*', Name),
        ],
        'root': [
            include('whitespace'),
            # functions
            (r'((?:[\w*\s])+?(?:\s|[*]))'  # return arguments
             r'([a-zA-Z_]\w*)'             # method name
             r'(\s*\([^;]*?\))'            # signature
             r'([^;{]*)(\{)',
             bygroups(using(this), Name.Function, using(this), using(this),
                      Punctuation),
             'function'),
            # function declarations
            (r'((?:[\w*\s])+?(?:\s|[*]))'  # return arguments
             r'([a-zA-Z_]\w*)'             # method name
             r'(\s*\([^;]*?\))'            # signature
             r'([^;]*)(;)',
             bygroups(using(this), Name.Function, using(this), using(this),
                      Punctuation)),
            default('statement'),
        ],
        'statement': [
            include('whitespace'),
            include('statements'),
            ('[{}]', Punctuation),
            (';', Punctuation, '#pop'),
        ],
        'function': [
            include('whitespace'),
            include('statements'),
            (';', Punctuation),
            (r'\{', Punctuation, '#push'),
            (r'\}', Punctuation, '#pop'),
        ],
        'string': [
            (r'"', String, '#pop'),
            (r'\\([\\abfnrtv"\']|x[a-fA-F0-9]{2,4}|'
             r'u[a-fA-F0-9]{4}|U[a-fA-F0-9]{8}|[0-7]{1,3})', String.Escape),
            (r'[^\\"\n]+', String),  # all other characters
            (r'\\\n', String),  # line continuation
            (r'\\', String),  # stray backslash
        ]
    }

lexers['ned'] = NedLexer(startinline=True)

#####
class MsgLexer(CppLexer):
    name = 'msg'
    filenames = ['*.msg']
    mimetypes = ['text/x-msg']

    tokens = {
        'statements': [
            (words(("import","cplusplus", "namespace", "struct", "message",
                "packet", "class", "noncobject", "enum", "extends"), suffix=r'\b'), Keyword),
            (words(("properties", "fields"), suffix=r'\b'), Keyword),
            (r'(abstract|readonly|bool|char|short|int|long|double|unsigned|string)\b', Keyword.Type),
            (r'[~!%^&*+=|?:<>/@-]', Operator),
            inherit,
        ],
    }

lexers['msg'] = MsgLexer(startinline=True)

#####
class IniLexer(RegexLexer):
    name = 'ini'
    filenames = ['*.ini']
    mimetypes = ['text/x-ini']

    tokens = {
        'root': [
            (r'[;#].*$', Comment.Single),
            (r'\s+', Text),
            (r'\[.*?\]', Keyword),
            (r'(.*?)([ \t]*)(=)([ \t]*)([^#\n]*(?:\n[ \t].+)*)',
             bygroups(Name.Attribute, Text, Operator, Text, String)),
        ],
    }

    def analyse_text(text):
        npos = text.find('\n')
        if npos < 3:
            return False
        return text[0] == '[' and text[npos-1] == ']'

lexers['ini'] = IniLexer(startinline=True)

#######################################################################
# -- setup the customizations
import tools.video
import tools.audio
import opptheme

def setup(app):
    app.connect("source-read", opp_preprocess)
    app.add_directive('youtube', tools.video.Youtube)
    app.add_directive('vimeo', tools.video.Vimeo)
    app.add_directive('video', tools.video.Video)
    app.add_directive('audio', tools.audio.Audio)
    app.add_directive('card', opptheme.CardDirective)
