#include "Runtime/Function/Physics/PhysicsScene.h"
#include "Runtime/Function/Physics/Jolt/Utils.h"
#include "Runtime/Function/Physics/PhysicsConfig.h"
#include "Runtime/Resource/ResType/Components/RigidBody.h"
#include "Runtime/Core/Base/Macro.h"

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystem.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/CollideShape.h>
#include <Jolt/Physics/Collision/CollisionCollectorImpl.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CompoundShapeVisitors.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include <Jolt/Physics/Collision/ShapeCast.h>
#include <Jolt/Physics/PhysicsSystem.h>

namespace MarsEngine
{
    PhysicsScene::PhysicsScene(Vec3 const& gravity)
    {
        static_assert(s_invalid_rigidbody_id == JPH::BodyID::cInvalidBodyID);

        JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();

        m_physics.m_jolt_physics_system              = new JPH::PhysicsSystem();
        m_physics.m_jolt_broad_phase_layer_interface = new BPLayerInterfaceImpl();

        m_physics.m_jolt_job_system =
            new JPH::JobSystemThreadPool(m_config.m_max_job_count,
                                         m_config.m_max_barrier_count,
                                         static_cast<int>(m_config.m_max_concurrent_job_count));

        // 16M temp memory
        m_physics.m_temp_allocator = new JPH::TempAllocatorImpl(16 * 1024 * 1024);

        m_physics.m_jolt_physics_system->Init(m_config.m_max_body_count,
                                              m_config.m_body_mutex_count,
                                              m_config.m_max_body_pairs,
                                              m_config.m_max_contact_constraints,
                                              *(m_physics.m_jolt_broad_phase_layer_interface),
                                              BroadPhaseCanCollide,
                                              ObjectCanCollide);
        // use the default setting
        m_physics.m_jolt_physics_system->SetPhysicsSettings(JPH::PhysicsSettings());

        m_physics.m_jolt_physics_system->SetGravity(toVec3(gravity));
        m_config.m_gravity = gravity;
    }

    PhysicsScene::~PhysicsScene()
    {
        delete m_physics.m_jolt_physics_system;
        delete m_physics.m_jolt_job_system;
        delete m_physics.m_temp_allocator;
        delete m_physics.m_jolt_broad_phase_layer_interface;

        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    uint32_t PhysicsScene::createRigidBody(Transform const&       global_transform,
                                           RigidBodyComponentRes const& rigidbody_actor_res)
    {
        JPH::BodyInterface& body_interface = m_physics.m_jolt_physics_system->GetBodyInterface();

        struct JPHShapeData
        {
            JPH::Shape*       shape {nullptr};
            Transform   local_transform;
            Vec3        global_position;
            Vec3        global_scale;
            Quaternion  global_rotation;
        };

        std::vector<JPHShapeData> jph_shapes;

        for (size_t shape_index = 0; shape_index < rigidbody_actor_res.m_shapes.size(); shape_index++)
        {
            RigidBodyShape const& shape = rigidbody_actor_res.m_shapes[shape_index];

            Mat4 const shape_global_transform = global_transform.getMatrix()
                * shape.m_local_transform.getMatrix();

            Vec3       global_position, global_scale;
            Quaternion global_rotation;

            shape_global_transform.decomposition(global_position, global_scale, global_rotation);

            JPH::Shape* jph_shape = toShape(shape, global_scale);

            if (jph_shape)
            {
                jph_shapes.push_back(
                    {jph_shape, shape.m_local_transform, global_position, global_scale, global_rotation});
            }
        }

        if (jph_shapes.empty())
        {
            LOG_ERROR("Create JPH Shapes Failed");
            return JPH::BodyID::cInvalidBodyID;
        }

        //$TODO: currently just support static object
        JPH::EMotionType motion_type = JPH::EMotionType::Static;
        JPH::ObjectLayer layer       = Layers::NON_MOVING;

        JPH::Ref<JPH::StaticCompoundShapeSettings> compund_shape_setting = new JPH::StaticCompoundShapeSettings;
        for (JPHShapeData const& shape_data : jph_shapes)
        {
            compund_shape_setting->AddShape(toVec3(shape_data.local_transform.m_position * shape_data.global_scale),
                                            toQuat(shape_data.local_transform.m_rotation),
                                            shape_data.shape);
        }

        JPH::Body* jph_body = body_interface.CreateBody(JPH::BodyCreationSettings(compund_shape_setting,
                                                                                  toVec3(global_transform.m_position),
                                                                                  toQuat(global_transform.m_rotation),
                                                                                  motion_type,
                                                                                  layer));

        if (jph_body == nullptr)
        {
            LOG_ERROR("Create JPH Body Failed");
            for (JPHShapeData const& shape_data : jph_shapes)
            {
                delete shape_data.shape;
            }

            return JPH::BodyID::cInvalidBodyID;
        }

        body_interface.AddBody(jph_body->GetID(), JPH::EActivation::Activate);
        LOG_INFO("Add Body: {}", jph_body->GetID().GetIndexAndSequenceNumber());

        return jph_body->GetID().GetIndexAndSequenceNumber();
    }

    void PhysicsScene::removeRigidBody(uint32_t body_id) { m_pending_remove_bodies.push_back(body_id); }

    void PhysicsScene::updateRigidBodyGlobalTransform(uint32_t body_id, Transform const& global_transform)
    {
        JPH::BodyInterface& body_interface = m_physics.m_jolt_physics_system->GetBodyInterface();

        body_interface.SetPositionAndRotation(JPH::BodyID(body_id),
                                              toVec3(global_transform.m_position),
                                              toQuat(global_transform.m_rotation),
                                              JPH::EActivation::Activate);
    }

    void PhysicsScene::tick(float delta_time)
    {
        float const time_step = 1.f / m_config.m_update_frequency;

        m_physics.m_jolt_physics_system->Update(time_step,
                                                m_physics.m_collision_steps,
                                                m_physics.m_integration_substeps,
                                                m_physics.m_temp_allocator,
                                                m_physics.m_jolt_job_system);

        JPH::BodyInterface& body_interface = m_physics.m_jolt_physics_system->GetBodyInterface();
        for (uint32_t body_id : m_pending_remove_bodies)
        {
            LOG_INFO("Remove Body {}", body_id)
            body_interface.RemoveBody(JPH::BodyID(body_id));
            body_interface.DestroyBody(JPH::BodyID(body_id));
        }
        m_pending_remove_bodies.clear();
    }

    bool PhysicsScene::raycast(Vec3                   ray_origin,
                               Vec3                   ray_directory,
                               float                        ray_length,
                               std::vector<PhysicsHitInfo>& out_hits)
    {
        JPH::NarrowPhaseQuery const& scene_query = m_physics.m_jolt_physics_system->GetNarrowPhaseQuery();

        JPH::RayCast ray;
        ray.mOrigin    = toVec3(ray_origin);
        ray.mDirection = toVec3(ray_directory.normalisedCopy() * ray_length);

        JPH::RayCastSettings raycast_setting;

        JPH::AllHitCollisionCollector<JPH::CastRayCollector> collector;

        scene_query.CastRay(ray, raycast_setting, collector);

        if (!collector.HadHit())
        {
            return false;
        }

        collector.Sort();

        std::vector<JPH::RayCastResult> raycast_results(collector.mHits.begin(), collector.mHits.end());

        out_hits.clear();
        out_hits.resize(raycast_results.size());

        for (size_t index = 0; index < raycast_results.size(); index++)
        {
            JPH::RayCastResult const& cast_result = raycast_results[index];

            PhysicsHitInfo& hit = out_hits[index];
            hit.hit_position    = toVec3(ray.mOrigin + cast_result.mFraction * ray.mDirection);
            hit.hit_distance    = cast_result.mFraction * ray_length;
            hit.body_id         = cast_result.mBodyID.GetIndexAndSequenceNumber();

            // get hit normal
            JPH::BodyLockRead body_lock(m_physics.m_jolt_physics_system->GetBodyLockInterface(), cast_result.mBodyID);
            JPH::Body const&  hit_body = body_lock.GetBody();

            hit.hit_normal =
                toVec3(hit_body.GetWorldSpaceSurfaceNormal(cast_result.mSubShapeID2, toVec3(hit.hit_position)));
        }

        return true;
    }

    bool PhysicsScene::sweep(RigidBodyShape const&        shape,
                             Mat4 const&            shape_transform,
                             Vec3                   sweep_direction,
                             float                        sweep_length,
                             std::vector<PhysicsHitInfo>& out_hits)
    {
        JPH::NarrowPhaseQuery const& scene_query = m_physics.m_jolt_physics_system->GetNarrowPhaseQuery();

        Mat4 const shape_global_transform = shape_transform * shape.m_local_transform.getMatrix();

        Vec3       global_position, global_scale;
        Quaternion global_rotation;

        shape_global_transform.decomposition(global_position, global_scale, global_rotation);

        JPH::Shape* jph_shape = toShape(shape, global_scale);

        if (jph_shape == nullptr)
        {
            return false;
        }

        JPH::ShapeCast shape_cast =
            JPH::ShapeCast::sFromWorldTransform(jph_shape,
                                                JPH::Vec3::sReplicate(1.f),
                                                toMat44(shape_global_transform),
                                                toVec3(sweep_direction.normalisedCopy() * sweep_length));

        JPH::AllHitCollisionCollector<JPH::CastShapeCollector> collector;
        scene_query.CastShape(shape_cast, JPH::ShapeCastSettings(), collector);
        if (!collector.HadHit())
        {
            return false;
        }

        collector.Sort();

        std::vector<JPH::ShapeCastResult> sweep_results(collector.mHits.begin(), collector.mHits.end());

        out_hits.clear();
        out_hits.resize(sweep_results.size());

        for (size_t index = 0; index < sweep_results.size(); index++)
        {
            JPH::ShapeCastResult const& sweep_result = sweep_results[index];

            PhysicsHitInfo& hit = out_hits[index];
            hit.hit_position    = toVec3(sweep_result.mContactPointOn2);
            hit.hit_normal      = toVec3(sweep_result.mPenetrationAxis.Normalized());
            hit.hit_distance    = sweep_result.mFraction * sweep_length;
            hit.body_id         = sweep_result.mBodyID2.GetIndexAndSequenceNumber();
        }

        return true;
    }

    bool PhysicsScene::isOverlap(RigidBodyShape const& shape, Mat4 const& global_transform)
    {
        JPH::NarrowPhaseQuery const& scene_query = m_physics.m_jolt_physics_system->GetNarrowPhaseQuery();

        Mat4 const shape_global_transform = global_transform * shape.m_local_transform.getMatrix();

        Vec3       global_position, global_scale;
        Quaternion global_rotation;

        shape_global_transform.decomposition(global_position, global_scale, global_rotation);

        JPH::Shape* jph_shape = toShape(shape, global_scale);

        if (jph_shape == nullptr)
        {
            return false;
        }

        JPH::AnyHitCollisionCollector<JPH::CollideShapeCollector> collector;
        scene_query.CollideShape(jph_shape,
                                 JPH::Vec3::sReplicate(1.0f),
                                 toMat44(shape_global_transform),
                                 JPH::CollideShapeSettings(),
                                 collector);

        return collector.HadHit();
    }

    void PhysicsScene::getShapeBoundingBoxes(uint32_t body_id,
                                             std::vector<AxisAlignedBox>& out_bounding_boxes) const
    {
        JPH::BodyLockRead body_lock(m_physics.m_jolt_physics_system->GetBodyLockInterface(), JPH::BodyID(body_id));
        JPH::Body const&  body = body_lock.GetBody();

        JPH::TransformedShape body_transformed_shape = body.GetTransformedShape();

        struct Collector : JPH::TransformedShapeCollector
        {
            virtual void AddHit(ResultType const& inResult) override { mShapes.push_back(inResult); }

            std::vector<JPH::TransformedShape> mShapes;
        };

        Collector collector;
        body_transformed_shape.CollectTransformedShapes(body_transformed_shape.GetWorldSpaceBounds(), collector);

        for (JPH::TransformedShape const& ts : collector.mShapes)
        {
            JPH::Shape const* shape = ts.mShape;

            assert(shape->GetType() == JPH::EShapeType::Convex);

            RigidBodyShape rigid_body_shape;

            JPH::AABox jph_bounding_box = ts.GetWorldSpaceBounds();
            Vec3    center           = toVec3(jph_bounding_box.GetCenter());
            Vec3    extent           = toVec3(jph_bounding_box.GetExtent());

            out_bounding_boxes.emplace_back(center, extent);
        }
    }

#ifdef ENABLE_PHYSICS_DEBUG_RENDERER
    void PhysicsScene::drawPhysicsScene(JPH::DebugRenderer* debug_renderer)
    {
#ifdef JPH_DEBUG_RENDERER
        m_physics.m_jolt_physics_system->DrawBodies(JPH::BodyManager::DrawSettings(), debug_renderer);

#endif
    }
#endif

} // namespace MarsEngine