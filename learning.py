import operator
import random
import re
import numpy as np
import pdb


def permutation(n):
	return list(np.random.permutation(n))

def generateRandomData(n=50,numberOfChoicesPerPerson = 5):
	men = []
	women = []
	for i in range(n):
		men.append([])
		women.append([])

	for i in men:
		for j in range(numberOfChoicesPerPerson):
			i.append(random.randint(0,n-1))
	for i in women:
		for j in range(numberOfChoicesPerPerson):
			i.append(random.randint(0,n-1))


	return (men,women)



def match(dataset):
	men = dataset[0]
	women = dataset[1]
	results = []
	


nA = 0 #number of nodes on left 
nB = 0 #number of nodes on right
data = [] #array of arrays that has m,n weight where m element of A, and n element of B


def readData(filename):
	global data
	global nA
	global nB
	f = open(filename,'r')
	lines = f.readlines()
	i = 0
	headerLine = lines[0]
	numbers = [int(i) for i in re.findall('\d+', headerLine)]
	nA = numbers[0]
	nB = numbers[1]

	i+=1 #skip over the header line
	i+=nA
	i+=1
	i+=nB
	i+=1

	#now i should be at the beginning of data
	#data += nA * [[]]
	data = [[] for i in range(nA)]

	while i<len(lines):
		line = lines[i]
		lineTokens = line.split(' ')
		m = int(lineTokens[1])
		n = int(lineTokens[2])
		w = float(lineTokens[-1])
		#print(m,n,w)
		#print(data)
		#data[m].insert(n,w)
		#print(data)
		#print(data[m])
		#print('m',m)
		#print(w)
		data[m].append(w)
		#print(data)
		#pdb.set_trace()
		#print(m)
		i+=1


#weight function
def w(ordering):
	r = 0.0
	i = 0
	for j in ordering:
		r += data[i][j]
		#print(i,j,data[i][j])
		i+=1
	return r

def getRandomOrder():
	return permutation(nA)

def computeA():
	pass



def analyze(dataset,results):
	pass



readData('even100.txt')
#readData('10Sample.txt')

Ahat = np.zeros((nA,nB))
scores = []

orderings = []

def fillRandomOrderings():
	for i in range(100):
		O = getRandomOrder()
		orderings.append(O)
	
def isIbeforeJ(i,j,O):
	if O.index(i) < O.index(j):
		return True
	return False

def getAhatScore(i,j):
	numerator = 0.0
	denominator = 0.0
	for o in orderings:
		if isIbeforeJ(i,j,o):
			numerator += w(o) + w(list(reversed(o)))
		else:
			denominator += w(o) + w(list(reversed(o)))

	return numerator/denominator
	
def computeAHat():
	for i in range(len(Ahat)):
		for j in range(len(Ahat)):
			if i == j:
				continue
			Ahat[i][j] = getAhatScore(i,j)

def power_method(mat, start, maxit = 10):
	result = start
	for i in range(maxit):
		result = mat.dot(result)
		result = result/np.linalg.norm(result)
	return result



fillRandomOrderings()
computeAHat()
#randVector = np.array([1 for i in range(10)])
randVector = np.ones((nA))

res = power_method(Ahat, randVector)
#print(res)
#print(Ahat.dot(randVector))

#print(Ahat)
#o = getRandomOrder()
#print(o,list(reversed(o)))

list_a = [i for i in range(nA)]
list_b = list(res)

sortedList = [k for k, v in sorted(zip(list_a, list_b), key=operator.itemgetter(1))]


print(sortedList)




print('done')





