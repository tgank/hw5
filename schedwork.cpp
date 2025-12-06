#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool valid(const AvailabilityMatrix& avail, const DailySchedule& sched,
            const vector<int>& worked, const size_t day,
            const size_t maxShifts, const size_t worker);
bool fillSched(const AvailabilityMatrix& avail, size_t dailyNeed,
                size_t day, size_t shift, DailySchedule& sched, 
                vector<int>& worked, const size_t maxShifts);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{   
    //no days --> no sched needed
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    size_t workerCnt = avail[0].size();
    size_t dayCnt = avail.size();

    //track how many shfits each worker has been assigned
    vector<int> shiftsWorked(workerCnt, 0);

    //prepare output struct (1 empty vect/day)
    sched = DailySchedule(dayCnt, vector<Worker_T>());

    //start recursive backtrack 
    return fillSched(avail, dailyNeed, 0, 0, sched, shiftsWorked, maxShifts);



}

bool fillSched(const AvailabilityMatrix& avail, size_t dailyNeed,
                size_t day, size_t shift, DailySchedule& sched, 
                vector<int>& worked, const size_t maxShifts)
{
    size_t nDays = avail.size();
    size_t workerCnt = avail[0].size();

    //base case: all days processed
    if(day == nDays) return 1;

    //filled all shifts for day? --> move to next day
    if(sched[day].size() == dailyNeed){
        return fillSched(avail, dailyNeed, day +1, 0, sched, worked, maxShifts);
    }

    //try all workers for next shift for current day
    for(size_t worker = 0; worker < workerCnt; worker++){
        if(valid(avail, sched, worked, day, maxShifts, worker)){
            //temp assign this wrker
            sched[day].push_back(worker);
            worked[worker] ++;

            //recurse assigning next shift/day
            if(fillSched(avail, dailyNeed, day, shift +1, sched, worked, maxShifts)){
                return 1; //found schedule
            }
            //backtrack (undo)
            worked[worker] --;
            sched[day].pop_back();
            
        }
    }
    //no worker can fill shift
    return 0;
}


bool valid(const AvailabilityMatrix& avail, const DailySchedule& sched,
            const vector<int>& worked, const size_t day,
            const size_t maxShifts, const size_t worker)
{
    //available this day
    if(!avail[day][worker]){
        return 0;
    }

    //not exceeding max shifts
    if((size_t)worked[worker] >= maxShifts){
        return 0;
    }

    //check if not scheduled same day
    if(find(sched[day].begin(), sched[day].end(), worker) != sched[day].end()){
      return 0;
    }

    return 1;
}