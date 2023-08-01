#pragma once

#include "Runtime/Function/Render/Interface/Rhi.h"
#include "Runtime/Function/Render/Interface/Vulkan/VulkanRhiResource.h"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <functional>
#include <map>
#include <vector>

namespace MarsEngine
{
    class VulkanRHI final : public RHI
    {
    public:
        // initialize
        virtual void initialize(RHIInitInfo init_info) override final;
        virtual void prepareContext() override final;

        // allocate and create
        bool allocateCommandBuffers(RHICommandBufferAllocateInfo const* pAllocateInfo, RHICommandBuffer*& pCommandBuffers) override;
        bool allocateDescriptorSets(RHIDescriptorSetAllocateInfo const* pAllocateInfo, RHIDescriptorSet*& pDescriptorSets) override;
        void createSwapchain() override;
        void recreateSwapchain() override;
        void createSwapchainImageViews() override;
        void createFramebufferImageAndView() override;
        RHISampler* getOrCreateDefaultSampler(RHIDefaultSamplerType type) override;
        RHISampler* getOrCreateMipmapSampler(uint32_t width, uint32_t height) override;
        RHIShader* createShaderModule(std::vector<unsigned char> const& shader_code) override;
        void createBuffer(RHIDeviceSize size, RHIBufferUsageFlags usage, RHIMemoryPropertyFlags properties, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory) override;
        void createBufferAndInitialize(RHIBufferUsageFlags usage, RHIMemoryPropertyFlags properties, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory, RHIDeviceSize size, void* data = nullptr, int datasize = 0) override;
        bool createBufferVMA(VmaAllocator allocator,
            RHIBufferCreateInfo const* pBufferCreateInfo,
            VmaAllocationCreateInfo const* pAllocationCreateInfo,
            RHIBuffer*& pBuffer,
            VmaAllocation* pAllocation,
            VmaAllocationInfo* pAllocationInfo) override;
        bool createBufferWithAlignmentVMA(
            VmaAllocator allocator,
            RHIBufferCreateInfo const* pBufferCreateInfo,
            VmaAllocationCreateInfo const* pAllocationCreateInfo,
            RHIDeviceSize minAlignment,
            RHIBuffer*& pBuffer,
            VmaAllocation* pAllocation,
            VmaAllocationInfo* pAllocationInfo) override;
        void copyBuffer(RHIBuffer* srcBuffer, RHIBuffer* dstBuffer, RHIDeviceSize srcOffset, RHIDeviceSize dstOffset, RHIDeviceSize size) override;
        void createImage(uint32_t image_width, uint32_t image_height, RHIFormat format, RHIImageTiling image_tiling, RHIImageUsageFlags image_usage_flags, RHIMemoryPropertyFlags memory_property_flags,
            RHIImage*& image, RHIDeviceMemory*& memory, RHIImageCreateFlags image_create_flags, uint32_t array_layers, uint32_t miplevels) override;
        void createImageView(RHIImage* image, RHIFormat format, RHIImageAspectFlags image_aspect_flags, RHIImageViewType view_type, uint32_t layout_count, uint32_t miplevels,
            RHIImageView*& image_view) override;
        void createGlobalImage(RHIImage*& image, RHIImageView*& image_view, VmaAllocation& image_allocation, uint32_t texture_image_width, uint32_t texture_image_height, void* texture_image_pixels, RHIFormat texture_image_format, uint32_t miplevels = 0) override;
        void createCubeMap(RHIImage*& image, RHIImageView*& image_view, VmaAllocation& image_allocation, uint32_t texture_image_width, uint32_t texture_image_height, std::array<void*, 6> texture_image_pixels, RHIFormat texture_image_format, uint32_t miplevels) override;
        bool createCommandPool(RHICommandPoolCreateInfo const* pCreateInfo, RHICommandPool*& pCommandPool) override;
        bool createDescriptorPool(RHIDescriptorPoolCreateInfo const* pCreateInfo, RHIDescriptorPool*& pDescriptorPool) override;
        bool createDescriptorSetLayout(RHIDescriptorSetLayoutCreateInfo const* pCreateInfo, RHIDescriptorSetLayout*& pSetLayout) override;
        bool createFence(RHIFenceCreateInfo const* pCreateInfo, RHIFence*& pFence) override;
        bool createFramebuffer(RHIFramebufferCreateInfo const* pCreateInfo, RHIFramebuffer*& pFramebuffer) override;
        bool createGraphicsPipelines(RHIPipelineCache* pipelineCache, uint32_t createInfoCount, RHIGraphicsPipelineCreateInfo const* pCreateInfos, RHIPipeline*& pPipelines) override;
        bool createComputePipelines(RHIPipelineCache* pipelineCache, uint32_t createInfoCount, RHIComputePipelineCreateInfo const* pCreateInfos, RHIPipeline*& pPipelines) override;
        bool createPipelineLayout(RHIPipelineLayoutCreateInfo const* pCreateInfo, RHIPipelineLayout*& pPipelineLayout) override;
        bool createRenderPass(RHIRenderPassCreateInfo const* pCreateInfo, RHIRenderPass*& pRenderPass) override;
        bool createSampler(RHISamplerCreateInfo const* pCreateInfo, RHISampler*& pSampler) override;
        bool createSemaphore(RHISemaphoreCreateInfo const* pCreateInfo, RHISemaphore*& pSemaphore) override;

        // command and command write
        bool waitForFencesPFN(uint32_t fenceCount, RHIFence* const* pFence, RHIBool32 waitAll, uint64_t timeout) override;
        bool resetFencesPFN(uint32_t fenceCount, RHIFence* const* pFences) override;
        bool resetCommandPoolPFN(RHICommandPool* commandPool, RHICommandPoolResetFlags flags) override;
        bool beginCommandBufferPFN(RHICommandBuffer* commandBuffer, const RHICommandBufferBeginInfo* pBeginInfo) override;
        bool endCommandBufferPFN(RHICommandBuffer* commandBuffer) override;
        void cmdBeginRenderPassPFN(RHICommandBuffer* commandBuffer, const RHIRenderPassBeginInfo* pRenderPassBegin, RHISubpassContents contents) override;
        void cmdNextSubpassPFN(RHICommandBuffer* commandBuffer, RHISubpassContents contents) override;
        void cmdEndRenderPassPFN(RHICommandBuffer* commandBuffer) override;
        void cmdBindPipelinePFN(RHICommandBuffer* commandBuffer, RHIPipelineBindPoint pipelineBindPoint, RHIPipeline* pipeline) override;
        void cmdSetViewportPFN(RHICommandBuffer* commandBuffer, uint32_t firstViewport, uint32_t viewportCount, RHIViewport const* pViewports) override;
        void cmdSetScissorPFN(RHICommandBuffer* commandBuffer, uint32_t firstScissor, uint32_t scissorCount, RHIRect2D const* pScissors) override;
        void cmdBindVertexBuffersPFN(
            RHICommandBuffer* commandBuffer,
            uint32_t firstBinding,
            uint32_t bindingCount,
            RHIBuffer* const* pBuffers,
            RHIDeviceSize const* pOffsets) override;
        void cmdBindIndexBufferPFN(RHICommandBuffer* commandBuffer, RHIBuffer* buffer, RHIDeviceSize offset, RHIIndexType indexType) override;
        void cmdBindDescriptorSetsPFN(
            RHICommandBuffer* commandBuffer,
            RHIPipelineBindPoint pipelineBindPoint,
            RHIPipelineLayout* layout,
            uint32_t firstSet,
            uint32_t descriptorSetCount,
            RHIDescriptorSet const* const* pDescriptorSets,
            uint32_t dynamicOffsetCount,
            uint32_t const* pDynamicOffsets) override;
        void cmdDrawIndexedPFN(RHICommandBuffer* commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) override;
        void cmdClearAttachmentsPFN(RHICommandBuffer* commandBuffer, uint32_t attachmentCount, RHIClearAttachment const* pAttachments, uint32_t rectCount, RHIClearRect const* pRects) override;

        bool beginCommandBuffer(RHICommandBuffer* commandBuffer, RHICommandBufferBeginInfo const* pBeginInfo) override;
        void cmdCopyImageToBuffer(RHICommandBuffer* commandBuffer, RHIImage* srcImage, RHIImageLayout srcImageLayout, RHIBuffer* dstBuffer, uint32_t regionCount, RHIBufferImageCopy const* pRegions) override;
        void cmdCopyImageToImage(RHICommandBuffer* commandBuffer, RHIImage* srcImage, RHIImageAspectFlagBits srcFlag, RHIImage* dstImage, RHIImageAspectFlagBits dstFlag, uint32_t width, uint32_t height) override;
        void cmdCopyBuffer(RHICommandBuffer* commandBuffer, RHIBuffer* srcBuffer, RHIBuffer* dstBuffer, uint32_t regionCount, RHIBufferCopy* pRegions) override;
        void cmdDraw(RHICommandBuffer* commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
        void cmdDispatch(RHICommandBuffer* commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) override;
        void cmdDispatchIndirect(RHICommandBuffer* commandBuffer, RHIBuffer* buffer, RHIDeviceSize offset) override;
        void cmdPipelineBarrier(RHICommandBuffer* commandBuffer, RHIPipelineStageFlags srcStageMask,
                                RHIPipelineStageFlags dstStageMask, RHIDependencyFlags dependencyFlags,
                                uint32_t memoryBarrierCount, RHIMemoryBarrier const* pMemoryBarriers,
                                uint32_t bufferMemoryBarrierCount, RHIBufferMemoryBarrier const* pBufferMemoryBarriers,
                                uint32_t imageMemoryBarrierCount, RHIImageMemoryBarrier const* pImageMemoryBarriers) override;
        bool endCommandBuffer(RHICommandBuffer* commandBuffer) override;
        void updateDescriptorSets(uint32_t descriptorWriteCount, RHIWriteDescriptorSet const* pDescriptorWrites, uint32_t descriptorCopyCount, RHICopyDescriptorSet const* pDescriptorCopies) override;
        bool queueSubmit(RHIQueue* queue, uint32_t submitCount, RHISubmitInfo const* pSubmits, RHIFence* fence) override;
        bool queueWaitIdle(RHIQueue* queue) override;
        void resetCommandPool() override;
        void waitForFences() override;
        bool waitForFences(uint32_t fenceCount, RHIFence const* const* pFences, RHIBool32 waitAll, uint64_t timeout);

        // query
        void getPhysicalDeviceProperties(RHIPhysicalDeviceProperties* pProperties) override;
        RHICommandBuffer* getCurrentCommandBuffer() const override;
        RHICommandBuffer* const* getCommandBufferList() const override;
        RHICommandPool* getCommandPoor() const override;
        RHIDescriptorPool* getDescriptorPoor()const override;
        RHIFence* const* getFenceList() const override;
        QueueFamilyIndices getQueueFamilyIndices() const override;
        RHIQueue* getGraphicsQueue() const override;
        RHIQueue* getComputeQueue() const override;
        RHISwapChainDesc getSwapchainInfo() override;
        RHIDepthImageDesc getDepthImageInfo() const override;
        uint8_t getMaxFramesInFlight() const override;
        uint8_t getCurrentFrameIndex() const override;
        void setCurrentFrameIndex(uint8_t index) override;

        // command write
        RHICommandBuffer* beginSingleTimeCommands() override;
        void            endSingleTimeCommands(RHICommandBuffer* command_buffer) override;
        bool prepareBeforePass(std::function<void()> passUpdateAfterRecreateSwapchain) override;
        void submitRendering(std::function<void()> passUpdateAfterRecreateSwapchain) override;
        void pushEvent(RHICommandBuffer* commond_buffer, char const* name, float const* color) override;
        void popEvent(RHICommandBuffer* commond_buffer) override;

        // destory
        virtual ~VulkanRHI() override final;
        void clear() override;
        void clearSwapchain() override;
        void destroyDefaultSampler(RHIDefaultSamplerType type) override;
        void destroyMipmappedSampler() override;
        void destroyShaderModule(RHIShader* shader) override;
        void destroySemaphore(RHISemaphore* semaphore) override;
        void destroySampler(RHISampler* sampler) override;
        void destroyInstance(RHIInstance* instance) override;
        void destroyImageView(RHIImageView* imageView) override;
        void destroyImage(RHIImage* image) override;
        void destroyFramebuffer(RHIFramebuffer* framebuffer) override;
        void destroyFence(RHIFence* fence) override;
        void destroyDevice() override;
        void destroyCommandPool(RHICommandPool* commandPool) override;
        void destroyBuffer(RHIBuffer*& buffer) override;
        void freeCommandBuffers(RHICommandPool* commandPool, uint32_t commandBufferCount, RHICommandBuffer* pCommandBuffers) override;

        // memory
        void freeMemory(RHIDeviceMemory*& memory) override;
        bool mapMemory(RHIDeviceMemory* memory, RHIDeviceSize offset, RHIDeviceSize size, RHIMemoryMapFlags flags, void** ppData) override;
        void unmapMemory(RHIDeviceMemory* memory) override;
        void invalidateMappedMemoryRanges(void* pNext, RHIDeviceMemory* memory, RHIDeviceSize offset, RHIDeviceSize size) override;
        void flushMappedMemoryRanges(void* pNext, RHIDeviceMemory* memory, RHIDeviceSize offset, RHIDeviceSize size) override;
        
        //semaphores
        RHISemaphore*& getTextureCopySemaphore(uint32_t index) override;
    public:
        static uint8_t const k_max_frames_in_flight {3};

        
        RHIQueue* m_graphics_queue{ nullptr };
        RHIQueue* m_compute_queue{ nullptr };

        RHIFormat m_swapchain_image_format{ RHI_FORMAT_UNDEFINED };
        std::vector<RHIImageView*> m_swapchain_imageviews;
        RHIExtent2D m_swapchain_extent;
        RHIViewport m_viewport;
        RHIRect2D m_scissor;

        RHIFormat m_depth_image_format{ RHI_FORMAT_UNDEFINED };
        RHIImageView* m_depth_image_view = new VulkanImageView();

        RHIFence* m_rhi_is_frame_in_flight_fences[k_max_frames_in_flight];

        RHIDescriptorPool* m_descriptor_pool = new VulkanDescriptorPool();

        RHICommandPool* m_rhi_command_pool; 

        RHICommandBuffer* m_command_buffers[k_max_frames_in_flight];
        RHICommandBuffer* m_current_command_buffer = new VulkanCommandBuffer();

        QueueFamilyIndices m_queue_indices;

        GLFWwindow*        m_window {nullptr};
        VkInstance         m_instance {nullptr};
        VkSurfaceKHR       m_surface {nullptr};
        VkPhysicalDevice   m_physical_device {nullptr};
        VkDevice           m_device {nullptr};
        VkQueue            m_present_queue {nullptr};

        VkSwapchainKHR           m_swapchain {nullptr};
        std::vector<VkImage>     m_swapchain_images;

        RHIImage*        m_depth_image = new VulkanImage();
        VkDeviceMemory m_depth_image_memory {nullptr};

        std::vector<VkFramebuffer> m_swapchain_framebuffers;

        // asset allocator use VMA library
        VmaAllocator m_assets_allocator;

        // function pointers
        PFN_vkCmdBeginDebugUtilsLabelEXT _vkCmdBeginDebugUtilsLabelEXT;
        PFN_vkCmdEndDebugUtilsLabelEXT   _vkCmdEndDebugUtilsLabelEXT;
        PFN_vkWaitForFences         _vkWaitForFences;
        PFN_vkResetFences           _vkResetFences;
        PFN_vkResetCommandPool      _vkResetCommandPool;
        PFN_vkBeginCommandBuffer    _vkBeginCommandBuffer;
        PFN_vkEndCommandBuffer      _vkEndCommandBuffer;
        PFN_vkCmdBeginRenderPass    _vkCmdBeginRenderPass;
        PFN_vkCmdNextSubpass        _vkCmdNextSubpass;
        PFN_vkCmdEndRenderPass      _vkCmdEndRenderPass;
        PFN_vkCmdBindPipeline       _vkCmdBindPipeline;
        PFN_vkCmdSetViewport        _vkCmdSetViewport;
        PFN_vkCmdSetScissor         _vkCmdSetScissor;
        PFN_vkCmdBindVertexBuffers  _vkCmdBindVertexBuffers;
        PFN_vkCmdBindIndexBuffer    _vkCmdBindIndexBuffer;
        PFN_vkCmdBindDescriptorSets _vkCmdBindDescriptorSets;
        PFN_vkCmdDrawIndexed        _vkCmdDrawIndexed;
        PFN_vkCmdClearAttachments   _vkCmdClearAttachments;

        // global descriptor pool
        VkDescriptorPool m_vk_descriptor_pool;

        // command pool and buffers
        uint8_t              m_current_frame_index {0};
        VkCommandPool        m_command_pools[k_max_frames_in_flight];
        VkCommandBuffer      m_vk_command_buffers[k_max_frames_in_flight];
        VkSemaphore          m_image_available_for_render_semaphores[k_max_frames_in_flight];
        VkSemaphore          m_image_finished_for_presentation_semaphores[k_max_frames_in_flight];
        RHISemaphore*        m_image_available_for_texturescopy_semaphores[k_max_frames_in_flight];
        VkFence              m_is_frame_in_flight_fences[k_max_frames_in_flight];

        // TODO: set
        VkCommandBuffer   m_vk_current_command_buffer;

        uint32_t m_current_swapchain_image_index;

    private:
        std::vector<char const*> const m_validation_layers {"VK_LAYER_KHRONOS_validation"};
        uint32_t                       m_vulkan_api_version {VK_API_VERSION_1_0};

        std::vector<char const*> m_device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        // default sampler cache
        RHISampler* m_linear_sampler = nullptr;
        RHISampler* m_nearest_sampler = nullptr;
        std::map<uint32_t, RHISampler*> m_mipmap_sampler_map;

    private:
        void createInstance();
        void initializeDebugMessenger();
        void createWindowSurface();
        void initializePhysicalDevice();
        void createLogicalDevice();
        void createCommandPool() override;
        void createCommandBuffers();
        void createDescriptorPool();
        void createSyncPrimitives();
        void createAssetAllocator();

    public:
        bool isPointLightShadowEnabled() override;

    private:
        bool m_enable_validation_Layers { true };
        bool m_enable_debug_utils_label { true };
        bool m_enable_point_light_shadow{ true };

        // used in descriptor pool creation
        uint32_t m_max_vertex_blending_mesh_count{ 256 };
        uint32_t m_max_material_count{ 256 };

        bool                     checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();
        void                     populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        VkDebugUtilsMessengerEXT m_debug_messenger = nullptr;
        VkResult                 createDebugUtilsMessengerEXT(VkInstance                                instance,
                                                              VkDebugUtilsMessengerCreateInfoEXT const* pCreateInfo,
                                                              VkAllocationCallbacks const*              pAllocator,
                                                              VkDebugUtilsMessengerEXT*                 pDebugMessenger);
        void                     destroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                                               VkDebugUtilsMessengerEXT     debugMessenger,
                                                               VkAllocationCallbacks const* pAllocator);

        QueueFamilyIndices      findQueueFamilies(VkPhysicalDevice physical_device);
        bool                    checkDeviceExtensionSupport(VkPhysicalDevice physical_device);
        bool                    isDeviceSuitable(VkPhysicalDevice physical_device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physical_device);

        VkFormat findDepthFormat();
        VkFormat findSupportedFormat(std::vector<VkFormat> const& candidates,
                                     VkImageTiling                tiling,
                                     VkFormatFeatureFlags         features);

        VkSurfaceFormatKHR
        chooseSwapchainSurfaceFormatFromDetails(std::vector<VkSurfaceFormatKHR> const& available_surface_formats);
        VkPresentModeKHR
                   chooseSwapchainPresentModeFromDetails(std::vector<VkPresentModeKHR> const& available_present_modes);
        VkExtent2D chooseSwapchainExtentFromDetails(VkSurfaceCapabilitiesKHR const& capabilities);
    };
} // namespace MarsEngine
