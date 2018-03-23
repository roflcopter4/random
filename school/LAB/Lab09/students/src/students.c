#include "students.h"
#include <bsd/bsd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static struct student_t *parse_list(struct s_array *list);
static void print_students(struct student_t *data, int size);
static void shootup_school(struct student_t *data, int size);

int
main (int argc, char **argv)
{
        program_name = argv[0];
        int optind = decode_switches (argc, argv);
        struct s_array data = parsefile(argc, argv, optind);

        struct student_t *students = parse_list(&data);
        int numstudents = data.num;
        free_s_array(&data);

        puts("===> Unsorted data.\n");
        print_students(students, numstudents);

        puts("\n===> Sorted by ID.\n");
        sort_students(students, numstudents, sID);
        print_students(students, numstudents);

        puts("\n===> Sorted by name.\n");
        sort_students(students, numstudents, sname);
        print_students(students, numstudents);

        puts("\n===> Sorted by grade.\n");
        sort_students(students, numstudents, sgrade);
        print_students(students, numstudents);

        shootup_school(students, numstudents);
        return 0;
}


static struct student_t
*parse_list(struct s_array *list)
{
        int ID, grade;
        char *name;
        char ch;
        char buf[BUFSIZ];

        struct student_t *students = die_malloc(list->num * sizeof(struct student_t));

        for (int st = 0; st < list->num; ++st) {
                int i, x;
                i = x = 0;
                while ((ch = list->arr[st][i++]) != ',')
                        buf[x++] = ch;
                buf[x] = '\0';
                ID = xatoi(buf);
                
                ++i;
                x = 0;
                while ((ch = list->arr[st][i++]) != ',')
                        buf[x++] = ch;

                buf[x] = '\0';
                xasprintf(&name, "%s", buf);

                maxlen = (maxlen < x) ? x : maxlen;

                ++i;
                x = 0;
                while ((ch = list->arr[st][i++]) != '\0')
                        buf[x++] = ch;
                buf[x] = '\0';
                grade = xatoi(buf);

                struct student_t tmp = {name, ID, grade};
                students[st] = tmp;
        }

        return students;
}


static void
print_students(struct student_t *data, int size)
{
        for (int i = 0; i < size; ++i)
                printf("%d, %*s, %d\n", data[i].ID, maxlen, data[i].name, data[i].grade);
        puts("");
}


static void shootup_school(struct student_t *data, int size)
{
        for (int i = 0; i < size; ++i)
                free(data[i].name);
        free(data);
}
