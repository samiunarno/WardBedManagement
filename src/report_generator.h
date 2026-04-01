#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include "../include/structures.h"

void generate_occupancy_report();
void generate_patient_list_report();
void generate_ward_summary_report();
void export_reports_to_csv();
void print_all_reports();

#endif