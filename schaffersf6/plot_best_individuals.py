import matplotlib.pyplot as plt
import pandas as pd
import os

# caminho relativo à raiz do projeto
file_path = os.path.join("data", "best_individuals.csv")

# leitura do CSV em DataFrame
df = pd.read_csv(file_path, sep="\s+", header=None, names=["geracao", "x", "y", "val"])

# converter colunas para listas
geracoes = df["geracao"].tolist()
xs = df["x"].tolist()
ys = df["y"].tolist()
vals = df["val"].tolist()

# plotagem
plt.figure(figsize=(10, 4))

# evolução de x e y
plt.subplot(1, 2, 1)
plt.plot(geracoes, xs, label='x')
plt.plot(geracoes, ys, label='y')
plt.xlabel('Generation')
plt.ylabel('Coordenates')
plt.title('Evolution of the coordenates of best individual')
plt.legend()

# evolução da função objetivo
plt.subplot(1, 2, 2)
plt.plot(geracoes, vals, label='Value of the function')
plt.xlabel('Generation')
plt.ylabel('Value')
plt.title('Evolution of the objective function')
plt.legend()

plt.tight_layout()
plt.show()