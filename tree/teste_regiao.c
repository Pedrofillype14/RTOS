#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REGION_LENGTH 50
#define MAX_CITY_LENGTH 100

typedef struct {
    char region[MAX_REGION_LENGTH];
    double total_population;
    int num_municipalities;
} RegionInfo;

// Function to read the CSV file and populate the regions array
void read_csv_file(FILE *csvfile, RegionInfo **regions, int *region_count, int *region_capacity) {
    char line[1024];
    char *csv_columns[7];
    int column_count = 0;

    fgets(line, sizeof(line), csvfile); // Skip the header line

    while (fgets(line, sizeof(line), csvfile)) {
        char *token = strtok(line, ";");
        column_count = 0;
        while (token != NULL) {
            csv_columns[column_count++] = token;
            token = strtok(NULL, ";");
        }

        char *region = csv_columns[5];
        double population = atof(csv_columns[6]);

        int region_index = -1;
        for (int i = 0; i < *region_count; i++) {
            if (strcmp((*regions)[i].region, region) == 0) {
                region_index = i;
                break;
            }
        }
        if (region_index == -1) {
            if (*region_count >= *region_capacity) {
                *region_capacity *= 2;
                *regions = realloc(*regions, sizeof(RegionInfo) * (*region_capacity));
                if (*regions == NULL) {
                    printf("Error reallocating memory.\n");
                    exit(1);
                }
            }
            strncpy((*regions)[*region_count].region, region, MAX_REGION_LENGTH - 1);
            (*regions)[*region_count].region[MAX_REGION_LENGTH - 1] = '\0'; // Ensure null-termination
            (*regions)[*region_count].total_population = 0;
            (*regions)[*region_count].num_municipalities = 0;
            region_index = (*region_count)++;
        }

        (*regions)[region_index].total_population += population;
        (*regions)[region_index].num_municipalities++;
    }
}

// Function to write the output file
void write_output_file(FILE *outputFile, RegionInfo *regions, int region_count) {
    fprintf(outputFile, "Media da populacao:\n");
    for (int i = 0; i < region_count; i++) {
        double avg_population = regions[i].total_population / regions[i].num_municipalities;
        fprintf(outputFile, "%s: %.2f de habitantes\n", regions[i].region, avg_population);
    }
}

int main() {
    FILE *csvfile, *outputFile;
    RegionInfo *regions = NULL;
    int region_count = 0;
    int region_capacity = 100;

    csvfile = fopen("dados_municipios.csv", "r");
    if (csvfile == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    outputFile = fopen("populacao_regiao.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening output file.\n");
        fclose(csvfile);
        return 1;
    }

    regions = malloc(sizeof(RegionInfo) * region_capacity);
    if (regions == NULL) {
        printf("Error allocating memory.\n");
        fclose(csvfile);
        fclose(outputFile);
        return 1;
    }

    read_csv_file(csvfile, &regions, &region_count, &region_capacity);
    write_output_file(outputFile, regions, region_count);

    fclose(csvfile);
    fclose(outputFile);
    free(regions);

    return 0;
}
