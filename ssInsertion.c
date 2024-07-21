#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<limits.h>
#include "coordReader.c"


int readNumOfCoords(char *filename);
double** readCoords(char *filename, int numOfCoords);
double** All_coordinate_Distance_Matrix(double **Data_points, int numOfCoords);
void smallestSumInsertion(double **Distance, int numOfCoords, int *tour);
void *writeTourToFile(int *tour, int tourLength, char *filename);


int main(int argument_count, char *argument_vector[]) {
    if (argument_count != 3) {
        fprintf(stderr, "Usage: %s <coordinate file> <output file>\n", argument_vector[0]);
        return EXIT_FAILURE;
    }
    
    char *coordFile = argument_vector[1];
    char *outputFile = argument_vector[2];
    
    int numOfCoords = readNumOfCoords(coordFile);
    double **Data_points = readCoords(coordFile, numOfCoords);
    double **Distance = All_coordinate_Distance_Matrix(Data_points, numOfCoords);
    
    int *tour = (int *)malloc((numOfCoords + 1) * sizeof(int)); 
    smallestSumInsertion(Distance, numOfCoords, tour);
    
    writeTourToFile(tour, numOfCoords+1, outputFile);
    
    for (int i = 0; i < numOfCoords; i++) {
        free(Data_points[i]);
        free(Distance[i]);
    }
    free(Data_points);
    free(Distance);
    free(tour);
    
    return EXIT_SUCCESS;
}
 

double EuclideanDistance(double *Data_point_1, double *Data_point_2) {
    return sqrt(pow(Data_point_1[0] - Data_point_2[0], 2) + pow(Data_point_1[1] - Data_point_2[1], 2));
}

double** All_coordinate_Distance_Matrix(double **Data_points, int numOfCoords) {
    double **Distance = (double **)malloc(numOfCoords * sizeof(double *));
    for (int i = 0; i < numOfCoords; i++) {
        
        Distance[i] = (double *)malloc(numOfCoords * sizeof(double));
        for (int j = 0; j < numOfCoords; j++) {
            Distance[i][j] = EuclideanDistance(Data_points[i], Data_points[j]);
            
            
        }
    }
    return Distance;
}



void smallestSumInsertion(double **Distance, int numOfCoords, int *tour) {
    int *visited_path = (int *)calloc(numOfCoords, sizeof(int));
    tour[0] = 0;
    visited_path[0] = 1;

    for (int i = 1; i < numOfCoords; i++) {
        int choose_next_vertex = -1;
        double Appropriate_cost = INFINITY;

        for (int j = 0; j < numOfCoords; j++) {
            
            if (visited_path[j] == 0) {
                double cost = 0;
                for (int k = 0; k < i; k++) {
                    cost += Distance[tour[k]][j];
                }
                if (cost < Appropriate_cost) {
                    Appropriate_cost = cost;
                    choose_next_vertex = j;
                }
            }
        }

        int optimal_insert_location = -1;
        double optimal_worth = INFINITY;

        for (int k = 0; k < i; k++) {
            
            int next_position;
            if (k + 1 == i) {
                next_position = 0;
            } else {
                next_position = k + 1;
            }

            double insertCost = Distance[tour[k]][choose_next_vertex] +
                                Distance[choose_next_vertex][tour[next_position]] -
                                Distance[tour[k]][tour[next_position]];
            if (insertCost < optimal_worth) {
                optimal_worth = insertCost;
                optimal_insert_location = k;
            }
        }

        for (int k = i; k > optimal_insert_location + 1; k--) {
            tour[k] = tour[k - 1];
        }
        tour[optimal_insert_location + 1] = choose_next_vertex;
        visited_path[choose_next_vertex] = 1;
    }
    free(visited_path);
}


