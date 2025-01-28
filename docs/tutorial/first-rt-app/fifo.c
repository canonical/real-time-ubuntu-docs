/* Copyright (C) 2024 Canonical Ltd

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 3 as
   published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Build with gcc fifo.c -o fifo */

#define _GNU_SOURCE

#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#define FIRST_CORE 0

int calls_on_thread1 = 0;
int calls_on_thread2 = 0;

void *thread_start(void *arg)
{
        int thread_num = (intptr_t)arg;

        static int calls_remaining = 100000;
        while (calls_remaining > 0) {
                if (1 == thread_num)
                        calls_on_thread1++;
                else
                        calls_on_thread2++;

                                                 /* Simulate doing some work */
                for (int i = 0; i < 20000; i++)
                        ;

                --calls_remaining;
        }

        return (void *)NULL;
}

int main(void)
{
        pthread_t thread1, thread2;                            /* Thread IDs */
        pthread_attr_t attr1, attr2;
        struct sched_param param1, param2;

        /*-- Run on first core only --*/

        cpu_set_t cpu_set;
        CPU_ZERO(&cpu_set);
        CPU_SET(FIRST_CORE, &cpu_set);
        if (0 != sched_setaffinity(0, sizeof(cpu_set), &cpu_set)) {
                printf("Failed to set CPU affinity\n");
                exit(EXIT_FAILURE);
        }

        /*-- Prepare thread1, assigning RT-class scheduler --*/

        pthread_attr_init(&attr1);               /* Init thread's attributes */
        pthread_attr_setschedpolicy(&attr1, SCHED_FIFO);

                                       /* Save scheduler-specific attributes */
        memset(&param1, 0, sizeof(param1));
        pthread_attr_getschedparam(&attr1, &param1);
        /* param1.sched_priority = MIN(89,
                                       sched_get_priority_max(SCHED_FIFO)); */
        printf("thread1 priority: %d\n", param1.sched_priority);

        /*-- Prepare thread2, assigning RT-class scheduler --*/

        pthread_attr_init(&attr2);               /* Init thread's attributes */
        pthread_attr_setschedpolicy(&attr2, SCHED_FIFO);

                                       /* Save scheduler-specific attributes */
        memset(&param2, 0, sizeof(param2));
        pthread_attr_getschedparam(&attr2, &param2);
        /* param2.sched_priority = sched_get_priority_min(SCHED_FIFO); */
        printf("thread2 priority: %d\n", param2.sched_priority);

        /*-- Call thread function on separate threads --*/

        if (0 != pthread_create(&thread1,
                                &attr1,
                                &thread_start,
                                (void *)(intptr_t)1)) {
                printf("Failed to create thread1\n");
                exit(EXIT_FAILURE);
        }
        pthread_setschedparam(thread1, SCHED_FIFO, &param1);

        if (0 != pthread_create(&thread2,
                                &attr2,
                                &thread_start,
                                (void *)(intptr_t)2)) {
                printf("Failed to create thread2\n");
                exit(EXIT_FAILURE);
        }
        pthread_setschedparam(thread1, SCHED_FIFO, &param2);

                              /* Ensure both threads are allowed to complete */
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        printf("Calls made on thread1: %d\nCalls made on thread2: %d\n",
               calls_on_thread1,
               calls_on_thread2);

        exit(EXIT_SUCCESS);
}
