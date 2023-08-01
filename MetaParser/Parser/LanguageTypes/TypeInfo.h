#pragma once

#include "Common/Namespace.h"
#include "Cursor/Cursor.h"
#include "Meta/MetaInfo.h"
#include "Parser/Parser.h"

class TypeInfo
{
public:
    TypeInfo(Cursor const& cursor, Namespace const& current_namespace);

    virtual ~TypeInfo(void) {}

    MetaInfo const& getMetaData(void) const;

    std::string getSourceFile(void) const;

    Namespace getCurrentNamespace() const;

    Cursor& getCurosr();

protected:
    MetaInfo m_meta_data;

    bool m_enabled;

    std::string m_alias_cn;

    Namespace m_namespace;

private:
    // cursor that represents the root of this language type
    Cursor m_root_cursor;
};