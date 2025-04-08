#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include "scene.hpp"
#include "tinyxml2.h"

class XMLParser
{
public:
    static Scene parseFromFile(const std::string &filepath);
};

#endif
