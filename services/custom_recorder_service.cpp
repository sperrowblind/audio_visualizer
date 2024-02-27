#include "custom_recorder_service.hpp"


bool CustomRecorder::onStart() {
    return true;
}

bool CustomRecorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) {
    audio_visualizer.clear();

        // Create points based on the audio amplitude
    for (std::size_t i = 0; i < sampleCount; ++i) {
        float amplitude = std::abs(samples[i]) / 32767.0f; // Normalize to [0, 1]
        audio_visualizer.emplace_back(i * 2, 300 + amplitude * 100);
    }

    return true;
}

void CustomRecorder::onStop() {
    ;
}

const std::vector<sf::Vector2f>& CustomRecorder::get_audio_visualizer() const {
    return audio_visualizer;
}

