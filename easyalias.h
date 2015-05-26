#ifndef EA_H
#define EA_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>

/* STRUCTS */
struct pair {
    char key[256];
    char value[1000];
    double weight;
    struct pair *next;
};

struct list {
    struct pair *top;
    int maxlen;
};


/* SETUP */
void initialize(char *filename);


/* INTERFACE */
void enter(struct list *L, char *R);
void learn(struct list *L, char *K);
void forget(struct list *L, char *R);
void show(struct list *L, char *R);


/* HELPER FUNCTIONS */
void print_usage     (char **argv);
void print_value     (struct pair *P);
void print_pair      (struct pair *P, int padding);
void print_list      (struct list *L);
void get_value       (struct pair *P);
void increase_weight (struct pair *P);
void decrease_weight (struct pair *P);
void clean_up        (char *S);
 int matches         (char *A, char *R);


/* LIST FUNCTIONS */
void          add         (struct list *L, struct pair *P);
void          drop        (struct list *L, struct pair *P);
void          freelist    (struct pair *P);
struct pair * find        (struct list *L, char *R);
void          filter_list (struct list *M, struct list *L, char *R);


/* FILE ACCESS */
struct list * read_file     ();
void          write_file    (struct list *L);
FILE        * get_data_file (const char *filename, const char *mode);

#endif
