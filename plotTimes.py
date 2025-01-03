import matplotlib.pyplot as plt

yBest = [0.000004,0.000004,0.000005,0.000006,0.000009,0.000010,0.000012,0.000011,0.000012,0.000015,0.000017,0.000019,0.000018,0.000017,0.000015,0.000018]
yWorst = [0.000005,0.000009,0.000011,0.000016,0.000021,0.000027,0.000032,0.000042,0.000050,0.000061,0.000074,0.000088,0.000139,0.000136,0.000212,0.000243]
x = [i for i in range(4,20)]

plt.plot(x,yBest,label="Melhor caso")
plt.plot(x,yWorst,label="Pior Caso")
plt.xlabel("|V|")
plt.ylabel("Tempo de execução")
plt.grid()
plt.legend()
plt.show()
