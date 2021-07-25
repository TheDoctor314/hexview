test_sources = files([
  'main.cpp',
  'ascii.cpp',
  'args.cpp',
])

unit_test_exe = executable('unit_test', test_sources, link_with: hexview_lib, include_directories : hexview_inc)

test('Unit tests', unit_test_exe)
