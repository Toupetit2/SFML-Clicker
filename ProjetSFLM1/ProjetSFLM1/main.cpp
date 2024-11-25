#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;

int totalScore = 0;
int goldCount = 4;
int clickPower = 1;

int timer = time(0);


bool isMouseOnButton(sf::Vector2i mousePosition, sf::Vector2f buttonPosition, int buttonSizeX, int buttonSizeY)
{
    if (mousePosition.x > buttonPosition.x && mousePosition.x < (buttonPosition.x + buttonSizeX))//Si dans le bouton en X
    {
        if (mousePosition.y > buttonPosition.y && mousePosition.y < (buttonPosition.y + buttonSizeY))
        {
            return true;
        }
    }

    return false;
}

const int WINDOW_WIDTH = 768;
const int WINDOW_HEIGHT = 720;

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Gestion des Entrées");
    window.setFramerateLimit(60);

    // Font Setup
    sf::Font font;
    font.loadFromFile("../Assets/Roboto.ttf");

    //Title Text
    sf::Text titleText("SFML Clicker", font, 50);
    titleText.setFillColor(sf::Color::White);
    
    //Main Click Button Visual
    int ClickerButtonSizeX = WINDOW_WIDTH - 40;
    int ClickerButtonSizeY = 120;
    sf::Vector2f ClickerButtonPosition(20, WINDOW_HEIGHT - (20 + ClickerButtonSizeY));
    sf::RectangleShape ClickerButton(sf::Vector2f(ClickerButtonSizeX, ClickerButtonSizeY));
    ClickerButton.setFillColor(sf::Color::White);
    ClickerButton.setPosition(ClickerButtonPosition.x, ClickerButtonPosition.y);

    // Background Title and Score Bar
    sf::Vector2f BackgroundHeaderPosition(20, 20);
    sf::RectangleShape BackgroundHeader(sf::Vector2f(ClickerButtonSizeX, ClickerButtonSizeY));
    BackgroundHeader.setFillColor(sf::Color::Color(26, 26, 26));
    BackgroundHeader.setPosition(BackgroundHeaderPosition.x, BackgroundHeaderPosition.y);

    titleText.setPosition(25, 20);


    // Shop Buttons
    int ShopButtonMargin = 10;
    int ShopButtonSize = (WINDOW_WIDTH - 30) / 6 - ShopButtonMargin;
    
    std::vector<sf::RectangleShape> ShopButtons;
        
    for (int i = 0; i < 6; i++) {
        sf::RectangleShape button(sf::Vector2f(ShopButtonSize, ShopButtonSize));
        button.setFillColor(sf::Color::White);
        button.setPosition(20 + i * ShopButtonSize + i * ShopButtonMargin, 153);
        ShopButtons.push_back(button);
    }

    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Escape)
                    window.close(); // Fermer avec Échap
            }
            if (event.type == sf:: Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {

                    //Main Clicker Button Tech
                    if (isMouseOnButton(sf::Mouse::getPosition(window), ClickerButtonPosition, ClickerButtonSizeX, ClickerButtonSizeY))
                    {
                        totalScore += clickPower;
                        for (int i = 0; i < clickPower; i++)
                        {
                            if (rand() % 10 == 0) // une chance sur 10 d'avoir un gold
                            {
                                goldCount += 1;
                            }
                        }
                        
                        cout << "ClickerButtonCLicked  ";
                    }

                    // Shop Button 1 Tech
                    if (isMouseOnButton(sf::Mouse::getPosition(window), ShopButtons[0].getPosition(), ShopButtonSize, ShopButtonSize))
                    {
                        if (goldCount > 4)
                        {
                            goldCount += -5;
                            clickPower += 1;

                        }

                        cout << "ShopButton1Clicked";
                    }
                    // Shop Button 2 Tech
                    if (isMouseOnButton(sf::Mouse::getPosition(window), ShopButtons[1].getPosition(), ShopButtonSize, ShopButtonSize))
                    {
                        if (goldCount > 49)
                        {
                            goldCount += -50;
                            clickPower += 10;

                        }

                        cout << "ShopButton1Clicked";
                    }

                }
            }
        }


        // Timer and Autoclick farm

        if (timer != time(0))
        {
            timer = time(0);
            totalScore += 1;
            // autoclickfarm
        }


        //Score text
        int scoreTextMargin = 30;
        sf::Text scoreText(to_string(totalScore), font, 60);
        scoreText.setFillColor(sf::Color::White);

        scoreText.setPosition(WINDOW_WIDTH - scoreText.getLocalBounds().width - scoreTextMargin , 10);

        sf::Text goldText(to_string(goldCount), font, 40);
        goldText.setFillColor(sf::Color::White);

        goldText.setPosition(WINDOW_WIDTH - goldText.getLocalBounds().width - scoreTextMargin, 20 + 60);




        window.clear();
        window.draw(BackgroundHeader);
        window.draw(ClickerButton);
        for (auto button : ShopButtons)
        {
            window.draw(button);
        }
        window.draw(titleText);
        window.draw(scoreText);
        window.draw(goldText);
        window.display();
    }

    return 0;
}