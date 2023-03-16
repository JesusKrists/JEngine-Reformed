#pragma once

#include <string_view>

#include "Graphics/IRendererAPI.hpp"

namespace JE
{
struct ColorRGBA;
}  // namespace JE

namespace JE::detail  // NOLINT(readability-identifier-naming)
{

class OpenGLRendererAPI : public IRendererAPI
{
  public:
    auto Name() const -> std::string_view override;

    auto SetClearColor(const ColorRGBA& color) -> bool override;
    auto ClearFramebuffer(AttachmentFlags flags) -> bool override;
    auto BindFramebuffer(FramebufferID bufferID) -> bool override;
};

}  // namespace JE::detail