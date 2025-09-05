import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Lê o CSV
df = pd.read_csv("data/knapsack_results.csv")

# Converte a coluna 'best_individual' para listas de inteiros
df['best_individual_list'] = df['best_individual'].apply(lambda x: [int(c) for c in str(x)])

# Gráfico 1: Evolução do fitness
plt.figure(figsize=(12,5))
plt.plot(df['generation'], df['best_fitness'], label='Best Fitness', marker='o')
plt.plot(df['generation'], df['avg_fitness'], label='Average Fitness', marker='x')
plt.xlabel('Generation')
plt.ylabel('Fitness')
plt.title('Evolution of Fitness over Generations')
plt.legend()
plt.grid(True)
plt.show()

# Gráfico 2: Evolução do peso do melhor indivíduo
plt.figure(figsize=(12,5))
plt.plot(df['generation'], df['best_value'], label='Best Value', color='green', marker='s')
plt.xlabel('Generation')
plt.ylabel('Value')
plt.title('Evolution of Best Individual Value')
plt.legend()
plt.grid(True)
plt.show()

# Gráfico 3: Representação do melhor indivíduo final
final_best = df['best_individual_list'].iloc[-1]
plt.figure(figsize=(12,3))
plt.bar(range(len(final_best)), final_best, color='skyblue')
plt.xlabel('Item Index')
plt.ylabel('Inclusion (0 or 1)')
plt.title('Final Best Individual Selection')
plt.show()
