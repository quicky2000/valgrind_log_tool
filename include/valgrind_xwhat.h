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

#ifndef VALGRIND_LOG_TOOL_VALGRIND_XWHAT_H
#define VALGRIND_LOG_TOOL_VALGRIND_XWHAT_H

#include <string>

namespace valgrind_log_tool
{
    class valgrind_xwhat
    {
      public:

        inline
        valgrind_xwhat();

        inline
        void set_text(std::string && p_text);

        inline
        void set_leaked_bytes(uint32_t p_leaked_bytes);

        inline
        void set_leaked_blocks(uint32_t p_leaked_bytes);

        inline
        const std::string & get_text() const;

        inline
        uint32_t get_leaked_bytes() const;

        inline
        uint32_t get_leaked_blocks() const;

      private:
        std::string m_text;
        uint32_t m_leaked_bytes;
        uint32_t m_leaked_blocks;
    };

    //-------------------------------------------------------------------------
    valgrind_xwhat::valgrind_xwhat()
    : m_leaked_bytes(0)
    , m_leaked_blocks(0)
    {

    }

    //-------------------------------------------------------------------------
    void valgrind_xwhat::set_text(std::string && p_text)
    {
        m_text = p_text;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_xwhat::set_leaked_bytes(uint32_t p_leaked_bytes)
    {
        m_leaked_bytes = p_leaked_bytes;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_xwhat::set_leaked_blocks(uint32_t p_leaked_bytes)
    {
        m_leaked_blocks = p_leaked_bytes;
    }

    //-------------------------------------------------------------------------
    const std::string &
    valgrind_xwhat::get_text() const
    {
        return m_text;
    }

    //-------------------------------------------------------------------------
    uint32_t
    valgrind_xwhat::get_leaked_bytes() const
    {
        return m_leaked_bytes;
    }

    //-------------------------------------------------------------------------
    uint32_t
    valgrind_xwhat::get_leaked_blocks() const
    {
        return m_leaked_blocks;
    }

}
#endif //VALGRIND_LOG_TOOL_VALGRIND_XWHAT_H
// EOF