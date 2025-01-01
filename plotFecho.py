import matplotlib.pyplot as plt
import numpy as np

def plot_regression_line(X, yAverage, degree=1):
    # Calcular a média de cada linha em yAverage
    y = yAverage.mean(axis=1)

    # Garantir que X seja um array numpy
    X = np.array(X)

    # Ajustar um polinômio de grau especificado
    coefficients = np.polyfit(X, y, degree)
    polynomial = np.poly1d(coefficients)

    # Gerar valores para a curva ajustada
    x_fit = np.linspace(min(X), max(X), 100)
    y_fit = polynomial(x_fit)

    # Plotar os dados originais e a curva ajustada
    plt.figure(figsize=(10, 6))
    for i in range(len(X)):
        for j in range(len(yAverage[0])):
            y_ = yAverage[i][j]
            plt.scatter(X[i], y_, color='blue')
    plt.plot(x_fit, y_fit, color='purple', label=f'Average case computacional')
    plt.title('Total de operações por número de vertices')
    plt.xlabel('|V|')
    plt.grid(True)


x = [i for i in range(4,20)]

yBest = [88,135,192,259,336,423,520,627,744,871,1008,1155,1312,1479,1656,1843]
yWorst = [188,320,498,728,1016,1368,1790,2288,2868,3536,4298,5160,6128,7208,8406,9728]

yAverage = [    [121,138,139,182,180],
                [310,305,305,315,320],
                [297,202,406,393,289],
                [679,679,270,728,644],
                [936,976,920,798,912],
                [1224,1251,453,847,1287],
                [1410,1470,520,1610,889],
                [928,1958,2167,1892,1136],
                [908,1125,2220,856,1958],
                [3367,3471,3471,2665,2522],
                [3864,3178,4172,1234,1652],
                [3900,3435,3540,3510,2356],
                [4160,6128,4096,4176,4832],
                [4028,4811,5083,1929,6987],
                [2597,5490,5706,5094,3721],
                [7619,5795,6802,5928,4000]
]

plot_regression_line(x,np.array(yAverage),2)
plt.plot(x,yBest,label='Best case computacional')
plt.plot(x,yWorst,label = 'Worst case computacional')

x_np = np.array(x)
yBestTeo = 3*x_np*(x_np+1) +x_np*x_np+x_np*(x_np-1)
plt.plot(x,yBestTeo,label="Best case teórico")

yWorstTeo = 6*x_np*x_np+x_np*(3*x_np+(x_np-1)*(x_np-1)-1)+x_np*(x_np-1)
plt.plot(x,yWorstTeo,label="Worst case teórico")
plt.legend()
plt.show()
