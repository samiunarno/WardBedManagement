#include "report_generator.h"
#include <stdio.h>
#include <time.h>

extern Ward* ward_head;
extern Patient* patient_head;

void generate_occupancy_report() {
    printf("\n========================================\n");
    printf("        OCCUPANCY REPORT\n");
    printf("========================================\n");
    printf("Generated: %s", ctime(&(time_t){time(NULL)}));
    printf("----------------------------------------\n");
    
    if (!ward_head) {
        printf("No wards available\n");
        return;
    }
    
    int total_occupied = 0;
    int total_beds = 0;
    
    Ward* ward = ward_head;
    while (ward != NULL) {
        int occupied = 0;
        Bed* bed = ward->beds;
        while (bed != NULL) {
            if (bed->is_occupied) occupied++;
            bed = bed->next;
        }
        
        total_occupied += occupied;
        total_beds += ward->total_beds;
        
        printf("Ward: %s\n", ward->ward_name);
        printf("  Occupied Beds: %d/%d\n", occupied, ward->total_beds);
        printf("  Occupancy Rate: %.1f%%\n\n", 
               (float)occupied / ward->total_beds * 100);
        
        ward = ward->next;
    }
    
    printf("----------------------------------------\n");
    printf("TOTAL OCCUPANCY: %d/%d (%.1f%%)\n", 
           total_occupied, total_beds, 
           (float)total_occupied / total_beds * 100);
    printf("========================================\n");
}
void generate_patient_list_report() {
    printf("\n========================================\n");
    printf("        PATIENT LIST REPORT\n");
    printf("========================================\n");
    printf("Generated: %s", ctime(&(time_t){time(NULL)}));
    printf("----------------------------------------\n");
    
    if (!patient_head) {
        printf("No patients admitted\n");
        return;
    }
    
    printf("%-10s %-20s %-10s %-20s %-15s\n", 
           "Patient ID", "Name", "Age", "Diagnosis", "Location");
    printf("----------------------------------------------------------------\n");
    
    Patient* patient = patient_head;
    while (patient != NULL) {
        
        char location[50] = "Not assigned";
        Ward* ward = ward_head;
        while (ward != NULL) {
            Bed* bed = ward->beds;
            while (bed != NULL) {
                if (bed->patient == patient) {
                    sprintf(location, "Ward %s, Bed %d", 
                            ward->ward_name, bed->bed_number);
                    break;
                }
                bed = bed->next;
            }
            ward = ward->next;
        }
        
        printf("%-10d %-20s %-10d %-20s %-15s\n",
               patient->patient_id, patient->name, patient->age,
               patient->diagnosis, location);
        
        patient = patient->next;
    }
    printf("========================================\n");
}
void generate_ward_summary_report() {
    printf("\n========================================\n");
    printf("        WARD SUMMARY REPORT\n");
    printf("========================================\n");
    printf("Generated: %s", ctime(&(time_t){time(NULL)}));
    printf("----------------------------------------\n");
    
    if (!ward_head) {
        printf("No wards available\n");
        return;
    }
    
    Ward* ward = ward_head;
    while (ward != NULL) {
        printf("\nWARD: %s (ID: %d)\n", ward->ward_name, ward->ward_id);
        printf("Total Beds: %d\n", ward->total_beds);
        
        int occupied = 0;
        Bed* bed = ward->beds;
        while (bed != NULL) {
            if (bed->is_occupied) occupied++;
            bed = bed->next;
        }
        
        printf("Occupied: %d\n", occupied);
        printf("Available: %d\n", ward->total_beds - occupied);
        printf("Occupancy Rate: %.1f%%\n", 
               (float)occupied / ward->total_beds * 100);
        if (occupied > 0) {
            printf("\nOccupied Beds:\n");
            bed = ward->beds;
            while (bed != NULL) {
                if (bed->is_occupied && bed->patient) {
                    printf("  Bed %d: %s (ID: %d)\n", 
                           bed->bed_number, bed->patient->name, 
                           bed->patient->patient_id);
                }
                bed = bed->next;
            }
        }
        
        printf("----------------------------------------\n");
        ward = ward->next;
    }
    printf("========================================\n");
}
void export_reports_to_csv() {
    FILE* occupancy_file = fopen("output/reports/occupancy_report.csv", "w");
    FILE* patients_file = fopen("output/reports/patients_report.csv", "w");
    
    if (!occupancy_file || !patients_file) {
        printf("Error: Could not create report files\n");
        if (occupancy_file) fclose(occupancy_file);
        if (patients_file) fclose(patients_file);
        return;
    }
    fprintf(occupancy_file, "Ward Name,Total Beds,Occupied Beds,Occupancy Rate\n");
    Ward* ward = ward_head;
    while (ward != NULL) {
        int occupied = 0;
        Bed* bed = ward->beds;
        while (bed != NULL) {
            if (bed->is_occupied) occupied++;
            bed = bed->next;
        }
        fprintf(occupancy_file, "%s,%d,%d,%.1f%%\n",
                ward->ward_name, ward->total_beds, occupied,
                (float)occupied / ward->total_beds * 100);
        ward = ward->next;
    }
    fprintf(patients_file, "Patient ID,Name,Age,Diagnosis,Ward,Bed\n");
    Patient* patient = patient_head;
    while (patient != NULL) {
        char ward_name[100] = "None";
        int bed_number = 0;
        
        Ward* w = ward_head;
        while (w != NULL) {
            Bed* b = w->beds;
            while (b != NULL) {
                if (b->patient == patient) {
                    strcpy(ward_name, w->ward_name);
                    bed_number = b->bed_number;
                    break;
                }
                b = b->next;
            }
            w = w->next;
        }
        
        fprintf(patients_file, "%d,%s,%d,%s,%s,%d\n",
                patient->patient_id, patient->name, patient->age,
                patient->diagnosis, ward_name, bed_number);
        patient = patient->next;
    }
    
    fclose(occupancy_file);
    fclose(patients_file);
    printf("Reports exported to CSV files in output/reports/\n");
}
void print_all_reports() {
    generate_occupancy_report();
    generate_patient_list_report();
    generate_ward_summary_report();
}


