project(
  'hexview',
  'cpp',
  version : '0.1',
  default_options : [
  'warning_level=3',
  'cpp_std=c++17',
  'werror=true',
  'default_library=static',
  ]
)

subdir('src')

hexview_lib = library('hexview', hexview_sources)

executable('hexview', hexview_main, link_with: hexview_lib)
