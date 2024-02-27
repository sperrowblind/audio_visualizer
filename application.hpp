#include "models/wav.hpp"
#include "services/custom_recorder_service.hpp"
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#ifndef AUDIOAPPLICATION_HPP
#define AUDIOAPPLICATION_HPP

class audioApplication {
    public:
        enum struct ScreenState {
            EnteringPath,
            Loaded
        };
        audioApplication();
        void run();

    private:
        sf::Texture backgroundTexture;
        sf::Sprite backgroundSprite;
        sf::Texture musicNoteOneTexture;
        sf::Texture musicNoteTwoTexture;

        wavFile audio_file;
        sf::RenderWindow window;
        ScreenState currentState;
        // Initial screen for choosing audio file elements
        sf::Font font;
        sf::Text mainText;
        sf::Text bottomText;
        sf::Text inputText;
        sf::Text loadButtonText;
        sf::RectangleShape textBox;
        sf::RectangleShape mainTextBox;
        sf::RectangleShape bottomTextBox;
        sf::RectangleShape cursor;
        sf::Sprite loadButton;
        sf::Clock cursorClock;
        std::string filePath;
        bool fileLoaded;
        bool cursorVisible;
        float cursorBlinkDuration;
        // After file loaded screen
        sf::Text backButtonText;
        sf::Text songTitleText;
        sf::RectangleShape songTitleBox;
        sf::Text reverseButtonText;
        sf::Text make8BitButtonText;
        sf::Text trimButtonText;
        //sf::Text leadingButtonText;
        //sf::Text trailingButtonText;
        sf::Text normalizeAudioButtonText;
        sf::Sprite backButton;
        sf::Sprite reverseButton;
        sf::Sprite make8BitButton;
        sf::Sprite trimButton;
        //sf::Sprite leadingButton;
        //sf::Sprite trailingButton;
        sf::Sprite normalizeAudioButton;
        sf::Text outputButtonText;
        sf::Sprite outputButton;

        sf::Texture pauseButtonTexture;
        sf::Texture playButtonTexture;
        sf::Sprite pausePlayButton;
        bool isPaused;
        sf::RectangleShape durationBar;
        sf::Sprite durationActualShape;
        sf::SoundBuffer audioBuffer;
        sf::Sound audio;
        sf::Text audioTotalDuration;
        sf::RectangleShape audioTotalDurationBox;
        sf::Text audioCurrentDuration;
        sf::RectangleShape audioCurrentDurationBox;
        sf::VertexArray sineWave;

        void loadTexture(sf::Texture &texture_in, string path_in, string texture_name_in);
        void setText(sf::Text &text_in, int character_size_in, sf::Color color_in, string text_string_in, int x_pos_in, int y_pos_in);
        void setRectangle(sf::RectangleShape &rectangle_in, int width_in, int height_in, sf::Color fill_in, sf::Color outline_in, int outline_thickness_in, int x_in, int y_in);
        void setSprite(sf::Sprite &sprite_in, sf::Texture &texture_in, float width_in, float height_in, int x_in, int y_in);
        void setInitialConfiguration();
        void setEnteringPathConfiguration();
        void setLoadedConfiguration();
        void updateSineWave(sf::Time currentTime);

        void loadFile(const std::string& filePath);

        void renderElements();

        void processEnteringPathEvents(sf::Event &event_in);

        void processLoadedEvents(sf::Event &event_in);
};

#endif // AUDIOAPPLICATION_HPP