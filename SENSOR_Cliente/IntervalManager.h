#ifndef INTERVAL_MANAGER_H
#define INTERVAL_MANAGER_H

struct Interval {
    int index;
    float min;
    float max;
};

class IntervalManager {
private:
    Interval intervals[4];
    int lastInterval;

public:
    IntervalManager();
    int getIntervalIndex(float distance);
    int getLastInterval() const;
    void setLastInterval(int interval);
    void updateIntervals(const Interval newIntervals[4]);
};

#endif