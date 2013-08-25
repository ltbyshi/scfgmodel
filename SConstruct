env = Environment()
env.Append(CCFLAGS = '-g3 -Wall')
test_cmgraph = env.Program(target = 'bin/test_cmgraph',
	source = ['tests/test_cmgraph.cpp',
		'tests/arraytree.cpp',
		'tests/utils.cpp'])
test_arraytree = env.Program(target = 'bin/test_arraytree',
	source = ['tests/arraytree.cpp',
		'tests/test_arraytree.cpp',
		'tests/utils.cpp'])
Default(test_cmgraph)
		
