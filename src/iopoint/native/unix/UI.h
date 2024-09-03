//////////////////////////////////////////////////////////////////////////
// UI.h
// Runs on all MCUs
// Copyright 2015 (c) Calvin Slater.
//////////////////////////////////////////////////////////////////////////

//
// History:
//   27 Oct 15  Calvin Slater  Creation
//

//////////////////////////////////////////////////////////////////////////
// Overview
//////////////////////////////////////////////////////////////////////////

//
// This Module provides functions that operate on the controller's
// Harware Universal Inputs. The data structure for UIs on all
// models of controller is defined here. Use this header for all
// files that instantiate or access controller Universal Inputs
//

//////////////////////////////////////////////////////////////////////////
// Includes

#include <stdint.h>
//#include "ADCCh1to8.h"

//////////////////////////////////////////////////////////////////////////
// Macros

//uiType
#define UNDEFINED       0
#define ANALOG          1
#define BINARY          2
#define RESISTANCE      3
#define PULSE           4
#define PULSE_WIDTH     5

//inputType
#define UNDEFINED       0
#define RAW             1
#define T_10K2          2
#define T_10K3          3
#define L_4_TO_20MA     4
#define L_0_TO_5V       5
#define L_0_TO_10V      10
#define T_20K_NTC       20


//////////////////////////////////////////////////////////////////////////
// Declarations

//data structure for hardware Universal Inputs
typedef struct universal_input_s{

  //which expander
  uint8_t expanderNumber;

  //the input number on the controller
  uint8_t inputNumber;

  //see type of UI above
  uint8_t uiType;

  //see input type above
  uint8_t inputType;

  //raw value returned from Analog to Digital Converter
  uint32_t rawADCCount;

  uint32_t rangeMax;

  uint32_t rangeMin;

  //final calculated value which is presented to higher software layers
  uint32_t presentValue;

}universal_input;

