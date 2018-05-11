import re
import codecs
import string


def extractAction(listWord):
	Actions= []
	for word in listWord :
		if len(word) == 0:
			continue
		elif word[0] == '!':
			Actions.append(word[1:])
	return Actions


Entry = codecs.open('monroe5000.txt', 'r', 'utf-8')
Output = codecs.open('monroeDomain', 'w', 'utf-8')

completePlan = ""
count = 0
for line in Entry:

	if len(line) > 2 :
		completePlan = completePlan + line


	elif len(completePlan) > 2 :
		count = count + 1
		print(count)
		completePlan = completePlan.replace('(', '')
		completePlan = completePlan.replace(')', '')
		words = completePlan.split(' ') 
		goal = words[0]
		actions = extractAction(words)
		out = ""
		for action in actions:
			out = out + ' ' + action
		Output.write(goal + ' : ' + out + '\n')

		completePlan = ""

