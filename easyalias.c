#include "easyalias.h" 

/* SETUP */

void initialize(char *filename) {
    FILE *f = get_data_file(filename, "w");

    fprintf(f, "# EASYALIAS FUNCTIONS\n\n");

    fprintf(f, "enterfunction() {\n");
    fprintf(f, "\tset -f; cd `easyalias -e \"$1\"`; set +f\n");
    fprintf(f, "}\n\n");

    fprintf(f, "learnfunction() {\n");
    fprintf(f, "\tset -f; easyalias -l \"$1\"; set +f\n");
    fprintf(f, "}\n\n");

    fprintf(f, "forgetfunction() {\n");
    fprintf(f, "\tset -f; easyalias -f \"$1\"; set +f\n");
    fprintf(f, "}\n\n");

    fprintf(f, "showfunction() {\n");
    fprintf(f, "\tif [ $# -ge 1 ]\n");
    fprintf(f, "\tthen\n");
    fprintf(f, "\t\tset -f; easyalias -s \"$1\"; set +f\n");
    fprintf(f, "\telse\n");
    fprintf(f, "\t\teasyalias -s\n");
    fprintf(f, "\tfi\n");
    fprintf(f, "}\n\n\n");

    fprintf(f, "# EASYALIAS INTERFACE\n\n");
    fprintf(f, "alias enter=enterfunction\n");
    fprintf(f, "alias goto=enterfunction\n");
    fprintf(f, "alias switch=enterfunction\n\n");
    fprintf(f, "alias e=enterfunction\n");
    fprintf(f, "alias gt=enterfunction\n");
    fprintf(f, "alias sw=enterfunction\n\n");

    fprintf(f, "alias learn=learnfunction\n");
    fprintf(f, "alias l=learnfunction\n\n");

    fprintf(f, "alias forget=forgetfunction\n");
    fprintf(f, "alias f=forgetfunction\n\n");

    fprintf(f, "alias show=showfunction\n");
    fclose(f);
}


/* INTERFACE */

void enter(struct list *L, char *R){
    struct pair *P = find(L, R);
    increase_weight(P);
    if (P) {
        print_value(P);
    }
    else if (*(R+strlen(R)-2) != '*') {
        char Rstar[1000];
        strncpy(Rstar, R, strlen(R));
        Rstar[strlen(R)] = '*';
        Rstar[strlen(R)+1] = '\0';
        enter(L, Rstar);
    }
    else {
        printf(".\n");
        fprintf(stderr, "%s - no matching alias\n", R);
    }
}

void learn(struct list *L, char *K){
    struct pair *P = malloc(sizeof(*P));
    strncpy((*P).key, K, strlen(K)+1);
    (*P).key[strlen(K)] = '\0';
    get_value(P);
    (*P).weight = 1;
    add(L, P);
    print_pair(P, 0);
}

void forget(struct list *L, char *R) {
    struct pair *P;
    while ((P = find(L, R))){
        drop(L, P);
    }
}

void show(struct list *L, char *R) {
    struct list *M = malloc(sizeof(*M));
    (*M).top = NULL;
    (*M).maxlen = -1; 
    filter_list(M, L, R); 
    print_list(M);
    free(M);
}


/* HELPER FUNCTIONS */

void print_usage(char **argv){
    printf("Usage: %s <flag> [<arg>]\n", argv[0]);
    printf("Flags:\n");
    printf("\t-i\t: Initialize EasyAlias.\n");
    printf("\t-e\t: Enter the directory specified by <arg>.\n");
    printf("\t-l\t: Associate cwd with <arg>.\n");
    printf("\t-f\t: Forget all aliases matching <arg>.\n");
    printf("\t-s\t: List all aliases currently known.\n");
    printf("\n\n");
    printf("After running make, place the following in ~/.bashrc or ~/.bash_profile\n");
    printf("\tif [ -f ~/.bash_easyaliases ]; then\n");
    printf("\t\t . ~/.bash_easyaliases\n");
    printf("\tfi\n");
}

void print_value(struct pair *P){
    printf("%s\n", (*P).value);
}

void print_pair(struct pair *P, int padding){
    if (padding){
        int i;
        char *K = malloc(padding+1);
        for (i = 0; i < padding; K[i++] = ' ');
        K[padding] = '\0';
        memcpy(K, (*P).key, strlen((*P).key));
        printf("%s\t:\t%s\n", K, (*P).value);
        free(K);
    } 
    else
        printf("%s\t:\t%s\n", (*P).key, (*P).value);
}

void print_list(struct list *L) {
    struct pair *P;
    for (P = (*L).top; P; print_pair(P, (*L).maxlen), P=(*P).next);
}

void get_value(struct pair *P){
    if (getcwd((*P).value, sizeof((*P).value)) == NULL)
        perror("getcwd() error");
}

void increase_weight(struct pair *P) {
    if (P) (*P).weight += 1;
    if (P) (*P).weight *= 1.05;
}

void decrease_weight(struct pair *P) {
    if (P) (*P).weight *= 0.99;
}

void clean_up(char *S){
    int i;
    for (i = 0; (S[i] != '\n' && S[i] != ' ' && S[i] != '\0'); i++);
    S[i] = '\0';
}

int matches(char *A, char *R){
    if (*A == '\0') return (*R == '\n' || *R == '\0' || (*R == '*' && matches(A, R+1)));
    if (*R != '*') return (*R != *A && *R != '?')? 0: matches(A+1, R+1);
    else return matches(A, R+1) || matches(A+1, R) || matches(A+1, R+1);
}


/* LIST FUNCTIONS */

void add(struct list *L, struct pair *P) {
    struct pair **cur;
    (*L).maxlen = (((*L).maxlen) < (int)strlen((*P).key))? strlen((*P).key) : (*L).maxlen; 
    for (cur = &(*L).top; (*cur) && (strcmp((**cur).key, (*P).key) < 0) ;cur = &((**cur).next));

    if (!*cur || strcmp((**cur).key, (*P).key)) {
        (*P).next = *cur;
        *cur = P;    
    } else {
        (*P).next = (**cur).next;
        free(*cur);
        *cur = P;
    }
}

void drop(struct list *L, struct pair *P){
    struct pair **cur;
    for (cur = &(*L).top; (*cur) && (strcmp((**cur).key, (*P).key) != 0) ;cur = &((**cur).next));
    if (*cur) {
        struct pair *tmp = *cur;
        *cur = (**cur).next;
        free(tmp);
    }

}

void freelist(struct pair *P){
    if (P != NULL) {
        freelist((*P).next);
        free(P);
    }
}

struct pair *find(struct list *L, char *R){ 
    struct pair *cur, *best = NULL;
    double maxweight = -1;
    for (cur = (*L).top; cur; cur=(*cur).next){
        if ((maxweight < (*cur).weight) && matches((*cur).key, R)){
            best = cur;
            maxweight = (*cur).weight; 
        }
    }
    return best;
}

void filter_list(struct list *M, struct list *L, char *R){
    struct pair *P;
    for (P = (*L).top; P; P = (*P).next){
        if (matches((*P).key, R)){
            struct pair *copy = malloc(sizeof(*copy));
            memcpy(copy, P, sizeof(*copy));
            add(M, copy);
        }
    }
}


/* FILE ACCESS */

struct list *read_file(){
    FILE *f;
    char line[1000];
    struct list *L = malloc(sizeof(*L));
    (*L).top = NULL;
    (*L).maxlen = -1;
    struct pair *cur;
    if ((f = get_data_file(".easyalias","r"))){
        int c = 0;
        while(fgets(line, 1000, f) != NULL){
            clean_up(line);
            if (! (c % 3)) {
                cur = malloc(sizeof(*cur));
                strncpy((*cur).key, line, sizeof((*cur).key));
                add(L, cur);
            } else if ((c % 3 == 1)) {
                strncpy((*cur).value, line, sizeof((*cur).value));
            } else {
                (*cur).weight = atof(line);
            }
            c++;
        }
        fclose(f);
    } else {
        perror("fopen error");
    }
    return L;
}

void write_file(struct list *L){
    FILE *f;
    if ((f = get_data_file(".easyalias", "w"))){
        struct pair *P;
        for (P = (*L).top; P; P=(*P).next){
            decrease_weight(P);
            fprintf(f, "%s\n", (*P).key);
            fprintf(f, "%s\n", (*P).value);
            fprintf(f, "%.6f\n", (*P).weight);
        }
        
        fclose(f);
    } else {
        perror("fopen error");
    }
    freelist((*L).top);
    free(L);
}

FILE *get_data_file(const char *filename, const char *mode){
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    char filepath[1000];
    sprintf(filepath, "%s/%s", homedir, filename);
    return fopen(filepath, mode);
}


/* MAIN */

int main(int argc, char **argv){
    if (argc < 2) {
        print_usage(argv);
    } else {
        struct list *L;
        if (!strncmp(argv[1], "-i", 2)){
            L = malloc(sizeof(*L));
            (*L).top = NULL;
            initialize(".bash_easyaliases");
        } else {
            L = read_file();
            if (argc > 2) clean_up(argv[2]);
            if (!strncmp(argv[1], "-l", 2)){
                if (argc > 2 && strlen(argv[2])) learn(L, argv[2]); else print_usage(argv);
            } 
            else if (!strncmp(argv[1], "-e", 2)){
                if (argc > 2 && strlen(argv[2])) enter(L, argv[2]); else print_usage(argv);
            }
            else if (!strncmp(argv[1], "-f", 2)){
                if (argc > 2 && strlen(argv[2])) forget(L, argv[2]); else print_usage(argv);
            }
            else if (!strncmp(argv[1], "-s", 2)){
                if (argc > 2 && strlen(argv[2])) show(L, argv[2]); else show(L, "*");
            }
        }
        write_file(L);
    }
    return 0;
}
