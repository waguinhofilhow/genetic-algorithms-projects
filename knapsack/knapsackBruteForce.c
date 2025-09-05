#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int* sum1(int* number, int size){
    
    for(int i = size - 1; i >= 0; i--){
        
        if(number[i] == 0){
            number[i] = 1;
            return number;
        } 
        else if(number[i] == 1){
            if(i == 0){
            return number;
            } else {
            number[i] = 0;
            }
        }
    }
    
    return number;
}

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

void knapsack(int maxItens, int maxWeight, int* weightList, int* valueList){
    
    int* listItens = calloc(maxItens,sizeof(int));
    
    int bestValue = 0;
    int currentValue = 0;
    int* bestItens = calloc(maxItens, sizeof(int));
    
    for(int i = 0; i < pow(2,maxItens); i++){
        
        if(weight(listItens, maxItens, weightList) <= maxWeight){
            currentValue = value(listItens, maxItens, valueList);
            if(currentValue > bestValue){
                bestValue = currentValue;
                memcpy(bestItens, listItens, maxItens * sizeof(int));
            }
        }
        
        listItens = sum1(listItens, maxItens);
    }
    
    printList(bestItens, maxItens);
    printf("\nValue: %d\nWeight: %d", bestValue, weight(bestItens, maxItens, weightList));
    
    free(listItens);
    free(bestItens);
}

int* randomList(int size, int lowestValue, int highestValue){
    
    int* list = calloc(size,sizeof(int));
    
    for(int i = 0; i < size; i++){
        list[i] = lowestValue + rand()%highestValue;
    }
    
    return list;
}

int main() {

srand(time(NULL));

int size = 30;
int maxWeight = 50;
int *weightList, *valueList;

weightList = randomList(size, 1, 20);
valueList = randomList(size, 10, 100);

printf("Weight list: "); printList(weightList, size);
printf("Value list: "); printList(valueList, size);

knapsack(size, maxWeight, weightList, valueList);

free(weightList);
free(valueList);

return 0;
}