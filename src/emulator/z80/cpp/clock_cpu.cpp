#include "emf/emf.h"
class zx81_clock_cpu {
public:
uint32_t cycles;
float period;
uint32_t timecum;
uint32_t uptimeTotal;
zx81_clock_cpu(hashmap &bus, hashmap &options) {
  setRate(820800);
reset();
}
void reset() {
timecum = 0;
uptimeTotal = 0;
}
uint32_t getUptime() {
return uptimeTotal;
}
void tick(uint32_t t) {
  timecum += t;
  uptimeTotal += t;
  while (timecum >= period) {
    timecum -= period;
  }
}
// Clock device : 
// cpu
void setRate(uint32_t hz) {
  cycles = hz;
  period = 1/hz;
}
uint32_t getFrequency() {
return cycles;
}
return {
  reset,
  tick,
  setRate,
  getFrequency,
  getUptime,
};
};
