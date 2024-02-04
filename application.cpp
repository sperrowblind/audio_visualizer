#include "application.hpp"

audioApplication::audioApplication() : window(sf::VideoMode(800, 600), "Audio Editor"), currentState(ScreenState::Loaded) {

    if (!backgroundTexture.loadFromFile("application_images/pexels-hermaion-104084.jpg")) {
        std::cerr << "Failed to load background image!" << std::endl;
    }
    else {
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(window.getSize().x
            / backgroundSprite.getLocalBounds().width, window.getSize().y
            / backgroundSprite.getLocalBounds().height);
    }

    if (!musicNoteOneTexture.loadFromFile("application_images/music-note-1.png")) {
        std::cerr << "Failed to load music-note-1!" << std::endl;
    }
    if (!musicNoteTwoTexture.loadFromFile("application_images/music-note-2.png")) {
        std::cerr << "Failed to load music-note-2!" << std::endl;
    }
    if (!pauseButtonTexture.loadFromFile("application_images/pause.png")) {
        std::cerr << "Failed to load pause button!" << std::endl;
    }
    if (!playButtonTexture.loadFromFile("application_images/play.png")) {
        std::cerr << "Failed to load play button!" << std::endl;
    }

    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }

    // Initial screen for choosing audio file elements

    mainText.setFont(font);
    mainText.setCharacterSize(20);
    mainText.setFillColor(sf::Color::White);
    mainText.setString("Enter the path to your desired audio file: ");
    mainText.setPosition(10, 53);
    mainTextBox.setSize(sf::Vector2f(375, 30));
    mainTextBox.setFillColor(sf::Color::Black);
    mainTextBox.setOutlineColor(sf::Color::White);
    mainTextBox.setOutlineThickness(2);
    mainTextBox.setPosition(5, 50);

    bottomText.setFont(font);
    bottomText.setCharacterSize(20);
    bottomText.setFillColor(sf::Color::White);
    bottomText.setString("Please only use wav files, other audio formats are not yet supported.");
    bottomText.setPosition(50, 153);
    bottomTextBox.setSize(sf::Vector2f(630, 30));
    bottomTextBox.setFillColor(sf::Color::Black);
    bottomTextBox.setOutlineColor(sf::Color::White);
    bottomTextBox.setOutlineThickness(2);
    bottomTextBox.setPosition(45, 150);

    textBox.setSize(sf::Vector2f(300, 30));
    textBox.setFillColor(sf::Color::White);
    textBox.setOutlineColor(sf::Color::Black);
    textBox.setOutlineThickness(2);
    textBox.setPosition(400, 50);

    cursor.setSize(sf::Vector2f(2.f, 25.f));
    cursor.setFillColor(sf::Color::Black);
    cursor.setPosition(408.f, 53.f);

    cursorVisible = true;
    cursorBlinkDuration = 0.5f;

    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(405, 53);

    loadButton.setTexture(musicNoteTwoTexture);
    loadButton.setScale(1.2f, 1.2f);
    loadButton.setPosition(320, 200);
    loadButtonText.setFont(font);
    loadButtonText.setCharacterSize(18);
    loadButtonText.setFillColor(sf::Color::White);
    loadButtonText.setString("Load");
    loadButtonText.rotate(-25);
    loadButtonText.setPosition(326, 295);

    fileLoaded = false;

    // After file loaded screen

    songTitleText.setFont(font);
    songTitleText.setCharacterSize(20);
    songTitleText.setFillColor(sf::Color::White);
    songTitleText.setString("Default Title");
    songTitleText.setPosition(350, 15);
    songTitleBox.setFillColor(sf::Color::Black);
    songTitleBox.setOutlineColor(sf::Color::White);
    songTitleBox.setOutlineThickness(2);
    songTitleBox.setPosition(350, 15);

    backButtonText.setFont(font);
    backButtonText.setCharacterSize(18);
    backButtonText.setFillColor(sf::Color::White);
    backButtonText.setString("Back");
    backButtonText.rotate(-25);
    backButtonText.setPosition(56, 105);

    backButton.setTexture(musicNoteOneTexture);
    backButton.setScale(1.2f, 1.2f);
    backButton.setPosition(50, 10);

    pausePlayButton.setTexture(pauseButtonTexture);
    pausePlayButton.setPosition(150, 150);
    isPaused = true;
    durationBar.setSize(sf::Vector2f(375, 30));
    durationBar.setFillColor(sf::Color::Black);
    durationBar.setOutlineColor(sf::Color::White);
    durationBar.setOutlineThickness(2);
    durationBar.setPosition(215, 170);

    audioTotalDuration.setFont(font);
    audioTotalDuration.setCharacterSize(15);
    audioTotalDuration.setFillColor(sf::Color::Red);
    audioTotalDuration.setString("0:00");
    audioTotalDuration.setPosition(400, 170);

    audioCurrentDuration.setFont(font);
    audioCurrentDuration.setCharacterSize(15);
    audioCurrentDuration.setFillColor(sf::Color::Red);
    audioCurrentDuration.setString("0:00");
    audioCurrentDuration.setPosition(215, 170);



    reverseButtonText.setFont(font);
    reverseButtonText.setCharacterSize(18);
    reverseButtonText.setFillColor(sf::Color::White);
    reverseButtonText.setString("Reverse");
    reverseButtonText.rotate(-25);
    reverseButtonText.setPosition(50, 425);

    reverseButton.setTexture(musicNoteTwoTexture);
    reverseButton.setScale(1.5f, 1.5f);
    reverseButton.setPosition(50, 300);

    make8BitButtonText.setFont(font);
    make8BitButtonText.setCharacterSize(18);
    make8BitButtonText.setFillColor(sf::Color::White);
    make8BitButtonText.setString("8-bit'ify");
    make8BitButtonText.rotate(-25);
    make8BitButtonText.setPosition(200, 425);

    make8BitButton.setTexture(musicNoteOneTexture);
    make8BitButton.setScale(1.5f, 1.5f);
    make8BitButton.setPosition(200, 300);

    trimButtonText.setFont(font);
    trimButtonText.setCharacterSize(18);
    trimButtonText.setFillColor(sf::Color::White);
    trimButtonText.setString("Trim");
    trimButtonText.rotate(-25);
    trimButtonText.setPosition(305, 415);

    trimButton.setTexture(musicNoteTwoTexture);
    trimButton.setScale(1.5f, 1.5f);
    trimButton.setPosition(300, 300);

    leadingButtonText.setFont(font);
    leadingButtonText.setCharacterSize(18);
    leadingButtonText.setFillColor(sf::Color::White);
    leadingButtonText.setString("Leading");
    leadingButtonText.rotate(-25);
    leadingButtonText.setPosition(400, 425);

    leadingButton.setTexture(musicNoteOneTexture);
    leadingButton.setScale(1.5f, 1.5f);
    leadingButton.setPosition(400, 300);

    trailingButtonText.setFont(font);
    trailingButtonText.setCharacterSize(18);
    trailingButtonText.setFillColor(sf::Color::White);
    trailingButtonText.setString("Trailing");
    trailingButtonText.rotate(-25);
    trailingButtonText.setPosition(500, 425);

    trailingButton.setTexture(musicNoteTwoTexture);
    trailingButton.setScale(1.5f, 1.5f);
    trailingButton.setPosition(500, 300);

    normalizeAudioButtonText.setFont(font);
    normalizeAudioButtonText.setCharacterSize(18);
    normalizeAudioButtonText.setFillColor(sf::Color::White);
    normalizeAudioButtonText.setString("Quiet");
    normalizeAudioButtonText.rotate(-25);
    normalizeAudioButtonText.setPosition(652, 423);

    normalizeAudioButton.setTexture(musicNoteOneTexture);
    normalizeAudioButton.setScale(1.5, 1.5f);
    normalizeAudioButton.setPosition(650, 300);

    outputButtonText.setFont(font);
    outputButtonText.setCharacterSize(18);
    outputButtonText.setFillColor(sf::Color::White);
    outputButtonText.setString("Output");
    outputButtonText.rotate(-25);
    outputButtonText.setPosition(375, 475);

    outputButton.setTexture(musicNoteTwoTexture);
    outputButton.rotate(180);
    outputButton.setScale(1.5f, 1.5f);
    outputButton.setPosition(405, 600);
}

void audioApplication::run() {
    sf::Event event;
    while (window.isOpen()){
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

void audioApplication::loadFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        fileLoaded = true;
        std::cout << "File opened successfully!" << std::endl;
        if (!music.openFromFile(filePath))
            std::cerr << "Couldn't play file \n";
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
            window.draw(leadingButton);
            window.draw(trailingButton);
            window.draw(normalizeAudioButton);
            window.draw(outputButton);
            window.draw(songTitleBox);
            window.draw(durationBar);
            window.draw(backButtonText);
            window.draw(songTitleText);
            window.draw(reverseButtonText);
            window.draw(make8BitButtonText);
            window.draw(trimButtonText);
            window.draw(leadingButtonText);
            window.draw(trailingButtonText);
            window.draw(normalizeAudioButtonText);
            window.draw(outputButtonText);
            window.draw(pausePlayButton);
            window.draw(audioCurrentDuration);
            window.draw(audioTotalDuration);
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

