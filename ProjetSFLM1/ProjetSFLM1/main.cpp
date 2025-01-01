#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

int totalScore = 0;
int points = 0;

int totalGold = 0;
int golds = 0;


int clickPower = 1;
int autoclickers = 0;

int timer = time(0);

int tickTimer = 0;

vector<int> buyTimings = {};
int goldDropRate = 10;
int tickSinceMaxGoldUpgrade = 60;

bool MainGameScreen = false;
bool MainMenuScreen = true;
bool SettingsMenuScreen = false;

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

    #pragma region init

    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Clicker");
    window.setFramerateLimit(60);

    // Font Setup
    sf::Font font;
    font.loadFromFile("../Assets/Roboto.ttf");

    #pragma endregion init

    #pragma region imagesSetup

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
    #pragma endregion imagesSetup
    
    #pragma region sound
    //SOUND
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("../Assets/ChillMenu_Loopable.wav")) {
        return -1;
    }
    sf::Sound music;
    music.setBuffer(buffer);
    music.setLoop(true);
    music.setVolume(75.f);
    music.play();

    sf::SoundBuffer clicSound;
    if (!clicSound.loadFromFile("../Assets/click.wav")) {
        return -1;
    }
    sf::Sound clicSoundPlayer;
    clicSoundPlayer.setBuffer(clicSound);
    clicSoundPlayer.setVolume(75.f);

    #pragma endregion sound

    #pragma region menu
    //Title Header Text
    sf::Text mainTitleText("SFML Clicker", font, 110);
    mainTitleText.setFillColor(sf::Color::White);
    mainTitleText.setPosition(sf::Vector2f((WINDOW_WIDTH - mainTitleText.getLocalBounds().width) / 2, (WINDOW_HEIGHT - mainTitleText.getLocalBounds().height) / 6));

    int launchAnim = 200;
    bool launchAnimTime = true;
    bool titleAnimSide = false;

    //Play Button
    int buttonWidth = WINDOW_WIDTH * 0.6;
    int buttonHeight = 100;
    sf::RectangleShape playButton(sf::Vector2f(buttonWidth, buttonHeight));
    playButton.setFillColor(sf::Color::White);
    playButton.setPosition(sf::Vector2f((WINDOW_WIDTH - buttonWidth) / 2, (WINDOW_HEIGHT - buttonHeight) / 2));
    //Play Button Text
    sf::Text playButtonText("PLAY", font, 60);
    playButtonText.setFillColor(sf::Color::Black);
    playButtonText.setStyle(sf::Text::Bold);
    playButtonText.setPosition(sf::Vector2f((WINDOW_WIDTH - playButtonText.getLocalBounds().width) / 2, WINDOW_HEIGHT/2 - playButtonText.getLocalBounds().height));

    //Settings Button
    int settingsButtonWidth = 80;
    int settingsButtonHeight = 80;
    sf::RectangleShape settingsButton(sf::Vector2f(settingsButtonWidth, settingsButtonHeight));
    settingsButton.setFillColor(sf::Color(60, 60, 60));
    settingsButton.setPosition(sf::Vector2f(WINDOW_WIDTH - settingsButtonWidth - 10, WINDOW_HEIGHT - settingsButtonHeight - 10));

    sf::Texture settingsIcon;
    settingsIcon.loadFromFile("../settingsIcon.png");

    settingsButton.setTexture(&settingsIcon);
    
    #pragma endregion menu
    
    #pragma region settings
    // SETTINGS
    
    //VolumeBar
    sf::RectangleShape track(sf::Vector2f(400, 5));
    track.setPosition(sf::Vector2f((WINDOW_WIDTH - track.getLocalBounds().width)/2, WINDOW_HEIGHT/3*2));
    track.setFillColor(sf::Color(60, 60, 60));

    int sliderMin = 0;
    int sliderMax = 100;
    float sliderValue = 75;

    sf::CircleShape knob(10);
    knob.setFillColor(sf::Color::White);
    knob.setOrigin(knob.getRadius(), knob.getRadius());
    knob.setPosition(track.getPosition().x + (track.getSize().x*sliderValue/100), track.getPosition().y + track.getSize().y / 2);

    float knobXMin = track.getPosition().x;
    float knobXMax = track.getPosition().x + track.getSize().x;

    bool isDragging = false;


    //VolumeIcon
    int volumeIconWidth = 80;
    int volumeIconHeight = 80;
    sf::RectangleShape volumeIcon(sf::Vector2f(volumeIconWidth, volumeIconHeight));
    volumeIcon.setPosition(sf::Vector2f(track.getPosition().x - volumeIcon.getLocalBounds().width - 10, track.getPosition().y - (volumeIcon.getLocalBounds().height/2)));

    sf::Texture volumeIconImage;
    volumeIconImage.loadFromFile("../Assets/volumeIcon.png");

    volumeIcon.setTexture(&volumeIconImage);

    #pragma endregion settings

    #pragma region game
    // GAME

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

    vector<sf::Texture> buttonTextureList = { button1, button2, button3, button4, button5, button6 };

    sf::Sprite button1Sprite;
    button1Sprite.setTexture(button1);

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

    // Max Gold Text (and background)
    sf::Text maxGoldUpgrade("Maximum Gold Upgrade", font, 60);
    sf::RectangleShape maxGoldUpgradeBackground(sf::Vector2f(maxGoldUpgrade.getLocalBounds().width + 20, maxGoldUpgrade.getLocalBounds().height + 20));
    maxGoldUpgrade.setFillColor(sf::Color::White);
    maxGoldUpgradeBackground.setFillColor(sf::Color::Red);
    maxGoldUpgrade.setPosition(sf::Vector2f((WINDOW_WIDTH - maxGoldUpgrade.getLocalBounds().width) / 2, (WINDOW_HEIGHT - maxGoldUpgrade.getLocalBounds().height) / 2));
    maxGoldUpgradeBackground.setPosition(sf::Vector2f((WINDOW_WIDTH - maxGoldUpgrade.getLocalBounds().width) / 2, (WINDOW_HEIGHT - maxGoldUpgrade.getLocalBounds().height) / 2));

    //Gold boost shop
    int goldBoostLvl = 1;

    #pragma endregion game

    while (window.isOpen()) {
        tickTimer += 1;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Escape)
                    window.close(); // Fermer avec �chap
            }
            if (event.type == sf:: Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    #pragma region left_click_actions
                    
                    clicSoundPlayer.play();

                    if(MainMenuScreen | SettingsMenuScreen)
                    {
                        // Settings Button
                        if (isMouseOnButton(sf::Mouse::getPosition(window), settingsButton.getPosition(), settingsButtonWidth, settingsButtonHeight))
                        {
                            MainMenuScreen = !MainMenuScreen;
                            SettingsMenuScreen = !SettingsMenuScreen;
                        }
                    }

                    if (MainMenuScreen)
                    {
                    
                        // Play Button
                        if (isMouseOnButton(sf::Mouse::getPosition(window), playButton.getPosition(), buttonWidth, buttonHeight))
                        {
                            MainMenuScreen = false;
                            MainGameScreen = true;
                        }
                    }

                    if (SettingsMenuScreen)
                    {
                        if (isMouseOnButton(sf::Mouse::getPosition(window), sf::Vector2f(knob.getPosition().x - 20, knob.getPosition().y - 29), 40, 40))
                        {
                            
                            isDragging = true;
                        }
                    }

                    if (MainGameScreen) {
                        //Main Clicker Button Tech
                        if (isMouseOnButton(sf::Mouse::getPosition(window), ClickerButtonPosition, ClickerButtonSizeX, ClickerButtonSizeY))
                        {
                            points += clickPower;
                            for (int i = 0; i < clickPower; i++)
                            {
                                if (rand() % goldDropRate == 0) // une chance sur 10 d'avoir un gold
                                {
                                    golds += 1;
                                }
                            }
                        }

                        // Shop Button 1 Tech
                        if (isMouseOnButton(sf::Mouse::getPosition(window), ShopButtons[0].getPosition(), ShopButtonSize, ShopButtonSize))
                        {
                            if (golds > 4)
                            {
                                golds += -5;
                                clickPower += 1;

                            }

                        }
                        // Shop Button 2 Tech
                        if (isMouseOnButton(sf::Mouse::getPosition(window), ShopButtons[1].getPosition(), ShopButtonSize, ShopButtonSize))
                        {
                            if (golds > 49)
                            {
                                golds += -50;
                                totalGold += -50;
                                clickPower += 10;

                            }

                        }
                        // Shop Button 3 Tech
                        if (isMouseOnButton(sf::Mouse::getPosition(window), ShopButtons[2].getPosition(), ShopButtonSize, ShopButtonSize))
                        {
                            if (golds > 14)
                        {
                            golds += -15;
                            autoclickers += 1;

                            buyTimings.push_back(tickTimer%300); // 300 en sachant qu'il y a 60 ticks par seconde, 5 secondes
                        }

                        }
                        // Shop Button 4 Tech
                        if (isMouseOnButton(sf::Mouse::getPosition(window), ShopButtons[3].getPosition(), ShopButtonSize, ShopButtonSize))
                        {
                            if (golds > 149)
                        {
                            golds += -150;
                            autoclickers += 10;
                            for (int i = 0; i < 10; i++)
                            {
                                buyTimings.push_back((tickTimer + i*3) % 300);
                            }
                            }
                        }

                        // Shop Button 5 Tech
                        if (isMouseOnButton(sf::Mouse::getPosition(window), ShopButtons[4].getPosition(), ShopButtonSize, ShopButtonSize))
                        {
                            if (goldDropRate != 1)
                            {
                                if (golds > (30 * goldBoostLvl * goldBoostLvl)-1)
                                {

                                    golds += -(30 * goldBoostLvl * goldBoostLvl);
                                    goldBoostLvl += 1;
                                    goldDropRate += -1;
                                }
                            }
                            else
                            {
                                tickSinceMaxGoldUpgrade = 0;
                            }
                        }
                    }
                }
                #pragma endregion left_click_actions
            }


            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isDragging = false;
                }
            }
        }
        #pragma region animationTitle
        // Anim Title
        if (launchAnim == -10)
        {
            launchAnimTime = false;
        }
        if (abs(launchAnim) > 10 && !launchAnimTime)
        {
            titleAnimSide = !titleAnimSide;
        }
        if (tickTimer % 3 == 0 | launchAnimTime) {
            if (titleAnimSide)
            {
                launchAnim += 1;
            }
            else
            {
                launchAnim += -1;
            }
        }
        mainTitleText.setPosition(sf::Vector2f((WINDOW_WIDTH - mainTitleText.getLocalBounds().width) / 2, ((WINDOW_HEIGHT - mainTitleText.getLocalBounds().height) / 6) - launchAnim));

        #pragma endregion animationTitle

        #pragma region scoreAndGoldsAnim

        // Points :
        if (totalScore != points)
        {
            totalScore += 1 + 0.1 * (points - totalScore);
        }
        
        
        //Golds : 
        if (totalGold < golds)
        {
            totalGold += 1 + 0.1*(golds-totalGold);
        }
        
        if (totalGold > golds)
        {
            totalGold -= 1 + 0.05 * (totalGold-golds);
        }
        #pragma endregion scoreAndGoldsAnim

        #pragma region autofarm
        // Autofarm
        for (auto autoclick : buyTimings)
        {

            if (tickTimer % 300 == autoclick)
            {
                // Si sur timing d'autoclick
                points += clickPower;

                for (int i = 0; i < clickPower; i++)
                {
                    totalScore += 1;
                    if (rand() % goldDropRate == 0)
                    {
                        golds += 1;
                    }
                }
            }
        }

        #pragma endregion autofarm

        #pragma region shopPriceText

        sf::Text shopPriceText(" 5             50             15            150            " + to_string(goldBoostLvl*30*goldBoostLvl), font, 30);
        shopPriceText.setPosition(60, 260);
        shopPriceText.setFillColor(sf::Color::White);

        #pragma endregion shopPriceText

        #pragma region statsText
        //Stats text
        ostringstream temp;
        ostringstream temp2;
        temp << fixed << setprecision(1) << (float(autoclickers) / 5);
        temp2 << fixed << setprecision(2) << (1 / float(goldDropRate));

        string statsTextString1 = "Click Power : " + to_string(clickPower) + "\n" + "AutoClick/s : " + temp.str() + "\n" + "Gold/point : " + temp2.str();

        sf::Text statsText1(statsTextString1, font, 30);
        statsText1.setPosition(50, 350);
        statsText1.setFillColor(sf::Color::White);

        #pragma endregion statsText

        #pragma region scoreText
        
        //Score text
        int scoreTextMargin = 30;
        sf::Text scoreText(to_string(totalScore), font, 60);
        scoreText.setFillColor(sf::Color::White);

        scoreText.setPosition(WINDOW_WIDTH - scoreText.getLocalBounds().width - scoreTextMargin, 10);

        sf::Text goldText(to_string(totalGold), font, 40);
        goldText.setFillColor(sf::Color::White);

        goldText.setPosition(WINDOW_WIDTH - goldText.getLocalBounds().width - scoreTextMargin, 20 + 60);
        
        #pragma endregion scoreText
        
        #pragma region soundGlider
        
        if (isDragging) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            float newKnobX = static_cast<float>(mousePos.x);

            if (newKnobX < knobXMin) 
            {
                newKnobX = knobXMin;
            }
            if (newKnobX > knobXMax) 
            {
                newKnobX = knobXMax;
            }

            knob.setPosition(newKnobX, knob.getPosition().y);

            // Calcul de la valeur actuelle du slider
            sliderValue = sliderMin + ((newKnobX - knobXMin) / (knobXMax - knobXMin)) * (sliderMax - sliderMin);

            music.setVolume(sliderValue);
        }
        #pragma endregion soundGlider

        #pragma region windowDraw
        window.clear();
        
        if (MainMenuScreen)
        {
            window.draw(mainTitleText);
            window.draw(playButton);
            window.draw(playButtonText);
            window.draw(settingsButton);
        }
        
        if (MainGameScreen) {
            window.draw(BackgroundHeader);
            window.draw(ClickerButton);
            window.draw(shopPriceText);

            for (auto button : ShopButtons)
            {
                window.draw(button);
            }

            window.draw(titleText);
            window.draw(scoreText);
            window.draw(goldText);

            window.draw(statsText1);

            // Max Gold Upgrade
            if (tickSinceMaxGoldUpgrade < 60)
            {
                tickSinceMaxGoldUpgrade += 1;
                window.draw(maxGoldUpgradeBackground);
                window.draw(maxGoldUpgrade);
            }
        }

        if (SettingsMenuScreen)
        {
            window.draw(mainTitleText);
            window.draw(settingsButton);
            window.draw(track);
            window.draw(knob);
            window.draw(volumeIcon);
        }

        window.display();
    }
    #pragma endregion windowDraw

    return 0;
}
