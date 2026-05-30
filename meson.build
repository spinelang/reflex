project(
    'reader',
    'cpp',
    default_options: [
        'cpp_std=c++23',
        'warning_level=3',
    ],
)

gtest_dep = dependency('gtest_main')

interpreter_src = [
  'src/lexer/token.cpp',
  'src/lexer/lexer.cpp',
  'src/parser/ast.cpp',
  'src/parser/list.cpp',
  'src/parser/parser.cpp',
  'src/eval/eval.cpp',
  'src/eval/env.cpp'
]

driver_src = [
    'src/main.cpp'
]

test_src = ['src/test/eval_test.cpp']

interp_lib = static_library('interp lib', interpreter_src)
test('tests', executable('testss', test_src, dependencies: gtest_dep, link_with: interp_lib))

executable('reader', interpreter_src + driver_src)
