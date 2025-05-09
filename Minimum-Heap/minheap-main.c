/****************************************************************************
 *
 * minheap-main.c -- Demo per Min-Heap binario
 *
 * Copyright (C) 2021, 2022, 2024 Moreno Marzolla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap.h"

int main( int argc, char *argv[] )
{
    char op;
    int n, key;
    double prio;
    MinHeap *h;
    FILE *filein = stdin;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s inputfile\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-") != 0) {
        filein = fopen(argv[1], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    if (1 != fscanf(filein, "%d", &n)) {
        fprintf(stderr, "Missing size\n");
        return EXIT_FAILURE;
    }
    printf("minheap_create(%d)\n", n);
    h = minheap_create(n);

    while (1 == fscanf(filein, " %c", &op)) {
        switch (op) {
        case '+': /* insert */
            fscanf(filein, "%d %lf", &key, &prio);
            printf("minheap_insert(h, %d, %f)\n", key, prio);
            minheap_insert(h, key, prio);
            break;
        case '-': /* delete min */
            key = minheap_delete_min(h);
            printf("minheap_delete_min(h) = %d\n", key);
            break;
        case '?': /* get min */
            key = minheap_min(h);
            printf("minheap_min(h) = %d\n", key);
            break;
        case 'c': /* change prio */
            fscanf(filein, "%d %lf", &key, &prio);
            printf("minheap_change_prio(h, %d, %f)\n", key, prio);
            minheap_change_prio(h, key, prio);
            break;
        case 's': /* get n of elements */
            printf("minheap_get_n(h) = %d\n", minheap_get_n(h));
            break;
        case 'p': /* print */
            minheap_print(h);
            break;
        default:
            printf("Unknown command %c\n", op);
            return EXIT_FAILURE;
        }
    }

    minheap_destroy(h);
    if (filein != stdin) fclose(filein);

    return EXIT_SUCCESS;
}
