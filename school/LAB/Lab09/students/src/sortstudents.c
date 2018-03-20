#include "students.h"
#include <bsd/bsd.h>
#include <stdio.h>
#include <stdlib.h>


static int get_id(struct student_t *student);
/*static int compare_name(struct student_t *student);*/
static int get_grade(struct student_t *student);

static void selection_sort(struct student_t *data, int size, int (*compare)(struct student_t *));
static void selection_sort_str(struct student_t *data, int size);

static void ss2(struct student_t *data, int size, int (*compare)(struct student_t *));


void
sort_students(struct student_t *data, int size, enum sort_type type)
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
        }
}


static int
get_id(struct student_t *student)
{
        return student->ID;
}

static int
get_grade(struct student_t *student)
{
        return student->grade;
}


static void
selection_sort(struct student_t *data, int size,
               int (*compare)(struct student_t *))
{
        int smallest = 0;
        for (int i = 0; i < size; ++i) {
                if (compare(&data[i]) < compare(&data[smallest]))
                        smallest = i;
                /*int a = compare(&data[i]);*/
                /*int b = compare(&data[smallest]);*/
                /*printf("%d - %d\n", a, b);*/
        }

        struct student_t tmp = data[0];
        data[0]               = data[smallest];
        data[smallest]        = tmp;

        if (size > 2)
                selection_sort(&data[1], size - 1, compare);
}


static void
selection_sort_str(struct student_t *data, int size)
{
        int smallest = 0;
        for (int i = 0; i < size; ++i)
                if (strcmp(data[i].name, data[smallest].name) < 0)
                        smallest = i;

        struct student_t tmp = data[0];
        data[0]               = data[smallest];
        data[smallest]        = tmp;

        if (size > 2)
                selection_sort_str(&data[1], size - 1);
}


static void
ss2(struct student_t *data, int size,
               int (*compare)(struct student_t *))
{
        for (int index = 0; index < size; ++index) {
                int smallest = index;
                for (int i = index; i < size; ++i) {
                        int a = compare(&data[i]);
                        int b = compare(&data[smallest]);
                        printf("%d - %d\n", a, b);
                        if (compare(&data[i]) < compare(&data[smallest]))
                                smallest = i;
                }

                struct student_t tmp   = data[index];
                data[index]    = data[smallest];
                data[smallest] = tmp;
        }
}
