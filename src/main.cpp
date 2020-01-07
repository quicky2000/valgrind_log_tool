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

#include "quicky_exception.h"
#include <iostream>

int main()
{
    try
    {
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
