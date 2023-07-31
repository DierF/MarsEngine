#pragma once

#include "Runtime/Core/Math/Vector2.h"

#include <memory>

namespace MarsEngine
{
    class EditorUI;
    class MarsEngine;

    class MarsEditor 
    {
        friend class EditorUI;

    public:
        MarsEditor();
        virtual ~MarsEditor();

        void initialize(MarsEngine* engine_runtime);
        void clear();

        void run();

    protected:
        std::shared_ptr<EditorUI> m_editor_ui;
        MarsEngine* m_engine_runtime{ nullptr };
    };
} // namespace MarsEngine
