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
		self.buffer = activityBuffer()

	def loadPolicy(self,file):
		scriptDir = os.path.dirname(__file__) #<-- absolute dir the script is in
		relPath = "Policy/" + file
		absFilePath = os.path.join(scriptDir, relPath)
		self.policy = Policy.policy(absFilePath)

	def generatePolicy(self,domain):

		scriptDir = os.path.dirname(__file__) #<-- absolute dir the script is in
		relPath = "PolicyGenerator/PolicyGenerator.sh"
		absFilePath = os.path.join(scriptDir, relPath)

		os.system('sh ' + absFilePath +' '+  domain)
		relPath = "Policy/" + domain + ".dot"
		absFilePath = os.path.join(scriptDir, relPath)

		self.policy = Policy.policy(absFilePath)

	def add(self, entry,prob):
		oldEntry = self.buffer.currentActivity
		self.buffer.add(entry,prob)
		if oldEntry != self.buffer.currentActivity:
			self.policy.update(self.buffer.currentActivity)
		

class activityBuffer:
	def __init__(self):	
		self.currentActivity = "void"
		self.buffer = []
		for i in range(1):
			self.buffer.append(["void",1.00])

	def add(self, entry, prob):
		old = self.buffer.pop(0)
		self.buffer.append([entry,prob])
		#self.currentActivitiesProb[old.split('_')[0]] = self.currentActivitiesProb[old.split('_')[0]] - float(old.split('_')[1])
		#self.buffer.append(entry)
		#self.currentActivitiesProb[entry.split('_')[0]] = self.currentActivitiesProb[entry.split('_')[0]] + float(entry.split('_')[1])

		self.update()

	def update(self):
		currentActivities = []
		currentActivitiesProb = []
		for i in self.buffer:
			index = 0
			find  = False 
			for j in currentActivities:			
				if i[0] == j:
					find = True
					currentActivitiesProb[index] = currentActivitiesProb[index] + i[1]
					break
				index = index + 1 
			if not find:
				currentActivities.append(i[0])
				currentActivitiesProb.append(i[1])

		print(currentActivities)

		self.currentActivity = currentActivities[currentActivitiesProb.index(max(currentActivitiesProb))]


		#self.currentActivity =  max(self.currentActivitiesProb, key=lambda key: self.currentActivitiesProb[key])

# test = activityBuffer()
# print test.buffer
# test.add("hello", 0.5)
# print '********'
# print test.buffer
# print '********'
# for i in range(50):
# 	test.add("hello", 0.5)
# print test.buffer
# print '********'
# print test.currentActivity
