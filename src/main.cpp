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

#include "valgrind_log_parser.h"
#include "quicky_exception.h"
#include <iostream>
#include <fstream>
#include <cassert>

int main(int p_argc, char ** p_argv)
{
    try
    {
        assert(2 == p_argc);
        std::string l_file_name{p_argv[1]};
        std::ifstream l_input_file;
        l_input_file.open(l_file_name);
        if(!l_input_file.is_open())
        {
            throw quicky_exception::quicky_logic_exception("Unable to open file \"" + l_file_name +"\"", __LINE__, __FILE__);
        }
        l_input_file.close();

        valgrind_log_tool::valgrind_log_content l_content;
        valgrind_log_tool::valgrind_log_parser l_parser(l_file_name, l_content);

    }
    catch(const quicky_exception::quicky_logic_exception & e)
    {
        std::cout << "ERROR : " << e.what() << std::endl;
    }
    catch(const quicky_exception::quicky_runtime_exception & e)
    {
        std::cout << "ERROR : " << e.what() << std::endl;
    }
}
// EOF
