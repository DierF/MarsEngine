#pragma once

#include "Runtime/Core/Math/Math.h"
#include "Runtime/Core/Math/Transform.h"
#include "Runtime/Resource/ResType/Data/SkeletonData.h"

namespace MarsEngine
{
    class Node
    {
    public:
        // Enumeration denoting the spaces which a transform can be relative to.
        enum class TransformSpace
        {
            /// Transform is relative to the local space
            LOCAL,
            /// Transform is relative to the space of the parent pNode
            AREN,
            /// Transform is relative to object space
            OBJECT
        };
#ifdef _DEBUG
    public:
#else
    protected:
#endif
        Node* m_parent {nullptr};

        std::string m_name;

        /// Stores the orientation/position/scale of the pNode relative to it's parent.
        Math::Quaternion m_orientation {Math::Quaternion::IDENTITY};
        Math::Vec3       m_position    {Math::Vec3::ZERO};
        Math::Vec3       m_scale       {Math::Vec3::UNIT_SCALE};

        // Cached combined orientation/position/scale.
        Math::Quaternion m_derived_orientation {Math::Quaternion::IDENTITY};
        Math::Vec3       m_derived_position    {Math::Vec3::ZERO};
        Math::Vec3       m_derived_scale       {Math::Vec3::UNIT_SCALE};

        /// The position/orientation/scale to use as a base for keyframe animation
        Math::Vec3       m_initial_position    {Math::Vec3::ZERO};
        Math::Quaternion m_initial_orientation {Math::Quaternion::IDENTITY};
        Math::Vec3       m_initial_scale       {Math::Vec3::UNIT_SCALE};

        Math::Mat4 m_inverse_Tpose;

        bool m_is_dirty {true};

    protected:
        /// Only available internally - notification of parent.
        virtual void setParent(Node* parent);

    public:
        Node(std::string const name);
        virtual ~Node();
        void               clear();
        std::string const& getName(void) const;
        virtual Node*      getParent(void) const;

        virtual Math::Quaternion const& getOrientation() const;

        virtual void setOrientation(Math::Quaternion const& q);
        virtual void resetOrientation(void);

        virtual void           setPosition(Math::Vec3 const& pos);
        virtual Math::Vec3 const& getPosition(void) const;

        virtual void           setScale(Math::Vec3 const& scale);
        virtual Math::Vec3 const& getScale(void) const;

        virtual void scale(Math::Vec3 const& scale);

        // Triggers the pNode to update it's combined transforms.
        virtual void updateDerivedTransform(void);

        virtual void translate(Math::Vec3 const& d, TransformSpace relativeTo = TransformSpace::AREN);

        // Rotate the pNode around an aritrary axis using a Quarternion.
        virtual void rotate(Math::Quaternion const& q, TransformSpace relativeTo = TransformSpace::LOCAL);

        // Gets the orientation of the pNode as derived from all parents.
        virtual Math::Quaternion const& _getDerivedOrientation(void) const;
        virtual Math::Vec3 const&    _getDerivedPosition(void) const;
        virtual Math::Vec3 const&    _getDerivedScale(void) const;
        virtual Math::Mat4 const&  _getInverseTpose(void) const;

        // dirty and update
        virtual bool isDirty() const;
        virtual void setDirty();
        virtual void update();

        virtual void setAsInitialPose(void);
        virtual void resetToInitialPose(void);

        virtual Math::Vec3 const&    getInitialPosition(void) const;
        virtual Math::Quaternion const& getInitialOrientation(void) const;
        virtual Math::Vec3 const&    getInitialScale(void) const;
    };

    class Bone : public Node
    {
        friend class LoDSkeleton;

    protected:
        std::shared_ptr<RawBone> m_definition {};
        // physics simulation and actor status

    public:
        Bone();
        void initialize(std::shared_ptr<RawBone> definition, Bone* parent_bone);

        size_t getID(void) const;
    };
} // namespace MarsEngine
