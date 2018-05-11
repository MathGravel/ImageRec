import pydot

class policy:
	def __init__(self, file):
		self.dot=  pydot.graph_from_dot_file(file)
		self.currentNode = self.dot[0].get_node('root')[0]
		self.currentPlan = 'UNKNOWN'
		self.currentPlanProb = 1.0

	def update(self, observation = 'none'):
		action = self.extractAction(self.currentNode)
		edgeChildren = self.getEdgeChildren(self.currentNode)
		oldNode = self.currentNode

		if 'cg_' in action:
			self.currentPlan = action.split('cg_')[1]

		for ec in edgeChildren:
			if self.extractObservation(ec) == observation:
				self.currentNode = self.dot[0].get_node(ec.get_destination())[0]
				self.currentPlanProb = self.extractPlanProb(self.currentNode)
				break

		action = self.extractAction(self.currentNode)		
		if 'cg_' in action:
			self.currentPlan = action.split('cg_')[1]
		if self.currentNode == oldNode:
			print('ERROR')
		if self.currentNode == 'finish' or self.currentNode == 'Finish':
			print('End')

	def nextPossibleAction(self):
		nPA = ""
		edgeChildren = self.getEdgeChildren(self.currentNode)
		for ec in edgeChildren:
			nPA = nPA + str(self.extractObservation(ec)) + str(self.extractProbabilite(ec))
		return nPA

	def getEdgeChildren(self, node):
		listEdge = self.dot[0].get_edge_list()
		listChildren = []
		for i in listEdge:
			if i.get_source() == node.get_name():
				listChildren.append(i)
		return listChildren	

	def extractAction(self, node):
		action = ''
		label = node.get_label()
		action = label.split('A (')[1]	
		action = action.split(')')[0]
		return action

	def extractObservation(self, edge):
		observation = ''
		label = edge.get_label()
		observation = label.split('o (')[1]
		observation = observation.split(')')[0]
		return observation

	def extractProbabilite(self, edge):
		prob = ''
		label = edge.get_label()
		prob = label.split('o (')[1]
		prob = prob.split(') ')[1]
		prob = prob.split('\\')[0]
		return float(prob)

	def extractPlanProb(self, node):
		prob = ''
		label = node.get_label()
		prob = label.split('Y ')[1]
		prob = prob.split('A ')[0]
		prob = prob.split(') ')[1]
		prob = prob.split('\\')[0]
		return float(prob)
