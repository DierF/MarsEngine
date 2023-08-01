#include "Runtime/Function/Animation/Node.h"
#include "Runtime/Core/Math/Math.h"

namespace MarsEngine
{
    //-----------------------------------------------------------------------
    Node::Node(std::string const name) { m_name = name; }
    Node::~Node()
    {
        // clear();
    }
    //-----------------------------------------------------------------------
    void Node::clear() {}
    //-----------------------------------------------------------------------
    Node* Node::getParent(void) const { return m_parent; }

    //-----------------------------------------------------------------------
    void Node::setParent(Node* parent)
    {
        m_parent = parent;
        setDirty();
    }
    //-----------------------------------------------------------------------
    void Node::update()
    {
        // Update transforms from parent
        updateDerivedTransform();
        m_is_dirty = false;
    }
    //-----------------------------------------------------------------------
    void Node::updateDerivedTransform(void)
    {
        if (m_parent)
        {
            // Update orientation
            Quaternion const& parentOrientation = m_parent->_getDerivedOrientation();
            {
                // Combine orientation with that of parent
                m_derived_orientation = parentOrientation * m_orientation;
                m_derived_orientation.normalise();
            }

            // Update scale
            Vec3 const& parentScale = m_parent->_getDerivedScale();
            {
                // Scale own position by parent scale, NB just combine
                // as equivalent axes, no shearing
                m_derived_scale = parentScale * m_scale;
            }

            // Change position vector based on parent's orientation & scale
            m_derived_position = parentOrientation * (parentScale * m_position);

            // Add altered position vector to parents
            m_derived_position = m_derived_position + m_parent->_getDerivedPosition();
        }
        else
        {
            // Root node, no parent
            m_derived_orientation = m_orientation;
            m_derived_position    = m_position;
            m_derived_scale       = m_scale;
        }
    }

    //-----------------------------------------------------------------------
    Quaternion const& Node::getOrientation() const { return m_orientation; }

    //-----------------------------------------------------------------------
    void Node::setOrientation(Quaternion const& q)
    {
        // ASSERT(!q.isNaN() && "Invalid orientation supplied as parameter");
        if (q.isNaN())
        {
            // LOG_ERROR(__FUNCTION__, "Invalid orientation supplied as parameter");
            m_orientation = Quaternion::IDENTITY;
        }
        else
        {
            m_orientation = q;
            m_orientation.normalise();
        }
        setDirty();
    }
    //-----------------------------------------------------------------------
    void Node::resetOrientation(void)
    {
        m_orientation = Quaternion::IDENTITY;
        setDirty();
    }

    //-----------------------------------------------------------------------
    void Node::setPosition(Vec3 const& pos)
    {
        if (pos.isNaN())
        {
            // LOG_ERROR
        }
        m_position = pos;
        setDirty();
    }

    //-----------------------------------------------------------------------
    Vec3 const& Node::getPosition(void) const { return m_position; }
    //-----------------------------------------------------------------------
    void Node::translate(Vec3 const& d, TransformSpace relativeTo)
    {
        switch (relativeTo)
        {
            case TransformSpace::LOCAL:
                // position is relative to parent so transform downwards
                m_position = m_position + m_orientation * d;
                break;
            case TransformSpace::OBJECT:
                // position is relative to parent so transform upwards
                if (m_parent)
                {
                    m_position =
                        m_position + (m_parent->_getDerivedOrientation().inverse() * d) / m_parent->_getDerivedScale();
                }
                else
                {
                    m_position = m_position + d;
                }
                break;
            case TransformSpace::AREN:
                m_position = m_position + d;
                break;
        }
        setDirty();
    }

    //-----------------------------------------------------------------------
    void Node::rotate(Quaternion const& q, TransformSpace relativeTo)
    {
        // Normalize Quaternionernion to avoid drift
        Quaternion qnorm = q;
        qnorm.normalise();

        switch (relativeTo)
        {
            case TransformSpace::AREN:
                // Rotations are normally relative to local axes, transform up
                m_orientation = qnorm * m_orientation;
                break;
            case TransformSpace::OBJECT:
                // Rotations are normally relative to local axes, transform up
                m_orientation = m_orientation * _getDerivedOrientation().inverse() * qnorm * _getDerivedOrientation();
                break;
            case TransformSpace::LOCAL:
                // Note the order of the mult, i.e. q comes after
                m_orientation = m_orientation * qnorm;
                break;
        }
        setDirty();
    }

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    Quaternion const& Node::_getDerivedOrientation(void) const { return m_derived_orientation; }
    //-----------------------------------------------------------------------
    Vec3 const& Node::_getDerivedPosition(void) const { return m_derived_position; }
    //-----------------------------------------------------------------------
    Vec3 const& Node::_getDerivedScale(void) const { return m_derived_scale; }

    Mat4 const& Node::_getInverseTpose(void) const { return m_inverse_Tpose; }

    //-----------------------------------------------------------------------
    void Node::setScale(Vec3 const& inScale)
    {
        if (inScale.isNaN())
        {
            // LOG_ERROR
        }
        m_scale = inScale;
        setDirty();
    }
    //-----------------------------------------------------------------------
    Vec3 const& Node::getScale(void) const { return m_scale; }
    //-----------------------------------------------------------------------
    void Node::scale(Vec3 const& inScale)
    {
        m_scale = m_scale * inScale;
        setDirty();
    }
    //-----------------------------------------------------------------------
    std::string const& Node::getName(void) const { return m_name; }
    //-----------------------------------------------------------------------
    void Node::setAsInitialPose(void)
    {
        m_initial_position    = m_position;
        m_initial_orientation = m_orientation;
        m_initial_scale       = m_scale;
    }
    //-----------------------------------------------------------------------
    void Node::resetToInitialPose(void)
    {
        // m_position = {};// m_initial_position;
        // m_orientation = { {},0,0,0,1 };// m_initial_orientation;
        // m_scale = { {},1,1,1 };//m_initial_scale;
        m_position    = m_initial_position;
        m_orientation = m_initial_orientation;
        m_scale       = m_initial_scale;
        setDirty();
    }
    //-----------------------------------------------------------------------
    Vec3 const& Node::getInitialPosition(void) const { return m_initial_position; }
    //-----------------------------------------------------------------------
    Quaternion const& Node::getInitialOrientation(void) const { return m_initial_orientation; }
    //-----------------------------------------------------------------------
    Vec3 const& Node::getInitialScale(void) const { return m_initial_scale; }
    //-----------------------------------------------------------------------
    void Node::setDirty() { m_is_dirty = true; }

    bool Node::isDirty() const { return m_is_dirty; }
    //---------------------------------------------------------------------
    Bone::Bone() : Node(std::string()) {}

    void Bone::initialize(std::shared_ptr<RawBone> definition, Bone* parent_bone)
    {
        m_definition = definition;

        if (definition)
        {
            m_name = definition->name;
            setOrientation(definition->binding_pose.m_rotation);
            setPosition(definition->binding_pose.m_position);
            setScale(definition->binding_pose.m_scale);
            m_inverse_Tpose = definition->tpose_matrix;
            setAsInitialPose();
        }
        m_parent = parent_bone;
    }

    //---------------------------------------------------------------------
    size_t Bone::getID(void) const
    {
        if (m_definition)
        {
            return m_definition->index;
        }
        return std::numeric_limits<size_t>().max();
    }
} // namespace MarsEngine