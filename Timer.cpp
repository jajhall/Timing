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

void multipleTimeCalls0(HighsTimer& timer, const int n) {
  for(int i = 0; i < n; i++)
    timer.getWallTime0();
}

void multipleTimeCalls1(HighsTimer& timer, const int n) {
  for(int i = 0; i < n; i++)
    timer.getWallTime1();
}

void multipleTickCalls(HighsTimer& timer, const int n) {
  for(int i = 0; i < n; i++)
    timer.getWallTick();
}

int main () {
  int num;
  report();
  std::cout << "Enter number of calls: ";
  std::cin >> num;
  std::cout << num << std::endl;
  if (!num) return 1;
  HighsTimer timer;
  int work_clock = timer.clock_def("Work", "Wrk");
  int time_clock = timer.clock_def("high_resolution", "Tim");
  int time0_clock = timer.clock_def("system_clock", "Tt0");
  int time1_clock = timer.clock_def("steady_clock", "Tt1");
  int tick_clock = timer.clock_def("Tick", "Tik");
  std::vector<int> clock_list;
  clock_list.push_back(work_clock);
  clock_list.push_back(time_clock);
  clock_list.push_back(time0_clock);
  clock_list.push_back(time1_clock);
  clock_list.push_back(tick_clock);
  timer.startRunHighsClock();

  timer.start(work_clock);
  double sum_log = sumLog(num);
  timer.stop(work_clock);

  timer.start(time_clock);
  multipleTimeCalls(timer, num);
  timer.stop(time_clock);

  timer.start(time0_clock);
  multipleTimeCalls0(timer, num);
  timer.stop(time0_clock);

  timer.start(time1_clock);
  multipleTimeCalls1(timer, num);
  timer.stop(time1_clock);

  timer.start(tick_clock);
  multipleTickCalls(timer, num);
  timer.stop(tick_clock);

  timer.stopRunHighsClock();

  // Totally abuse the clock call mechanism to aid reporting
  timer.clock_num_call[time_clock] = num;
  timer.clock_num_call[time0_clock] = num;
  timer.clock_num_call[time1_clock] = num;
  timer.clock_num_call[tick_clock] = num;
  timer.report("", clock_list);
  return 0;
}
