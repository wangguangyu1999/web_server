//
// Created by root on 23-3-11.
//

#include "epoller.h"

epoller::epoller(int max_event)
{
    assert(_epoll_fd >= 0 && _events.size() > 0);
}

epoller::~epoller()
{
    close(_epoll_fd);
}

bool epoller::add_fd(int fd, uint32_t events)
{
    if(fd < 0)
        return false;
    epoll_event ev = { 0 };
    ev.data.fd = fd;
    ev.events = events;
    return 0 == epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev);
}

bool epoller::mod_fd(int fd, uint32_t events)
{
    if(fd < 0)
        return false;
    epoll_event ev = { 0 };
    ev.data.fd = fd;
    ev.events = events;
    return 0 == epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, fd, &ev);
}

bool epoller::del_fd(int fd)
{
    if(fd < 0)
        return false;
    epoll_event ev = { 0 };
    return 0 == epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, &ev);
}

int epoller::wait(int timeout)
{
    return epoll_wait(_epoll_fd, &_events[0], static_cast<int>(_events.size()), timeout);
}

int epoller::get_event_fd(size_t i) const
{
    assert(i < _events.size() && i >= 0);
    return _events[i].data.fd;
}

uint32_t epoller::get_events(size_t i) const
{
    assert(i < _events.size() && i >= 0);
    return _events[i].events;
}
