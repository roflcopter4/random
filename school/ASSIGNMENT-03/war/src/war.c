#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
#include <bsd/bsd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "war.h"

#define FILENAME "/home/bml/random/school/ASSIGNMENT-03/shuffledDeck.txt"
#define NUMCARDS 3
#define MAXBUF ((D_SIZE * (C_SIZE + 2)) + 1)
#define xasprintf(BUF, FMT, ...)                                          \
        do {                                                              \
                if (asprintf((BUF), (FMT), __VA_ARGS__) < 0) {            \
                        eprintf("Error allocating memory for asprintf."); \
                        exit(2);                                          \
                }                                                         \
        } while (0)

static bool war(struct queue_t *hand, struct table_t *table, int numcards);
static int compare_cards(char *a, char *b);
static void clear_table(struct queue_t *hand, struct table_t *table);
static void distribute(struct s_array *data, struct queue_t *hands);
static void play(int numcards, struct queue_t *hands);
static void print_update(struct queue_t *hands, struct table_t *table, char faceup[2][C_SIZE + 1]);
static struct s_array parse_file(int argc, char **argv, int optind);
static struct table_t init_table();

static inline int card2int(char *card);
static inline void place_on_table(struct table_t *table, char *card, bool updown);


int
main(int argc, char **argv)
{
        program_name = argv[0];
        int numcards = NUMCARDS;
        int optind = decode_switches(argc, argv, &numcards);
        struct s_array data = parse_file(argc, argv, optind);
        struct queue_t hands[2];

        validate_data(&data);
        distribute(&data, hands);
        play(numcards, hands);

        free_s_array(&data);
        destroy_queue(&hands[0]);
        destroy_queue(&hands[1]);
}


static struct s_array
parse_file(int argc, char **argv, int optind)
{
        char filename[BUFSIZ];
        if (argc > optind)
                strlcpy(filename, argv[optind], BUFSIZ);
        else
                strlcpy(filename, FILENAME, BUFSIZ);

        int numlines     = countlines(filename);
        int *numarr      = die_malloc(numlines * sizeof(int));
        char **str_array = die_malloc(numlines * sizeof(char *));

        /* We already opened this once, so this SHOULD work... */
        FILE *fp = fopen(filename, "r");
        for (int i = 0; i < numlines; ++i) {
                str_array[i] = shitty_fgetline(fp);
                numarr[i]    = strlen(str_array[i]);
        }
        fclose(fp);

        struct s_array data = {str_array, numarr, numlines};
        return data;
}


static void
distribute(struct s_array *data, struct queue_t *hands)
{
        hands[0] = init_queue(D_SIZE);
        hands[1] = init_queue(D_SIZE);
        shuffle(data->arr, data->num);

        for (int i = 0; i < data->num; i += 2) {
                enqueue(&hands[0], data->arr[i]);
                enqueue(&hands[1], data->arr[i + 1]);
        }
}


static struct table_t
init_table()
{
        struct table_t table;
        table.count = 0;

        for (int i = 0; i < D_SIZE; ++i) {
                table.face_up[i] = false;
                table.cards[i][0] = '\0';
        }

        return table;
}


static inline void
place_on_table(struct table_t *table, char *card, bool updown)
{
        memcpy(table->cards[table->count], card, C_SIZE + 1);
        table->face_up[table->count++] = updown;
}


static void
play(int numcards, struct queue_t *hands)
{
        struct table_t table = init_table();
        char faceup[2][3];
        int winner;

        while (true) {
                for (int i = 0; i < 2; ++i) {
                        dequeue(&hands[i], faceup[i]);
                        place_on_table(&table, faceup[i], true);
                }

                print_update(hands, &table, faceup);
                int comp = compare_cards(faceup[0], faceup[1]);

                if (comp == 0) {
                        puts("Cards are equal - it's war!");
                        for (int i = 0; i < 2; ++i) {
                                if (!war(&hands[i], &table, numcards)) {
                                        winner = modulo(i + 1, 2);
                                        goto gameover;
                                }
                        }
                } else {
                        int round_winner = (comp == 1) ? 0 : 1;
                        printf("Player %d wins this round.\n",
                               round_winner + 1);
                        clear_table(&hands[round_winner], &table);
                }

                for (int i = 0; i < 2; ++i) {
                        if (hands[i].count == 0) {
                                winner = modulo(i + 1, 2);
                                goto gameover;
                        }
                }
                printf("\nPress any key to continue...\n");
                getchar();
        }

gameover:
        printf("\nGame over! Player %d wins!\n", winner + 1);
}


static void
print_update(struct queue_t *hands,
             struct table_t *table,
             char faceup[2][C_SIZE + 1])
{
        int i, clen = (C_SIZE + 2);
        char buf[MAXBUF];

        system("clear");

        for (i = 0; i < 2; ++i)
                printf("Player %d just placed: %s\n", i + 1, faceup[i]);

        puts("\nAll cards on the table (including above two)");

        for (i = 0; i < table->count; ++i) {
                if (table->face_up[i]) {
                        memcpy(&buf[i * clen], table->cards[i], C_SIZE);
                        buf[(i * clen) + C_SIZE]     = ',';
                        buf[(i * clen) + C_SIZE + 1] = ' ';
                } else {
                        memcpy(&buf[i * clen], "XX, ", clen);
                }
        }
        buf[(i * clen) - 2] = '\0';
        puts(buf);

        for (i = 0; i < 2; ++i)
                printf("Player %d cards remaining: %2d\n", i + 1, hands[i].count);
        puts("");
}


static inline int
card2int(char *card)
{
        switch (card[0]) {
        case 'A':
                return 14;
        case 'K':
                return 13;
        case 'Q':
                return 12;
        case 'J':
                return 11;
        case '0':
                return 10;
        default:
                return atoi(&card[0]);
        }
}


static int
compare_cards(char *a, char *b)
{
        int IA, IB;
        IA = card2int(a);
        IB = card2int(b);

        if (IA > IB)
                return 1;
        else if (IA == IB)
                return 0;
        else
                return -1;
}


static bool
war(struct queue_t *hand, struct table_t *table, int numcards)
{
        char buf[C_SIZE + 1];
        for (int i = 0; i < numcards; ++i) {
                if (hand->count > 0) {
                        dequeue(hand, buf);
                        place_on_table(table, buf, false);
                } else {
                        return false;
                }
        }
        return true;
}

static void
clear_table(struct queue_t *hand, struct table_t *table)
{
        int nstr = table->count;
        char **cpy = die_malloc(nstr * sizeof(char *));
        for (int i = 0; i < nstr; ++i)
                xasprintf(&cpy[i], "%s", table->cards[i]);
        shuffle(cpy, nstr);

        while (table->count)
                enqueue(hand, cpy[--table->count]);

        for (int i = 0; i < nstr; ++i)
                free(cpy[i]);
        free(cpy);
}
