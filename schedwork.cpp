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
bool search(int currentDay, size_t daily, size_t maxShift, size_t worker, std::vector<size_t>& currentShift, DailySchedule& sched, const AvailabilityMatrix& avail);
bool allComplete(DailySchedule schedule, const size_t dailyNeed);
bool isValid(DailySchedule schedule, size_t worker, size_t currentDay, size_t maxShift, const AvailabilityMatrix avail, std::vector<size_t> workerShift);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    std::vector<size_t> workerShift(avail[0].size()); //make a new vector to keep track of worker shifts 
    for (size_t i = 0; i < avail[0].size(); i++){ //set them all to 0 first 
      workerShift[i] = 0;
    }
    sched.resize(avail.size()); //resize schedule to fit avail 
    if (!(search(0, dailyNeed, maxShifts, 0, workerShift, sched,avail))){ //return false if no avaiable schedule 
        return false; 
    }
    return true; 
}

bool allComplete(DailySchedule schedule, const size_t dailyNeed){ //check if the shedule is complite 
    //std::cout << "allComplete checked" << std::endl;
    if (schedule.empty()==true){ //return false if empty 
        return false; 
    }
    for (DailySchedule::iterator day = schedule.begin(); day!=schedule.end(); ++day){ //go through schedule and check if day's shifts align with need 
        std::vector<Worker_T> currentDay = *day; 
        if(currentDay.size()!=dailyNeed){ //return fasle if not align
            return false; 
        }
    }
    return true; 
}

bool isValid(DailySchedule schedule, size_t worker, size_t currentDay, size_t maxShift,const AvailabilityMatrix avail, std::vector<size_t> workerShift)
{
    //std::cout << "Isvalid checked" << std::endl;
    if (avail[currentDay][worker]== 1 && workerShift[worker]<maxShift){ //checks if the worker is avaiable & if their  shift is less than max (constraints)
        return true; 
    }
    return false; 
}

bool search(int currentDay, size_t daily, size_t maxShift, size_t worker, std::vector<size_t>& currentShift, DailySchedule& sched, const AvailabilityMatrix& avail)
{
    if (allComplete(sched, daily)){ //base case and return true if all complete 
        return true; 
    }
    if (sched[currentDay].size()== daily){ //move on to next day if the day is full 
      return search(currentDay+1, daily, maxShift, 0, currentShift, sched, avail); //start with worker 0
    }

    for (size_t w = worker; w < avail[currentDay].size(); ++w){ //iterate through the workers 
      if (isValid(sched, w, currentDay, maxShift,avail, currentShift)){ 
        sched[currentDay].push_back(w); //if the worker is valid to work then push it into schedule 
        currentShift[w] +=1;  //update the shift vector to keep track of worker 
        bool status = search(currentDay, daily, maxShift, worker + 1, currentShift, sched, avail); //checks if still valid with next worker 
        if (status){ 
          return true; 
        }
        sched[currentDay].pop_back(); //backtrack and undo if not working
        currentShift[w] -= 1; //also subtract the worker from the shift
      }
    }
    return false; 
}

