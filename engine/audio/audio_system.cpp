#include "audio/audio_system.h"
#include "core/logging/logger.h"

namespace kow {

AudioManager& AudioManager::instance() {
    static AudioManager manager;
    return manager;
}

void AudioManager::initialize() {
    if (!audio_) {
        audio_ = std::make_unique<AudioSystemAdapter>();
    }
    audio_->initialize();
    KOW_LOG_INFO("audio", "Audio system initialized");
}

void AudioManager::shutdown() {
    if (audio_) {
        audio_->shutdown();
        KOW_LOG_INFO("audio", "Audio system shutdown");
    }
}

void AudioManager::update(float delta_time) {
    if (audio_) {
        audio_->update(delta_time);
    }
}

} // namespace kow