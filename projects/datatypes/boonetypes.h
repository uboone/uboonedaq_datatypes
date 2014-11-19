#ifndef BOONETYPES_H
#define BOONETYPES_H

typedef struct pmt_window_header 
{
  uint16_t ch_and_disc; 
  uint16_t frame_and_sample1; //bits 6-8: last 3 readout frame bits; bits 1-5: upper 5 bits of readout sample
  uint16_t sample2; //lower 12 bits of readout sample
} pmt_window_header_t;


#endif /* #ifndef BOONETYPES_H */



