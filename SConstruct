env = Environment()
env.Append(CCFLAGS = '-O3 -Wall')
#env.Append(CCFLAGS = '-g3 -O0 -Wall')

test_cmgraph = env.Program(target = 'bin/test_cmgraph',
	source = ['src/test_cmgraph.cpp',
		'src/parsetree.cpp',
		'src/utils.cpp',
		'src/cmgraph.cpp',
		'src/grammar.cpp'])
test_parsetree = env.Program(target = 'bin/test_parsetree',
	source = ['src/parsetree.cpp',
		'src/test_parsetree.cpp',
		'src/utils.cpp'])
test_grammar = env.Program(target = 'bin/test_grammar',
	source = ['src/parsetree.cpp',
		'src/utils.cpp',
		'src/grammar.cpp',
		'src/cmgraph.cpp',
		'src/test_grammar.cpp'])
		
Default(test_grammar)