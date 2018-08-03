#!/usr/bin/env
import numpy
import codecs
import pydot
import socket
import os
from GraphNavigator import Policy
from PolicyGenerator import Domain
from subprocess import call
import Solver

mainSolver = Solver.solver()
mainSolver.loadPolicy("Beverage.dot")
print(mainSolver.policy.currentPlan)
mainSolver.add("teakettle",76.22)
print(mainSolver.policy.currentPlan)
print(mainSolver.policy.nextPossibleAction())
print(mainSolver.policy.currentPlanProb)
