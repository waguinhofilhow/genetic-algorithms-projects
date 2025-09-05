#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define M_PI 3.14159265359

double alpine2(double* x, int n) {
    
    double f_x = 1;
    for (int i = 0; i < n; i++) {
        f_x *= sqrt(x[i]) * sin(x[i]);
    }
    return f_x;
}

double** startPopulation(int n, int populationSize) {
    double** population = calloc(populationSize, sizeof(double*));
    for (int i = 0; i < populationSize; i++) {
        population[i] = calloc(n, sizeof(double));
        for (int j = 0; j < n; j++) {
            population[i][j] = ((double) rand() / RAND_MAX * 10.0);
        }
    }
    return population;
}

double normalizedFitness(double value) {
    return (value + 10.0) / 20.0;
}

double* copyIndividual(double* source, int n) {
    double* copy = calloc(n, sizeof(double));
    memcpy(copy, source, n * sizeof(double));
    return copy;
}

double* tournamentSelection(double** generation, double* generationFitness, int populationSize, int k, int n) {
    double* best = NULL;
    double bestFitness = -1.0;

    for (int i = 0; i < k; i++) {
        int index = rand() % populationSize;
        if (generationFitness[index] > bestFitness) {
            bestFitness = generationFitness[index];
            best = generation[index];
        }
    }

    return copyIndividual(best, n);
}

double* findBestInGeneration(double** generation, int populationSize, int index, int n) {
    double* best, *secondBest;
    double f0 = alpine2(generation[0], n);
    double f1 = alpine2(generation[1], n);

    if (f0 > f1) {
        best = generation[0];
        secondBest = generation[1];
    } else {
        best = generation[1];
        secondBest = generation[0];
    }

    for (int i = 2; i < populationSize; i++) {
        double fit = alpine2(generation[i], n);
        if (fit > alpine2(best, n)) {
            secondBest = best;
            best = generation[i];
        } else if (fit > alpine2(secondBest, n)) {
            secondBest = generation[i];
        }
    }

    return (index == 0) ? copyIndividual(best, n) : copyIndividual(secondBest, n);
}

double minimum (double a, double b) { return (a < b) ? a : b; }
double maximum (double a, double b) { return (a > b) ? a : b; }

double* crossover_blx(double* p1, double* p2, double alpha, int n) {
    double* child = calloc(n, sizeof(double));
    for (int i = 0; i < n; i++) {
        double minVal = minimum(p1[i], p2[i]);
        double maxVal = maximum(p1[i], p2[i]);
        double range = maxVal - minVal;
        double low = minVal - alpha * range;
        double high = maxVal + alpha * range;
        double r = (double) rand() / RAND_MAX;
        child[i] = low + r * (high - low);

        if (child[i] > 10.0) child[i] = 10.0;
        if (child[i] < 0.0) child[i] = 0.0;
    }
    return child;
}

double gaussiana(double media, double sigma) {
    double u1 = ((double) rand() + 1) / ((double) RAND_MAX + 2);
    double u2 = ((double) rand() + 1) / ((double) RAND_MAX + 2);
    return media + sigma * sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
}

double* mutacao_gaussiana(double* individuo, double taxa, double sigma, int n) {
    for (int i = 0; i < n; i++) {
        if (((double) rand() / RAND_MAX) < taxa) {
            individuo[i] += gaussiana(0.0, sigma);
            if (individuo[i] < 0.0) individuo[i] = 0.0;
            if (individuo[i] > 10.0) individuo[i] = 10.0;
        }
    }
    return individuo;
}

void maxalpine2(int n, int populationSize, int maxGenerations) {
    double** generation = startPopulation(n, populationSize);
    double* generationFitness = calloc(populationSize, sizeof(double));

    FILE *fp = fopen("best_individuals.csv", "w");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo para salvar resultados");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < populationSize; i++) {
        generationFitness[i] = normalizedFitness(alpine2(generation[i], n));
    }

    int generationCount = 0;
    while (generationCount < maxGenerations) {
        double** nextGen = calloc(populationSize, sizeof(double*));
        double* nextFitness = calloc(populationSize, sizeof(double));

        nextGen[0] = findBestInGeneration(generation, populationSize, 0, n);
        nextFitness[0] = normalizedFitness(alpine2(nextGen[0], n));

        nextGen[1] = findBestInGeneration(generation, populationSize, 1, n);
        nextFitness[1] = normalizedFitness(alpine2(nextGen[1], n));

        // Escreve no arquivo os dados do melhor indivíduo da geração
        fprintf(fp, "%d %f %f %f\n",
                generationCount,
                nextGen[0][0], nextGen[0][1],
                alpine2(nextGen[0], n));

        for (int i = 2; i < populationSize; i += 2) {
            double* p1 = tournamentSelection(generation, generationFitness, populationSize, 3, n);
            double* p2 = tournamentSelection(generation, generationFitness, populationSize, 3, n);

            double* c1;
            double* c2;

            if (((double) rand() / RAND_MAX) < 0.7) {
                c1 = crossover_blx(p1, p2, 0.5, n);
                c2 = crossover_blx(p1, p2, 0.5, n);
            } else {
                c1 = copyIndividual(p1, n);
                c2 = copyIndividual(p2, n);
            }

            free(p1);
            free(p2);

            mutacao_gaussiana(c1, 0.05, 0.2, n);
            mutacao_gaussiana(c2, 0.05, 0.2, n);

            nextGen[i] = c1;
            nextFitness[i] = normalizedFitness(alpine2(c1, n));

            if (i + 1 < populationSize) {
                nextGen[i + 1] = c2;
                nextFitness[i + 1] = normalizedFitness(alpine2(c2, n));
            } else {
                free(c2);
            }
        }

        for (int i = 0; i < populationSize; i++) {
            free(generation[i]);
        }
        free(generation);
        free(generationFitness);

        generation = nextGen;
        generationFitness = nextFitness;
        generationCount++;
    }

    double* best = findBestInGeneration(generation, populationSize, 0, n);
    printf("Best individual final: ");
    for (int i = 0; i < n; i++) printf("%f ", best[i]);
    printf("- Valor: %f - Fitness: %f\n", alpine2(best, n), normalizedFitness(alpine2(best, n)));
    free(best);

    for (int i = 0; i < populationSize; i++) {
        free(generation[i]);
    }
    free(generation);
    free(generationFitness);

    fclose(fp);
}


int main() {
    srand(time(NULL));

    int n = 2;
    int populationSize = 700;
    int maxGenerations = 500;

    maxalpine2(n, populationSize, maxGenerations);
    return 0;
}