// Amount System v2.0
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
#define _print_list_text(_Ptr, i)                                              \
    do {                                                                       \
        printf("%05d   %-16s%-11d%-12d\n", _Ptr[i].id, _Ptr[i].name,           \
               _Ptr[i].income, _Ptr[i].outcome);                               \
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

// function for qsort
int _void_strcmp(const void *s1, const void *s2) {
    return strcmp(((amount *) s2)->name, ((amount *) s1)->name);
}

// main function
void Input_record(void);
void Sort_and_list(void);
void Search_records(void);
void Calculate(void);
void List_more(void);
void List_all_records(void);
void Exit(void);

// Some little function
void _list_beginning(void) {
    printf("ID	UserName	Income	   Expenses\n"
           "-----   --------        ------     --------\n");
}

void _list_ending(void) {
    printf("-----   --------        ------     --------\n");
}

void _list_records(amount *amount_ptr) {
    _list_beginning();
    for (int i = 0; i < num; i++) {
        _print_list_text(amount_ptr, i);
    }
    _list_ending();
}

// the MAIN functions' Point
void (*function[7])() = {
    Exit,      Input_record, Sort_and_list,   Search_records,
    Calculate, List_more,    List_all_records};

// main
int main(void) {
    int            input = 0, _input_count = 0;
    extern amount *amounts;
    do {
        do {
            if (_input_count++) {
                scanf("%*[^\n]%*c");
            }
            printf("1.Input record\n"
                   "2.Sort and list records in reverse order by user name\n"
                   "3.Search records by user name\n"
                   "4.Calculate and list per capita income and expenses\n"
                   "5.List records which have more expenses than per capita "
                   "expenses\n"
                   "6.List all records\n"
                   "0.Exit\n"
                   "  Please enter your choice:");
        } while ((scanf("%d", &input) != 1) || (input >= 7 || input < 0));
        function[input](amounts, num);
    } while (1);

    return 0;
}

////the main fuctions,ordered in 1 2 3 4 5 6 0
void Input_record(void) {
    if (amounts) {
        free(amounts);
    }
    get_infn("Input the number of recorders:", num, num > 0 && num <= 10);
    amounts = (amount *) calloc(num, sizeof(amount));
    for (int i = 0; i < num; i++) {
        get_infn("Input your ID (length less than 5):", amounts[i].id,
                 0 <= amounts[i].id && amounts[i].id < 100000);
        scanf("%*[^\n]");
        getchar();
        printf("Input your name :");
        scanf("%10[^\n]", amounts[i].name);
        get_infn("Input your income :", amounts[i].income,
                 amounts[i].income >= 0);
        get_infn("Input your outcome :", amounts[i].outcome,
                 amounts[i].outcome >= 0);
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
        printf("Please input the datas firse.(choose 1).\n");
}
void Search_records(void) {
    if (amounts) {
        char _input_for_search[MAX_NAME_LENGTH];
        int  i = 0;
        printf("Please input the user name:");
        scanf("%*[^\n]");
        getchar();
        scanf("%10[^\n]", _input_for_search);
        for (i = 0; i < num; i++) {
            if (!strcmp(_input_for_search, amounts[i].name)) {
                break;
            }
        }
        if (i < num) {
            _list_beginning();
            _print_list_text(amounts, i);
            _list_ending();
        } else
            printf("404 NOT FOUND\n");
    } else
        printf("Please input the datas firse.(choose 1).\n");
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
        printf("Please input the datas firse.(choose 1).\n");
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
                _print_list_text(amounts, i);
            }
        }
        _list_ending();
    } else
        printf("Please input the datas firse.(choose 1).\n");
}

void List_all_records(void) {
    if (amounts) {
        _list_records(amounts);
    } else
        printf("Please input the datas firse.(choose 1).\n");
}

void Exit(void) {
    free(amounts);
    exit(0);
}
