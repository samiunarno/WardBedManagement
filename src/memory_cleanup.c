#include "../include/structures.h"
#include <stdio.h>
#include <stdlib.h>

extern Ward* ward_head;
extern Patient* patient_head;

int free_all_patients() {
    int count = 0;
    Patient* current = patient_head;
    while (current != NULL) {
        Patient* temp = current;
        current = current->next;
        free(temp);
        count++;
    }
    patient_head = NULL;
    return count;
}

int free_beds_in_ward(Ward* ward) {
    int count = 0;
    Bed* current = ward->beds;
    while (current != NULL) {
        Bed* temp = current;
        current = current->next;
        free(temp);
        count++;
    }
    ward->beds = NULL;
    return count;
}

int free_all_wards() {
    int ward_count = 0;
    int bed_count = 0;
    
    Ward* current = ward_head;
    while (current != NULL) {
        Ward* temp = current;
        current = current->next;
        bed_count += free_beds_in_ward(temp);
        
        free(temp);
        ward_count++;
    }
    ward_head = NULL;
    
    printf("Memory cleanup: Freed %d wards and %d beds\n", ward_count, bed_count);
    return ward_count + bed_count;
}
int cleanup_all_memory() {
    printf("\n=== Starting Memory Cleanup ===\n");
    
    int patients_freed = free_all_patients();
    int wards_and_beds_freed = free_all_wards();
    
    int total_freed = patients_freed + wards_and_beds_freed;
    
    printf("Total memory freed: %d structures\n", total_freed);
    printf("=== Memory Cleanup Complete ===\n\n");
    
    return total_freed;
}
void log_memory_cleanup() {
    FILE* log_file = fopen("docs/memory_cleanup_log.txt", "a");
    if (!log_file) {
        printf("Error: Could not open log file\n");
        return;
    }

    time_t now;
    time(&now);
    
    fprintf(log_file, "\n=== Memory Cleanup Log ===\n");
    fprintf(log_file, "Timestamp: %s", ctime(&now));
    fprintf(log_file, "Memory cleanup performed successfully\n");
    fprintf(log_file, "All wards, beds, and patient records freed\n");
    fprintf(log_file, "========================\n");
    
    fclose(log_file);
    printf("Memory cleanup logged to docs/memory_cleanup_log.txt\n");
}