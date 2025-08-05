/*
   Copyright 2025 Htie digital

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "ConfigParser.h"
#include<algorithm>

std::wstring encrypt::ConfigParser::_trim(const std::wstring& str)
{
    size_t first = str.find_first_not_of(L" \t\r");
    if (std::wstring::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(L" \t\r");
    return str.substr(first, (last - first + 1));
}
