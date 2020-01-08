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
#include <string>

namespace valgrind_log_tool
{
    class valgrind_log_parser
    {
      public:

        inline
        valgrind_log_parser(const std::string & p_log_name);

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

      private:
    };

    //-------------------------------------------------------------------------
    valgrind_log_parser::valgrind_log_parser(const std::string & p_log_name)
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
        treat(l_node);
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat(const XMLNode & p_node)
    {
        std::string l_node_name{p_node.getName()};
        if("valgrindoutput" == l_node_name)
        {
            default_treat(p_node);
        }
        else if("protocolversion" == l_node_name)
        {
            ignore_treat(p_node);
        }
        else if("protocoltool" == l_node_name)
        {
            ignore_treat(p_node);
        }
        else if("preamble" == l_node_name)
        {
            ignore_treat(p_node);
        }
        else if("pid" == l_node_name)
        {
            ignore_treat(p_node);
        }
        else if("ppid" == l_node_name)
        {
            ignore_treat(p_node);
        }
        else if("tool" == l_node_name)
        {
            ignore_treat(p_node);
        }
        else if("args" == l_node_name)
        {
            ignore_treat(p_node);
        }
        else if("status" == l_node_name)
        {
            ignore_treat(p_node);
        }
        else if("error" == l_node_name)
        {
            treat_error(p_node);
        }
        else if("errorcounts" == l_node_name)
        {
            treat_errorcounts(p_node);
        }
        else if("suppcounts" == l_node_name)
        {
            ignore_treat(p_node);
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
        //default_treat(p_node);
    }

    //-------------------------------------------------------------------------
    void
    valgrind_log_parser::treat_errorcounts(const XMLNode & p_node)
    {

    }
}
#endif //VALGRIND_LOG_TOOL_VALGRIND_LOG_PARSER_H
// EOF