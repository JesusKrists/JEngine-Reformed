#pragma once

namespace JE
{

class IRenderTarget
{
  public:
    IRenderTarget(const IRenderTarget& other) = delete;
    IRenderTarget(IRenderTarget&& other) = delete;
    auto operator=(const IRenderTarget& other) -> IRenderTarget& = delete;
    auto operator=(IRenderTarget&& other) -> IRenderTarget& = delete;

    IRenderTarget() = default;
    virtual ~IRenderTarget() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
};

}  // namespace JE