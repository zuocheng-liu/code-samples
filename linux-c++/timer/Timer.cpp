#include "timer.h"
 
/*
 * Save the global data such as file descriptors of timerfd and 
 * create a new thread for epoll
 *
 */
class TimerPrivate {
    public:
        TimerPrivate();
        ~TimerPrivate() {
            pthread_mutex_destroy(&m_mutex);
        }
 
        // Some constant
        enum {
            MaxEPOLLSize = 20000,    
        };
 
        /*
         *  Name: epoll_proc
         *  Brief: this function run on new thread for epoll
         *
         */
        static void* epoll_proc(void *);
 
        /*
         *  Get the timer event by fd
         *
         */
        static Timer::TimerEvent get_timer_event(int fd);
 
        /*
         *  Add the timer event to map and epoll
         *
         */ 
        static bool add_timer_event(const Timer::TimerEvent &te); 
 
        /*
         *  Remove the timer event from map adn epoll by fd
         *
         */
        static void remove_timer_event(const int fd);
 
        // Map of file descriptor
        int m_epoll_fd;
        typedef map<int, Timer::TimerEvent> MapTimerEvent;
        MapTimerEvent m_map_te;
        pthread_t m_tid;
        pthread_mutex_t m_mutex; 
};
 
// The declare of TimerPrivate
static TimerPrivate g_tp;
 
TimerPrivate::TimerPrivate() {
    try {
        // Initialization
        // Init mutex
        int res = pthread_mutex_init(&m_mutex, 0);
        if(res == -1) {
            perror("pthread_mutex_init");
            throw;
        }
 
        // Create epoll
        m_epoll_fd = epoll_create(MaxEPOLLSize);
        if(m_epoll_fd == -1) {
            perror("epoll_create");
            throw;
        }
 
        // Create thread for epoll
        res = pthread_create(&m_tid, 0, TimerPrivate::epoll_proc, 0);
        if(res == -1) {
            perror("pthread_create");
            throw;
        }
    } catch (...) {} 
}
 
void* TimerPrivate::epoll_proc(void *) {
    struct epoll_event events[MaxEPOLLSize];
    while(1) {
        // Wait for notice
        int n =epoll_wait(g_tp.m_epoll_fd, events, MaxEPOLLSize, -1); 
        pthread_mutex_lock(&g_tp.m_mutex);
        for(int i=0; i<n; ++i) {
            int fd = events[i].data.fd;
            // Clear buffer
            uint64_t buf;
            read(fd, &buf, sizeof(uint64_t));
 
            // Call the callback function when timer expiration
            Timer::TimerEvent te = TimerPrivate::get_timer_event(events[i].data.fd);
            if(te.cbf) {
                te.cbf(te.args);
            }
        }
        pthread_mutex_unlock(&g_tp.m_mutex);
    }
    return 0;
}
 
Timer::TimerEvent TimerPrivate::get_timer_event(int fd) {
    return g_tp.m_map_te[fd];
}
 
bool TimerPrivate::add_timer_event(const Timer::TimerEvent &te) {
    // Add timer event for epoll
    struct epoll_event epe;
    epe.data.fd = te.fd;
    epe.events = EPOLLIN | EPOLLET;
    int res = epoll_ctl(g_tp.m_epoll_fd, EPOLL_CTL_ADD, te.fd, &epe); 
    if(res == -1) {
        perror("epoll_ctl");
        return false;
    }
 
    // Insert timer event to map
    g_tp.m_map_te[te.fd] = te;
 
    return true;
}
 
void TimerPrivate::remove_timer_event(const int fd) {
    // Remove from epoll
    int res = epoll_ctl(g_tp.m_epoll_fd, EPOLL_CTL_DEL, fd,0);
    if(res == -1) {
        perror("epoll_ctl");
        return; 
    }
 
    // Remove from map
    MapTimerEvent::iterator iter = g_tp.m_map_te.find(fd);
    g_tp.m_map_te.erase(iter);
}
 
Timer::Timer() : m_is_start(false) {
    ::memset(&m_te, 0, sizeof(TimerEvent));
}
 
Timer::~Timer() {
    if(m_is_start) {
        stop();
        m_is_start = false;
    }
}
 
bool Timer::start(const uint interval, CALLBACK_FN cbf, void *args, const bool triggered_on_start) {
    pthread_mutex_lock(&g_tp.m_mutex);
 
    if(!m_is_start) {
        if(!cbf) {
            cout << "start:" << "callback function can't set to be null" << endl;
            return false;
        }
 
        // Create timer
        struct itimerspec timer;
        double dfsec = (double)interval/1000;
        uint32_t sec=dfsec;
        uint64_t number_ns = 1000000000;
        uint64_t nsec = dfsec>=1 ? fmod(dfsec,(int)dfsec)*number_ns : dfsec*number_ns;
        timer.it_value.tv_nsec = triggered_on_start ? 0 : nsec;
        timer.it_value.tv_sec = triggered_on_start ? 0 : sec;
        timer.it_interval.tv_nsec = nsec;
        timer.it_interval.tv_sec = sec;
 
        int fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
        if(fd == -1) {
            perror("timerfd_create");
            return false;
        }
 
        int res = timerfd_settime(fd, 0, &timer, 0);
        if(res == -1) {
            perror("timerfd_settime");
            return false;
        }
 
        // Add timer for epoll
        TimerEvent te;
        te.fd = fd;
        te.cbf = cbf;
        te.args = args;
        res = TimerPrivate::add_timer_event(te);
        if(res == false) {
            return false;
        }
 
        // Change the attributes of class
        m_te = te;
        m_is_start = true;
    } else {
        cout << "start:Timer already start" << endl;
        return false;
    }
 
    pthread_mutex_unlock(&g_tp.m_mutex);
    return true;
}
 
void Timer::stop() {
    pthread_mutex_lock(&g_tp.m_mutex);
 
    // Remove from map and epoll
    TimerPrivate::remove_timer_event(m_te.fd);
 
    // Close the timer
    int res = close(m_te.fd);
    if(res == -1) {
        perror("close");
    }
 
    // Clear the attributes of class
    m_is_start = false;
 
    pthread_mutex_unlock(&g_tp.m_mutex);
}
 
/**************************************************************************/
// Test
void timer_proc(void *args) {
    cout << args << endl;
}
 
int main() {
    list<Timer*> l;
    for(int i=0; i<10;++i) {
        Timer *t = new Timer();
        t->start(500, timer_proc, reinterpret_cast<void*>(i));
        l.push_back(t);
    }
    sleep(3);
    return 0;

}