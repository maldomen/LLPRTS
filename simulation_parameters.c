#include "simulation_parameters.h"

unsigned long long interval=1000; // Interrupt interval in microseconds

int deterministic = 1;            // The simulation will be deterministic

float lunar_gravity = 0.2f;       // Moon gravity in m/s2

int disable_crash_detection = 0;  // If 1, disable crash detection

float pump_acceleration = -0.4f;  // Acceleration of a pump
int pump_actuator_delay = 50;     // Delay of the pump actuator in miliseconds

float drift_acceleration = 0.1f;  // Drift acceleratin in any direction
int drift_actuator_delay = 500;   // Delay of the drift actuator in miliseconds