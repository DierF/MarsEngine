#pragma once

#include "Runtime/Function/Render/DebugDraw/DebugDrawPrimitive.h"
#include "Runtime/Function/Render/DebugDraw/DebugDrawFont.h"
#include "Runtime/Core/Math/MathHeaders.h"

#include <mutex>
#include <list>

namespace MarsEngine
{
    class DebugDrawGroup
    {
    private:
        std::mutex m_mutex;

        std::string m_name;

        std::list<DebugDrawPoint>    m_points;
        std::list<DebugDrawLine>     m_lines;
        std::list<DebugDrawTriangle> m_triangles;
        std::list<DebugDrawQuad>     m_quads;
        std::list<DebugDrawBox>      m_boxes;
        std::list<DebugDrawCylinder> m_cylinders;
        std::list<DebugDrawSphere>   m_spheres;
        std::list<DebugDrawCapsule>  m_capsules;
        std::list<DebugDrawText>     m_texts;

    public:
        virtual ~DebugDrawGroup();
        void initialize();
        void clear();
        void clearData();
        void setName(const std::string& name);
        const std::string& getName() const;
        
        void addPoint(Math::Vec3 const& position, 
                      Math::Vec4 const& color, 
                      float const       life_time = k_debug_draw_one_frame, 
                      bool const        no_depth_test = true);

        void addLine(Math::Vec3 const& point0,
                     Math::Vec3 const& point1,
                     Math::Vec4 const& color0,
                     Math::Vec4 const& color1,
                     float const       life_time = k_debug_draw_one_frame,
                     bool const        no_depth_test = true);

        void addTriangle(Math::Vec3 const& point0,
                         Math::Vec3 const& point1,
                         Math::Vec3 const& point2,
                         Math::Vec4 const& color0,
                         Math::Vec4 const& color1,
                         Math::Vec4 const& color2,
                         float const       life_time = k_debug_draw_one_frame,
                         bool const        no_depth_test = true,
                         FillMode const    fillmod = _FillMode_wireframe);

        void addQuad(Math::Vec3 const& point0,
                     Math::Vec3 const& point1,
                     Math::Vec3 const& point2,
                     Math::Vec3 const& point3,
                     Math::Vec4 const& color0,
                     Math::Vec4 const& color1,
                     Math::Vec4 const& color2,
                     Math::Vec4 const& color3,
                     float const       life_time = k_debug_draw_one_frame,
                     bool const        no_depth_test = true,
                     FillMode const    fillmode = _FillMode_wireframe);

        void addBox(Math::Vec3 const& center_point,
                    Math::Vec3 const& half_extends,
                    Math::Vec4 const& rotate,
                    Math::Vec4 const& color,
                    float const       life_time = k_debug_draw_one_frame,
                    bool const        no_depth_test = true);

        void addSphere(Math::Vec3 const& center, 
                       float const       radius,
                       Math::Vec4 const& color, 
                       float const       life_time, 
                       bool const        no_depth_test = true);

        void addCylinder(Math::Vec3 const& center,
                         float const       radius,
                         float const       height,
                         Math::Vec4 const& rotate,
                         Math::Vec4 const& color,
                         float const       life_time = k_debug_draw_one_frame, 
                         bool const        no_depth_test = true);

        void addCapsule(Math::Vec3 const& center,
                        Math::Vec4 const& rotation,
                        Math::Vec3 const& scale,
                        float const       radius,
                        float const       height,
                        Math::Vec4 const& color,
                        float const       life_time = k_debug_draw_one_frame,
                        bool const        no_depth_test = true);

        void addText(std::string const& content,
                     Math::Vec4 const&  color,
                     Math::Vec3 const&  coordinate,
                     int const          size,
                     bool const         is_screen_text,
                     float const        life_time = k_debug_draw_one_frame);

        void removeDeadPrimitives(float delta_time);
        void mergeFrom(DebugDrawGroup* group);

        size_t getPointCount(bool no_depth_test) const;
        size_t getLineCount(bool no_depth_test) const;
        size_t getTriangleCount(bool no_depth_test) const;
        size_t getUniformDynamicDataCount() const;

        void writePointData(std::vector<DebugDrawVertex>& vertexs, bool no_depth_test);
        void writeLineData(std::vector<DebugDrawVertex>& vertexs, bool no_depth_test);
        void writeTriangleData(std::vector<DebugDrawVertex>& vertexs, bool no_depth_test);
        void writeUniformDynamicDataToCache(std::vector<std::pair<Math::Mat4, Math::Vec4> >& datas);
        void writeTextData(std::vector<DebugDrawVertex>& vertexs, DebugDrawFont* font, Math::Mat4 m_proj_view_matrix);

        size_t getSphereCount(bool no_depth_test) const;
        size_t getCylinderCount(bool no_depth_test) const;
        size_t getCapsuleCount(bool no_depth_test) const;
        size_t getTextCharacterCount() const;
    };
} // namespace MarsEngine