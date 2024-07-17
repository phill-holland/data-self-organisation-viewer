#include "graphics/vulkan/stage.h"
#include "graphics/vulkan/buffer.h"

bool vulkan::stage::create(interfaces::vulkan *device,
                        shader::interfaces::shader *vertex,
                        shader::interfaces::shader *fragment,
                        std::vector<interfaces::mesh*> meshes,
                        ::vulkan::constants *constants)
{
    this->device = device;
    this->vertex = vertex;
    this->fragment = fragment;
    this->meshes = meshes;
    this->constants = constants;

    return true;
}

void vulkan::stage::destroy()
{
    VkDevice vkDevice = device->getDevice();

    vkDestroyDescriptorPool(vkDevice, vkDescriptorPool, nullptr);

    for(int i = vkDescriptorSetLayouts.size() - 1; i >= 0; i--)
    {
        vkDestroyDescriptorSetLayout(vkDevice, vkDescriptorSetLayouts[i], nullptr);    
    }    
}

bool vulkan::stage::createDescriptorPool()
{
    VkDevice vkDevice = device->getDevice();

    std::vector<VkDescriptorPoolSize> sizes =
    {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 }, // count these
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 10 }, // count these
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10 } // count these
    };

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = 0;
    pool_info.maxSets = 10;
    pool_info.poolSizeCount = (uint32_t)sizes.size();
    pool_info.pPoolSizes = sizes.data();

    if(vkCreateDescriptorPool(vkDevice, &pool_info, nullptr, &vkDescriptorPool) != VK_SUCCESS) return false;

    return true;
}

bool vulkan::stage::createDescriptorSets()
{
    int sets = max();

    vkDescriptorSetLayouts.resize(sets + 1);
    vkDescriptorSets.resize(sets + 1);

    VkDevice vkDevice = device->getDevice();

    for(auto &it: buffers)
    {
        auto buffer = std::get<1>(std::get<1>(it).front());

        VkDescriptorSetLayoutBinding bindings = {};
        bindings.binding = 0;
        bindings.descriptorCount = 1;
        if(dynamic_cast<buffer::uniformBuffer*>(buffer)) bindings.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        else bindings.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        info.pNext = nullptr;
        info.bindingCount = std::get<1>(it).size();
        info.flags = 0;
        info.pBindings = &bindings;

        if(vkCreateDescriptorSetLayout(vkDevice, &info, nullptr, &vkDescriptorSetLayouts[std::get<0>(it)]) != VK_SUCCESS) return false;

        VkDescriptorSetAllocateInfo alloc_info = {};
        alloc_info.pNext = nullptr;
        alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info.descriptorPool = vkDescriptorPool;
        alloc_info.descriptorSetCount = 1;
        alloc_info.pSetLayouts = &vkDescriptorSetLayouts[std::get<0>(it)];

        if(vkAllocateDescriptorSets(vkDevice, &alloc_info, &vkDescriptorSets[std::get<0>(it)]) != VK_SUCCESS) return false;
    }

    for(auto &it: textures)
    {
        VkDescriptorSetLayoutBinding binding {};
        binding.binding = 0;
        binding.descriptorCount = 1;
        binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        binding.pImmutableSamplers = nullptr;
        binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        info.pNext = nullptr;
        info.bindingCount = std::get<1>(it).size();
        info.flags = 0;
        info.pBindings = &binding;

        if(vkCreateDescriptorSetLayout(vkDevice, &info, nullptr, &vkDescriptorSetLayouts[std::get<0>(it)]) != VK_SUCCESS) return false;

        VkDescriptorSetAllocateInfo alloc_info = {};
        alloc_info.pNext = nullptr;
        alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info.descriptorPool = vkDescriptorPool;
        alloc_info.descriptorSetCount = 1;
        alloc_info.pSetLayouts = &vkDescriptorSetLayouts[std::get<0>(it)];

        if(vkAllocateDescriptorSets(vkDevice, &alloc_info, &vkDescriptorSets[std::get<0>(it)]) != VK_SUCCESS) return false;
    }

    return true;
}

bool vulkan::stage::createDescriptors()
{
    VkDevice vkDevice = device->getDevice();
    
    for(auto &sets: buffers)
    {
        auto set = std::get<0>(sets);
        auto data = std::get<1>(sets);

        for(auto &buffer: data)
        {
            auto binding = std::get<0>(buffer);
            auto b = std::get<1>(buffer);

            VkDescriptorBufferInfo buffer_info {};
            buffer_info.buffer = b->getBuffer();
            buffer_info.offset = 0;
            buffer_info.range = b->getLength();
            
            VkWriteDescriptorSet setWriteBuffer = {};
            setWriteBuffer.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            setWriteBuffer.pNext = nullptr;
            setWriteBuffer.dstBinding = binding;
            setWriteBuffer.dstSet = vkDescriptorSets[set];
            setWriteBuffer.descriptorCount = 1;
            setWriteBuffer.descriptorType = b->getDescriptorType();
            setWriteBuffer.pBufferInfo = &buffer_info;
                    
            vkUpdateDescriptorSets(vkDevice, 1, &setWriteBuffer, 0, nullptr);
        }
    }

    for(auto &sets: textures)
    {
        auto set = std::get<0>(sets);
        auto data = std::get<1>(sets);

        for(auto &texture: data)
        {         
            auto binding = std::get<0>(texture);
            auto t = std::get<1>(texture);

            VkDescriptorImageInfo samplerInfo {};
            samplerInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            samplerInfo.imageView = t->vkTextureImageView;
            samplerInfo.sampler = t->vkTextureSampler;

            VkWriteDescriptorSet setWriteSampler = {};
            setWriteSampler.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            setWriteSampler.pNext = nullptr;
            setWriteSampler.dstBinding = binding;
            setWriteSampler.dstArrayElement = 0;
            setWriteSampler.dstSet = vkDescriptorSets[set];
            setWriteSampler.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            setWriteSampler.descriptorCount = 1;
            setWriteSampler.pImageInfo = &samplerInfo;

            vkUpdateDescriptorSets(vkDevice, 1, &setWriteSampler, 0, nullptr);
        }
    }

    return true;
}

bool vulkan::stage::BindPipeline(VkCommandBuffer &vkCommandBuffer, VkPipelineLayout &vkPipelineLayout)
{
    vkCmdPushConstants(vkCommandBuffer, vkPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(primatives::matrices::matrix4x4), constants);
        
    int setIndex = 0;
    for(std::vector<VkDescriptorSet>::iterator it = vkDescriptorSets.begin(); it < vkDescriptorSets.end(); ++it)
    {            
        auto set = *it;
        VkDescriptorSet *descriptorSet = &set;
        vkCmdBindDescriptorSets(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,  vkPipelineLayout, setIndex, 1, descriptorSet, 0, nullptr);

        ++setIndex;
    }            

    return setIndex > 0;
}

bool vulkan::stage::Draw(VkCommandBuffer &vkCommandBuffer, VkBuffer vkIndirectBuffer)
{
    int index = 0;
    for(std::vector<interfaces::mesh*>::iterator it = meshes.begin(); it < meshes.end(); ++it)
    {
        interfaces::mesh *temp = (*it);
        VkBuffer vertexBuffers[] = { temp->getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, vertexBuffers, offsets);

        VkDeviceSize indirect_offset = index * sizeof(VkDrawIndirectCommand);
        uint32_t indirect_stride = sizeof(VkDrawIndirectCommand);
                                        
        vkCmdDrawIndirect(vkCommandBuffer, vkIndirectBuffer, indirect_offset, 1, indirect_stride);

        ++index;
    }

    if(vertex->getVertices().has_value())    
        vkCmdDraw(vkCommandBuffer, vertex->getVertices().value(), 1, 0, 0);
}

bool vulkan::stage::push_back(interfaces::buffer* source, shader::bind bindings)
{
    if(source == NULL) return false;
    
    for(auto &it: textures)
    {
        if(it.first == bindings.set) return false;
    }

    if(buffers.find(bindings.set)==buffers.end())
    {
        buffers[bindings.set] = { std::tuple<int,interfaces::buffer*>(bindings.binding,source) };
        return true;
    }   
        
    for(auto it = buffers[bindings.set].begin(); it != buffers[bindings.set].end(); ++it)
    {
        if(std::get<0>(*it) == bindings.binding) return false;

        if((dynamic_cast<buffer::uniformBuffer*>(source))&&(!dynamic_cast<buffer::uniformBuffer*>(std::get<1>(*it))))
            return false;
        else if((dynamic_cast<buffer::storageBuffer*>(source))&&(!dynamic_cast<buffer::storageBuffer*>(std::get<1>(*it))))
            return false;
    }

    buffers[bindings.set].push_back(std::tuple<int,interfaces::buffer*>(bindings.binding,source));
    
    return true;
}

bool vulkan::stage::push_back(texture* source, shader::bind bindings)
{
    if(source == NULL) return false;

    for(auto &it: buffers)
    {
        if(it.first == bindings.set) return false;
    }

    if(textures.find(bindings.set)==textures.end())
    {
        textures[bindings.set] = { std::tuple<int,texture*>(bindings.binding,source) };
        return true;
    }   
    
    for(auto it = textures[bindings.set].begin(); it != textures[bindings.set].end(); ++it)
    {
        if(std::get<0>(*it) == bindings.binding) return false;
    }
    
    textures[bindings.set].push_back(std::tuple<int,texture*>(bindings.binding,source));
    
    return true;
}

size_t vulkan::stage::max()
{
    size_t result = 0;

    for(auto &it: buffers)
    {
        if(it.first > result) result = it.first;
    }

    for(auto &it: textures)
    {
        if(it.first > result) result = it.first;
    }

    return result;
}