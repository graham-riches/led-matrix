/**
 * \file font_parser.hpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief bdf font file parser
 * \version 0.1
 * \date 2021-02-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include <vector>
#include <map>

/********************************** Types *******************************************/

namespace fonts
{
    struct properties {

    };

    struct character {

    };

    class font {
        public:

        private:
        std::map<char, character> _characters;
    };

};

