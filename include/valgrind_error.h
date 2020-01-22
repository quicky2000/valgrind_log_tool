/*
      This file is part of valgrind_log_tool
      Copyright (C) 2020  Julien Thevenon ( julien_thevenon at yahoo.fr )

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef VALGRIND_LOG_TOOL_VALGRIND_ERROR_H
#define VALGRIND_LOG_TOOL_VALGRIND_ERROR_H

#include "valgrind_frame.h"
#include "valgrind_xwhat.h"
#include <cinttypes>
#include <vector>
#include <cassert>
#include <functional>

namespace valgrind_log_tool
{
    class valgrind_error
    {
      public:

        inline
        valgrind_error();

        inline
        ~valgrind_error();

        inline
        void set_unique(uint64_t p_unique);

        inline
        void set_tid(uint64_t p_tid);

        inline
        void set_kind(std::string && p_kind);

        inline
        void set_xwhat(const valgrind_xwhat & p_xwhat);

        inline
        void set_what(std::string && p_what);

        inline
        void set_aux_what(std::string && p_aux_what);

        inline
        void add_frame(const valgrind_frame & p_frame);

        inline
        const uint64_t & get_unique() const;

        inline
        const uint64_t & get_tid() const;

        inline
        const std::string & get_kind() const;

        inline
        bool has_xwhat() const;

        inline
        const valgrind_xwhat & get_xwhat() const;

        inline
        const std::string & get_what() const;

        inline
        const std::string & get_aux_what() const;

        inline
        void process_stack(const std::function<void(const valgrind_frame&)> & p_func) const;

      private:

        uint64_t m_unique;
        uint64_t m_tid;
        std::string m_kind;
        const valgrind_xwhat * m_xwhat;
        std::string m_what;
        std::string m_aux_what;
        std::vector<const valgrind_frame *> m_stack;
    };

    //-------------------------------------------------------------------------
    valgrind_error::valgrind_error()
    : m_unique(0)
    , m_tid(0)
    , m_xwhat(nullptr)
    {

    }

    //-------------------------------------------------------------------------
    void
    valgrind_error::set_unique(uint64_t p_unique)
    {
        m_unique = p_unique;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_error::set_tid(uint64_t p_tid)
    {
        m_tid = p_tid;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_error::set_kind(std::string && p_kind)
    {
        m_kind = p_kind;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_error::set_what(std::string && p_what)
    {
        m_what = p_what;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_error::set_aux_what(std::string && p_aux_what)
    {
        m_aux_what = p_aux_what;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_error::add_frame(const valgrind_frame & p_frame)
    {
        m_stack.push_back(&p_frame);
    }

    //-------------------------------------------------------------------------
    valgrind_error::~valgrind_error()
    {
        for(auto l_iter: m_stack)
        {
            delete l_iter;
        }
        delete m_xwhat;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_error::set_xwhat(const valgrind_xwhat & p_xwhat)
    {
        m_xwhat = & p_xwhat;
    }

    //-------------------------------------------------------------------------
    const std::string &
    valgrind_error::get_kind() const
    {
        return m_kind;
    }

    //-------------------------------------------------------------------------
    const uint64_t &
    valgrind_error::get_unique() const
    {
        return m_unique;
    }

    //-------------------------------------------------------------------------
    const uint64_t &
    valgrind_error::get_tid() const
    {
        return m_tid;
    }

    //-------------------------------------------------------------------------
    bool
    valgrind_error::has_xwhat() const
    {
        return m_xwhat;
    }

    //-------------------------------------------------------------------------
    const valgrind_xwhat &
    valgrind_error::get_xwhat() const
    {
        assert(m_xwhat);
        return *m_xwhat;
    }

    //-------------------------------------------------------------------------
    const std::string &
    valgrind_error::get_what() const
    {
        return m_what;
    }

    //-------------------------------------------------------------------------
    const std::string &
    valgrind_error::get_aux_what() const
    {
        return m_aux_what;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_error::process_stack(const std::function<void(const valgrind_frame &)> & p_func) const
    {
        for(auto l_iter:m_stack)
        {
            p_func(*l_iter);
        }
    }

}
#endif //VALGRIND_LOG_TOOL_VALGRIND_ERROR_H
