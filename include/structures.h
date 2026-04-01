#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Patient {
    int patient_id;
    char name[100];
    int age;
    char diagnosis[200];
    struct Patient* next;
} Patient;
typedef struct Bed {
    int bed_number;
    int is_occupied;
    Patient* patient;  // NULL if not occupied
    struct Bed* next;
} Bed;

typedef struct Ward {
    int ward_id;
    char ward_name[100];
    int total_beds;
    Bed* beds;  // Linked list of beds
    struct Ward* next;
} Ward;

extern Ward* ward_head;
extern Bed* bed_head;
extern Patient* patient_head;

Ward* create_ward(int id, char* name, int total_beds);
Bed* create_bed(int bed_number);
Patient* create_patient(int id, char* name, int age, char* diagnosis);

#endif // STRUCTURES_H