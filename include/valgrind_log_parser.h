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

#ifndef VALGRIND_LOG_TOOL_VALGRIND_LOG_PARSER_H
#define VALGRIND_LOG_TOOL_VALGRIND_LOG_PARSER_H

#include "xmlParser.h"
#include "quicky_exception.h"
#include "valgrind_error.h"
#include <string>
#include <map>
#include <cassert>
#include <iostream>
#include <vector>
#include <map>

namespace valgrind_log_tool
{
    class valgrind_log_parser
    {
      public:

        inline
        valgrind_log_parser(const std::string & p_log_name);

        inline
        ~valgrind_log_parser();

      private:

        inline
        void treat(const XMLNode & p_node);

        inline
        void default_treat(const XMLNode & p_node);

        inline
        void ignore_treat(const XMLNode & p_node);

        inline
        void treat_error(const XMLNode & p_node);

        inline
        void treat_errorcounts(const XMLNode & p_node);

        inline
        void treat_unique(const XMLNode & p_node);

        inline
        void treat_tid(const XMLNode & p_node);

        inline
        void treat_what(const XMLNode & p_node);

        inline
        void treat_aux_what(const XMLNode & p_node);

        inline
        void treat_kind(const XMLNode & p_node);

        inline
        void treat_xwhat(const XMLNode & p_node);

        inline
        void treat_frame(const XMLNode & p_node);

        inline
        void treat_ip(const XMLNode & p_node);

        inline
        void treat_obj(const XMLNode & p_node);

        inline
        void treat_fn(const XMLNode & p_node);

        inline
        void treat_dir(const XMLNode & p_node);

        inline
        void treat_file(const XMLNode & p_node);

        inline
        void treat_line(const XMLNode & p_node);

        inline
        void treat_text(const XMLNode & p_node);

        inline
        void treat_leakedbytes(const XMLNode & p_node);

        inline
        void treat_leakedblocks(const XMLNode & p_node);

        inline
        void treat_pair(const XMLNode & p_node);

        inline
        void treat_count(const XMLNode & p_node);

        typedef void (valgrind_log_parser::*t_method)(const XMLNode &);
        typedef std::map<std::string, t_method> t_name_methods;
        t_name_methods m_methods;

        valgrind_error * m_current_error;
        valgrind_xwhat * m_current_xwhat;
        valgrind_frame * m_current_frame;
        std::pair<uint64_t, uint32_t> m_current_pair;

        std::vector<valgrind_error *> m_errors;
        std::map<uint64_t, uint32_t> m_error_counts;
    };

    //-------------------------------------------------------------------------
    valgrind_log_parser::valgrind_log_parser(const std::string & p_log_name)
    : m_current_error(nullptr)
    , m_current_xwhat(nullptr)
    , m_current_frame(nullptr)
    , m_current_pair{0,0}
    {
        XMLResults l_err= {eXMLErrorNone,0,0};
        XMLNode l_node = XMLNode::parseFile( p_log_name.c_str(), "valgrindoutput", &l_err);

        if(eXMLErrorNone != l_err.error)
        {
            if(eXMLErrorFileNotFound == l_err.error)
            {
                throw quicky_exception::quicky_runtime_exception( "File \"" + p_log_name + "\" not found"
                                                                , __LINE__
                                                                , __FILE__
                                                                );
            }
            else
            {
                std::string l_error_msg = XMLNode::getError(l_err.error);
                throw quicky_exception::quicky_logic_exception( "\"" + l_error_msg + "\" at line " + std::to_string(l_err.nLine) + " and column " + std::to_string(l_err.nColumn) + " of file \"" + p_log_name + "\""
                                                              , __LINE__
                                                              , __FILE__
                                                              );
            }
        }

        m_methods.insert(t_name_methods::value_type("valgrindoutput", &valgrind_log_parser::default_treat));
        m_methods.insert(t_name_methods::value_type("protocolversion", &valgrind_log_parser::ignore_treat));
        m_methods.insert(t_name_methods::value_type("protocoltool", &valgrind_log_parser::ignore_treat));
        m_methods.insert(t_name_methods::value_type("preamble", &valgrind_log_parser::ignore_treat));
        m_methods.insert(t_name_methods::value_type("pid", &valgrind_log_parser::ignore_treat));
        m_methods.insert(t_name_methods::value_type("ppid", &valgrind_log_parser::ignore_treat));
        m_methods.insert(t_name_methods::value_type("tool", &valgrind_log_parser::ignore_treat));
        m_methods.insert(t_name_methods::value_type("args", &valgrind_log_parser::ignore_treat));
        m_methods.insert(t_name_methods::value_type("status", &valgrind_log_parser::ignore_treat));
        m_methods.insert(t_name_methods::value_type("error", &valgrind_log_parser::treat_error));
        m_methods.insert(t_name_methods::value_type("unique", &valgrind_log_parser::treat_unique));
        m_methods.insert(t_name_methods::value_type("tid", &valgrind_log_parser::treat_tid));
        m_methods.insert(t_name_methods::value_type("kind", &valgrind_log_parser::treat_kind));
        m_methods.insert(t_name_methods::value_type("what", &valgrind_log_parser::treat_what));
        m_methods.insert(t_name_methods::value_type("auxwhat", &valgrind_log_parser::treat_aux_what));
        m_methods.insert(t_name_methods::value_type("stack", &valgrind_log_parser::default_treat));
        m_methods.insert(t_name_methods::value_type("frame", &valgrind_log_parser::treat_frame));
        m_methods.insert(t_name_methods::value_type("ip", &valgrind_log_parser::treat_ip));
        m_methods.insert(t_name_methods::value_type("obj", &valgrind_log_parser::treat_obj));
        m_methods.insert(t_name_methods::value_type("fn", &valgrind_log_parser::treat_fn));
        m_methods.insert(t_name_methods::value_type("dir", &valgrind_log_parser::treat_dir));
        m_methods.insert(t_name_methods::value_type("file", &valgrind_log_parser::treat_file));
        m_methods.insert(t_name_methods::value_type("line", &valgrind_log_parser::treat_line));
        m_methods.insert(t_name_methods::value_type("xwhat", &valgrind_log_parser::treat_xwhat));
        m_methods.insert(t_name_methods::value_type("text", &valgrind_log_parser::treat_text));
        m_methods.insert(t_name_methods::value_type("leakedbytes", &valgrind_log_parser::treat_leakedbytes));
        m_methods.insert(t_name_methods::value_type("leakedblocks", &valgrind_log_parser::treat_leakedblocks));
        m_methods.insert(t_name_methods::value_type("pair", &valgrind_log_parser::treat_pair));
        m_methods.insert(t_name_methods::value_type("count", &valgrind_log_parser::treat_count));
        m_methods.insert(t_name_methods::value_type("errorcounts", &valgrind_log_parser::treat_errorcounts));
        m_methods.insert(t_name_methods::value_type("suppcounts", &valgrind_log_parser::ignore_treat));

        treat(l_node);
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat(const XMLNode & p_node)
    {
        std::string l_node_name{p_node.getName()};

        t_name_methods::const_iterator l_method_iter = m_methods.find(l_node_name);
        if(m_methods.end() != l_method_iter)
        {
            (this->*(l_method_iter->second))(p_node);
        }
        else
        {
            throw quicky_exception::quicky_logic_exception("Unsupported type of node \"" + l_node_name + "\"", __LINE__, __FILE__);
        }
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::default_treat(const XMLNode & p_node)
    {
        int l_nb_child = p_node.nChildNode();
        for(int l_index = 0; l_index < l_nb_child; ++l_index)
        {
            treat(p_node.getChildNode(l_index));
        }
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::ignore_treat(const XMLNode & p_node)
    {
        // Do nothing
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_error(const XMLNode & p_node)
    {
        m_current_error = new valgrind_error();
        default_treat(p_node);
        m_errors.push_back(m_current_error);
        m_current_error = nullptr;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_errorcounts(const XMLNode & p_node)
    {
        default_treat(p_node);
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_unique(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(1 == p_node.nText());
        uint64_t l_unique = std::stoull(p_node.getText(), nullptr, 0);
        std::string l_parent_name = p_node.getParentNode().getName();
        if("error" == l_parent_name)
        {
            assert(m_current_error);
            m_current_error->set_unique(l_unique);
        }
        else if("pair" == l_parent_name)
        {
            m_current_pair.first = l_unique;
        }
        else
        {
            throw quicky_exception::quicky_logic_exception("Unsupported parent name \"" + l_parent_name + "\" for node \"" + p_node.getName() + "\"", __LINE__, __FILE__);
        }
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_tid(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_error);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("error" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_error->set_tid(std::stoull(p_node.getText(), nullptr, 0));
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_kind(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_error);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("error" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_error->set_kind(p_node.getText());
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_what(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_error);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("error" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_error->set_what(p_node.getText());
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_aux_what(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_error);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("error" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_error->set_aux_what(p_node.getText());
    }

    //-------------------------------------------------------------------------
    valgrind_log_parser::~valgrind_log_parser()
    {
        for(auto l_iter:m_errors)
        {
            delete l_iter;
        }
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_frame(const XMLNode & p_node)
    {
        assert(m_current_error);
        m_current_frame = new valgrind_frame();
        default_treat(p_node);
        m_current_error->add_frame(*m_current_frame);
        m_current_frame = nullptr;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_ip(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_frame);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("frame" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_frame->set_ip(std::stoull(p_node.getText(), nullptr, 0));
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_obj(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_frame);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("frame" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_frame->set_obj(p_node.getText());

    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_fn(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_frame);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("frame" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_frame->set_fn(p_node.getText());

    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_dir(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_frame);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("frame" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_frame->set_dir(p_node.getText());

    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_file(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_frame);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("frame" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_frame->set_file(p_node.getText());

    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_line(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_frame);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("frame" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_frame->set_line(std::stoul(p_node.getText(), nullptr, 0));
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_xwhat(const XMLNode & p_node)
    {
        assert(!m_current_xwhat);
        m_current_xwhat = new valgrind_xwhat();
        default_treat(p_node);
        m_current_error->set_xwhat(*m_current_xwhat);
        m_current_xwhat = nullptr;
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_text(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_xwhat);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("xwhat" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_xwhat->set_text(p_node.getText());
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_leakedbytes(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_xwhat);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("xwhat" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_xwhat->set_leaked_bytes(std::stoul(p_node.getText(), nullptr, 0));
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_leakedblocks(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        assert(m_current_xwhat);
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("xwhat" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_xwhat->set_leaked_blocks(std::stoul(p_node.getText(), nullptr, 0));
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_pair(const XMLNode & p_node)
    {
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("errorcounts" == l_parent_name);
        default_treat(p_node);
        m_error_counts.insert(m_current_pair);
        m_current_pair = {0, 0};
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_count(const XMLNode & p_node)
    {
        assert(!p_node.nChildNode());
        std::string l_parent_name = p_node.getParentNode().getName();
        assert("pair" == l_parent_name);
        assert(1 == p_node.nText());
        m_current_pair.second = std::stoul(p_node.getText(), nullptr, 0);
    }

}
#endif //VALGRIND_LOG_TOOL_VALGRIND_LOG_PARSER_H
// EOF