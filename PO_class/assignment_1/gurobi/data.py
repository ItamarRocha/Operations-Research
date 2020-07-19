import pandas as pd
import numpy as np
import gurobipy as gp
from gurobipy import GRB
import re



class Data:
    '''
        n =  Number of vertices (1 to n)
        m =  Number of Archs (1 to m)
        s = Source index
        t = Sink index
        arch_data = Dataframe of Archs
    '''

    def __init__(self, filename, commodity = 'Commodity'):
        self.arch_data = pd.DataFrame({'i':[np.nan], 'j':[np.nan], 'c':[np.nan]})
        self.commodity = commodity
        with open(filename,'r') as file:
            data_l = file.readlines()
            #print(data_l)
            
            self.n = int(re.sub(r'\n','',data_l[0]))
            self.m = int(re.sub(r'\n','',data_l[1]))
            self.s = int(re.sub(r'\n','',data_l[2]))
            self.t = int(re.sub(r'\n','',data_l[3]))
            
            archs = data_l[4:]
            datalist = []
            for arch in archs:
                datalist.append([int(s) for s in arch.split() if s.isdigit()])
                
            
            for i in range(len(datalist)):
                self.arch_data.loc[i] = datalist[i]
            
            self.max_demand = 0
            for i in range(len(self.arch_data)):
                if self.arch_data.iloc[i].i == self.s:
                    self.max_demand += self.arch_data.iloc[i].c
                
            file.close()

    def gurobi_solver(self):

        # Get Archs info for initializing gurobi dict
        self.arch_data['archs'] = list(zip(self.arch_data.i.astype(int), self.arch_data.j.astype(int)))

        # Set series cost triplets for gurobi
        self.arch_data['commodity'] = self.commodity
        self.arch_data['cost_t'] = list(zip(self.arch_data['commodity'],self.arch_data.i.astype(int), self.arch_data.j.astype(int)))

        # setting cost dict
        cost_dist = {}
        for i in range(len(self.arch_data)):
            cost_dist[self.arch_data.iloc[i].cost_t] = 0
            
        cost_dist[(self.commodity, self.s, self.t)] = 1

        # Dict for gurobi multidict, contains archs and respectives capacities
        dist = {}
        for i in range(len(self.arch_data)):
            dist[self.arch_data.iloc[i].archs] = self.arch_data.iloc[i].c.astype(int)

        dist[(self.s, self.t)] = GRB.INFINITY

        # Dict to set demands
        inflow_dist = {}
        for i in range(self.t+1):
            if i == 0:
                pass
            
            elif i == self.s:
                inflow_dist[(self.arch_data.iloc[i].commodity, i)] = int(self.max_demand)
                
            elif i == self.t:
                
                inflow_dist[(self.arch_data.iloc[i].commodity, i)] = -int(self.max_demand)
            else:
                inflow_dist[(self.arch_data.iloc[i].commodity, i)] = 0

        # Base data ## Solution ###############################################################################################################################
        commodities = ['Commodity']
        nodes = range(self.s, self.t+1)

        arcs, capacity = gp.multidict(dist)

        # Cost for triplets commodity-source-destination
        cost = cost_dist

        # Demand for pairs of commodity-city
        inflow = inflow_dist

        # Create optimization model
        m = gp.Model('netflow')

        # Create variables
        flow = m.addVars(commodities, arcs, obj=cost, name="flow")

        # Arc-capacity constraints
        m.addConstrs(
            (flow.sum('*', i, j) <= capacity[i, j] for i, j in arcs), "cap")

        # Flow-conservation constraints
        m.addConstrs(
            (flow.sum(h, '*', j) + inflow[h, j] == flow.sum(h, j, '*')
                for h in commodities for j in nodes), "node")

        # Compute optimal solution
        m.optimize()

        z = 0
        # Print solution
        if m.status == GRB.OPTIMAL:
            solution = m.getAttr('x', flow)
            for h in commodities:
                print('\nOptimal flows for %s:' % h)
                for i, j in arcs:
                    if solution[h, i, j] > 0:
                        if i == self.s and j != self.t:
                            z+= solution[h, i, j]
                        print('%s -> %s: %g' % (i, j, solution[h, i, j]))

        print(f'\nMaximum Flow for the instace: {int(z)}')



    

