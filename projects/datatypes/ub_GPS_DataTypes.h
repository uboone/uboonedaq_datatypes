#ifndef _UBOONE_TYPES_GPSDDATA_H
#define _UBOONE_TYPES_GPSDDATA_H 1

//#include "gps/trigBoardClock.h"

/* The gps structure is what is read off the card
 * in the trigger crate and represents the GPS time at the arrival of a PPS.
 */
typedef struct gps {
    uint32_t lower;
    uint32_t upper;
} gps_t;


/**
   This replaces tmub ../gps/symm.h
 **/
typedef struct gps_time 
{
  // 2^32 = 4.E9 . Thus 32 bits allows for both (2013-1970)*3.14e7 seconds and 
  // enough nanoseconds to span a second.
  uint32_t second; // seconds since the epoch. 
  uint32_t micro;  // microseconds since the second. 
  uint32_t nano;  // nanoseconds since the second. 
  gps_time(){};
} gps_time_t;


#endif //_UBOONE_TYPES_GPSDDATA_H



