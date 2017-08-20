#include<iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
using namespace std;

typedef struct tm date_time;
typedef unsigned long long ull_time_t;

#define SECSPERDAY (86400UL) /* == ( 24 * 60 * 60) */
#define SECSPERHOUR (3600UL) /* == ( 60 * 60) */
#define SECSPERMIN (60UL) /* == ( 60) */

#define LEAPYEAR(year)          (!((year) % 4) && (((year) % 100) || !((year) % 400)))


const int _ytab[2][12] = {
  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

date_time *bschgmtime(const ull_time_t *timer);
ull_time_t bschmktime(date_time *timeptr);


