\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 1200 x1 + 1800 x2 + 500 x3 + 10000 x4 + 3800 x5
Subject To
 c1: - 0.2 x1 + 0.05 x2 - 0.3 x3 + 0.7 x4 - 0.3 x5 >= 0
 c2: 0.035 x1 - 0.005 x2 - 0.01 x3 - 0.015 x4 - 0.015 x5 >= 0
 c3: 0.005 x1 - 0.025 x2 + 0.045 x3 - 0.035 x4 + 0.965 x5 >= 0
 c4: x1 + x2 + x3 + x4 + x5  = 1
End
