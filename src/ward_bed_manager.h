#ifndef WARD_BED_MANAGER_H
#define WARD_BED_MANAGER_H

#include "../include/structures.h"

void add_ward(int id, char* name, int total_beds);
void display_all_wards();
Ward* find_ward_by_id(int ward_id);

void add_bed_to_ward(int ward_id, int bed_number);
void display_beds_in_ward(int ward_id);
Bed* find_available_bed(int ward_id);

int admit_patient(int patient_id, char* name, int age, char* diagnosis, int ward_id);
int transfer_patient(int patient_id, int from_ward_id, int to_ward_id);
int discharge_patient(int patient_id);
void release_bed(int ward_id, int bed_number);

void display_all_patients();
void display_ward_occupancy();

#endif // WARD_BED_MANAGER_H