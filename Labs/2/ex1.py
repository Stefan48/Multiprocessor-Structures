import numpy as np
import matplotlib.pyplot as plt
from math import log2

p = 32
x = np.arange(1, p + 1, 1)

def O1(n):
    return n**3
    
def On(n):
    return n**3 + n**2 * log2(n)
    
def T1(n):
    return n**3
    
def Tn(n):
    return 4*n**3 / (n + 3)
    
def S(n):
    return T1(n) / Tn(n)
    
def E(n):
    return S(n) / n

def R(n):
    return On(n) / O1(n)
    
def U(n):
    return R(n) * E(n)
    
def Q(n):
    return S(n) * E(n) / R(n)
    

def Values(Func):
    return np.array([Func(n) for n in range(1, p + 1)])



plt.figure()

plt.subplot()
plt.plot(x, Values(S), 'blue')

plt.subplot()
plt.plot(x, Values(E), 'purple')

plt.subplot()
plt.plot(x, Values(R), 'red')

plt.subplot()
plt.plot(x, Values(U), 'green')

plt.subplot()
plt.plot(x, Values(Q), 'orange')

plt.legend(['S', 'E', 'R', 'U', 'Q'])
plt.show()
