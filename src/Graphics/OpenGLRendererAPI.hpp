#pragma once

#include <cstdint>
#include <string_view>

#include "Graphics/IRendererAPI.hpp"

namespace JE
{
    struct RGBA;
}  // namespace JE

namespace JE::detail
{

    class OpenGLRendererAPI final : public IRendererAPI
    {
      public:
        auto Name() const -> std::string_view override;

        auto SetClearColor(const RGBA& color) -> bool override;
        auto ClearFramebuffer(AttachmentFlags flags) -> bool override;
        auto BindFramebuffer(FramebufferID buffer_id) -> bool override;
        auto DrawIndexed(Primitive primitive_type, std::uint32_t index_count, Type index_type) -> bool override;
    };

}  // namespace JE::detail