// Amount System v1.0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define some macros and constants
#define MAX_NAME_LENGTH 11
#define get_infn(prompt, input, cont)                                          \
    do {                                                                       \
        do {                                                                   \
            scanf("%*[^\n]%*c");                                               \
            printf(prompt);                                                    \
        } while (scanf("%d", &input) != 1 || !(cont));                         \
    } while (0)
#define _print_list_text(_Ptr, i, address)                                     \
    do {                                                                       \
        fprintf(address, "%05d   %-16s%-11d%-12d\n", _Ptr[i].id, _Ptr[i].name, \
                _Ptr[i].income, _Ptr[i].outcome);                              \
    } while (0)

// some global variables
typedef struct Amount {
    int  id;
    char name[MAX_NAME_LENGTH];
    int  income;
    int  outcome;
} amount;

amount *amounts = NULL;
int     num;
FILE   *fp = NULL;

// function for qsort
int _void_strcmp(const void *s1, const void *s2) {
    return strcmp(((amount *) s1)->name, ((amount *) s2)->name);
}

int _void_int_cmp(const void *x1, const void *x2) {
    return ((amount *) x1)->id - ((amount *) x2)->id;
}

// main function
void Input_record(void);
void Sort_and_list(void);
void Search_records(void);
void Calculate(void);
void List_more(void);
void List_all_records(void);
void Read_all_records(void);
void Write_all_records(void);
void Exit(void);

// Some little function
void _list_beginning(void) { printf("ID	UserName	Income	   Expenses\n"); }

void _list_records(amount *amount_ptr) {
    _list_beginning();
    for (int i = 0; i < num; i++) {
        _print_list_text(amount_ptr, i, stdout);
    }
}

// the MAIN functions' Pointer
void (*function[9])() = {
    Exit,      Input_record,     Sort_and_list,    Search_records,   Calculate,
    List_more, List_all_records, Read_all_records, Write_all_records};

// main
int main(void) {
    int input = 0, _input_count = 0;
    printf("1.Input record\n"
           "2.Sort and list records in reverse order by user name\n"
           "3.Search records by user name\n"
           "4.Calculate and list per capita income and expenses\n"
           "5.List records which have more expenses than per capita "
           "expenses\n"
           "6.List all records\n"
           "7.Write amounts into .txt\n"
           "8.Output the .txt\n"
           "0.Exit\n");
    do {
        do {
            if (_input_count++) {
                scanf("%*[^\n]%*c");
            }
            printf("  Please enter your choice:");
        } while ((scanf("%d", &input) != 1) || (input >= 9 || input < 0));
        function[input]();
    } while (1);

    return 0;
}

////the main fuctions,ordered in 1 2 3 4 5 6 7 8 0
void Input_record(void) {
    if (amounts) {
        free(amounts);
    }
    get_infn("Input the number of recorders:", num, num > 0 && num <= 10);
    amounts = (amount *) calloc(num, sizeof(amount));
    scanf("%*[^\n]%*c");
    for (int i = 0; i < num; i++) {
        do {
            printf("Please input the datas in format:ID NAME INCOME OUTCOME:");
            scanf("%d", &amounts[i].id);
            scanf("%*c%10[^ ]", amounts[i].name);
            if (getchar() != ' ') {
                scanf("%*[^ ]%*c");
            }
            scanf("%d %d", &amounts[i].income, &amounts[i].outcome);
        } while (!(0 <= amounts[i].id && amounts[i].id < 100000) ||
                 !(amounts[i].income >= 0) || !(amounts[i].outcome >= 0));
    }
}

void Sort_and_list(void) {
    if (amounts) {
        amount *ptr = (amount *) calloc(num, sizeof(amount));
        for (int i = 0; i < num; i++) {
            ptr[i] = amounts[i];
        }
        qsort(ptr, num, sizeof(amount), _void_strcmp);
        _list_records((amount *) ptr);
        free(ptr);
    } else
        printf("Please input the datas first.(choose 1).\n");
}
void Search_records(void) {
    /*
     * Direct search
     */
    if (amounts) {
        char _input_for_search[MAX_NAME_LENGTH];
        int  i = 0, j = 0;
        printf("Please input the user name:");
        scanf("%*[^\n]");
        getchar();
        scanf("%10[^\n]", _input_for_search);
        for (i = 0; i < num; i++) {
            if (!strcmp(_input_for_search, amounts[i].name)) {
                j++;
                if (j == 1) _list_beginning();
                _print_list_text(amounts, i, stdout);
            }
        }
        if (!j) printf("NOT FOUND\n");
    } else
        printf("Please input the datas first.(choose 1).\n");
    /*
     * Binary search,but can only output one of all the users who have the same
     * name.
     */
    // if (amounts) {
    //     amount  _input_for_search[1];
    //     amount *item;
    //     printf("Please input the user name:");
    //     scanf("%*[^\n]");
    //     getchar();
    //     scanf("%10[^\n]", _input_for_search[0].name);
    //     item = (amount *) bsearch(_input_for_search, amounts, num,
    //                               sizeof(amount), _void_strcmp);
    //     if (item != NULL) {
    //         _list_beginning();
    //         _print_list_text(item,0,stdout);
    //     } else
    //         printf("NOT FOUND\n");
    // } else
    //     printf("Please input the datas first.(choose 1).\n");
}

void Calculate(void) {
    if (amounts) {
        float _per_capita_income = 0, _per_capita_expenses = 0;
        for (int i = 0; i < num; i++) {
            _per_capita_income += amounts[i].income;
            _per_capita_expenses += amounts[i].outcome;
        }
        printf("Per capita income:	%.2f\n", _per_capita_income / num);
        printf("Per capita expenses:	%.2f\n", _per_capita_expenses / num);
    } else
        printf("Please input the datas first.(choose 1).\n");
}

void List_more(void) {
    if (amounts) {
        float _per_capita_expenses = 0;
        for (int i = 0; i < num; i++) {
            _per_capita_expenses += amounts[i].outcome;
        }
        _per_capita_expenses /= num;
        _list_beginning();
        for (int i = 0; i < num; i++) {
            if (amounts[i].outcome > _per_capita_expenses) {
                _print_list_text(amounts, i, stdout);
            }
        }
    } else
        printf("Please input the datas first.(choose 1).\n");
}

void List_all_records(void) {
    if (amounts) {
        amount *ptr = (amount *) calloc(num, sizeof(amount));
        for (int i = 0; i < num; i++) {
            ptr[i] = amounts[i];
        }
        qsort(ptr, num, sizeof(amount), _void_int_cmp);
        _list_records((amount *) ptr);
        free(ptr);
    } else
        printf("Please input the datas first.(choose 1).\n");
}

void Read_all_records(void) {
    if (amounts) {
        fp = fopen("Amount system.txt", "w");
        fprintf(fp, "ID      UserName        Income     Expenses\n");
        for (int i = 0; i < num; i++) {
            _print_list_text(amounts, i, fp);
        }
        printf("Save Successfully!\n");
        fclose(fp);
    } else
        printf("Please input the datas first.(choose 1).\n");
}

void Write_all_records(void) {
    fp = fopen("Amount system.txt", "r");
    if (!feof(fp)) {
        char _read_file[11];
        while (fgets(_read_file, 10, fp)) {
            fputs(_read_file, stdout);
        }
    } else
        printf("Please write something into the file first.\n");
    fclose(fp);
}

void Exit(void) {
    printf("Exit sucessfully!\n");
    free(amounts);
    exit(0);
}
