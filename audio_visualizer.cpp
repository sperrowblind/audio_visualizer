#include <iostream>
#include <fstream>
#include <vector>
#include "models/wav.cpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>



using namespace std;



int main(int argc, char *argv[]) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Audio Editor");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Text mainText;
    mainText.setFont(font);
    mainText.setCharacterSize(20);
    mainText.setFillColor(sf::Color::White);
    mainText.setString("Enter the path to your desired audio file: ");
    mainText.setPosition(10, 10);

    sf::Text bottomText;
    bottomText.setFont(font);
    bottomText.setCharacterSize(20);
    bottomText.setFillColor(sf::Color::White);
    bottomText.setString("Please only use wav files, other audio formats are not supported.");
    bottomText.setPosition(50, 100);


    sf::RectangleShape textBox(sf::Vector2f(300, 30));
    textBox.setFillColor(sf::Color::White);
    textBox.setOutlineColor(sf::Color::Black);
    textBox.setOutlineThickness(2);
    textBox.setPosition(400, 10);

    sf::RectangleShape cursor(sf::Vector2f(2.f, 25.f));
    cursor.setFillColor(sf::Color::Black);
    cursor.setPosition(410.f, 20.f);

    sf::Clock cursorClock;
    bool cursorVisible = true;
    float cursorBlinkDuration = 0.5f;

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(405, 13);

    sf::RectangleShape button(sf::Vector2f(100, 40));
    button.setFillColor(sf::Color::Blue);
    button.setOutlineColor(sf::Color::Black);
    button.setOutlineThickness(2);
    button.setPosition(320, 50);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setString("Load");
    buttonText.setPosition(350, 55);

    std::string filePath;
    bool fileLoaded = false;

    sf::Music music;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) { // Backspace key pressed
                    if (!filePath.empty()) {
                        filePath.pop_back();
                        float textWidth = inputText.getGlobalBounds().width;
                        // Update cursor position
                        cursor.setPosition(410.f + textWidth, 20.f);
                    }
                } else if (event.text.unicode < 128) {
                    filePath += static_cast<char>(event.text.unicode);
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (button.getGlobalBounds().contains(mousePos)) {
                        std::ifstream file(filePath);
                        if (file.is_open()) {
                            fileLoaded = true;
                            std::cout << "File opened successfully!" << std::endl;
                            if (!music.openFromFile(filePath))
                                return -1; // error
                            music.play();

                            // Perform your file processing here
                            // ...
                        } else {
                            std::cout << "Failed to open file!" << std::endl;
                        }
                    }
                }
            }
        }

        inputText.setString(filePath);
        float textWidth = inputText.getGlobalBounds().width;

        cursor.setPosition(410.f + textWidth, 20.f);

        if (cursorVisible) {
            window.draw(cursor);
        }

        window.clear();
        window.draw(mainText);
        window.draw(bottomText);
        window.draw(textBox);
        window.draw(inputText);
        window.draw(button);
        window.draw(buttonText);

        if (cursorClock.getElapsedTime().asSeconds() >= cursorBlinkDuration) {
            cursorClock.restart();
            cursorVisible = !cursorVisible;
        }

        // Draw the cursor if it's currently visible
        if (cursorVisible) {
            window.draw(cursor);
        }
        window.display();
    }

    return EXIT_SUCCESS;
}