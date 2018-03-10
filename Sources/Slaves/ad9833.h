//------------------------------------------------------------------------
// NAME         : ad9833.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : Driver class for the AD9833 waveform generator
// -----------------------------------------------------------------------

#ifndef AD9833_H
#define AD9833_H

#include <interface.h>
#include <slave.h>
#include <spirpi.h>
#include <vector>


#define AD9833_FREQUENCY                ((double)25000000.0)
#define AD9833_FREQUENCY_DIV            (double)(2<<27)

#define AD9833_CONTROL_REG              0x0000
#define AD9833_FREQ0_REG                0x4000
#define AD9833_FREQ1_REG                0x8000
#define AD9833_PHASE0_REG               0xC000
#define AD9833_PHASE1_REG               0xE000


#define AD9833_MODE_BIT                 (1 << 1)
#define AD9833_DIV2_BIT                 (1 << 3)
#define AD9833_OPBITEN_BIT              (1 << 5)
#define AD9833_SLEEP12_BIT              (1 << 6)
#define AD9833_SLEEP1_BIT               (1 << 7)
#define AD9833_RESET_BIT                (1 << 8)
#define AD9833_PSELECT_BIT              (1 << 10)
#define AD9833_FSELECT_BIT              (1 << 11)
#define AD9833_HLB_BIT                  (1 << 12)
#define AD9833_B28_BIT                  (1 << 13)

/* Waveform generator output mode */
typedef enum {
 Sinus,
 Triangle,
    Rectangle
}Output_Mode_t;


/* Waveform generator class */
class AD9833 : public Slave
{
public:


    AD9833()=default;

    /* Waveform generator output mode */
    int setFrequency(int const freq);

    /* Write to the control register */
    int setControl(uint16_t mask,bool reset = false);

    /* Output mode */
    int setOutputMode(Output_Mode_t mode);

private:
     /* Write to internal register */
     int writeRegister(uint16_t const reg, uint16_t const value);

};

#endif // AD9833_H
