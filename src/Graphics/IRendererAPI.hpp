#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <utility>

#include "Base.hpp"
#include "Memory.hpp"

namespace JE
{
    struct RGBA;
}  // namespace JE

namespace JE  // NOLINT(readability-identifier-naming)
{

    class IRendererAPI
    {
      public:
        enum class API
        {
            OPENGL
        };

        enum AttachmentFlag : std::uint32_t
        {
            COLOR = Bit(0),
            DEPTH = Bit(1),
            STENCIL = Bit(2)
        };
        using AttachmentFlags = BitFieldType;
        using FramebufferID = std::uint32_t;
        using BufferID = std::uint32_t;
        using ProgramID = std::uint32_t;

        enum class Primitive
        {
            TRIANGLES
        };

        enum class Type
        {
            FLOAT,
            UNSIGNED_SHORT,
            UNSIGNED_INT
        };

        IRendererAPI(const IRendererAPI& other) = delete;
        IRendererAPI(IRendererAPI&& other) = delete;
        auto operator=(const IRendererAPI& other) -> IRendererAPI& = delete;
        auto operator=(IRendererAPI&& other) -> IRendererAPI& = delete;

        IRendererAPI() = default;
        virtual ~IRendererAPI() = default;

        virtual auto Name() const -> std::string_view = 0;

        virtual auto SetClearColor(const RGBA& color) -> bool = 0;
        virtual auto ClearFramebuffer(AttachmentFlags flags) -> bool = 0;
        virtual auto BindFramebuffer(FramebufferID bufferID) -> bool = 0;
        virtual auto DrawIndexed(Primitive primitiveType, std::uint32_t indexCount, Type indexType) -> bool = 0;
    };

    constexpr auto TypeByteCount(IRendererAPI::Type type) -> std::size_t
    {
        switch (type) {
            case IRendererAPI::Type::FLOAT:
                return 4;
                break;
            default:
                return 0;
                break;
        }
    }

    namespace detail  // NOLINT(readability-identifier-naming)
    {

        void SetCustomRendererAPI(Scope<IRendererAPI> rendererAPI);

        template<typename T, typename... Args>
        inline void InjectCustomRendererAPI(Args&&... args)
        {
            static_assert(std::is_base_of_v<IRendererAPI, T>, "Custom RendererAPI has to derive from IRendererAPI");
            SetCustomRendererAPI(CreateScope<T>(std::forward<Args>(args)...));
        }

    }  // namespace detail

    auto RendererAPI() -> IRendererAPI&;

}  // namespace JE