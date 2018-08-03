import numpy
import copy

class Goal:
	def __init__(self, name, listAction = [] , matrixAdj = numpy.zeros((0, 0)), initialState = []):
		self.name = name
		self.listAction = listAction
		self.matrixAdj = matrixAdj

	def addPlan(self,listAction):
		# copy the initial list and add Start and Finish
		if '' in listAction:
			listAction.remove('')
		templistAction = ["Start"]
		templistAction.extend(listAction)
		templistAction.append("Finish")

		listAllAction = copy.deepcopy(self.listAction)
		listAllAction.extend(templistAction)
		listAllAction = list(set(listAllAction))

		
		size = len(listAllAction)
		entryMatrixAdj = numpy.zeros((size, size))
		newMatrixAdj = numpy.zeros((size, size))

		for laaLine in range(0,len(listAllAction)):
			for laLine in range(0,len(self.listAction)):
				if listAllAction[laaLine] == self.listAction[laLine]:				
					for laaCol in range(0,len(listAllAction)):
						for laCol in range(0,len(self.listAction)):
							if listAllAction[laaCol] == self.listAction[laCol]:
								newMatrixAdj[laaLine,laaCol] = self.matrixAdj[laLine,laCol]
								if  listAllAction[laaLine] == 'Finish' and  listAllAction[laaCol] == 'Finish':
									newMatrixAdj[laaLine,laaCol] = 1
								break
					break


		for la in range(0,len(templistAction)-1):
			for laaLine in range(0,len(listAllAction)):
				if templistAction[la] == listAllAction[laaLine]:
					for laaCol in range(0, len(listAllAction)):
						if templistAction[la + 1] ==  listAllAction[laaCol]:
							newMatrixAdj[laaLine,laaCol] =	newMatrixAdj[laaLine,laaCol] + 1
							break
					break


		self.matrixAdj = newMatrixAdj
		self.listAction = listAllAction
		return 0

	def toString(self):
		out = []
		out = "****" + self.name + "**** \n"
		for action in self.listAction:
			out = out + action + ' '
		out  = out + "\n"
		#out = out + self.matrixAdj
		return out 

	def normalize(self):
		size = len(self.listAction)
		for line in range(0, size):
				self.matrixAdj[line] = self.matrixAdj[line] / numpy.sum(self.matrixAdj[line])


class Observation:
	def __init__(self, listAction, matrixConf):
		self.listAction = listAction
		self.matrixConf = matrixConf

class Domain:
	def __init__(self, listGoal, observation = [], initialState = [], reward = []):
		self.listGoal = listGoal
		for goal in listGoal:
			goal.normalize()
		if not observation:
			listAllAction = []
			for i in listGoal:
				for j in i.listAction:
					listAllAction.append(j)
			listAllAction = list(set(listAllAction))
			self.observation = Observation(listAllAction, numpy.identity(len(listAllAction)))
		else:
			self.observation = observation
		if not initialState:
			self.initialState = "Start"
		else : 
			self.initialState = initialState

	def toPOMDPxString(self):
		out = ""
		out = out + "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?> \n"
		out = out + "<pomdpx version=\"0.1\" id=\"undefined\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"pomdpx.xsd\"> \n"
		out = out + "<Discount> 0.95 </Discount> \n"

		out = out + self.toPOMDPxVariables()
		out = out + self.toPOMDPxInitialState()
		out = out + self.toPOMDPxTransition()
		out = out + self.toPOMDPxObservation()
		out = out + self.toPOMDPxReward()

		out = out + "</pomdpx>"

		return out

	def toPOMDPxVariables(self):
		out = ""
		out = out + "<Variable> \n"
		out = out + self.toPOMDPxVariableState()
		out = out + self.toPOMDPxVariableGoal()
		out = out + self.toPOMDPxVariableEsGoal()
		out = out + self.toPOMDPxVariableAction()
		out = out + self.toPOMDPxVariableObservation()
		out = out + "<RewardVar vname=\"reward\"/> \n"
		out = out + "</Variable> \n"
		return out

	def toPOMDPxVariableState(self):
		out = ""
		# State variables of the observee
		out = out + "<StateVar vnamePrev=\"startAction\" vnameCurr=\"nextAction\" fullyObs=\"false\"> \n"
		out = out + "<ValueEnum> "

		listAllAction = []
		for i in self.listGoal:
			for j in i.listAction:
				listAllAction.append(j)
		listAllAction = list(set(listAllAction))

		for i in listAllAction:
			out = out + i + ' '

		out = out + "</ValueEnum> \n"
		out = out + "</StateVar> \n"
		return out

	def toPOMDPxVariableGoal(self):
		out = ""
		# Goal of the obsevee
		out = out + "<StateVar vnamePrev=\"startGoal\" vnameCurr=\"nextGoal\" fullyObs=\"false\"> \n"
		out = out + "<ValueEnum> "
		for i in self.listGoal:
			out = out + i.name + ' '
		out = out + "</ValueEnum> \n"
		out = out + "</StateVar> \n"
		return out 

	def toPOMDPxVariableEsGoal(self):
		out = ""
		# Estimated goal of the obsevee
		out = out + "<StateVar vnamePrev=\"startEsGoal\" vnameCurr=\"nextEsGoal\" fullyObs=\"true\"> \n"
		out = out + "<ValueEnum> "
		out = out + "Unknown "
		for i in self.listGoal:
			out = out + i.name + ' '
		out = out + "</ValueEnum> \n"
		out = out + "</StateVar> \n"
		return out 

	def toPOMDPxVariableAction(self):
		out = ""
		# list of actions
		out = out + "<ActionVar vname = \"Action\"> \n"
		out = out + "<ValueEnum> "
		out = out + "Obs "
		out = out + "None "
		for i in self.listGoal:
			out = out + "cg_" + i.name + ' '
		out = out + "</ValueEnum> \n"
		out = out + "</ActionVar> \n"
		return out

	def toPOMDPxVariableObservation(self):
		out = ""
		# list of observation
		out = out + "<ObsVar vname=\"ObsState\"> \n"
		out = out + "<ValueEnum> "
		for i in self.observation.listAction:
			out = out + i + ' '
		out = out + "None "
		out = out + "</ValueEnum> \n"
		out = out + "</ObsVar> \n" 
		return out

	def toPOMDPxInitialState(self):
		out = ""
		out = out + "<InitialStateBelief> \n"
		out = out + self.toPOMDPxTBLHead("startAction", ["null"])

		listAllAction = []
		for i in self.listGoal:
			for j in i.listAction:
				if j == '':
					print('ERROR')
				listAllAction.append(j)
		listAllAction = list(set(listAllAction))


		if self.initialState == "Start":
			table = []
			for i in listAllAction:
				if i == "Start":
					table.append("1.0")
				else:
					table.append("0.0")
			out = out + self.toPOMDPxTBLEntry([], table)

			
		elif self.initialState == "Unknown":
			out = out + self.toPOMDPxTBLEntry([], ["uniform"])
		else:
			table = []
			add = False
			for i in listAllAction:
				for j in self.initialState:
					if i == j[0]:
						table.append(j[1])
						add = True
						break
				if not add:
					table.append("0.0")
				add = False
			out = out + self.toPOMDPxTBLEntry([], table)

		out = out + self.toPOMDPxTBLQueu()

		out = out + self.toPOMDPxTBLHead("startGoal", ["null"])
		out = out + self.toPOMDPxTBLEntry([], ["uniform"])
		out = out + self.toPOMDPxTBLQueu()

		out = out + self.toPOMDPxTBLHead("startEsGoal", ["null"])
		table = ["1.0"]
		for i in self.listGoal:
			table.append("0.0")
		out = out + self.toPOMDPxTBLEntry([], table)
		out = out + self.toPOMDPxTBLQueu()

		out = out + "</InitialStateBelief> \n"
		return out

	def toPOMDPxTransition(self):
		out = ""
		out = out + "<StateTransitionFunction> \n"
		out = out + self.toPOMDPxTBLHead("nextAction", ["startAction", "startGoal", "startEsGoal"])

		listAllnextAction = []
		for i in self.listGoal:
			for j in i.listAction:
				listAllnextAction.append(j)
		listAllnextAction = list(set(listAllnextAction))
		size = len(listAllnextAction)

		listAllEsGoal = ["Unknown"]
		listAllGoal = []
		listMatrixAdj = []
		for i in self.listGoal:
			listAllEsGoal.append(i.name)
			listAllGoal.append(i.name)
			listMatrixAdj.append(numpy.zeros((size, size)))
			for laaLine in range(0,len(listAllnextAction)):
				for laLine in range(0,len(i.listAction)):
					if listAllnextAction[laaLine] == i.listAction[laLine]:				
						for laaCol in range(0,len(listAllnextAction)):
							for laCol in range(0,len(i.listAction)):
								if listAllnextAction[laaCol] == i.listAction[laCol]:
									listMatrixAdj[-1][laaLine,laaCol] = i.matrixAdj[laLine,laCol]
									break
						break
				if numpy.sum(listMatrixAdj[-1][laaLine]) == 0:
					for laaCol in range(0,len(listAllnextAction)):
						if listAllnextAction[laaCol] == "Finish":
							listMatrixAdj[-1][laaLine,laaCol] = 1
							break

		listAllAction = ["Obs", "None"]
		for i in self.listGoal:
			listAllAction.append("cg_" + i.name)

		indexGoal = 0
		for lg in listAllGoal:
			for leg in listAllEsGoal:
				table = []
				indexNextAction = 0
				for ln in listAllnextAction:
					table = listMatrixAdj[indexGoal][indexNextAction]
					out = out + self.toPOMDPxTBLEntry([ln, lg, leg], table)
					indexNextAction = indexNextAction +1
			indexGoal = indexGoal +1

		out = out + self.toPOMDPxTBLQueu()

		out = out + self.toPOMDPxTBLHead("nextEsGoal", ["Action", "startEsGoal"])
		for i in listAllAction:
			for j in listAllEsGoal:
				table = []
				for k in listAllEsGoal:
					if i == "Obs":
						if j == k:
							table.append("1.0")
						else:
							table.append("0.0")
					elif i == "None":
						if j == k:
							table.append("1.0")
						else:
							table.append("0.0")
					else:
						if i[3:] == k:
							table.append("1.0")
						else:
							table.append("0.0")
					
				out = out + self.toPOMDPxTBLEntry([i, j], table)

		out = out + self.toPOMDPxTBLQueu()

		out = out + self.toPOMDPxTBLHead("nextGoal", ["startGoal"])
		for i in listAllGoal:
			table = []
			for j in listAllGoal:
				if i == j:
					table.append("1.0")
				else:
					table.append("0.0")

			out = out + self.toPOMDPxTBLEntry([i], table)
		out = out + self.toPOMDPxTBLQueu()

		out = out + "</StateTransitionFunction> \n"
		return out

	def toPOMDPxObservation(self):
		out = ""
		out = out + "<ObsFunction>"
		out = out + self.toPOMDPxTBLHead("ObsState", ["Action", "nextAction"])

		listAllGoal = []
		for i in self.listGoal:
			listAllGoal.append(i.name)
		listAllAction = ["Obs", "None"]
		for i in listAllGoal:
			listAllAction.append("cg_" + i)

		listAllnextAction = []
		for i in self.listGoal:
			for j in i.listAction:
				listAllnextAction.append(j)
		listAllnextAction= list(set(listAllnextAction))

		for la in listAllAction:
			indexNextAction = 0
			for ln in listAllnextAction:
				table = self.observation.matrixConf[indexNextAction]
				out = out + self.toPOMDPxTBLEntry([la,ln],numpy.append(table, "0.0"))
				indexNextAction = indexNextAction + 1
		out = out + self.toPOMDPxTBLQueu()
		out = out + "</ObsFunction> \n"
		return out

	def toPOMDPxReward(self):
		out = ""
		out = out + "<RewardFunction> \n"
		out = out + "<Func> \n"
		out = out + "<Var>reward</Var> \n"
		out = out + "<Parent> Action startGoal startEsGoal</Parent> \n"
		out = out + "<Parameter type=\"TBL\"> \n"

		listAllGoal = []
		listAllEsGoal = ["Unknown"]
		listAllAction = ["Obs", "None"]
		for i in self.listGoal:
			listAllGoal.append(i.name)
			listAllEsGoal.append(i.name)
			listAllAction.append("cg_" + i.name)

		reward = 0
		for aa in listAllAction:
			for ag in listAllGoal:
				for ae in listAllEsGoal:
					if aa == "Obs":
						reward = reward - 1
					if not ag == ae:
						reward = reward - 10 
					if ag == ae:
						reward = reward + 10
					out = out + self.toPOMDPxTBLEntryReward([aa, ag, ae], reward)
					reward = 0


		out = out + "</Parameter> \n"
		out = out + "</Func> \n"
		out = out + "</RewardFunction> \n" 
		return out

	def toPOMDPxTBLHead(self, var, parents):
		out = ""
		out = out + "<CondProb> \n"
		out = out + "<Var>" + var + "</Var> \n"
		out = out + "<Parent> "
		for i in parents:
			out = out + i + ' '
		out = out + "</Parent> \n"
		out = out + "<Parameter type=\"TBL\"> \n"
		return out

	def toPOMDPxTBLQueu(self):
		out = ""
		out = out + "</Parameter> \n"
		out = out + "</CondProb> \n"
		return out

	def toPOMDPxTBLEntry(self, instance, table):
		out = ""
		out = out + "<Entry> \n"
		out = out + "<Instance> "
		for i in instance:
			out = out + i + ' '
		out = out + " - </Instance> \n"
		out = out + "<ProbTable>"
		for i in table:
			out = out + str(i) + ' '
		out = out + "</ProbTable> \n"			
		out = out + "</Entry> \n"
		return out

	def toPOMDPxTBLEntryReward(self, instance, table):
		out = ""
		out = out + "<Entry> \n"
		out = out + "<Instance> "
		for i in instance:
			out = out + i + ' '
		out = out + " </Instance> \n"
		out = out + "<ValueTable>"
		out = out + str(table)
		out = out + "</ValueTable> \n"			
		out = out + "</Entry> \n"
		return out
