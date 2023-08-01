#pragma once

#include "Runtime/Function/Global/GlobalContext.h"
#include "Runtime/Function/Render/RenderSystem.h"
#include "Runtime/Function/Render/Interface/Rhi.h"
#include "Runtime/Resource/ConfigManager/ConfigManager.h"

#include <stdexcept>
#include <vector>

namespace MarsEngine
{
    class DebugDrawFont
    {
    public:
        void getCharacterTextureRect(unsigned char const character, float& x1, float& y1, float& x2, float& y2);
        RHIImageView* getImageView() const;
        void inialize();
        void destroy();
        
    private:
        unsigned char const m_range_l = 32, m_range_r = 126;
        int const m_singleCharacterWidth = 32;
        int const m_singleCharacterHeight = 64;
        int const m_numOfCharacterInOneLine = 16;
        int const m_numOfCharacter = (m_range_r - m_range_l + 1);
        int const m_bitmap_w = m_singleCharacterWidth * m_numOfCharacterInOneLine;
        int const m_bitmap_h = m_singleCharacterHeight * ((m_numOfCharacter + m_numOfCharacterInOneLine - 1) / m_numOfCharacterInOneLine);

        RHIImage*        m_font_image = nullptr;
        RHIImageView*    m_font_imageView = nullptr;
        RHIDeviceMemory* m_font_imageMemory = nullptr;
        VmaAllocation    m_allocation;

        void loadFont();
    };
} // namespace MarsEngine