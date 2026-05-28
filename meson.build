project(
    'reader',
    'cpp',
    default_options: [
        'cpp_std=c++23',
        'warning_level=3',
    ],
)

gtest_dep = dependency('gtest_main')
bench_dep = dependency('benchmark_main')

interpreter_src = [
  'src/main.cpp',
  'src/lexer/token.cpp',
  'src/lexer/lexer.cpp',
  'src/parser/ast.cpp',
  'src/parser/list.cpp',
  'src/parser/parser.cpp',
  'src/eval/eval.cpp'
]

executable('reader', interpreter_src)
