#pragma once

#include "Cursor/Cursor.h"

class MetaInfo
{
public:
    MetaInfo(Cursor const& cursor);

    std::string getProperty(std::string const& key) const;

    bool getFlag(std::string const& key) const;

private:
    typedef std::pair<std::string, std::string> Property;

    std::unordered_map<std::string, std::string> m_properties;

private:
    std::vector<Property> extractProperties(Cursor const& cursor) const;
};