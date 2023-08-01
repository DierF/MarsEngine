#pragma once

#include "Common/Precompiled.h"
#include "Common/Namespace.h"
#include "Common/SchemaModule.h"
#include "Cursor/Cursor.h"
#include "Generator/Generator.h"
#include "TemplateManager/TemplateManager.h"

class Class;

class MetaParser
{
public:
    static void prepare(void);

    MetaParser(std::string const project_input_file,
               std::string const include_file_path,
               std::string const include_path,
               std::string const include_sys,
               std::string const module_name,
               bool              is_show_errors);
    ~MetaParser(void);
    void finish(void);
    int  parse(void);
    void generateFiles(void);

private:
    std::string m_project_input_file;

    std::vector<std::string> m_work_paths;
    std::string              m_module_name;
    std::string              m_sys_include;
    std::string              m_source_include_file_name;

    CXIndex           m_index;
    CXTranslationUnit m_translation_unit;

    std::unordered_map<std::string, std::string>  m_type_table;
    std::unordered_map<std::string, SchemaMoudle> m_schema_modules;

    std::vector<char const*> arguments = {{"-x",
                                           "c++",
                                           "-std=c++11",
                                           "-D__REFLECTION_PARSER__",
                                           "-DNDEBUG",
                                           "-D__clang__",
                                           "-w",
                                           "-MG",
                                           "-M",
                                           "-ferror-limit=0",
                                           "-o clangLog.txt"}};
    std::vector<Generator::GeneratorInterface*> m_generators;

    bool m_is_show_errors;

private:
    bool        parseProject(void);
    void        buildClassAST(Cursor const& cursor, Namespace& current_namespace);
    std::string getIncludeFile(std::string name);
};
