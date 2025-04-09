#include "IntervalManager.h"

IntervalManager::IntervalManager() : lastInterval(-1) {
    intervals[0] = {0, 0, 9};
    intervals[1] = {1, 10, 19};
    intervals[2] = {2, 20, 29};
    intervals[3] = {3, 30, 100};
}

int IntervalManager::getIntervalIndex(float distance) {
    if (distance <= 0) return 3;

    for (int i = 0; i < 3; ++i) {
        if (distance > intervals[i].min && distance <= intervals[i].max) {
            return i;
        }
    }
    return 3;
}

int IntervalManager::getLastInterval() const {
    return lastInterval;
}

void IntervalManager::setLastInterval(int interval) {
    lastInterval = interval;
}

void IntervalManager::updateIntervals(const Interval newIntervals[4]) {
    for (int i = 0; i < 4; ++i) {
        intervals[i] = newIntervals[i];
    }
}