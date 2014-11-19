#ifndef _UBOONE_TYPES_TRIGGERDATA_H
#define _UBOONE_TYPES_TRIGGERDATA_H 1

/* Not very well-structured, but the bits are all scrambled so this is actually easiest to manipulate.
 * See the datatypes code for unpacking. --NJT
 */
#define N_ACTIVITY_HIST 4
typedef struct trigger_data {
  uint16_t    word1; /* holds sample number, reminder, busy flag      */
  uint16_t    word2; /* holds frame number (low bits)                 */
  uint16_t    word3; /* hold trigger number, frame number (high bits) */
  uint16_t    word4; /* holds trigger number (high bits)              */
  uint16_t    word5; /* holds trigger bits                            */
  uint16_t    word6; /* bit0 is phase[1], rest is unusued             */
  uint16_t    word7; /* unused, should be 0xffff                      */
  uint16_t    word8; /* unused, should be 0xfff                       */
} trigger_data_t;  /* total 16 bytes */

#endif //_UBOONE_TYPES_TRIGGERDATA_H



