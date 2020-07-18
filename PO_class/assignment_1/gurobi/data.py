import pandas as pd
import numpy as np
import re

class Data:
    '''
        n =  Number of vertices (1 to n)
        m =  Number of Archs (1 to m)
        s = Origin index
        t = Sink index
        arch_data = Dataframe of Archs
    '''

    def __init__(self, filename):
        self.arch_data = pd.DataFrame({'i':[np.nan], 'j':[np.nan], 'c':[np.nan]})

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
            
            file.close()

    

