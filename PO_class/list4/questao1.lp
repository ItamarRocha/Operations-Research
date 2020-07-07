\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: - X00 - X01 - X02 - X10 - X11 - X12 - X20 - X21 - X22 + 45
Subject To
 c1:  X00 + X01 + X02  = 15
 c2:  X10 + X11 + X12  = 15
 c3:  X20 + X21 + X22  = 15
 c4:  X00 + X10 + X20  = 15
 c5:  X01 + X11 + X21  = 15
 c6:  X02 + X12 + X22  = 15
 c7:  X02 - X20 <= -1
 c8:  X00 - X22 >= 1
 c9:  X00 + X11 + X22  = 15
 c10: X02 + X11 + X20  = 15
Bounds
 0 <= X00 <= 9
 0 <= X01 <= 9
 0 <= X02 <= 9
 0 <= X10 <= 9
 0 <= X11 <= 9
 0 <= X12 <= 9
 0 <= X20 <= 9
 0 <= X21 <= 9
 0 <= X22 <= 9
End
