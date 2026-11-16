#pragma once
#include <string>

namespace kow::assets {

struct AssetHandle {
    std::uint32_t index{0};
    std::uint32_t generation{0};
    [[nodiscard]] bool valid() const { return generation != 0; }
};

enum class AssetType { Unknown, Mesh, Texture, Material, Audio, Script, MapData };

// Interface only: no file decoding/loading backend implemented.
class IAssetManager {
public:
    virtual ~IAssetManager() = default;
    [[nodiscard]] virtual std::string name() const = 0;
    virtual AssetHandle register_asset(const std::string& virtual_path, AssetType type) = 0;
    [[nodiscard]] virtual bool is_loaded(AssetHandle handle) const = 0;
    virtual void release(AssetHandle handle) = 0;
    [[nodiscard]] virtual std::size_t registered_count() const = 0;
};

class NullAssetManager final : public IAssetManager {
public:
    [[nodiscard]] std::string name() const override { return "NullAssetManager (no loader backend)"; }
    AssetHandle register_asset(const std::string&, AssetType) override {
        AssetHandle h{next_++, 1};
        ++count_;
        (void)h;
        return AssetHandle{next_ - 1, 1};
    }
    [[nodiscard]] bool is_loaded(AssetHandle) const override { return false; } // no backend loads
    void release(AssetHandle) override { if (count_ > 0) --count_; }
    [[nodiscard]] std::size_t registered_count() const override { return count_; }
private:
    std::uint32_t next_{1};
    std::size_t count_{0};
};

} // namespace kow::assets
