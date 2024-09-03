#include "sedona.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "UI.h"

#define MAX_BUF_SIZE 64
#define IIO_DEVICE_PATH "/sys/bus/iio/devices/iio:device0"
#define IIO_CHANNEL_NAME "in_voltage0_raw"
#define IIO_CHAN_1_RAW "in_voltage1_raw"
#define IIO_CHAN_2_RAW "in_voltage2_raw"
#define IIO_CHAN_3_RAW "in_voltage3_raw"
#define IIO_CHAN_4_RAW "in_voltage4_raw"
#define IIO_CHAN_5_RAW "in_voltage5_raw"
#define IIO_CHAN_6_RAW "in_voltage6_raw"

#define NUM_UIS_DEV0 7

universal_input Input[NUM_UIS_DEV0];

// Lookup table
// x = 12 bit ADC count
float x[] = {3654, 3629, 3602, 3574, 3546, 3516, 3485, 3452, 3419, 3385,
3350, 3313, 3276, 3237, 3197, 3157, 3114, 3072, 3028, 2984,
2939, 2893, 2846, 2799, 2751, 2702, 2653, 2603, 2553, 2503,
2453, 2402, 2351, 2301, 2249, 2199, 2148, 2098, 2048, 1998,
1949, 1900, 1852, 1803, 1756, 1709, 1663, 1618, 1574, 1530,
1487, 1444, 1403, 1362, 1322, 1283, 1245, 1208, 1172, 1136,
1102, 1068, 1035, 1003, 972, 942, 912, 884, 856, 829,
803, 778, 753, 729, 706, 684, 662, 641, 620, 601 }; // Input values

// y = Temperature in degrees fahrenheit
float y[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19,
21, 23, 25, 27, 29, 31, 33, 35, 37, 39,
41, 43, 45, 47, 49, 51, 53, 55, 57, 59,
61, 63, 65, 67, 69, 71, 73, 75, 77, 79,
81, 83, 85, 87, 89, 91, 93, 95, 97, 99,
101, 103, 105, 107, 109, 111, 113, 115, 117, 119,
121, 123, 125, 127, 129, 131, 133, 135, 137, 139,
141, 143, 145, 147, 149, 151, 153, 155, 157, 159 }; // Corresponding output values

// Function to perform linear interpolation
float linear_interp(float x_val) {
    int i;
    float x0, x1, y0, y1;

    // Find the interval in which x_val lies
    for (i = 0; i < sizeof(x) / sizeof(x[0]) - 1; i++) {
        if (x_val <= x[i] && x_val >= x[i + 1]) {
            x0 = x[i];
            x1 = x[i + 1];
            y0 = y[i];
            y1 = y[i + 1];
            break;
        }
    }

    // Perform linear interpolation
    return y0 + (y1 - y0) / (x1 - x0) * (x_val - x0);
}

// float  AnalogIn.doRead(int address)
Cell iopoint_AnalogIn_doRead(SedonaVM* vm, Cell* params)
{
  int address = params[0].ival;
  int inputType   = params[1].ival;

  Cell result;

  FILE *fp;
  char buf[MAX_BUF_SIZE];
  char filepath[MAX_BUF_SIZE];
  int value;
  
  // Construct the path to the IIO channel 
  switch(address) {

    case 1:
      snprintf(filepath, sizeof(filepath), "%s/%s", IIO_DEVICE_PATH, IIO_CHAN_1_RAW);
	  break;

    case 2:
      snprintf(filepath, sizeof(filepath), "%s/%s", IIO_DEVICE_PATH, IIO_CHAN_2_RAW);
	  break;

    case 3:
      snprintf(filepath, sizeof(filepath), "%s/%s", IIO_DEVICE_PATH, IIO_CHAN_3_RAW);
	  break;

    case 4:
      snprintf(filepath, sizeof(filepath), "%s/%s", IIO_DEVICE_PATH, IIO_CHAN_4_RAW);
	  break;

    case 5:
      snprintf(filepath, sizeof(filepath), "%s/%s", IIO_DEVICE_PATH, IIO_CHAN_5_RAW);
	  break;

    case 6:
      snprintf(filepath, sizeof(filepath), "%s/%s", IIO_DEVICE_PATH, IIO_CHAN_6_RAW);
	  break;

    default:
      snprintf(filepath, sizeof(filepath), "%s/%s", IIO_DEVICE_PATH, IIO_CHANNEL_NAME);
  }

  // Open the file for reading
  fp = fopen(filepath, "r");
  if (fp == NULL) {
      // perror("Failed to open file");
      return result;
  }

  // Read the value from the file
  if (fgets(buf, sizeof(buf), fp) == NULL) {
      // perror("Failed to read from file");
      fclose(fp);
      return result;
  }

  // Convert the string to an integer
  value = atoi(buf);

  // Close the file
  fclose(fp);

  // Print the read value
  //printf("Channel raw ADC count: %d\n", value);

  float adc_count;

  adc_count = (float)value;

  // Output based on:  
  switch(inputType) {

    case 1: // Raw ADC Count
      result.fval = adc_count;
	  break;

    case 2: // Thermistor 10K-2
      result.fval = linear_interp(adc_count);
	  break;

    case 3: // Thermistor 10K-3
      result.fval = 0;
	  break;

    case 4: // 4-20mA = 0-100
      result.fval = 0;
	  break;

    case 5: // 0-5V = 0-100
      result.fval = 0;
	  break;

    case 10: // 0-10V = 0-100
      result.fval = 0;
	  break;

    default:
      result.fval = 0;
  }

  return result;

}
