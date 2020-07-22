import dataQuestion
from dataQuestion import Data
import sys

if len(sys.argv) < 2:
    print('Missing Parameters\nExample: python exec.py filepath/instance.txt')
elif len(sys.argv) > 2:
    print('Too much Parameters\nExample: python exec.py filepath/instance.txt')
else:
    instance = str(sys.argv[1])

    q2 = Data(instance, commodity = 'Water')
    q2.gurobi_solver()