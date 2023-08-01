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
        Quaternion m_orientation {Quaternion::IDENTITY};
        Vec3       m_position    {Vec3::ZERO};
        Vec3       m_scale       {Vec3::UNIT_SCALE};

        // Cached combined orientation/position/scale.
        Quaternion m_derived_orientation {Quaternion::IDENTITY};
        Vec3       m_derived_position    {Vec3::ZERO};
        Vec3       m_derived_scale       {Vec3::UNIT_SCALE};

        /// The position/orientation/scale to use as a base for keyframe animation
        Vec3       m_initial_position    {Vec3::ZERO};
        Quaternion m_initial_orientation {Quaternion::IDENTITY};
        Vec3       m_initial_scale       {Vec3::UNIT_SCALE};

        Mat4 m_inverse_Tpose;

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

        virtual Quaternion const& getOrientation() const;

        virtual void setOrientation(Quaternion const& q);
        virtual void resetOrientation(void);

        virtual void           setPosition(Vec3 const& pos);
        virtual Vec3 const& getPosition(void) const;

        virtual void           setScale(Vec3 const& scale);
        virtual Vec3 const& getScale(void) const;

        virtual void scale(Vec3 const& scale);

        // Triggers the pNode to update it's combined transforms.
        virtual void updateDerivedTransform(void);

        virtual void translate(Vec3 const& d, TransformSpace relativeTo = TransformSpace::AREN);

        // Rotate the pNode around an aritrary axis using a Quarternion.
        virtual void rotate(Quaternion const& q, TransformSpace relativeTo = TransformSpace::LOCAL);

        // Gets the orientation of the pNode as derived from all parents.
        virtual Quaternion const& _getDerivedOrientation(void) const;
        virtual Vec3 const&    _getDerivedPosition(void) const;
        virtual Vec3 const&    _getDerivedScale(void) const;
        virtual Mat4 const&  _getInverseTpose(void) const;

        // dirty and update
        virtual bool isDirty() const;
        virtual void setDirty();
        virtual void update();

        virtual void setAsInitialPose(void);
        virtual void resetToInitialPose(void);

        virtual Vec3 const&    getInitialPosition(void) const;
        virtual Quaternion const& getInitialOrientation(void) const;
        virtual Vec3 const&    getInitialScale(void) const;
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
