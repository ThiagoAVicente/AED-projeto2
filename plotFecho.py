import matplotlib.pyplot as plt
import numpy as np

def r_squared(y_true, y_pred):
    ss_res = np.sum((y_true - y_pred) ** 2)
    ss_tot = np.sum((y_true - np.mean(y_true)) ** 2)
    return 1 - (ss_res / ss_tot)

def plot_regression_line(X, yAverage, degree, lbl):
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

    yMeaned = [row.mean() for row in yAverage]
    interpolated_y_fit = polynomial(X)  # Fit y-values for original X
    r2 = r_squared(yMeaned, interpolated_y_fit)
    print(f"degree: {degree}. -> r2= {r2}")


    # Plotar os dados originais e a curva ajustada
    for i in range(len(X)):
        for j in range(len(yAverage[0])):
            y_ = yAverage[i][j]
            plt.scatter(X[i], y_, color='blue')
    plt.plot(x_fit, y_fit, label=lbl)
    plt.title('Total de operações por número de vertices')
    plt.xlabel('|V|')
    plt.grid(True)


xStart = 4
yBest =  []
yWorst = []
yAverage = []

with open("out/best","r") as fin:
    for line in fin:
        yBest.append(int(line))

with open("out/worst","r") as fin:
    for line in fin:
        yWorst.append(int(line))


with open("out/average","r") as fin:
    for line in fin:
        line = line.replace("\n","").strip()
        values = line.split(" ")
        v = [int(i) for i in values]
        yAverage.append(v)

x = [i for i in range (xStart, len(yBest)+xStart)]

plot_regression_line(x,np.array(yAverage),3,f'Average case computacional--3')
plot_regression_line(x,np.array(yAverage),2,f'Average case computacional--2')

plt.plot(x,yBest,label='Best case computacional')
plt.plot(x,yWorst,label = 'Worst case computacional')

x_np = np.array(x)
yBestTeo = 3*x_np*(x_np+1) +x_np*x_np+x_np*(x_np-1)
plt.plot(x,yBestTeo,label="Best case teórico")

yWorstTeo = 6*x_np*x_np+x_np*(3*x_np+(x_np-1)*(x_np-1)-1)+x_np*(x_np-1)
plt.plot(x,yWorstTeo,label="Worst case teórico")
plt.legend()
plt.show()
