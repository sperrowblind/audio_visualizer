#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>

class CustomRecorder : public sf::SoundRecorder {
    public:
        virtual bool onStart() override;

        virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override;

        virtual void onStop() override;

        const std::vector<sf::Vector2f>& get_audio_visualizer() const;

    private:
        std::vector<sf::Vector2f> audio_visualizer;

};

