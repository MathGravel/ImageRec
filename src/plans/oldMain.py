#!/usr/bin/env
import numpy
import codecs
import pydot
import socket
import os
from GraphNavigator import Policy
from PolicyGenerator import Domain
from subprocess import call

def simpleParser(file):
	entry = codecs.open("Domain/" + file, 'r', 'utf-8')
	listGoal = []

	for line in entry:
		if len(line) > 2:
			line = line.split('\n')[0]
			goalName = line.split(' : ')[0]
			goalPlan = line.split(' : ')[1]

			check = False
			for goal in listGoal:
				if goal.name == goalName:
					goal.addPlan(goalPlan.split(' '))
					check = True
					break

			if not check:
				currentGoal= Domain.Goal(goalName)
				listGoal.append(currentGoal)
				listGoal[-1].addPlan(goalPlan.split(' '))
	return listGoal

def generateDomain(domain, outputFileName):
	out = codecs.open("Domain/" + outputFileName + ".pomdpx", 'w', 'utf-8')
	out.write(domain.toPOMDPxString())

class solver:
	def __init__(self):
		self.policy = []

	def loadPolicy(self,file):
		scriptDir = os.path.dirname(__file__) #<-- absolute dir the script is in
		relPath = "Policy/" + file
		absFilePath = os.path.join(scriptDir, relPath)
		self.policy = Policy.policy(absFilePath)

	def Start(self, port = 15555):
		return 0

	def end(self):
		return 0

	def generatePolicy(self,domain):

		scriptDir = os.path.dirname(__file__) #<-- absolute dir the script is in
		relPath = "PolicyGenerator/PolicyGenerator.sh"
		absFilePath = os.path.join(scriptDir, relPath)

		os.system('sh ' + absFilePath +' '+  domain)
		relPath = "Policy/" + domain + ".dot"
		absFilePath = os.path.join(scriptDir, relPath)

		self.policy = Policy.policy(absFilePath)

class activityBuffer:

	def __init__(self):	
		self.currentActivity = "void"	
		self.currentActivitiesProb = {}
		self.currentActivitiesProb["void"] = 60.0 
		self.buffer = []
		for i in range(60):
			self.buffer.append("void_1.00")

	def add(self, entry):
		old = self.buffer.pop()
		self.currentActivitiesProb[old.split('_')[0]] = self.currentActivitiesProb[old.split('_')[0]] - float(old.split('_')[1])
		self.buffer.append(entry)
		self.currentActivitiesProb[entry.split('_')[0]] = self.currentActivitiesProb[entry.split('_')[0]] + float(entry.split('_')[1])
		self.update()

	def update(self):
		self.currentActivity =  max(self.currentActivitiesProb, key=lambda key: self.currentActivitiesProb[key])

mainSolver = solver()
while 1:
	entry = raw_input("cmd : ")

	if entry == "quit":
		break

	elif entry.split(" ")[0] == "load":
		if entry.split(" ")[1].split(".")[0] ==  entry.split(" ")[1]:
			listGoal = simpleParser(entry.split(" ")[1])
			domain = Domain.Domain(listGoal)
			generateDomain(domain, entry.split(" ")[1])
			mainSolver.generatePolicy(entry.split(" ")[1])

		elif entry.split(" ")[1].split(".")[1] == "dot":
			mainSolver.loadPolicy(entry.split(" ")[1])
		elif entry.split(" ")[1].split(".")[1] == "pomdpx":
			mainSolver.generatePolicy(entry.split(" ")[1])  

		print("domain loaded")

	elif entry == "status":
		print(mainSolver.policy.currentPlan)
		print(mainSolver.policy.currentNode)

	elif entry.split(" ")[0] == "update":
		mainSolver.policy.update(entry.split(" ")[1])
		print(mainSolver.policy.currentPlan)
		print(mainSolver.policy.currentNode)



#monroeDomain = Domain.Domain(simpleParser('Domain/monroeDomain'))
#generateDomain(monroeDomain, "monroeDomain")

#test = solver()
#test.generatePolicy("monroeDomain")




# testVal = Policy.policy('test.dot')
# testVal.dot[0].write_png('test.png')


