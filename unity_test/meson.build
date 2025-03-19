project('ctest','c')
executable('bin','src/main.c')
unity_subproject = subproject('unity')
#官方提供的依赖项
unity_dependency = unity_subproject.get_variable('unity_dep')
unity_gen_runner = unity_subproject.get_variable('gen_test_runner')
#本地必须有subprojects文件夹才会生效
inc = include_directories('src')
src1 = files([
    'src/netinfo.c',
    'test/testcode1.c'
])
test1=executable('test1',
    sources: [
        src1,
        unity_gen_runner.process('test/testcode1.c')
    ],
    include_directories: [inc],
    dependencies: [unity_dependency]
)
test('test1',test1)