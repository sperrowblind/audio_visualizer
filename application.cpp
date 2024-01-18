#include "application.hpp"

audioApplication::audioApplication() : window(sf::VideoMode(800, 600), "Audio Editor"), currentState(ScreenState::EnteringPath) {

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


    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }

    // Initial screen for choosing audio file elements

    mainText.setFont(font);
    mainText.setCharacterSize(20);
    mainText.setFillColor(sf::Color::White);
    mainText.setString("Enter the path to your desired audio file: ");
    mainText.setPosition(10, 10);

    bottomText.setFont(font);
    bottomText.setCharacterSize(20);
    bottomText.setFillColor(sf::Color::White);
    bottomText.setString("Please only use wav files, other audio formats are not supported.");
    bottomText.setPosition(50, 100);

    textBox.setSize(sf::Vector2f(300, 30));
    textBox.setFillColor(sf::Color::White);
    textBox.setOutlineColor(sf::Color::Black);
    textBox.setOutlineThickness(2);
    textBox.setPosition(400, 10);

    cursor.setSize(sf::Vector2f(2.f, 25.f));
    cursor.setFillColor(sf::Color::Black);
    cursor.setPosition(410.f, 20.f);

    cursorVisible = true;
    cursorBlinkDuration = 0.5f;

    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(405, 13);

    loadButton.setSize(sf::Vector2f(100, 40));
    loadButton.setFillColor(sf::Color::Blue);
    loadButton.setOutlineColor(sf::Color::Black);
    loadButton.setOutlineThickness(2);
    loadButton.setPosition(320, 50);

    loadButtonText.setFont(font);
    loadButtonText.setCharacterSize(20);
    loadButtonText.setFillColor(sf::Color::White);
    loadButtonText.setString("Load");
    loadButtonText.setPosition(350, 55);

    fileLoaded = false;

    // After file loaded screen

    songTitleText.setFont(font);
    songTitleText.setCharacterSize(20);
    songTitleText.setFillColor(sf::Color::White);
    songTitleText.setString("Default Title");
    songTitleText.setPosition(350, 15);

    backButtonText.setFont(font);
    backButtonText.setCharacterSize(20);
    backButtonText.setFillColor(sf::Color::White);
    backButtonText.setString("Go Back");
    backButtonText.setPosition(50, 50);

    backButton.setTexture(musicNoteOneTexture);
    backButton.setPosition(50, 10);

    reverseButtonText.setFont(font);
    reverseButtonText.setCharacterSize(20);
    reverseButtonText.setFillColor(sf::Color::White);
    reverseButtonText.setString("Reverse");
    reverseButtonText.setPosition(100, 400);

    reverseButton.setTexture(musicNoteTwoTexture);
    reverseButton.setPosition(100, 350);

    make8BitButtonText.setFont(font);
    make8BitButtonText.setCharacterSize(20);
    make8BitButtonText.setFillColor(sf::Color::White);
    make8BitButtonText.setString("8-bit'ify");
    make8BitButtonText.setPosition(200, 400);

    make8BitButton.setTexture(musicNoteOneTexture);
    make8BitButton.setPosition(200, 350);

    trimButtonText.setFont(font);
    trimButtonText.setCharacterSize(20);
    trimButtonText.setFillColor(sf::Color::White);
    trimButtonText.setString("Trim");
    trimButtonText.setPosition(300, 400);

    trimButton.setTexture(musicNoteTwoTexture);
    trimButton.setPosition(300, 350);

    leadingButtonText.setFont(font);
    leadingButtonText.setCharacterSize(20);
    leadingButtonText.setFillColor(sf::Color::White);
    leadingButtonText.setString("Leading");
    leadingButtonText.setPosition(400, 400);

    leadingButton.setTexture(musicNoteOneTexture);
    leadingButton.setPosition(400, 350);

    trailingButtonText.setFont(font);
    trailingButtonText.setCharacterSize(20);
    trailingButtonText.setFillColor(sf::Color::White);
    trailingButtonText.setString("Trailing");
    trailingButtonText.setPosition(500, 400);

    trailingButton.setTexture(musicNoteTwoTexture);
    trailingButton.setPosition(500, 350);

    normalizeAudioButtonText.setFont(font);
    normalizeAudioButtonText.setCharacterSize(20);
    normalizeAudioButtonText.setFillColor(sf::Color::White);
    normalizeAudioButtonText.setString("Normalize Audio");
    normalizeAudioButtonText.setPosition(600, 400);

    normalizeAudioButton.setTexture(musicNoteOneTexture);
    normalizeAudioButton.setPosition(600, 350);

    outputButtonText.setFont(font);
    outputButtonText.setCharacterSize(20);
    outputButtonText.setFillColor(sf::Color::White);
    outputButtonText.setString("Output Song");
    outputButtonText.setPosition(300, 500);

    outputButton.setTexture(musicNoteTwoTexture);
    outputButton.setPosition(300, 450);
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

        cursor.setPosition(410.f + textWidth, 20.f);

        renderElements();
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
        //music.play();
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
            window.draw(mainText);
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
            window.draw(backButtonText);
            window.draw(songTitleText);
            window.draw(reverseButtonText);
            window.draw(make8BitButtonText);
            window.draw(trimButtonText);
            window.draw(leadingButtonText);
            window.draw(trailingButtonText);
            window.draw(normalizeAudioButtonText);
            window.draw(outputButtonText);
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
                cursor.setPosition(410.f + textWidth, 20.f);
            }
        }
        else if (event_in.text.unicode < 128) {
            filePath += static_cast<char>(event_in.text.unicode);
        }
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
        }
    }
}

