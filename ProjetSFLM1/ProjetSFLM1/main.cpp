#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <windows.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

int totalScore = 0;
int goldCount = 1000000;
int clickPower = 1;
int autoclickers = 0;

int timer = time(0);

vector<milliseconds> buyTimings = {};

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

    //Images Setup
    sf::Texture button1;
    sf::Texture button2;
    sf::Texture button3;
    sf::Texture button4;
    sf::Texture button5;
    sf::Texture button6;

    if (!button1.loadFromFile("../Assets/button1.png"))
    {
        cout << "impossible de charger l'image 1";
    }
    if (!button2.loadFromFile("../Assets/button2.png"))
    {
        cout << "impossible de charger l'image 2";
    }
    if (!button3.loadFromFile("../Assets/button3.png"))
    {
        cout << "impossible de charger l'image 3";
    }
    if (!button4.loadFromFile("../Assets/button4.png"))
    {
        cout << "impossible de charger l'image 4";
    }
    if (!button5.loadFromFile("../Assets/button5.png"))
    {
        cout << "impossible de charger l'image 5";
    }
    if (!button6.loadFromFile("../Assets/button6.png"))
    {
        cout << "impossible de charger l'image 6";
    }

    vector<sf::Texture> buttonTextureList = { button1, button2, button3, button4, button5, button6 };

    sf::Sprite button1Sprite;
    button1Sprite.setTexture(button1);
    

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
        sf::RectangleShape buttonSprite(sf::Vector2f(ShopButtonSize, ShopButtonSize));
        
        buttonSprite.setTexture(&buttonTextureList[i]);
        buttonSprite.setPosition(20 + i * ShopButtonSize + i * ShopButtonMargin, 153);
        window.draw(buttonSprite);
        ShopButtons.push_back(buttonSprite);

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
                    // Shop Button 3 Tech
                    if (isMouseOnButton(sf::Mouse::getPosition(window), ShopButtons[2].getPosition(), ShopButtonSize, ShopButtonSize))
                    {
                        if (goldCount > 14)
                        {
                            goldCount += -15;
                            autoclickers += 1;

                            milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

                            buyTimings.push_back(ms);
                        }

                        cout << "ShopButton1Clicked";
                    }
                    // Shop Button 4 Tech
                    if (isMouseOnButton(sf::Mouse::getPosition(window), ShopButtons[3].getPosition(), ShopButtonSize, ShopButtonSize))
                    {
                        if (goldCount > 149)
                        {
                            goldCount += -150;
                            autoclickers += 10;
                            for (int i = 0; i < 10; i++)
                            {
                                milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
                                buyTimings.push_back(ms + milliseconds(1000));
                                
                                
                            }
                            
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
            for (milliseconds autoclicker : buyTimings)
            {
                cout << autoclicker.count() << ", " << autoclicker.count() - time(0) << endl;

                if ((autoclicker.count()) == time(0) % 5)
                {
                    totalScore += 1;
                    if (rand() % 10 == 0) // une chance sur 10 d'avoir un gold
                    {
                        goldCount += 1;
                    }
                }
            }
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