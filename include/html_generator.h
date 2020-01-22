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

        /**
         * Compute kind id that will be uased as local anchor
         * @param p_kind string representing error kind
         * @return string representing kind id
         */
        inline
        std::string get_kind_id(const std::string & p_kind) const;

        inline
        std::string get_kind_link(const std::string & p_kind) const;

        /**
         * Compute file id that will be used as local anchor
         * @param p_file string representing error frame file
         * @return string representing file id
         */
        inline
        std::string get_file_id(const std::string & p_file) const;

        inline
        std::string get_file_link(const std::string & p_kind) const;

        inline
        std::string get_error_id(const valgrind_error & p_error) const;

        inline
        std::string get_error_link(const valgrind_error & p_error) const;

        inline
        void collect_kind_info(const valgrind_log_content & p_content);

        inline
        void collect_file_info(const valgrind_log_content & p_content);

        inline
        void generate_html(const valgrind_error & p_error);

        inline
        void generate_kinds_html(const valgrind_log_content & p_content);

        inline
        void generate_files_html(const valgrind_log_content & p_content);

        std::ofstream m_file;

        /**
         * List of kind and associated id
         */
        std::map<std::string, unsigned int> m_kinds;

        /**
         * Kind sorted per number of occurence
         */
        std::multimap<unsigned int, std::string> m_sorted_kinds;

        /**
         * List of files and associated id
         */
        std::map<std::string, unsigned int> m_files;

        /**
         * files sorted per number of occurence
         */
        std::multimap<unsigned int, std::string> m_sorted_files;

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
        std::string l_title = "Valgrind_report";
        m_file << "<!DOCTYPE html>" << std::endl;
        m_file << "<html>" << std::endl;
        m_file << "<head>" << std::endl;
        m_file << R"(<meta http-equiv="Content-Type" content="text/html; charset=utf-8">)" << std::endl;
        m_file << "<title>" << l_title << "</title>" << std::endl;
        m_file << "</head>" << std::endl;
        m_file << "<body>" << std::endl;
        m_file << "<H1>" << l_title << "</H1>" << std::endl;

        collect_kind_info(p_content);

        m_file << "<H2>Encountered kinds</H2>" << std::endl;
        m_file << "<ul>" << std::endl;
        for(const auto & l_iter: m_sorted_kinds)
        {
            m_file << "<li>" << get_kind_link(l_iter.second) << " : " << l_iter.first << "</li>" << std::endl;
        }
        m_file << "</ul>" << std::endl;

        collect_file_info(p_content);

        m_file << "<H2>Encountered files</H2>" << std::endl;
        m_file << "<ul>" << std::endl;
        for(const auto & l_iter: m_sorted_files)
        {
            m_file << "<li>" << get_file_link(l_iter.second) << " : " << l_iter.first << "</li>" << std::endl;
        }
        m_file << "</ul>" << std::endl;

        generate_files_html(p_content);
        generate_kinds_html(p_content);

        const auto l_treat_error = [&](const valgrind_error & p_error)
        {
            this->generate_html(p_error);
        };
        m_file << "<H2>Errors</H2>" << std::endl;
        p_content.process_errors(l_treat_error);

        m_file << "</body>" << std::endl;
        m_file << "</html>" << std::endl;

    }

    //-------------------------------------------------------------------------
    void
    html_generator::generate_html(const valgrind_error & p_error)
    {
        m_file << "<hr id=\"" << get_error_id(p_error) << "\">" << std::endl;
        m_file << "Error <b>" << p_error.get_unique() << "</b>" << std::endl;
        m_file << "<ul>" << std::endl;
        m_file << "<li>Kind : <b>" << get_kind_link(p_error.get_kind()) << "</b></li>" << std::endl;
        if(!p_error.get_what().empty())
        {
            m_file << "<li>What : <b>" << p_error.get_what() << "</b></li>" << std::endl;
        }
        if(!p_error.get_aux_what().empty())
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
            m_file << "<td>" << (!p_frame.get_file().empty() ? get_file_link(p_frame.get_file()) : "" )<< "</td>" << std::endl;
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

    //-------------------------------------------------------------------------
    std::string
    html_generator::get_kind_id(const std::string & p_kind) const
    {
        auto l_iter = m_kinds.find(p_kind);
        assert(m_kinds.end() != l_iter);
        return "Kind_" + std::to_string(l_iter->second);
    }

    //-------------------------------------------------------------------------
    std::string
    html_generator::get_kind_link(const std::string & p_kind) const
    {
        return "<a href=\"#" + get_kind_id(p_kind) + "\">" + p_kind + "</a>";
    }

    //-------------------------------------------------------------------------
    std::string
    html_generator::get_file_id(const std::string & p_file) const
    {
        auto l_iter = m_files.find(p_file);
        assert(m_files.end() != l_iter);
        return "File_" + std::to_string(l_iter->second);
    }

    //-------------------------------------------------------------------------
    std::string
    html_generator::get_file_link(const std::string & p_kind) const
    {
        return "<a href=\"#" + get_file_id(p_kind) + "\">" + p_kind + "</a>";
    }

    //-------------------------------------------------------------------------
    std::string
    html_generator::get_error_id(const valgrind_error & p_error) const
    {
        return "Error_" + std::to_string(p_error.get_unique());
    }

    //-------------------------------------------------------------------------
    std::string
    html_generator::get_error_link(const valgrind_error & p_error) const
    {
        return "<a href=\"#" + get_error_id(p_error) + "\">" + std::to_string(p_error.get_unique()) + "</a>";
    }

    //-------------------------------------------------------------------------
    void
    html_generator::generate_kinds_html(const valgrind_log_content & p_content)
    {
        m_file << "<H2>Errors per kind</H2>" << std::endl;
        for(auto l_iter: m_sorted_kinds)
        {
            bool l_first = true;
            m_file << "<hr id=\"" << get_kind_id(l_iter.second) << "\">" << std::endl;
            m_file << "Errors of kind <b>" << l_iter.second << "</b>" << std::endl;
            const auto l_collecter_errors_per_kind = [&](const valgrind_error & p_error)
            {
                if(l_iter.second == p_error.get_kind())
                {
                    if(!l_first)
                    {
                        m_file << ", ";
                    }
                    else
                    {
                        l_first = false;
                    }
                    m_file << get_error_link(p_error);
                }
            };
            m_file << "<ul><li>" << std::endl;
            p_content.process_errors(l_collecter_errors_per_kind);
            m_file << "</li></ul>" << std::endl;
        }

    }

    //-------------------------------------------------------------------------
    void
    html_generator::collect_kind_info(const valgrind_log_content & p_content)
    {
        m_kinds.clear();
        const auto l_collect_kind = [&](const valgrind_error & p_error)
        {
            m_kinds.insert(std::pair<std::string, unsigned int>(p_error.get_kind(), m_kinds.size()));
        };
        p_content.process_errors(l_collect_kind);

        /**
         * Number of occurence per kind
         */
        std::map<std::string, unsigned int> l_kind_number;
        l_kind_number.clear();
        for(const auto & l_iter: m_kinds)
        {
            l_kind_number[l_iter.first] = 0;
        }

        const auto l_count_kind = [&](const valgrind_error & p_error)
        {
            l_kind_number[p_error.get_kind()]++;
        };

        p_content.process_errors(l_count_kind);

        m_sorted_kinds.clear();
        for(const auto & l_iter:l_kind_number)
        {
            m_sorted_kinds.insert(std::pair<unsigned int, std::string>(l_iter.second, l_iter.first));
        }
    }

    //-------------------------------------------------------------------------
    void
    html_generator::collect_file_info(const valgrind_log_content & p_content)
    {
        m_files.clear();
        std::map<std::string, unsigned int> l_file_number;
        l_file_number.clear();
        const auto l_collected_files_from_frame = [&](const valgrind_frame & p_frame)
        {
            const std::string & l_file_name = p_frame.get_file();
            if(!l_file_name.empty())
            {
                m_files.insert(std::pair<std::string, unsigned int>(l_file_name, m_files.size()));
                auto l_counter_iter = l_file_number.find(l_file_name);
                if(l_file_number.end() == l_counter_iter)
                {
                    l_file_number.insert(make_pair(l_file_name, 1));
                }
                else
                {
                    ++(l_counter_iter->second);
                }
            }
        };

        const auto l_collect_files_from_error = [&](const valgrind_error & p_error)
        {
            p_error.process_stack(l_collected_files_from_frame);
        };
        p_content.process_errors(l_collect_files_from_error);

        m_sorted_files.clear();
        for(const auto & l_iter:l_file_number)
        {
            m_sorted_files.insert(std::pair<unsigned int, std::string>(l_iter.second, l_iter.first));
        }
    }

    //-------------------------------------------------------------------------
    void
    html_generator::generate_files_html(const valgrind_log_content & p_content)
    {
        m_file << "<H2>Errors per files</H2>" << std::endl;
        for(auto l_iter: m_sorted_files)
        {
            bool l_first = true;
            m_file << "<hr id=\"" << get_file_id(l_iter.second) << "\">" << std::endl;
            m_file << "Errors whose call stack mention file <b>" << l_iter.second << "</b>" << std::endl;

            bool l_contain = false;
            const auto l_frame_contain_file = [&](const valgrind_frame & p_frame)
            {
                if(l_iter.second == p_frame.get_file())
                {
                    l_contain = true;
                }
            };
            const auto l_collecter_errors_per_file = [&](const valgrind_error & p_error)
            {
                l_contain = false;
                p_error.process_stack(l_frame_contain_file);
                if(l_contain)
                {
                    if(!l_first)
                    {
                        m_file << ", ";
                    }
                    else
                    {
                        l_first = false;
                    }
                    m_file << get_error_link(p_error);
                }
            };
            m_file << "<ul><li>" << std::endl;
            p_content.process_errors(l_collecter_errors_per_file);
            m_file << "</li></ul>" << std::endl;
        }

    }

}
#endif //VALGRIND_LOG_TOOL_HTML_GENERATOR_H
// EOF