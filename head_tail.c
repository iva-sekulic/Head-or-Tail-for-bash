/* 
Iva Sekulic
 Summary of Code
 Usage: head_tail head|tail [-n number] [-h] [-V] [-e] [-o] [FILE]
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#define HEAD "head"
#define TAIL "tail"

// this is an element of a linked list, containing a pointer to the next element in the linked list and a pointer to a line/buffer
// we are not putting a string in the linked list, because we don't know how long it is, that's why we are using a pointer
// this is just like recursion
struct line_buff {
    struct line_buff * next; // that is a pointer to the next line
    char * line; // pointer to buff
};

int call_tail(int lines, int line_count, struct line_buff * first_line, int even, int odd);
int call_head(int lines, int line_count, char * buff, int even, int odd);

int main(int argc, char *argv[]) // argv is the address of each string; argc no. of parameters
{
    int version = 0;
    int help = 0;
    int even = 0, odd = 0;
    int head = 0, tail = 0;
    char *cvalue = NULL;
    int index;
    int c;
    extern char *optarg;
    extern int optind, opterr, optopt;
    int lines = 10; // default
    opterr = 0;

    // error check  - if the user inputs both head and tail into the command line
    if (argc == 3 || argc > 3) {
        if ((strcmp(argv[1], HEAD) == 0) && (strcmp(argv[2], TAIL) == 0)) {
            printf("Please specify whether you want to execute 'head' or 'tail' as your first argument\n");
            exit(EXIT_FAILURE);
        } else if ((strcmp(argv[2], HEAD) == 0) && (strcmp(argv[1], TAIL) == 0)) {
            printf("Please specify whether you want to execute 'head' or 'tail' as your first argument\n");
            exit(EXIT_FAILURE);
        }
    }
    // if the user only inputs the executable into the command line it will automatically default to stdin head first ten lines
    if (argc == 1) {
        printf("You did not specify whether you want head or tail. The default is head.\n\n");
        head = 1;
    }

    // if you have more than one argument you will have to see if it is head or tail or neither (defaults to head)
    if (argc > 1) {
        if (strcmp(argv[1], HEAD) == 0) { // first argument is head
            head = 1;

            // this 3 lines of code will make sure that head or tail are removed from the argc array otherwise they will get printed as unrecognisable arguments
            argc--;
            for (int i = 1; i<argc; i++) {
                argv[i] = argv[i+1];
            }

        } else if (strcmp(argv[1], TAIL) == 0) { // first argument is tail
            tail = 1;

            argc--;
            for (int i = 1; i<argc; i++) {
                argv[i] = argv[i+1];
            }

        }
        else {
            // if the user didn't specify whether they want head or tail it will default to head
            printf("You did not specify whether you want head or tail. The default is head.\n\n");
            head = 1;
        }

        // this is a switch statement used for all the optional arguments
    while ((c = getopt (argc, argv, "n:Vheo")) != -1)
        switch (c)
        {
            case 'V':
                printf("\nName: Iva Sekulic\nEmail: iva.sekulic@ucdconnect.ie\nStudent Number: 21309706\n");
                version = 1;
                break;
            case 'h':
                printf("\n\n*---------------Help Page----------------*\n\n");
                printf("usage: ./a.out { head | tail } [OPTION][ FILE ]\n");
                printf("OPTIONS LIST: \n -h displays this menu\n -V output version info: Your name, email, student number.\n -n K output the first|last K lines\n -e|-o print even|odd lines\n\n");
                help = 1;
                break;
            case 'e':
                even = 1;
                lines *= 2;
                break;
            case 'o':
                odd = 1;
                lines *= 2;
                break;
            case 'n':
                lines = atoi(optarg);
                break;
            case '?':
                if (optopt == 'n') //error checks
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
        }

    }

        for (index = optind + 1; index < argc; index++) // the only other option is file name
            printf("Non-option argument: %s\n", argv[index]);


    FILE * fp;
    char * buff = NULL; // if buff is null it mallocs it for you
    long unsigned len = 0;
    ssize_t read; //can be negative, used for error indication
    int line_count = 0;
    struct line_buff * first_line = NULL; // initially the list is empty and the head of the linked list points nowhere
    struct line_buff * previous_line;

    if (optind == argc || argc == 1) { //means there is nothing left
          printf("No file, please use hihihmestandard input:\n");
          fp = stdin; // this will open the standard input
    } else {
        fp = fopen(argv[argc-1], "r"); // opens the file to read
    }

    if (fp == NULL) { // error check to see if the file isn't able to open
        printf("Was not able to open the file.\n");
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&buff, &len, fp)) != -1) { // this will read from stdin
        struct line_buff * current_line = malloc(sizeof(struct line_buff)); // allocated room for buff
        current_line -> next = NULL; // we do not know what the next line is
        current_line -> line = buff; // created an element of a linked list, but it is not linked to anything yet

        if (first_line == NULL) { // first time as we set to NULL
            first_line = current_line;
        } else {
            previous_line -> next = current_line;
        }
        previous_line = current_line;

        if(head == 1) {
            call_head(lines,line_count,buff, even, odd);
        }

        line_count++;
        buff = NULL;
    }
    fclose(fp);

    if (tail == 1) {
        if (lines > line_count)
            lines = line_count;

        call_tail(lines, line_count, first_line, even, odd);

    }
    printf("\n");
    exit(EXIT_SUCCESS);
}

// this is a function for tail
int call_tail (int lines, int line_count, struct line_buff * first_line, int even, int odd) {
    for (int k = 0; k < line_count - lines; k++) { // we are getting rid of these lines
        first_line = first_line -> next;
    }
    int count = 0;
    for (int k = line_count - lines; k < line_count; k++) {
        if (!even && !odd) {
            printf("%s", first_line -> line);
        } else if (even && count % 2 == 0) {
            printf("%s", first_line -> line);
        } else if (odd && count % 2 != 0) {
            printf("%s", first_line -> line);
        }
        count++;
        first_line = first_line -> next;
    }
}

// this is a function for head
int call_head (int lines, int line_count, char * buff, int even, int odd){
    if (!even && !odd)
        printf("%s", buff);
    else if (even && line_count % 2 == 0)
        printf("%s", buff);
    else if (odd && line_count % 2 != 0)
        printf("%s", buff);
    if (line_count == (lines-1)) {
        exit(EXIT_SUCCESS);
    }
}
