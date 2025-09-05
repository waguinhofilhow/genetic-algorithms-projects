#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

typedef struct individual_{
    int* listItens;
    double fitness;
} individual;


int weight(int* listItens, int size, int* weightList){
    
    int weight = 0;
    
    for(int i = 0; i < size; i++){
        if(listItens[i] == 1){
            weight += weightList[i];
        }
    }
    
    return weight;
}

int value(int* listItens, int size, int* valueList){
    
    int value = 0;
    
    for(int i = 0; i < size; i++){
        if(listItens[i] == 1){
            value += valueList[i];
        }
    }
    
    return value;
}

void printList(int* list, int size){
    
    for(int i = 0; i < size; i++){
        printf("%d ", list[i]);
    }
    printf("\n\n");
}

int* randomList(int size, int lowestValue, int highestValue){
    
    int* list = calloc(size,sizeof(int));
    
    for(int i = 0; i < size; i++){
        list[i] = lowestValue + rand()%(highestValue + 1 - lowestValue);
    }
    
    return list;
}

int* randomIndividual(int size, double chanceOf1){
    
    int* list = calloc(size,sizeof(int));
    
    for(int i = 0; i < size; i++){
        if((rand() / (double)RAND_MAX) < chanceOf1)
        list[i] = 1;
        else
        list[i] = 0;
    }
    
    return list;
}

individual* startPopulation(individual* population, int populationSize, int size){
    
    int* auxList;
    
    for(int i = 0; i < populationSize; i++){
        population[i].listItens = calloc(size,sizeof(int));
        auxList = randomIndividual(size, 0.2);
        memcpy(population[i].listItens, auxList, size*sizeof(int));
        free(auxList);
    }
    
    return population;
}

double normalizedFitness(int value, int maxValue){
    return (double)value/maxValue;
}

individual* setPopulationFitness(individual* population, int populationSize, int size, int maxWeight, int* weightList, int* valueList){
    
    int individualWeight;
    int individualValue;
    
    for(int i = 0; i < populationSize; i++){
        individualWeight = weight(population[i].listItens, size, weightList);
        
        if(individualWeight > maxWeight){
            population[i].fitness = 0;
            continue;
        } else {
            individualValue = value(population[i].listItens, size, valueList);
            population[i].fitness = normalizedFitness(individualValue, size*100);
        }
    }
    
    return population;
}

individual tournamentSelection(individual* population, int populationSize, int k) {
    
    individual best;
    best.fitness = -1.0;

    for (int i = 0; i < k; i++) {
        int index = rand() % populationSize;
        if (population[index].fitness > best.fitness) {
            best = population[index];
        }
        if((i == k - 1) && (best.fitness == 0))
        i = 0;
    }
    
    return best;
}

individual onePointCrossover(individual parent1, individual parent2, int size){
    individual auxIndividual;
    
    auxIndividual.listItens = calloc(size, sizeof(int));
    
    int crossPoint = 1 + rand()%(size - 1);
    
    for(int i = 0; i < crossPoint; i++){
        auxIndividual.listItens[i] = parent1.listItens[i];
    }
    for(int i = crossPoint; i < size; i++){
        auxIndividual.listItens[i] = parent2.listItens[i];
    }
    
    return auxIndividual;
}

individual mutate(individual ind, int size, int* weightList, int maxWeight){
    for(int i = 0; i < size; i++){
        if(rand()/(double)RAND_MAX < 0.01){
            if(ind.listItens[i] == 0){
                ind.listItens[i] = 1;
                if(weight(ind.listItens, size, weightList) > maxWeight){
                    ind.listItens[i] = 0;
                }
            } else{
                ind.listItens[i] = 0;
            }
        }
    }
    
    return ind;
}

void freeUsedSpace(individual* initialPopulation, individual* generation, int populationSize, int size){
    for(int i = 0; i < populationSize; i++){
        free(initialPopulation[i].listItens);
        free(generation[i].listItens);
    }
    
    free(initialPopulation);
    free(generation);
}

void salvar_csv (const char *nome_arquivo, int num_generations,
                double best_fitness[], double avg_fitness[],
                int best_value[], int **best_individuals, int num_items) {
    FILE *fp = fopen(nome_arquivo, "w");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    // Cabeçalho do CSV
    fprintf(fp, "generation,best_fitness,avg_fitness,best_value,best_individual\n");

    for (int g = 0; g < num_generations; g++) {
        fprintf(fp, "%d,%.2f,%.2f,%d,", g, best_fitness[g], avg_fitness[g], best_value[g]);

        // Escreve o indivíduo como string binária
        for (int i = 0; i < num_items; i++) {
            fprintf(fp, "%d", best_individuals[g][i]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void knapsack(int size, int maxWeight, int* weightList, int* valueList,
              int populationSize, int maxGenerations) {
    
    individual* initialPopulation = malloc(populationSize*sizeof(individual));
    individual* generation = malloc(populationSize*sizeof(individual));
    
    initialPopulation = startPopulation(initialPopulation, populationSize, size);
    initialPopulation = setPopulationFitness(initialPopulation, populationSize, size, maxWeight, weightList, valueList);
    memcpy(generation, initialPopulation, populationSize*sizeof(individual));

    // Arrays para estatísticas
    double *best_fitness = malloc(maxGenerations * sizeof(double));
    double *avg_fitness = malloc(maxGenerations * sizeof(double));
    int *best_value = malloc(maxGenerations * sizeof(int));
    int **best_individuals = malloc(maxGenerations * sizeof(int*));
    for (int g = 0; g < maxGenerations; g++) {
        best_individuals[g] = malloc(size * sizeof(int));
    }

    individual parent1, parent2;
    individual child1, child2;
    
    for (int numGenerations = 0; numGenerations < maxGenerations; numGenerations++) {
        
        // Evolução da população
        for (int i = 0; i < populationSize; i += 2) {
            parent1 = tournamentSelection(generation, populationSize, 5);
            parent2 = tournamentSelection(generation, populationSize, 5);
            
            if ((rand() / (double)RAND_MAX) < 0.7) {
                child1 = onePointCrossover(parent1, parent2, size);
                child2 = onePointCrossover(parent2, parent1, size);
            } else {
                child1 = parent1;
                child2 = parent2;
            }
            
            child1 = mutate(child1, size, weightList, maxWeight);
            child2 = mutate(child2, size, weightList, maxWeight);
            
            generation[i] = child1;
            generation[i + 1] = child2;
        }
        
        setPopulationFitness(generation, populationSize, size, maxWeight, weightList, valueList);

        // Estatísticas da geração
        int bestIndex = 0;
        double sumFitness = 0.0, bestFitness = -1.0, currentFitness = 0.0;
        int bestValue = 0;  // Armazena o valor da mochila do melhor indivíduo

        for (int i = 0; i < populationSize; i++) {
            currentFitness = generation[i].fitness;
            sumFitness += generation[i].fitness;

            // Calcula o valor da mochila do indivíduo atual
            int currentValue = value(generation[i].listItens, size, valueList);

            if (currentFitness > bestFitness) {
                bestIndex = i;
                bestFitness = currentFitness;
                bestValue = currentValue;  // Aqui registramos o valor ao invés do peso
            }
        }

        // Atualiza os vetores de estatísticas
        best_fitness[numGenerations] = bestFitness;
        avg_fitness[numGenerations] = sumFitness / populationSize;
        best_value[numGenerations] = bestValue;  // Novo vetor para armazenar o valor da mochila
        memcpy(best_individuals[numGenerations], generation[bestIndex].listItens, size * sizeof(int));
    }
    
    // Salvar estatísticas no CSV
    salvar_csv ("knapsack_results.csv", maxGenerations,
               best_fitness, avg_fitness, best_value,
               best_individuals, size);
    
    // Liberar memória
    for (int g = 0; g < maxGenerations; g++) {
        free(best_individuals[g]);
    }
    free(best_individuals);
    free(best_fitness);
    free(avg_fitness);
    free(best_value);

    freeUsedSpace(initialPopulation, generation, populationSize, size);
}

int main() {

srand(time(NULL));

int size = 30;
int maxWeight = 50;
int populationSize = 500;
int maxGenerations = 500;
int *weightList, *valueList;

weightList = randomList(size, 1, 20);
valueList = randomList(size, 10, 100);

printf("Weight list: "); printList(weightList, size);
printf("Value list: "); printList(valueList, size);

knapsack(size, maxWeight, weightList, valueList, populationSize, maxGenerations);

free(weightList);
free(valueList);

return 0;
}