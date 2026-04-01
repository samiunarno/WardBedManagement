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

void display_all_wards() {
    if (!ward_head) {
        printf("No wards found\n");
        return;
    }
    
    printf("\n=== All Wards ===\n");
    Ward* current = ward_head;
    while (current != NULL) {
        printf("Ward ID: %d, Name: %s, Total Beds: %d\n", 
               current->ward_id, current->ward_name, current->total_beds);
        current = current->next;
    }
}
void display_beds_in_ward(int ward_id) {
    Ward* ward = find_ward_by_id(ward_id);
    if (!ward) {
        printf("Ward not found\n");
        return;
    }
    
    printf("\n=== Beds in Ward %s ===\n", ward->ward_name);
    Bed* current = ward->beds;
    while (current != NULL) {
        printf("Bed %d: %s", current->bed_number, 
               current->is_occupied ? "Occupied" : "Available");
        if (current->is_occupied && current->patient) {
            printf(" - Patient: %s (ID: %d)", 
                   current->patient->name, current->patient->patient_id);
        }
        printf("\n");
        current = current->next;
    }
}

void display_all_patients() {
    if (!patient_head) {
        printf("No patients admitted\n");
        return;
    }
    
    printf("\n=== All Patients ===\n");
    Patient* current = patient_head;
    while (current != NULL) {
        printf("ID: %d, Name: %s, Age: %d, Diagnosis: %s\n",
               current->patient_id, current->name, 
               current->age, current->diagnosis);
        current = current->next;
    }
}
void display_ward_occupancy() {
    if (!ward_head) {
        printf("No wards found\n");
        return;
    }
    
    printf("\n=== Ward Occupancy ===\n");
    Ward* current = ward_head;
    while (current != NULL) {
        int occupied = 0;
        Bed* bed = current->beds;
        while (bed != NULL) {
            if (bed->is_occupied) occupied++;
            bed = bed->next;
        }
        printf("Ward %s: %d/%d beds occupied (%.1f%%)\n",
               current->ward_name, occupied, current->total_beds,
               (float)occupied / current->total_beds * 100);
        current = current->next;
    }
}
int transfer_patient(int patient_id, int from_ward_id, int to_ward_id) {
    Patient* patient = NULL;
    Patient* prev_patient = NULL;
    Patient* current_patient = patient_head;
    while (current_patient != NULL) {
        if (current_patient->patient_id == patient_id) {
            patient = current_patient;
            break;
        }
        prev_patient = current_patient;
        current_patient = current_patient->next;
    }
    
    if (!patient) {
        printf("Error: Patient not found\n");
        return 0;
    }
    Ward* from_ward = find_ward_by_id(from_ward_id);
    if (!from_ward) {
        printf("Error: Source ward not found\n");
        return 0;
    }
    
    Bed* source_bed = from_ward->beds;
    int found = 0;
    while (source_bed != NULL) {
        if (source_bed->patient == patient) {
            source_bed->is_occupied = 0;
            source_bed->patient = NULL;
            found = 1;
            break;
        }
        source_bed = source_bed->next;
    }
    
    if (!found) {
        printf("Error: Patient not found in source ward\n");
        return 0;
    }
    Bed* dest_bed = find_available_bed(to_ward_id);
    if (!dest_bed) {
        printf("Error: No available beds in destination ward\n");
        // Reassign to source bed
        source_bed->is_occupied = 1;
        source_bed->patient = patient;
        return 0;
    }
    dest_bed->is_occupied = 1;
    dest_bed->patient = patient;
    
    printf("Patient %s transferred from Ward %d to Ward %d\n", 
           patient->name, from_ward_id, to_ward_id);
    return 1;
}
int discharge_patient(int patient_id) {
    Patient* current = patient_head;
    Patient* prev = NULL;
    
    while (current != NULL) {
        if (current->patient_id == patient_id) {
            
            if (prev == NULL) {
                patient_head = current->next;
            } else {
                prev->next = current->next;
            }
            Ward* ward = ward_head;
            while (ward != NULL) {
                Bed* bed = ward->beds;
                while (bed != NULL) {
                    if (bed->patient == current) {
                        bed->is_occupied = 0;
                        bed->patient = NULL;
                        printf("Patient %s discharged, Bed %d in Ward %s released\n",
                               current->name, bed->bed_number, ward->ward_name);
                        break;
                    }
                    bed = bed->next;
                }
                ward = ward->next;
            }
            
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    
    printf("Error: Patient not found\n");
    return 0;
}
void release_bed(int ward_id, int bed_number) {
    Ward* ward = find_ward_by_id(ward_id);
    if (!ward) {
        printf("Error: Ward not found\n");
        return;
    }
    
    Bed* current = ward->beds;
    while (current != NULL) {
        if (current->bed_number == bed_number) {
            if (current->is_occupied) {
                current->is_occupied = 0;
                current->patient = NULL;
                printf("Bed %d in Ward %d released\n", bed_number, ward_id);
            } else {
                printf("Bed %d is already available\n", bed_number);
            }
            return;
        }
        current = current->next;
    }
    
    printf("Error: Bed not found\n");
}

