/* getstats.c 
 *
 * CSC 360, Summer 2023
 *
 * - If run without an argument, dumps information about the PC to STDOUT.
 *
 * - If run with a process number created by the current user, 
 *   dumps information about that process to STDOUT.
 *
 * Please change the following before submission:
 *
 * Author: Matthew Lowe-Ng
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Note: You are permitted, and even encouraged, to add other
 * support functions in order to reduce duplication of code, or
 * to increase the clarity of your solution, or both.
 */


void get_full_info_line(char * file_name, char * line_start) {
    FILE *fptr = fopen(file_name, "r");
    char line[256];
    while (fgets(line, sizeof(line), fptr)) {
        if (strncmp(line_start, line, strlen(line_start)) == 0) {
            printf("%s", line);
            break;
        }
    }
    fclose(fptr);

}

void get_mem_total() {
    char memory[100];
    FILE *fptr = fopen("/proc/meminfo", "r");
    fscanf(fptr, "%[^\n]", memory);
    printf("%s\n", memory);
    fclose(fptr);
}

void get_uptime() {
    int uptime;
    FILE *fptr = fopen("/proc/uptime", "r");

    fscanf(fptr, "%d", &uptime);

    int uptime_days = uptime / (60 * 60 * 24);
    uptime -= uptime_days * (60 * 60 * 24);

    int uptime_hours = uptime / (60 * 60);
    uptime -= uptime_hours * (60 * 60);

    int uptime_minutes = uptime / 60;
    uptime -= uptime_minutes * 60;

    printf("Uptime: %d days, %d hours, %d minutes, %d seconds\n", uptime_days, uptime_hours, uptime_minutes, uptime);
    fclose(fptr);

}

void get_context_switches(char * file_name) {

    unsigned long long vol_switches = 0, nonvol_switches = 0;
    
    char * voluntary = "voluntary_ctxt_switches";
    char * nonvoluntary = "nonvoluntary_ctxt_switches";
    FILE *fptr = fopen(file_name, "r");
    char line[256];
    while (fgets(line, sizeof(line), fptr)) {
        if (strncmp(voluntary, line, strlen(voluntary)) == 0) { // checking if we're at the right line
            sscanf(line, "voluntary_ctxt_switches: %llu", &vol_switches);
        }
        if (strncmp(nonvoluntary, line, strlen(nonvoluntary)) == 0) { // checking if we're at the right line
            sscanf(line, "nonvoluntary_ctxt_switches: %llu", &nonvol_switches);

        }
    }
    fclose(fptr);
    unsigned long long total_switches = vol_switches + nonvol_switches; // adding both totals together for total context switches
    printf("Total context switches: %llu\n", total_switches);

}

void print_process_info(char * process_num) {
    
    char file_name[256];
    char buffer[256];
    FILE *fptr;

    sprintf(file_name, "/proc/%s/status", process_num);
    fptr = fopen(file_name, "r");

    if (fptr == NULL) { // checking if the process exists
        printf("Process number %s not found.\n", process_num);

    } else { // process exists, print info
        printf("Process number: %s\n", process_num);
        get_full_info_line(file_name, "Name"); // searching through status file for name
        sprintf(file_name, "/proc/%s/comm", process_num); // looking at comm file
        fptr = fopen(file_name, "r");
        if (fptr == NULL) { // if filename does not exist, do nothing

        } else {
            if (fgets(buffer, sizeof(buffer), fptr) != NULL) {
                printf("Filename: %s", buffer);
            }
        }

        sprintf(file_name, "/proc/%s/status", process_num); // changing back to status file
        get_full_info_line(file_name, "Threads"); // print number of threads
        get_context_switches(file_name); // getting context switches
        fclose(fptr);
    }
} 


void print_full_info() {

    get_full_info_line("/proc/cpuinfo", "model name");
    get_full_info_line("/proc/cpuinfo", "cpu cores");
    get_full_info_line("/proc/version", "Linux");
    get_mem_total();
    get_uptime();

}


int main(int argc, char ** argv) {  
    if (argc == 1) {
        print_full_info();
    } else {
        print_process_info(argv[1]);
    }
}
