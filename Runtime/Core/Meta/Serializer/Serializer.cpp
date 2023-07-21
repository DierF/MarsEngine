#include "Runtime/Core/Meta/Serializer/Serializer.h"

#include <assert.h>

namespace MarsEngine
{
    template<>
    Json Serializer::write(char const& instance)
    {
        return Json(instance);
    }
    template<>
    char& Serializer::read(Json const& json_context, char& instance)
    {
        assert(json_context.is_number());
        return instance = json_context.number_value();
    }

    template<>
    Json Serializer::write(int const& instance)
    {
        return Json(instance);
    }
    template<>
    int& Serializer::read(Json const& json_context, int& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<int>(json_context.number_value());
    }

    template<>
    Json Serializer::write(unsigned int const& instance)
    {
        return Json(static_cast<int>(instance));
    }
    template<>
    unsigned int& Serializer::read(Json const& json_context, unsigned int& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<unsigned int>(json_context.number_value());
    }

    template<>
    Json Serializer::write(float const& instance)
    {
        return Json(instance);
    }
    template<>
    float& Serializer::read(Json const& json_context, float& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<float>(json_context.number_value());
    }

    template<>
    Json Serializer::write(double const& instance)
    {
        return Json(instance);
    }
    template<>
    double& Serializer::read(Json const& json_context, double& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<float>(json_context.number_value());
    }

    template<>
    Json Serializer::write(bool const& instance)
    {
        return Json(instance);
    }
    template<>
    bool& Serializer::read(Json const& json_context, bool& instance)
    {
        assert(json_context.is_bool());
        return instance = json_context.bool_value();
    }

    template<>
    Json Serializer::write(std::string const& instance)
    {
        return Json(instance);
    }
    template<>
    std::string& Serializer::read(Json const& json_context, std::string& instance)
    {
        assert(json_context.is_string());
        return instance = json_context.string_value();
    }

    // template<>
    // Json Serializer::write(Reflection::object const& instance)
    //{
    // return Json::object();
    //}
    // template<>
    // Reflection::object& Serializer::read(Json const& json_context, Reflection::object& instance)
    //{
    //	return instance;
    //}

    //////////////////////////////////
    // template of generation coder
    //////////////////////////////////

    /*template<>
    Json Serializer::write(ss const& instance)
    {
        return Json();
    }
    template<>
    Json Serializer::write(jkj const& instance)
    {
        return Json();
    }

    template<>
    Json Serializer::write(test_class const& instance)
    {
        Json::array aa;
        for(auto& item: instance.c)
        {
            aa.emplace_back(Serializer::write(item));
        }
        ss jj;
        reflection::object* jjj1 = &jj;
        auto kkk = Serializer::write(jjj1);
        auto jjj = kkk.dump();
        return Json::object{
            {"a",Serializer::write(instance.a)},
            {"b",Serializer::write(instance.b)},
            {"c",aa}
        };
    }
    template<>
    test_class& Serializer::read(Json const& json_context, test_class& instance)
    {
        assert(json_context.is_object());
        Serializer::read(json_context["a"], instance.a);
        Serializer::read(json_context["b"], instance.b);

        assert(json_context["c"].is_array());
        Json::array cc = json_context["c"].array_items();
        instance.c.resize(cc.size());
        for (size_t index=0; index < cc.size();++index)
        {
            Serializer::read(cc[index], instance.c[index]);
        }
        return instance;
    }*/

    ////////////////////////////////////
} // namespace MarsEngine