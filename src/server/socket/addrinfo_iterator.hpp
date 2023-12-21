///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "types.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Addrinfo iterator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AddrinfoIt
{
public:
    AddrinfoIt(Addrinfo* ptr) : m_ptr(ptr)
    {}

    AddrinfoIt& operator++ ()
    {
        m_ptr = m_ptr->ai_next;
        return *this;
    }

    Addrinfo* operator* ()
    {
        return m_ptr; 
    }

    bool operator!= (const AddrinfoIt& arg) const noexcept
    {
        return this->m_ptr != arg.m_ptr;
    }

private:
    Addrinfo* m_ptr;
};
