///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stream_wrapper.hpp"

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <poll.h>

class Process
{
public:
    enum class State
    {
        Waiting,
        Ready,
        ErrorTimeout,
        ErrorUnknown
    };

    Process(const std::string& command, size_t max_output_size, int timeout_ms)
        : m_stream_wrapper( command.c_str() )
        , m_monitor_thread( &Process::output_monitor, this, timeout_ms )
        , m_max_output_size( max_output_size )
    {}

    Process(const Process&) = delete;
    Process& operator= (const Process&) = delete;

    ~Process() noexcept
    {
        m_monitor_thread.join();
    }

    State get_state() const noexcept
    {
        return m_state.load();
    }

    std::string get_and_clear_output() noexcept
    {
        std::cout << __func__ << '\n';

        std::string ret {};
        std::lock_guard lock(m_output_mu);
        std::swap(m_output, ret);
        return ret;
    }

private:
    void output_monitor(int timeout_ms)
    {
        pollfd pollfds[1];
        pollfds[0].fd = m_stream_wrapper.get_fd();
        pollfds[0].events = POLLIN;

        const int ready = poll(pollfds, 1, timeout_ms);

        if (ready == -1)
        {
            m_state.store( State::ErrorUnknown );
        }
        else if (ready == 0)
        {
            m_state.store( State::ErrorTimeout );
        }

        if ( pollfds[0].revents & POLLIN )
        {
            {
                std::lock_guard lock(m_output_mu);
                m_stream_wrapper.read( m_output, m_max_output_size );
            }

            m_state.store( State::Ready );
        }
    }

    StreamWrapper m_stream_wrapper;

    std::thread m_monitor_thread {};

    std::atomic<State> m_state { State::Waiting };

    std::mutex m_output_mu {};
    std::string m_output {};
    size_t m_max_output_size {};
};
