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

#ifndef VALGRIND_LOG_TOOL_VALGRIND_FRAME_H
#define VALGRIND_LOG_TOOL_VALGRIND_FRAME_H

#include <cinttypes>
#include <string>

namespace valgrind_log_tool
{
    class valgrind_frame
    {
      public:
        inline
        valgrind_frame();

        inline
        void set_ip(uint64_t p_ip);

        inline
        void set_obj(std::string && p_obj);

        inline
        void set_fn(std::string && p_fn);

        inline
        void set_dir(std::string && p_dir);

        inline
        void set_file(std::string && p_file);

        inline
        void set_line(uint32_t p_line);

        inline
        const uint64_t & get_ip() const;

        inline
        const std::string & get_obj() const;

        inline
        const std::string & get_fn() const;

        inline
        const std::string & get_dir() const;

        inline
        const std::string & get_file() const;

        inline
        const uint32_t & get_line() const;

      private:
        uint64_t m_ip;
        std::string m_obj;
        std::string m_fn;
        std::string m_dir;
        std::string m_file;
        uint32_t m_line;
    };

    //-------------------------------------------------------------------------
    valgrind_frame::valgrind_frame()
    : m_ip(0)
    , m_line(0)
    {

    }

    //-------------------------------------------------------------------------
    void
    valgrind_frame::set_ip(uint64_t p_ip)
    {
        m_ip = p_ip;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_frame::set_obj(std::string && p_obj)
    {
        m_obj = p_obj;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_frame::set_fn(std::string && p_fn)
    {
        m_fn = p_fn;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_frame::set_dir(std::string && p_dir)
    {
        m_dir = p_dir;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_frame::set_file(std::string && p_file)
    {
        m_file = p_file;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_frame::set_line(uint32_t p_line)
    {
        m_line = p_line;
    }

    //-------------------------------------------------------------------------
    const uint64_t &
    valgrind_frame::get_ip() const
    {
        return m_ip;
    }

    //-------------------------------------------------------------------------
    const std::string &
    valgrind_frame::get_obj() const
    {
        return m_obj;
    }

    //-------------------------------------------------------------------------
    const std::string &
    valgrind_frame::get_fn() const
    {
        return m_fn;
    }

    //-------------------------------------------------------------------------
    const std::string &
    valgrind_frame::get_dir() const
    {
        return m_dir;
    }

    //-------------------------------------------------------------------------
    const std::string &
    valgrind_frame::get_file() const
    {
        return m_file;
    }

    //-------------------------------------------------------------------------
    const uint32_t &
    valgrind_frame::get_line() const
    {
        return m_line;
    }
}
#endif //VALGRIND_LOG_TOOL_VALGRIND_FRAME_H
