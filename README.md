# Genetic Algorithms 🧬  

This repository contains implementations of **Genetic Algorithms (GA)** applied to different optimization problems.  
GA is inspired by natural evolution and uses **selection**, **crossover**, and **mutation** operators to explore the solution space.  

## 📌 Problems Addressed  
### 🔹 1. Alpine2 Function  
- A mathematical benchmark function for continuous optimization.  
- Goal: find the global minimum.  

### 🔹 2. Schaffers F6 Function  
- A classical multimodal optimization benchmark.  
- Goal: find the global maximum while avoiding local optimal traps.  

### 🔹 3. Knapsack Problem  
- A classical combinatorial optimization problem.  
- Goal: maximize the total value of items placed in the knapsack without exceeding its weight capacity.  

## ✨ Features  
- Solution representation using chromosomes (real-valued or binary).  
- Fitness evaluation according to each problem.  
- Genetic operators:  
  - Selection (roulette, tournament, etc.)  
  - Crossover  
  - Mutation  
- Configurable stopping criteria (number of generations, size of generation, etc.).  
- Evolution statistics: best solution, population average, etc.  

## 📂 Project Structure  
├── alpine2/ # GA applied to the Alpine2 function  
│   ├── alpine2Genetic.c  
│   ├── plot_best_individuals.py  
│   └── data/ # Charts and csv files  
├── schaffersf6/ # GA applied to the Schaffersf6 function  
│   ├── schaffersf6Genetic.c  
│   ├── plot_best_individuals.py  
│   └── data/ # Charts and csv files   
├── knapsack/ # GA applied to the knapsack problem  
│   ├── knapsackBruteForce.c  
│   ├── knapsackGenetic.c  
│   ├── plot_best_individuals.py  
│   └── data/ # Charts and csv files     
└── README.md # Documentation  
└── LICENSE
