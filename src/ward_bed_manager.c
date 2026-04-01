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
void add_bed_to_ward(int ward_id, int bed_number) {
    Ward* ward = find_ward_by_id(ward_id);
    if (!ward) {
        printf("Error: Ward not found\n");
        return;
    }
    
    Bed* new_bed = create_bed(bed_number);
    if (!new_bed) {
        printf("Error: Failed to create bed\n");
        return;
    }
    
    
    if (ward->beds == NULL) {
        ward->beds = new_bed;
    } else {
        Bed* temp = ward->beds;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_bed;
    }
}
Ward* find_ward_by_id(int ward_id) {
    Ward* current = ward_head;
    while (current != NULL) {
        if (current->ward_id == ward_id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
Bed* find_available_bed(int ward_id) {
    Ward* ward = find_ward_by_id(ward_id);
    if (!ward) return NULL;
    
    Bed* current = ward->beds;
    while (current != NULL) {
        if (!current->is_occupied) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
int admit_patient(int patient_id, char* name, int age, char* diagnosis, int ward_id) {
    Patient* temp = patient_head;
    while (temp != NULL) {
        if (temp->patient_id == patient_id) {
            printf("Error: Patient ID already exists\n");
            return 0;
        }
        temp = temp->next;
    }
    Bed* available_bed = find_available_bed(ward_id);
    if (!available_bed) {
        printf("Error: No available beds in ward %d\n", ward_id);
        return 0;
    }
     Patient* new_patient = create_patient(patient_id, name, age, diagnosis);
    if (!new_patient) return 0;
    if (patient_head == NULL) {
        patient_head = new_patient;
    } else {
        Patient* last = patient_head;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new_patient;
    }
    available_bed->is_occupied = 1;
    available_bed->patient = new_patient;
    
    printf("Patient %s admitted to Ward %d, Bed %d\n", name, ward_id, available_bed->bed_number);
    return 1;
}



