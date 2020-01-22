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

#ifndef VALGRIND_LOG_TOOL_VALGRIND_LOG_CONTENT_H
#define VALGRIND_LOG_TOOL_VALGRIND_LOG_CONTENT_H

#include "valgrind_error.h"
#include <vector>
#include <map>
#include <cinttypes>
#include <functional>

namespace valgrind_log_tool
{
    class valgrind_log_content
    {
      public:

        inline
        void add_error(const valgrind_error & p_error);

        inline
        void add_error_count( uint64_t p_unique
                            , uint32_t p_count
                            );

        inline
        void process_errors(const std::function<void(const valgrind_error&)> & p_func) const;

        inline
        ~valgrind_log_content();

      private:
        std::vector<const valgrind_error *> m_errors;
        std::map<uint64_t, uint32_t> m_error_counts;
    };

    //-------------------------------------------------------------------------
    valgrind_log_content::~valgrind_log_content()
    {
        for(auto l_iter:m_errors)
        {
            delete l_iter;
        }
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_content::add_error(const valgrind_error & p_error)
    {
        m_errors.push_back(&p_error);
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_content::add_error_count( uint64_t p_unique
                                         , uint32_t p_count
                                         )
    {
        m_error_counts.insert(std::pair<uint64_t, uint32_t>(p_unique, p_count));
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_content::process_errors(const std::function<void(const valgrind_error &)> & p_func) const
    {
        for(auto l_iter: m_errors)
        {
            p_func(*l_iter);
        }
    }
}
#endif //VALGRIND_LOG_TOOL_VALGRIND_LOG_CONTENT_H
// EOF