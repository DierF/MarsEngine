#pragma once

#include "Common/Namespace.h"
#include "Cursor/Cursor.h"

namespace Utils
{
    void toString(CXString const& str, std::string& output);

    std::string getQualifiedName(CursorType const& type);

    std::string getQualifiedName(std::string const& display_name, Namespace const& current_namespace);

    std::string getQualifiedName(Cursor const& cursor, Namespace const& current_namespace);

    std::string formatQualifiedName(std::string& source_string);

    fs::path makeRelativePath(fs::path const& from, fs::path const& to);

    void fatalError(std::string const& error);

    template<typename A, typename B>
    bool rangeEqual(A startA, A endA, B startB, B endB);

    std::vector<std::string> split(std::string input, std::string pat);

    std::string getFileName(std::string path);

    std::string getNameWithoutFirstM(std::string& name);

    std::string getTypeNameWithoutNamespace(CursorType const& type);

    std::string getNameWithoutContainer(std::string name);

    std::string getStringWithoutQuot(std::string input);

    std::string replace(std::string& source_string, std::string sub_string, std::string const new_string);

    std::string replace(std::string& source_string, char taget_char, char const new_char);

    std::string toUpper(std::string& source_string);

    std::string join(std::vector<std::string> context_list, std::string separator);

    std::string trim(std::string& source_string, std::string const trim_chars);

    std::string loadFile(std::string path);

    void saveFile(std::string const& outpu_string, std::string const& output_file);

    void replaceAll(std::string& resource_str, std::string sub_str, std::string new_str);

    unsigned long formatPathString(std::string const& path_string, std::string& out_string);

    std::string convertNameToUpperCamelCase(std::string const& name, std::string pat);

} // namespace Utils

#include "MetaUtils.hpp"
