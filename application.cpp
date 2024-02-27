#include "application.hpp"
#include <algorithm>

audioApplication::audioApplication() : window(sf::VideoMode(800, 600), "Audio Editor"), currentState(ScreenState::EnteringPath) {

    setInitialConfiguration();

    // Initial screen for choosing audio file elements

    setEnteringPathConfiguration();

    // After file loaded screen

    setLoadedConfiguration();

}

void audioApplication::run() {
    sf::Event event;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            switch (currentState) {
                case ScreenState::EnteringPath:
                    processEnteringPathEvents(event);
                    break;
                case ScreenState::Loaded:
                    processLoadedEvents(event);
                    break;
            }
        }

        if (audio.getStatus() == sf::Sound::Playing) {
            sf::Time currentTime = audio.getPlayingOffset();
            updateSineWave(currentTime);
        }


        inputText.setString(filePath);
        float textWidth = inputText.getGlobalBounds().width;
        cursor.setPosition(408.f + textWidth, 53.f);

        renderElements();
    }
}


void audioApplication::loadTexture(sf::Texture &texture_in, string path_in, string texture_name_in) {
    if (!texture_in.loadFromFile(path_in)) {
        std::cerr << "Failed to load " << texture_name_in << " button!" << '\n';
    }
}

void audioApplication::setText(sf::Text &text_in, int character_size_in, sf::Color color_in, string text_string_in, int x_pos_in, int y_pos_in) {
    text_in.setFont(font);
    text_in.setCharacterSize(character_size_in);
    text_in.setFillColor(color_in);
    text_in.setString(text_string_in);
    text_in.setPosition(x_pos_in, y_pos_in);
}

void audioApplication::setRectangle(sf::RectangleShape &rectangle_in, int width_in, int height_in, sf::Color fill_in, sf::Color outline_in, int outline_thickness_in, int x_in, int y_in) {
    rectangle_in.setSize(sf::Vector2f(width_in, height_in));
    rectangle_in.setFillColor(fill_in);
    rectangle_in.setOutlineColor(outline_in);
    rectangle_in.setOutlineThickness(outline_thickness_in);
    rectangle_in.setPosition(x_in, y_in);
}

void audioApplication::setSprite(sf::Sprite &sprite_in, sf::Texture &texture_in, float width_in, float height_in, int x_in, int y_in) {
    sprite_in.setTexture(texture_in);
    sprite_in.setScale(width_in, height_in);
    sprite_in.setPosition(x_in, y_in);
}

void audioApplication::setInitialConfiguration() {
    loadTexture(backgroundTexture, "application_images/pexels-hermaion-104084.jpg", "background texture");

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(window.getSize().x
        / backgroundSprite.getLocalBounds().width, window.getSize().y
        / backgroundSprite.getLocalBounds().height);

    loadTexture(musicNoteOneTexture, "application_images/music-note-1.png", "music-note-1");
    loadTexture(musicNoteTwoTexture, "application_images/music-note-2.png", "music-note-2");
    loadTexture(pauseButtonTexture, "application_images/pause.png", "pause");
    loadTexture(playButtonTexture, "application_images/play.png", "play");


    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }
}

void audioApplication::setEnteringPathConfiguration() {
    setText(mainText, 20, sf::Color::White, "Enter the path to your desired audio file: ", 10, 53);

    setRectangle(mainTextBox, 375, 30, sf::Color::Black, sf::Color::White, 2, 5, 50);

    setText(bottomText, 20, sf::Color::White, "Please only use wav files, other audio formats are not yet supported.", 50, 153);

    setRectangle(bottomTextBox, 630, 30, sf::Color::Black, sf::Color::White, 2, 45, 150);

    setRectangle(textBox, 300, 30, sf::Color::White, sf::Color::Black, 2, 400, 50);

    cursor.setSize(sf::Vector2f(2.f, 25.f));
    cursor.setFillColor(sf::Color::Black);
    cursor.setPosition(408.f, 53.f);

    cursorVisible = true;
    cursorBlinkDuration = 0.5f;

    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(405, 53);

    setSprite(loadButton, musicNoteTwoTexture, 1.2f, 1.2f, 320, 200);

    setText(loadButtonText, 18, sf::Color::White, "Load", 326, 295);
    loadButtonText.rotate(-25);

    fileLoaded = false;
}

void audioApplication::setLoadedConfiguration() {
    setText(songTitleText, 20, sf::Color::White, "Default Title", 350, 15);
    songTitleBox.setFillColor(sf::Color::Black);
    songTitleBox.setOutlineColor(sf::Color::White);
    songTitleBox.setOutlineThickness(2);
    songTitleBox.setPosition(350, 15);

    setText(backButtonText, 18, sf::Color::White, "Back", 56, 105);
    backButtonText.rotate(-25);

    setSprite(backButton, musicNoteOneTexture, 1.2f, 1.2f, 50, 10);

    setSprite(pausePlayButton, pauseButtonTexture, 1.0f, 1.0f, 150, 150);

    isPaused = true;

    setRectangle(durationBar, 375, 30, sf::Color::Blue, sf::Color::Black, 2, 215, 160);
    setSprite(durationActualShape, musicNoteTwoTexture, 1.0f, 1.0f, 215, 160);
    sineWave.setPrimitiveType(sf::LineStrip);
    sineWave.resize(1000);
    for (std::size_t i = 0; i < sineWave.getVertexCount(); ++i) {
        sineWave[i].color = sf::Color::Red;
    }

    setText(audioTotalDuration, 15, sf::Color::Red, "0:00", 500, 195);
    setRectangle(audioTotalDurationBox, 60, 20, sf::Color::Black, sf::Color::White, 2, 490, 195);
    setText(audioCurrentDuration, 15, sf::Color::Red, "0:00", 245, 195);
    setRectangle(audioCurrentDurationBox, 60, 20, sf::Color::Black, sf::Color::White, 2, 235, 195);

    setText(reverseButtonText, 18, sf::Color::White, "Reverse", 100, 425);
    reverseButtonText.rotate(-25);

    setSprite(reverseButton, musicNoteTwoTexture, 1.5f, 1.5f, 100, 300);

    setText(make8BitButtonText, 18, sf::Color::White, "8-bit'ify", 205, 425);
    make8BitButtonText.rotate(-25);

    setSprite(make8BitButton, musicNoteOneTexture, 1.5f, 1.5f, 200, 300);

    setText(trimButtonText, 18, sf::Color::White, "Trim", 412, 420);
    trimButtonText.rotate(-25);

    setSprite(trimButton, musicNoteTwoTexture, 1.5f, 1.5f, 400, 300);

    setText(normalizeAudioButtonText, 18, sf::Color::White, "Quiet", 562, 420);
    normalizeAudioButtonText.rotate(-25);

    setSprite(normalizeAudioButton, musicNoteOneTexture, 1.5f, 1.5f, 550, 300);

    setText(outputButtonText, 18, sf::Color::White, "Output", 350, 530);
    outputButtonText.rotate(-25);

    setSprite(outputButton, musicNoteTwoTexture, 1.5f, 1.5f, 415, 650);
    outputButton.rotate(180);
}

void audioApplication::updateSineWave(sf::Time currentTime) {
    float sineWaveAmplitude = 14.0f;
    float sineWaveFrequency = 0.5f;
    float sineWavePropagationSpeed = 10.0f;
    float sineWaveWidth = 0.0f;

    double sineWaveLength = (currentTime.asSeconds() / audio_file.get_audio_file_duration());

    const float centerY = 175.0f;  // Adjust as needed
    const float stepSize = 0.1f;    // Adjust as needed

    // Update sine wave properties
    sineWaveWidth += sineWavePropagationSpeed * currentTime.asSeconds();
    sineWaveLength = sineWaveLength * durationBar.getSize().x;
    sineWave.resize(sineWaveLength);


    for (std::size_t i = 0; i < sineWave.getVertexCount(); ++i) {
        float x = (i * stepSize) + sineWaveLength;
        float y = centerY + sineWaveAmplitude * std::sin(x * sineWaveFrequency) * std::cos((x + sineWaveWidth) * sineWaveFrequency);
        sineWave[i].position = sf::Vector2f(215 + x, y);
        sineWave[i].color = sf::Color::Red;
    }
}

void audioApplication::loadFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        fileLoaded = true;
        std::cout << "File opened successfully!" << std::endl;
        currentState = ScreenState::Loaded;
        std::string file_name = filePath.substr(0, filePath.find("."));
        wavFile file_in(file, file_name);
        audio_file = file_in;
        songTitleText.setString(filePath.substr(0, filePath.find(".")));
        songTitleBox.setSize(sf::Vector2f(songTitleText.getLocalBounds().width + 20, 30));

        audioBuffer.loadFromSamples(reinterpret_cast<const sf::Int16*>(audio_file.get_audio_data().data()), audio_file.get_audio_data().size(), audio_file.get_fmt_chunk().numChannels, audio_file.get_fmt_chunk().sampleRate);
        audio.setBuffer(audioBuffer);
    }
    else {
        std::cout << "Failed to open file!" << std::endl;
    }
}


void audioApplication::renderElements() {
    window.clear();
    window.draw(backgroundSprite);
    switch (currentState) {
        case ScreenState::EnteringPath:
            window.draw(mainTextBox);
            window.draw(mainText);
            window.draw(bottomTextBox);
            window.draw(bottomText);
            window.draw(textBox);
            window.draw(inputText);
            window.draw(loadButton);
            window.draw(loadButtonText);
            if (cursorClock.getElapsedTime().asSeconds() >= cursorBlinkDuration) {
                cursorClock.restart();
                cursorVisible = !cursorVisible;
            }
            if (cursorVisible) {
                window.draw(cursor);
            }
            break;
        case ScreenState::Loaded:
            window.draw(backButton);
            window.draw(reverseButton);
            window.draw(make8BitButton);
            window.draw(trimButton);
            //window.draw(leadingButton);
            //window.draw(trailingButton);
            window.draw(normalizeAudioButton);
            window.draw(outputButton);
            window.draw(songTitleBox);
            window.draw(durationBar);
            window.draw(backButtonText);
            window.draw(songTitleText);
            window.draw(reverseButtonText);
            window.draw(make8BitButtonText);
            window.draw(trimButtonText);
            //window.draw(leadingButtonText);
            //window.draw(trailingButtonText);
            window.draw(normalizeAudioButtonText);
            window.draw(outputButtonText);
            window.draw(pausePlayButton);
            window.draw(audioTotalDurationBox);
            window.draw(audioCurrentDurationBox);
            window.draw(audioCurrentDuration);
            window.draw(audioTotalDuration);
            window.draw(sineWave);
            break;
    }
    window.display();
}


void audioApplication::processEnteringPathEvents(sf::Event &event_in) {
    if (event_in.type == sf::Event::TextEntered) {
        if (event_in.text.unicode == 8) {
            if (!filePath.empty()) {
                filePath.pop_back();
                float textWidth = inputText.getGlobalBounds().width;
                cursor.setPosition(408.f + textWidth, 53.f);
            }
        }
        else if (event_in.text.unicode < 128) {
            filePath += static_cast<char>(event_in.text.unicode);
        }
    }
    if (event_in.type == sf::Event::KeyPressed && event_in.key.code == sf::Keyboard::Enter) {
        loadFile(filePath);
    }
    if (event_in.type == sf::Event::MouseButtonReleased) {
        if (event_in.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (loadButton.getGlobalBounds().contains(mousePos)) {
                loadFile(filePath);
            }
        }
    }
}

void audioApplication::processLoadedEvents(sf::Event &event_in) {
    if (event_in.type == sf::Event::MouseButtonReleased) {
        if (event_in.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (backButton.getGlobalBounds().contains(mousePos)) {
                filePath = "";
                fileLoaded = false;
                currentState = ScreenState::EnteringPath;
            }
            else if (reverseButton.getGlobalBounds().contains(mousePos)) {
                std::cout << "THIS WOULD REVERSE STUFF \n";
                audio_file.reverse_audio();
            }
            else if (outputButton.getGlobalBounds().contains(mousePos)) {
                std::cout << "THIS WOULD output \n";
                audio_file.output_song();
            }
            else if (pausePlayButton.getGlobalBounds().contains(mousePos)) {
                isPaused = !isPaused;
                if (!isPaused) {
                    pausePlayButton.setTexture(playButtonTexture);
                    audio.play();
                }
                else {
                    pausePlayButton.setTexture(pauseButtonTexture);
                    audio.pause();
                }

            }
        }
    }
}

