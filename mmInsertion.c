#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
//#include "coordReader.c"

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



void minMaxInsertion(double **Distance, int numOfCoords, int *tour) {
    int *visited_path = (int *)calloc(numOfCoords, sizeof(int));
    tour[0] = 0;
    visited_path[0] = 1;

    for (int i = 1; i < numOfCoords; i++) {
        int next_vertex = -1;
        double min_max_distance = INFINITY;

        for (int j = 0; j < numOfCoords; j++) {
            if (!visited_path[j]) {
                double max_distance = 0;
                for (int k = 0; k < i; k++) {
                    if (Distance[tour[k]][j] > max_distance) {
                        max_distance = Distance[tour[k]][j];
                    }
                }
                if (max_distance < min_max_distance) {
                    min_max_distance = max_distance;
                    next_vertex = j;
                }
            }
        }

        int insert_location = -1;
        double min_insert_cost = INFINITY;

        for (int k = 0; k < i; k++) {
            int next_k = (k + 1) % i;
            double insert_cost = Distance[tour[k]][next_vertex] + Distance[tour[next_k]][next_vertex] - Distance[tour[k]][tour[next_k]];
            if (insert_cost < min_insert_cost) {
                min_insert_cost = insert_cost;
                insert_location = k;
            }
        }

        for (int k = i; k > insert_location + 1; k--) {
            tour[k] = tour[k - 1];
        }
        tour[insert_location + 1] = next_vertex;
        visited_path[next_vertex] = 1;
    }
    free(visited_path);
}




int main(int argument_counter, char *argument_vector[]) {
    if (argument_counter != 3) {
        fprintf(stderr, "Usage: %s <coordinate file> <output file>\n", argument_vector[0]);
        return EXIT_FAILURE;
    }
    
    char *coordFile = argument_vector[1];
    char *outputFile = argument_vector[2];
    
    int numOfCoords = readNumOfCoords(coordFile);
    double **Data_points = readCoords(coordFile, numOfCoords);
    double **Distance = All_coordinate_Distance_Matrix(Data_points, numOfCoords);
    
    int *tour = (int *)malloc(numOfCoords * sizeof(int));
    minMaxInsertion(Distance, numOfCoords, tour);
    
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
