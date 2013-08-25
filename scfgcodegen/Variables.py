#variables used in the templates
SCFGStates = ['SS', 'SL', 'SR', 'P', 'L', 'R', 'B', 'E']
CMStates = ['S', 'ML', 'MR', 'MP', 'IL', 'IR', 'D', 'E']

class CMNode:
	def __init__(self, MDState, InsState):
		self.MD = MDState
		self.Ins = InsState

#dictionary format:
#states in parse tree: [MD states, insertion states]
CMGraphVars = {
	'S': CMNode(['S'], ['IL', 'IR']),
	'SL': CMNode(['S'], []),
	'SR': CMNode(['S'], ['IL']),
	'P': CMNode(['ML', 'D', 'MP', 'MR'], ['IL', 'IR']),
	'IL': CMNode(['ML', 'D'], ['IL']),
	'R': CMNode(['MR', 'D'], ['IR']),
	'E': CMNode(['E'], []),
	'B': CMNode(['B'], [])
}
