#include "graphics/renderer_interface.h"
#include "core/logging/logger.h"

namespace kow {

IRenderer* create_null_renderer();

class RendererManager {
public:
    static RendererManager& instance();

    void initialize();
    void shutdown();
    void begin_frame();
    void end_frame();
    void present();

    IRenderer* renderer() { return renderer_.get(); }
    const IRenderer* renderer() const { return renderer_.get(); }

    void set_renderer(std::unique_ptr<IRenderer> renderer) { renderer_ = std::move(renderer); }

private:
    RendererManager() = default;
    std::unique_ptr<IRenderer> renderer_;
};

#define KOW_RENDERER kow::RendererManager::instance().renderer()

} // namespace kow