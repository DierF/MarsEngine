#pragma once

#include "Runtime/Core/Meta/Json.h"
#include "Runtime/Core/Meta/Reflection/Reflection.h"

#include <cassert>

namespace MarsEngine
{
    template<typename...>
    inline constexpr bool always_false = false;

    class Serializer
    {
    public:
        template<typename T>
        static Json writePointer(T* instance)
        {
            return Json::object {{"$typeName", Json {"*"}}, {"$context", Serializer::write(*instance)}};
        }

        template<typename T>
        static T*& readPointer(Json const& json_context, T*& instance)
        {
            assert(instance == nullptr);
            std::string type_name = json_context["$typeName"].string_value();
            assert(!type_name.empty());
            if ('*' == type_name[0])
            {
                instance = new T;
                read(json_context["$context"], *instance);
            }
            else
            {
                instance = static_cast<T*>(
                    Reflection::TypeMeta::newFromNameAndJson(type_name, json_context["$context"]).m_instance);
            }
            return instance;
        }

        template<typename T>
        static Json write(Reflection::ReflectionPtr<T> const& instance)
        {
            T*          instance_ptr = static_cast<T*>(instance.operator->());
            std::string type_name    = instance.getTypeName();
            return Json::object {{"$typeName", Json(type_name)},
                                  {"$context", Reflection::TypeMeta::writeByName(type_name, instance_ptr)}};
        }

        template<typename T>
        static T*& read(Json const& json_context, Reflection::ReflectionPtr<T>& instance)
        {
            std::string type_name = json_context["$typeName"].string_value();
            instance.setTypeName(type_name);
            return readPointer(json_context, instance.getPtrReference());
        }

        template<typename T>
        static Json write(T const& instance)
        {

            if constexpr (std::is_pointer<T>::value)
            {
                return writePointer((T)instance);
            }
            else
            {
                static_assert(always_false<T>, "Serializer::write<T> has not been implemented yet!");
                return Json();
            }
        }

        template<typename T>
        static T& read(Json const& json_context, T& instance)
        {
            if constexpr (std::is_pointer<T>::value)
            {
                return readPointer(json_context, instance);
            }
            else
            {
                static_assert(always_false<T>, "Serializer::read<T> has not been implemented yet!");
                return instance;
            }
        }
    };

    // implementation of base types
    template<>
    Json Serializer::write(char const& instance);
    template<>
    char& Serializer::read(Json const& json_context, char& instance);

    template<>
    Json Serializer::write(int const& instance);
    template<>
    int& Serializer::read(Json const& json_context, int& instance);

    template<>
    Json Serializer::write(unsigned int const& instance);
    template<>
    unsigned int& Serializer::read(Json const& json_context, unsigned int& instance);

    template<>
    Json Serializer::write(float const& instance);
    template<>
    float& Serializer::read(Json const& json_context, float& instance);

    template<>
    Json Serializer::write(double const& instance);
    template<>
    double& Serializer::read(Json const& json_context, double& instance);

    template<>
    Json Serializer::write(const bool& instance);
    template<>
    bool& Serializer::read(Json const& json_context, bool& instance);

    template<>
    Json Serializer::write(std::string const& instance);
    template<>
    std::string& Serializer::read(Json const& json_context, std::string& instance);

    // template<>
    // Json Serializer::write(Reflection::object const& instance);
    // template<>
    // Reflection::object& Serializer::read(Json const& json_context, Reflection::object& instance);

    ////////////////////////////////////
    ////sample of generation coder
    ////////////////////////////////////
    // class test_class
    //{
    // public:
    //     int a;
    //     unsigned int b;
    //     std::vector<int> c;
    // };
    // class ss;
    // class jkj;
    // template<>
    // Json Serializer::write(ss const& instance);
    // template<>
    // Json Serializer::write(jkj const& instance);

    /*REFLECTION_TYPE(jkj)
    CLASS(jkj,Fields)
    {
        REFLECTION_BODY(jkj);
        int jl;
    };

    REFLECTION_TYPE(ss)
    CLASS(ss:public jkj,WhiteListFields)
    {
        REFLECTION_BODY(ss);
        int jl;
    };*/

    ////////////////////////////////////
    ////template of generation coder
    ////////////////////////////////////
    // template<>
    // Json Serializer::write(test_class const& instance);
    // template<>
    // test_class& Serializer::read(Json const& json_context, test_class& instance);

    //
    ////////////////////////////////////
} // namespace MarsEngine
