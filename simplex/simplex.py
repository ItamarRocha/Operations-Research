
'''
	This code was written by Igor Malheiros for Operational Research Module - 2018 UFPB
	https://github.com/igormalheiros/Simplex
'''

import numpy as np

class Simplex:
	def __init__(self, OF, Max):
		self.OF = [1] + OF

		if Max == 'max' or Max == "Max":
			self.OF = [i * -1 for i in self.OF]
			print(self.OF)
		elif Max != 'min' or Max != "Min":
			print("ERROR!")

		self.rows = []
		self.cons = []
		self.numVariables = len(self.OF)-1
		self.numLackVariables = 0
		self.BigM = 10000
		self.numOfConstraints = -1
		self.equalConstraintIndex = []
		self.greaterConstraintIndex = []
		self.greaterConstraintVariables = []
		self.BV = []
		self.NBV = [i for i in range(1, self.numVariables+1)]

	def __str__(self):

		s = "VB\Z|"
		for e in self.OF[1:]:
		    s += (" %8.2f |" % e)
		s += ("\n")
		s += ("-"*len(s))
		s += ("\n")
		k = 0
		for l in self.rows[0:]:
		    s += (" x%d |" % self.BV[k])
		    k += 1
		    for e in l[1:]:
		        s += (" %8.2f |" % e)
		    s += ("\n")
		return s

	def add_constraint(self, coefficients, value, sign):
		self.numOfConstraints += 1
		self.rows.append([0] + coefficients)
		self.cons.append(value)
		self.numLackVariables += 1

		if sign == '<=':
			self.OF.append(0)
			self.BV.append(self.numVariables+self.numLackVariables)
		elif sign == '=':
			self.OF.append(self.BigM)
			self.equalConstraintIndex.append(self.numOfConstraints)
			self.BV.append(self.numVariables+self.numLackVariables)
		elif sign == '>=':
			self.OF.append(0)
			self.OF.append(self.BigM)
			self.greaterConstraintVariables.append((self.numVariables + self.numLackVariables, self.numVariables + self.numLackVariables+1)) 
			self.numLackVariables += 1
			self.BV.append(self.numVariables+self.numLackVariables)
		else:
			print("CONSTRAINT OPERATOR ERROR")


	'''
		Build a initial table of tableau simplex method
	'''
	def initTableau(self):

		identity = np.identity(self.numOfConstraints+1)
		aux1 = np.concatenate( (np.array(self.rows, dtype=float), identity), axis=1)
		aux2 = np.array([self.cons], dtype=float)
		self.rows = np.concatenate( (aux1,aux2.T), axis=1 )
		self.OF.append(0)
		self.OF = np.array(self.OF, dtype=float)
		
		#Duplicate Line for constraints with sign: >=
		for i in self.greaterConstraintVariables:
			self.rows = np.insert(self.rows, i[0], self.rows[:,i[0]], axis=1)
			for j in range(len(self.rows[:,i[0]])):
				if self.rows[j][i[0]] == 1.0:
					self.rows[j][i[0]] = -1
					self.greaterConstraintIndex.append(j)

		#Method Big M must change the Objective Function
		for i in self.equalConstraintIndex:
			self.OF -= self.BigM * self.rows[i]
		for i in self.greaterConstraintIndex:
			self.OF -= self.BigM * self.rows[i]

	'''
		Test if current solution in Tableau is not optimal
	'''
	def notOptTest(self):
		if min(self.OF[1:-1]) < 0:
			return True
		else:
			return False

	'''
		Select the pivot column
	'''
	def selectColumn(self):
		lowerValue = 0
		lowerIndex = 0
		for i in range(1, len(self.OF)-1):
			if self.OF[i] < lowerValue:
				lowerValue = self.OF[i]
				lowerIndex = i

		return lowerIndex

	'''
		Select the pivot row by minimum ratio test
	'''
	def selectRow(self, columnIndx):
		column = self.rows[:,columnIndx]
		values = self.rows[:,-1]
		ratio = []
		#Minimum ratio test
		for i in range(len(values)):

			if column[i] <= 0:
				ratio.append(99999 * abs(max(column)))
			elif values[i] == 0:
				ratio.append(9999)			
			else:
				ratio.append(values[i]/column[i])

		return np.argmin(ratio)

	'''
		Algebric operation in each row
	'''
	def rowsOperation(self, row, col):
		selected = self.rows[row][col]
		print(selected)
		self.rows[row] /= selected

		for i in range(len(self.rows)):
			if i != row:
				self.rows[i] -= self.rows[i][col] * self.rows[row]

		self.OF = self.OF - self.OF[col] * self.rows[row]

	'''
		Udapte Tableau table
	'''
	def updateTableau(self):
		It = 1
		while self.notOptTest():
			print("Iteration " + str(It))
			c = self.selectColumn()
			r = self.selectRow(c)
			self.rowsOperation(r,c)
			print ('\npivot column: %s\npivot row: %s'%(c,r+2))
			print ('\nX%s in \nX%s out'%(c, self.BV[r]))
			print()

			for i in range(len(self.NBV)):
				if self.NBV[i] == c:
					self.NBV[i], self.BV[r] = self.BV[r], self.NBV[i]
			print(self)
			It += 1


	def solve(self):
		self.initTableau()
		print("First Result:")
		print(self)
		self.updateTableau()
		self.showSolution()

	def showSolution(self):
		print("Tableau Result:")
		print(self)
		print("Optimal Solution = " + str(self.OF[-1]))
		print("Basic Variables:")
		s = ""
		for b in range(len(self.BV)):
			s += ("x" + str(self.BV[b]) + " = " + str(self.rows[b][-1]) + " ")
		print(s)

if __name__ == '__main__':

	"""
	Ex 1
	max z = 3x1 + 5x2
	sa
	x1 <= 4
	2x2 <= 12
	3x1 + 2x2 <= 18
	x1,x2 >= 0
	Ex2
	max z = 2x + 3y + 2z
	st
	2x + y + z <= 4
	x + 2y + z <= 7
	z          <= 5
	x,y,z >= 0
	"""
	t = Simplex([3,5], "Max")
	t.add_constraint([1, 0], 4, "<=")
	t.add_constraint([0, 2], 12, "<=")
	t.add_constraint([3, 2], 18, "=")
	t.solve()
	
	'''t = Simplex([2,3,2], "Max")
	t.add_constraint([2, 1, 1], 4, "<=")
	t.add_constraint([1, 2, 1], 7, "<=")
	t.add_constraint([0, 0, 1], 5, "<=")
	t.solve()'''

	'''t = Simplex([1,1, 1], "Max")
	t.add_constraint([1, 1, 0], 1, "<=")
	t.add_constraint([0,-1, 1], 0, "<=")
	t.solve()'''

	'''t = Simplex([5,5,3], "Max")
	t.add_constraint([1,3,1],3, "<=")
	t.add_constraint([-1,0,3],2, "<=")
	t.add_constraint([2,-1,2],4, "<=")
	t.add_constraint([2,3,-1],2, "<=")
	t.solve()'''

	'''t = Simplex([2,1], "Max")
	t.add_constraint([1, 1], 3, "<=")
	t.add_constraint([2, 1], 5, "<=")
	t.add_constraint([0, 1], 1, "=")
	t.solve()'''
	"""
	t = Simplex([0.4, 0.5], "Min")
	t.add_constraint([0.3, 0.1], 2.7, "<=")
	t.add_constraint([0.5, 0.5], 6, "=")
	t.add_constraint([0.6, 0.4], 6, ">=")
	t.solve()
	"""


