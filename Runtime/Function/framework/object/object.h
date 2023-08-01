#pragma once

#include "Runtime/Function/Framework/Component/Component.h"
#include "Runtime/Function/Framework/Object/ObjectIdAllocator.h"
#include "Runtime/Resource/ResType/Common/Object.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace MarsEngine
{
    /// GObject : Game Object base class
    class GObject : public std::enable_shared_from_this<GObject>
    {
        typedef std::unordered_set<std::string> TypeNameSet;

    public:
        GObject(GObjectID id) : m_id {id} {}
        virtual ~GObject();

        virtual void tick(float delta_time);

        bool load(ObjectInstanceRes const& object_instance_res);
        void save(ObjectInstanceRes& out_object_instance_res);

        GObjectID getID() const { return m_id; }

        void               setName(std::string name) { m_name = name; }
        std::string const& getName() const { return m_name; }

        bool hasComponent(std::string const& compenent_type_name) const;

        std::vector<Reflection::ReflectionPtr<Component>> getComponents() { return m_components; }

        template<typename TComponent>
        TComponent* tryGetComponent(std::string const& compenent_type_name)
        {
            for (auto& component : m_components)
            {
                if (component.getTypeName() == compenent_type_name)
                {
                    return static_cast<TComponent*>(component.operator->());
                }
            }

            return nullptr;
        }

        template<typename TComponent>
        TComponent const* tryGetComponentConst(std::string const& compenent_type_name) const
        {
            for (auto const& component : m_components)
            {
                if (component.getTypeName() == compenent_type_name)
                {
                    return static_cast<TComponent const*>(component.operator->());
                }
            }
            return nullptr;
        }

#define tryGetComponent(COMPONENT_TYPE) tryGetComponent<COMPONENT_TYPE>(#COMPONENT_TYPE)
#define tryGetComponentConst(COMPONENT_TYPE) tryGetComponentConst<COMPONENT_TYPE const>(#COMPONENT_TYPE)

    protected:
        GObjectID   m_id {k_invalid_gobject_id};
        std::string m_name;
        std::string m_definition_url;

        // we have to use the ReflectionPtr due to that the components need to be reflected 
        // in editor, and it's polymorphism
        std::vector<Reflection::ReflectionPtr<Component>> m_components;
    };
} // namespace MarsEngine
