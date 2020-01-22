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

#ifndef VALGRIND_LOG_TOOL_HTML_GENERATOR_H
#define VALGRIND_LOG_TOOL_HTML_GENERATOR_H

#include "valgrind_log_content.h"
#include "quicky_exception.h"
#include <fstream>
#include <string>
#include <map>

namespace valgrind_log_tool
{
    class html_generator
    {
      public:

        inline
        html_generator(const std::string & p_output_file_name);

        inline
        ~html_generator();

        inline
        void generate(const valgrind_log_content & p_content);

      private:

        inline
        void generate_html(const valgrind_error & p_error);

        std::ofstream m_file;
        std::map<std::string, unsigned int> m_kinds;
    };

    //-------------------------------------------------------------------------
    html_generator::html_generator(const std::string & p_output_file_name)
    {
        m_file.open(p_output_file_name);
        if(!m_file.is_open())
        {
            throw quicky_exception::quicky_runtime_exception("Unable to create file \"" + p_output_file_name + "\"", __LINE__, __FILE__);
        }
    }

    //-------------------------------------------------------------------------
    html_generator::~html_generator()
    {
        m_file.close();
    }

    //-------------------------------------------------------------------------
    void
    html_generator::generate(const valgrind_log_content & p_content)
    {
        m_kinds.clear();
        const auto l_collect_kind = [&](const valgrind_error & p_error)
        {
            m_kinds.insert(std::pair<std::string, unsigned int>(p_error.get_kind(), m_kinds.size()));
        };

        p_content.process_errors(l_collect_kind);
        std::string l_title = "Valgrind_report";
        m_file << "<!DOCTYPE html>" << std::endl;
        m_file << "<html>" << std::endl;
        m_file << "<head>" << std::endl;
        m_file << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">" << std::endl;
        m_file << "<title>" << l_title << "</title>" << std::endl;
        m_file << "</head>" << std::endl;
        m_file << "<body>" << std::endl;
        m_file << "<H1>" << l_title << "</H1>" << std::endl;
        m_file << "<H2>Kind of encountered errors</H2>" << std::endl;

        m_file << "<ul>" << std::endl;
        for(auto l_iter: m_kinds)
        {
            m_file << "<li>" << l_iter.first << "</li>" << std::endl;
        }
        m_file << "</ul>" << std::endl;

        const auto l_treat_error = [&](const valgrind_error & p_error)
        {
            this->generate_html(p_error);
        };
        p_content.process_errors(l_treat_error);

        m_file << "</body>" << std::endl;
        m_file << "</html>" << std::endl;

    }

    //-------------------------------------------------------------------------
    void
    html_generator::generate_html(const valgrind_error & p_error)
    {
        m_file << "<hr id=\"Error_" << p_error.get_unique() << "\">" << std::endl;
        m_file << "Error <b>" << p_error.get_unique() << "</b>" << std::endl;
        m_file << "<ul>" << std::endl;
        m_file << "<li>Kind : <b>" << p_error.get_kind() << "</b></li>" << std::endl;
        if("" != p_error.get_what())
        {
            m_file << "<li>What : <b>" << p_error.get_what() << "</b></li>" << std::endl;
        }
        if("" != p_error.get_aux_what())
        {
            m_file << "<li>Aux What : <b>" << p_error.get_aux_what() << "</b></li>" << std::endl;
        }
        if(p_error.has_xwhat())
        {
            m_file << "<li>What : <b>" << p_error.get_xwhat().get_text() << "</b></li>" << std::endl;
            m_file << "<ul>" << std::endl;
            m_file << "<li>Leaked bytes : <b>" << p_error.get_xwhat().get_leaked_bytes() << "</b></li>" << std::endl;
            m_file << "<li>Leaked blocks : <b>" << p_error.get_xwhat().get_leaked_blocks() << "</b></li>" << std::endl;
            m_file << "</ul>" << std::endl;
        }
        m_file << "<li>Tid: <b>" << p_error.get_tid() << "</b></li>" << std::endl;
        m_file << "<li>Call stack:</li>" << std::endl;
        m_file << "<table border=1>" << std::endl;
        m_file << "<tr>" << std::endl;
        //m_file << "<th>Ip</th>" << std::endl;
        m_file << "<th>Object</th>" << std::endl;
        m_file << "<th>Function</th>" << std::endl;
        m_file << "<th>Directory</th>" << std::endl;
        m_file << "<th>File</th>" << std::endl;
        m_file << "<th>Line</th>" << std::endl;
        m_file << "</tr>" << std::endl;

        const auto l_treat_frame = [&](const valgrind_frame & p_frame)
        {
            m_file << "<tr>" << std::endl;
            //m_file << "<td>" << p_frame.get_ip() << "</td>" << std::endl;
            m_file << "<td>" << p_frame.get_obj() << "</td>" << std::endl;
            m_file << "<td>" << p_frame.get_fn() << "</td>" << std::endl;
            m_file << "<td>" << p_frame.get_dir() << "</td>" << std::endl;
            m_file << "<td>" << p_frame.get_file() << "</td>" << std::endl;
            if(p_frame.get_line())
            {
                m_file << "<td>" << p_frame.get_line() << "</td>" << std::endl;
            }
            else
            {
                m_file << "<td></td>" << std::endl;
            }
            m_file << "</tr>" << std::endl;
        };

        p_error.process_stack(l_treat_frame);

        m_file << "</table>" << std::endl;
        m_file << "</ul>" << std::endl;
    }

}
#endif //VALGRIND_LOG_TOOL_HTML_GENERATOR_H
// EOF