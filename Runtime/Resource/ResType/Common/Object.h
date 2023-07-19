#pragma once

#include <string>
#include <vector>

namespace MarsEngine
{
    class Component;

    //REFLECTION_TYPE(ComponentDefinitionRes)
        //CLASS(ComponentDefinitionRes, Fields)
    class ComponentDefinitionRes
    {
        //REFLECTION_BODY(ComponentDefinitionRes);

    public:
        std::string m_type_name;
        std::string m_component;
    };

    //REFLECTION_TYPE(ObjectDefinitionRes)
        //CLASS(ObjectDefinitionRes, Fields)
    class ObjectDefinitionRes
    {
        //REFLECTION_BODY(ObjectDefinitionRes);

    public:
        //std::vector<Reflection::ReflectionPtr<Component>> m_components;
    };

    //REFLECTION_TYPE(ObjectInstanceRes)
        //CLASS(ObjectInstanceRes, Fields)
    class ObjectInstanceRes
    {
        //REFLECTION_BODY(ObjectInstanceRes);

    public:
        std::string              m_name;
        std::string              m_definition;

        //std::vector<Reflection::ReflectionPtr<Component>> m_instanced_components;
    };
} // namespace MarsEngine