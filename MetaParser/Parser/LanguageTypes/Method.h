#pragma once

#include "TypeInfo.h"

class Class;

class Method : public TypeInfo
{

public:
    Method(Cursor const& cursor, Namespace const& current_namespace, Class* parent = nullptr);

    virtual ~Method(void) {}

    bool shouldCompile(void) const;

public:

    Class* m_parent;

    std::string m_name;

    bool isAccessible(void) const;
};