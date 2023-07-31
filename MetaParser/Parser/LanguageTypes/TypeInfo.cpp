#include "Common/Precompiled.h"
#include "TypeInfo.h"

TypeInfo::TypeInfo(Cursor const& cursor, Namespace const& current_namespace)
    : m_meta_data(cursor), m_enabled(m_meta_data.getFlag(NativeProperty::Enable)),
    m_root_cursor(cursor), m_namespace(current_namespace)
{
}

MetaInfo const& TypeInfo::getMetaData(void) const { return m_meta_data; }

std::string TypeInfo::getSourceFile(void) const { return m_root_cursor.getSourceFile(); }

Namespace TypeInfo::getCurrentNamespace() const { return m_namespace; }

Cursor& TypeInfo::getCurosr() { return m_root_cursor; }
