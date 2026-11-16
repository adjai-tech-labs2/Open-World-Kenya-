#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "core/error_handling/result.h"

namespace kow {

enum class AudioFormat : uint8_t {
    WAV = 0,
    OGG = 1,
    FLAC = 2,
    MP3 = 3
};

struct AudioData {
    AudioFormat format = AudioFormat::WAV;
    uint32_t sample_rate = 44100;
    uint16_t channels = 2;
    uint16_t bits_per_sample = 16;
    std::vector<uint8_t> data;
};

struct AudioSource {
    std::string name;
    float volume = 1.0f;
    float pitch = 1.0f;
    float pan = 0.0f; // -1 to 1
    bool loop = false;
    bool playing = false;
    bool spatial = false;
    Vec3 position{0, 0, 0};
    float min_distance = 1.0f;
    float max_distance = 100.0f;
    float current_time = 0.0f;
    float duration = 0.0f;
};

struct AudioListener {
    Vec3 position{0, 0, 0};
    Vec3 forward{0, 0, -1};
    Vec3 up{0, 1, 0};
};

class IAudioSystem {
public:
    virtual ~IAudioSystem() = default;

    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual void update(float delta_time) = 0;

    virtual uint32_t play_sound(const std::string& sound_path, const Vec3& position = {}, bool spatial = false) = 0;
    virtual void stop_sound(uint32_t source_id) = 0;
    virtual void stop_all_sounds() = 0;
    virtual void set_source_volume(uint32_t source_id, float volume) = 0;
    virtual void set_source_position(uint32_t source_id, const Vec3& position) = 0;
    virtual bool is_playing(uint32_t source_id) const = 0;
    virtual void set_listener(const AudioListener& listener) = 0;
    virtual AudioListener listener() const = 0;

    virtual void set_master_volume(float volume) = 0;
    virtual float master_volume() const = 0;
    virtual void set_music_volume(float volume) = 0;
    virtual float music_volume() const = 0;
    virtual void set_sfx_volume(float volume) = 0;
    virtual float sfx_volume() const = 0;

    virtual uint32_t source_count() const = 0;
    virtual bool is_initialized() const = 0;
};

class AudioSystemAdapter : public IAudioSystem {
public:
    using InitializeFn = std::function<bool()>;
    using PlayFn = std::function<uint32_t(const std::string&, const Vec3&, bool)>;

    void set_on_initialize(InitializeFn fn) { on_initialize_ = std::move(fn); }
    void set_on_play(PlayFn fn) { on_play_ = std::move(fn); }

    bool initialize() override {
        if (on_initialize_) return on_initialize_();
        return true;
    }

    void shutdown() override {}
    void update(float delta_time) override {}

    uint32_t play_sound(const std::string& sound_path, const Vec3& position, bool spatial) override {
        if (on_play_) return on_play_(sound_path, position, spatial);
        return 0;
    }

    void stop_sound(uint32_t) override {}
    void stop_all_sounds() override {}
    void set_source_volume(uint32_t, float) override {}
    void set_source_position(uint32_t, const Vec3&) override {}
    bool is_playing(uint32_t) const override { return false; }
    void set_listener(const AudioListener&) override {}
    AudioListener listener() const override { return AudioListener{}; }

    void set_master_volume(float) override {}
    float master_volume() const override { return 1.0f; }
    void set_music_volume(float) override {}
    float music_volume() const override { return 1.0f; }
    void set_sfx_volume(float) override {}
    float sfx_volume() const override { return 1.0f; }

    uint32_t source_count() const override { return 0; }
    bool is_initialized() const override { return false; }
};

class AudioManager {
public:
    static AudioManager& instance();

    void initialize();
    void shutdown();
    void update(float delta_time);

    IAudioSystem* audio() { return audio_.get(); }
    const IAudioSystem* audio() const { return audio_.get(); }

    void set_audio_system(std::unique_ptr<IAudioSystem> audio) { audio_ = std::move(audio); }

private:
    audioManager() = default;
    std::unique_ptr<IAudioSystem> audio_;
};

#define KOW_AUDIO kow::AudioManager::instance().audio()

} // namespace kow