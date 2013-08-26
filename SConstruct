env = Environment()
env.Append(CCFLAGS = '-g3 -Wall')
test_cmgraph = env.Program(target = 'bin/test_cmgraph',
	source = ['src/test_cmgraph.cpp',
		'src/parsetree.cpp',
		'src/utils.cpp',
		'src/cmgraph.cpp'])
test_parsetree = env.Program(target = 'bin/test_parsetree',
	source = ['src/parsetree.cpp',
		'src/test_parsetree.cpp',
		'src/utils.cpp'])
Default(test_cmgraph)