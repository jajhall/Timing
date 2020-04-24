#include <iostream>
#include <vector>
#include <cmath>
#include "HighsTimer.h"
#include "ClockProperties.h"

double sumLog(const int n) {
  double sum;
  for(int i = 0; i < n; i++)
    sum += std::log(i+1);
  return sum;
}

void multipleTimeCalls(HighsTimer& timer, const int n) {
  for(int i = 0; i < n; i++)
    timer.getWallTime();
}

void timeTimerCall(const int num) {
  HighsTimer timer;
  int work_clock = timer.clock_def("Work", "Wrk");
  int time_clock = timer.clock_def("high_resolution", "Tim");
  std::vector<int> clock_list;
  clock_list.push_back(work_clock);
  clock_list.push_back(time_clock);
  timer.startRunHighsClock();

  timer.start(work_clock);
  double sum_log = sumLog(num);
  timer.stop(work_clock);

  timer.start(time_clock);
  multipleTimeCalls(timer, num);
  timer.stop(time_clock);

  timer.stopRunHighsClock();

  // Totally abuse the clock call mechanism to aid reporting
  timer.clock_num_call[time_clock] = num;
  timer.report("", clock_list);
}

void timeMethodCall(const int num) {
  HighsTimer timer;
  int call0_clock = timer.clock_def("Call 0", "TT0");
  int call1_clock = timer.clock_def("Call 1", "TT1");
  int call2_clock = timer.clock_def("Call 2", "TT2");
  int call3_clock = timer.clock_def("Call 3", "TT3");
  std::vector<int> clock_list;
  clock_list.push_back(call0_clock);
  clock_list.push_back(call1_clock);
  clock_list.push_back(call2_clock);
  clock_list.push_back(call3_clock);

  double tt0, tt1;
  bool use_timing = true;

  std::cout << "Use timing?: ";
  std::cin >> use_timing;
  std::cout << use_timing << std::endl;

  timer.startRunHighsClock();
  tt0 = timer.readRunHighsClock();
  for (int k = 0; k < num; k++) {
    timer.start(call0_clock);
    sumLog(k);
    timer.stop(call0_clock);
  }
  timer.stopRunHighsClock();
  
  tt1 = timer.readRunHighsClock();
  printf("Takes %9.4g seconds when clock calls hard coded\n", tt1-tt0);
 
  timer.startRunHighsClock();
  tt0 = timer.readRunHighsClock();
  for (int k = 0; k < num; k++) {
    if (use_timing)
      timer.start(call1_clock);
    sumLog(k);
    if (use_timing)
      timer.stop(call1_clock);
  }
  timer.stopRunHighsClock();
  
  tt1 = timer.readRunHighsClock();
  printf("Takes %9.4g seconds when clock calls conditional but called\n", tt1-tt0);

  use_timing = !use_timing;
  timer.startRunHighsClock();
  tt0 = timer.readRunHighsClock();
  for (int k = 0; k < num; k++) {
    if (use_timing)
      timer.start(call2_clock);
    sumLog(k);
    if (use_timing)
      timer.stop(call2_clock);
  }
  timer.stopRunHighsClock();
  
  tt1 = timer.readRunHighsClock();
  printf("Takes %9.4g seconds when clock calls conditional and not called\n", tt1-tt0);

  timer.only_highs_clock = true;
  timer.startRunHighsClock();
  tt0 = timer.readRunHighsClock();
  for (int k = 0; k < num; k++) {
    timer.start(call3_clock);
    sumLog(k);
    timer.stop(call3_clock);
  }
  timer.stopRunHighsClock();
  
  tt1 = timer.readRunHighsClock();
  printf("Takes %9.4g seconds when clock calls returning\n", tt1-tt0);

  timer.report("", clock_list);
}

int main () {
  int num;
  // Report on clock properties
  //  reportClockProperties();
  /*
  std::cout << "Enter number of calls: ";
  std::cin >> num;
  std::cout << num << std::endl;
  */
  num = 100000;
  if (!num) return 1;

  //  timeTimerCall(num);

  timeMethodCall(num);

  return 0;
}
