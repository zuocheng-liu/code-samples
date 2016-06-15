#ifndef TIMER_
#define TIMER_

class TimedEvent {
}

class Timer {
    public : 
        Timer() {
            make_heap(_eventHeap.begin(), _eventHeap.end(), EventHeapCmp);
        }
        void addPeriodicEvent(TimedEvent);
        void addOneOffEvent();
    private :
        void _processEvents();
        static bool EventHeapCmp(const TimedEvent& e1 , TimedEvent& e2) {
            return e1.time <= e2.time;
        }
        vector<TimedEvent> _eventHeap;
};
#endif
