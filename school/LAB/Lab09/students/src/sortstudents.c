#include "students.h"
#include <bsd/bsd.h>
#include <stdlib.h>


static inline int get_id(struct student *myStudent);
static inline int get_grade(struct student *myStudent);
static void selection_sort(struct student *data, int size,
                           int (*compare)(struct student *));
static void selection_sort_str(struct student *data, int size);


void
sort_students(struct student *data, int size, enum sort_type type)
{
        switch (type) {
        case sID:
                selection_sort(data, size, &get_id);
                break;
        case sname:
                selection_sort_str(data, size);
                break;
        case sgrade:
                selection_sort(data, size, &get_grade);
                break;
        default:
                eprintf("Invalid usage of sort_students.\n");
                exit(1);
        }
}


static inline int
get_id(struct student *myStudent)
{
        return myStudent->ID;
}

static inline int
get_grade(struct student *myStudent)
{
        return myStudent->grade;
}


static void
selection_sort(struct student *data, int size,
               int (*compare)(struct student *))
{
        int smallest = 0;
        for (int i = 0; i < size; ++i)
                if (compare(&data[i]) > compare(&data[smallest]))
                        smallest = i;

        struct student tmp = data[0];
        data[0] = data[smallest];
        data[smallest] = tmp;

        if (size > 2)
                selection_sort(&data[1], size - 1, compare);
}


static void
selection_sort_str(struct student *data, int size)
{
        int smallest = 0;
        for (int i = 0; i < size; ++i)
                if (strcmp(data[i].name, data[smallest].name) < 0)
                        smallest = i;

        struct student tmp = data[0];
        data[0] = data[smallest];
        data[smallest] = tmp;

        if (size > 2)
                selection_sort_str(&data[1], size - 1);
}
