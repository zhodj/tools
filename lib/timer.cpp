#include <iostream> 
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <sys/fcntl.h>
#include <unistd.h>

#include <sys/epoll.h>
#include <sys/timerfd.h>

int main(int argc, char **argv)
{
    /* epoll instance */
    int efd = epoll_create(EPOLL_CLOEXEC);

    if(efd < 0)
    {
        std::cerr << "epoll_create error: " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    struct epoll_event ev;
    struct epoll_event events[128];

    /* timer instance */
    int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);

    struct timespec ts;
    //first expiration in 3. seconds after program start
    ts.tv_sec = 3;
    ts.tv_nsec = 0;

    struct itimerspec new_timeout;
    struct itimerspec old_timeout;

    bzero(&new_timeout, sizeof(new_timeout));
    bzero(&old_timeout, sizeof(old_timeout));

    //value
    new_timeout.it_value = ts;

    //no interval;
    //timer will be armed in epoll_wait event trigger
    new_timeout.it_interval.tv_sec = 0;
    new_timeout.it_interval.tv_nsec = 0;

    //Add the timer descriptor to epoll.
    if (tfd != -1)
    {
        ev.events = EPOLLIN | EPOLLERR;
        ev.data.ptr = &tfd;
        epoll_ctl(efd, EPOLL_CTL_ADD, tfd, &ev);
    }

    int flags = 0;
    if (timerfd_settime(tfd, flags, &new_timeout, &old_timeout) < 0)
    {
        std::cerr << "timerfd_settime error: " << strerror(errno) << std::endl;
    }

    int numEvents = 0;
    int timeout = 0;
    bool checkTimer = false;
    while(1)
    {
        checkTimer = false;
        numEvents = epoll_wait(efd, events, 128, timeout);
        if(numEvents > 0)
        {

            std::cout << "numEvents = " << numEvents << std::endl;
            for(int i = 0; i < numEvents; ++i)
            {
                if(events[i].data.ptr == &tfd)
                {
                    std::cout << "timeout" << std::endl;
                    checkTimer = true;
                }
            }
        }else if(numEvents == 0)
        {
            continue;
        }else
        {
            std::cerr << "An error occured: " << strerror(errno) << std::endl;
        }

        if(checkTimer)
        {
            uint64_t value;
            ssize_t readBytes;

            itimerspec new_timeout;
            itimerspec old_timeout;

            new_timeout.it_value.tv_sec = rand() % 3 + 1;
            new_timeout.it_value.tv_nsec = 0;
            new_timeout.it_interval.tv_sec = 
            new_timeout.it_interval.tv_nsec = 0;

            timerfd_settime(tfd,flags, &new_timeout, &old_timeout);

        }

    }
    return EXIT_SUCCESS;

}
