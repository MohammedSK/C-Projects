#include <iostream>
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


/************************************************************************
 *
 * FUNCTION: ull_time_t bschmktime(date_time *timeptr)
 *
 * DESCRIPTION: Function to convert date and time values to epoch value
 *
 * PARAMETER: date_time *timeptr
 *
 * RETURNVALUE: ull_time_t
 *
*************************************************************************/
date_time *bschgmtime(const ull_time_t *timer)
{
  static date_time s_date_time={0};
  date_time *tp=&s_date_time;

  int64_t  seconds;
  int32_t minutes,hours, days, year, month;
  uint32_t dayOfWeek,yday,daysInYear,daysInMonth,dim; 
  bool leapYear;

  if(NULL !=timer)
  {
    seconds = *timer;

    /* calculate minutes */
    minutes  = seconds / 60;
    seconds -= minutes * 60;
    /* calculate hours */
    hours    = minutes / 60;
    minutes -= hours   * 60;
    /* calculate days */
    days     = hours   / 24;
    hours   -= days    * 24;

    dayOfWeek = 4;
    
    
      for (year=1970;;++year)
      {
        leapYear=LEAPYEAR(year);
        daysInYear=leapYear ? 366 : 365;
        if(days>=daysInYear)
        {
        dayOfWeek += leapYear ? 2 : 1;
        days-= daysInYear;
        if (dayOfWeek >= 7)
          dayOfWeek -= 7;
        }
        else
        {
          break;
        }
      }
        yday = days;
        dayOfWeek  += days;
        dayOfWeek  %= 7;
        for(month = 0; month < 12; ++month)
        {
          daysInMonth=0;
          if (month == 1 && leapYear)
             daysInMonth=1;

            dim =_ytab[daysInMonth][month];

          if (days >= dim)
            days -= dim;
          else
            break;
        }
  }
  if(NULL !=tp)
  {
    tp->tm_sec = seconds;
    tp->tm_min = minutes;
    tp->tm_hour = hours;
    tp->tm_mday=days+1; //1-31
    tp->tm_mon=month;
    tp->tm_year=year-1900;
    tp->tm_isdst=0;
    tp->tm_wday=dayOfWeek;
    tp->tm_yday=yday;
  }

  return tp;
}


ull_time_t bschmktime(date_time *timeptr)
{
  ull_time_t epoch=1970; // epoch year
  ull_time_t secs = 0;
  int countleap = 0;
  int i;
  int dayspermonth;

  if(NULL!=timeptr)
  {
    int year=timeptr->tm_year-70;
    int mon=timeptr->tm_mon;

    /**total secs after 1970**/
    secs = year * (SECSPERDAY * 365);
    for (i = 0; i < year; i++)
    {
      if (LEAPYEAR((epoch + i)))
        countleap++;
    }
    /**total secs including leap years= total secs +
    ** total number of leapdays*secs **/
    secs += (countleap * SECSPERDAY);

    /**total secs including leap years= total secs +
    ** current days secs/min/hour/ previous day **/
    secs += timeptr->tm_sec;
    secs += (timeptr->tm_hour * SECSPERHOUR);
    secs += (timeptr->tm_min * SECSPERMIN);
    secs += ((timeptr->tm_mday-1) * SECSPERDAY);


    /** only if Month is gretaer than January
    ** - calculaye elapsed months secs value
    ** **/
    if (mon > 0)
    {
      dayspermonth = 0;

      if (LEAPYEAR((epoch + year))) // Only counts when we're on leap day or past it
      {
        if (mon > 1)
        {
          dayspermonth = 1;
        } else if (mon == 1 && timeptr->tm_mday >= 29) {
          dayspermonth = 1;
        }
      }

      for (i = 0; i < mon; i++)
      {
        secs += (_ytab[dayspermonth][i] * SECSPERDAY); // total secs of month elapsed
      }
    }
    /*resetting the dst always to 0 as per
    ** mirko req
    tm_yday,tm_wday the orginal values will be ignored and appropriate 
    value are calculated and filled in.
    */
    const ull_time_t  tempsecs=secs;
    date_time *temp=bschgmtime(&tempsecs);
    if(NULL!=temp)
    {
      timeptr->tm_yday=temp->tm_yday;
      timeptr->tm_wday=temp->tm_wday;
      timeptr->tm_isdst=0;
    }
  }   
  return secs;
}




int main()
{
  ull_time_t epochvalue=100223999;
  date_time *time=bschgmtime(&epochvalue);
  cout<<"Year"<<(time->tm_year+1900)<<endl;
  cout<<"Month"<<time->tm_mon+1<<endl; 
  cout<<"Day"<<(time->tm_mday)<<endl;
  cout<<"Year Day"<<time->tm_yday+1<<endl;
  cout<<"Week Day"<<time->tm_wday<<endl;
  cout<<"hour"<<time->tm_hour<<endl;
  cout<<"min"<<time->tm_min<<endl;
  cout<<"secs"<<time->tm_sec<<endl;
  ull_time_t epochresult=bschmktime(time);
  cout<<"epochresult"<<epochresult<<endl;
  return 0;
}
