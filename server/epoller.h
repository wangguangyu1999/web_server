//
// Created by root on 23-3-11.
//

#ifndef WEB_SERVER_EPOLLER_H
#define WEB_SERVER_EPOLLER_H

#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <vector>
#include <cerrno>

class epoller
{
public:
    explicit epoller(int max_event = 1024);

    ~epoller();

    bool add_fd(int fd, uint32_t events);

    bool mod_fd(int fd, uint32_t events);

    bool del_fd(int fd);

    int wait(int timeout = -1);

    int get_event_fd(size_t i) const;

    uint32_t get_events(size_t i) const;


private:
    int _epoll_fd;
    std::vector<struct epoll_event> _events;
};

#endif //WEB_SERVER_EPOLLER_H
