///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <stdexcept>
#include <thread>
#include <atomic>
#include <mutex>
#include <iostream>

#include <stdio.h>

#include <unistd.h>

#include <poll.h>


enum class State
{
    Waiting,
    Ready,
    ErrorTimeout,
    ErrorUnknown
};

class StreamWrapper
{
public:
    StreamWrapper(const std::string& command) : m_stream( popen(command.c_str(), "r") )
    {
        if (!m_stream)
        {
            throw std::logic_error("Failed to start a process!");
        }
    }

    StreamWrapper(const StreamWrapper&) = delete;
    StreamWrapper& operator= (const StreamWrapper&) = delete;

    ~StreamWrapper() noexcept
    {
        pclose(m_stream);
        m_stream = NULL;
    }

    int get_fd()
    {
        return fileno(m_stream);
    }

    size_t read(void* ptr, size_t max_size)
    {
        return fread(ptr, 1, max_size, m_stream);
    }

private:
    FILE* m_stream {};
};

class Process
{
public:
    Process(const std::string& command, size_t max_output_size)
        : m_stream_wrapper( command.c_str() )
        , m_monitor_thread( &Process::output_monitor, this )
    {
        m_output.reserve(max_output_size);
    }

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
    void output_monitor()
    {
        pollfd pollfds[1];
        pollfds[0].fd = m_stream_wrapper.get_fd();
        pollfds[0].events = POLLIN;

        const int ready = poll(pollfds, 1, 5000);

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
                m_output.resize(1024);
                const size_t wn = m_stream_wrapper.read( &m_output[0], m_output.capacity() );
                m_output.resize(wn);
            }

            m_state.store( State::Ready );
        }
    }

    StreamWrapper m_stream_wrapper;

    std::thread m_monitor_thread {};

    std::atomic<State> m_state { State::Waiting };

    std::mutex m_output_mu {};
    std::string m_output {};
};
