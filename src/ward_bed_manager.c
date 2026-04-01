#include "ward_bed_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Ward* ward_head = NULL;
Patient* patient_head = NULL;
Ward* create_ward(int id, char* name, int total_beds) {
    Ward* new_ward = (Ward*)malloc(sizeof(Ward));
    if (!new_ward) return NULL;
    
    new_ward->ward_id = id;
    strcpy(new_ward->ward_name, name);
    new_ward->total_beds = total_beds;
    new_ward->beds = NULL;
    new_ward->next = NULL;
    
    return new_ward;
}
Bed* create_bed(int bed_number) {
    Bed* new_bed = (Bed*)malloc(sizeof(Bed));
    if (!new_bed) return NULL;
    
    new_bed->bed_number = bed_number;
    new_bed->is_occupied = 0;
    new_bed->patient = NULL;
    new_bed->next = NULL;
    
    return new_bed;
}

Patient* create_patient(int id, char* name, int age, char* diagnosis) {
    Patient* new_patient = (Patient*)malloc(sizeof(Patient));
    if (!new_patient) return NULL;
    
    new_patient->patient_id = id;
    strcpy(new_patient->name, name);
    new_patient->age = age;
    strcpy(new_patient->diagnosis, diagnosis);
    new_patient->next = NULL;
    
    return new_patient;
}
void add_ward(int id, char* name, int total_beds) {
    Ward* new_ward = create_ward(id, name, total_beds);
    if (!new_ward) {
        printf("Error: Failed to create ward\n");
        return;
    }
    
    
    if (ward_head == NULL) {
        ward_head = new_ward;
    } else {
        Ward* temp = ward_head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_ward;
    }
    
    
    for (int i = 1; i <= total_beds; i++) {
        add_bed_to_ward(id, i);
    }
    
    printf("Ward '%s' added successfully with %d beds\n", name, total_beds);
}

