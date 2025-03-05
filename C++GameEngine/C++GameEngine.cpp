#include <iostream>
#include <memory>
#include <fstream>
#include "Vector2.hpp"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

void LoadFromFile(const std::string& filepath)
{
    std::ifstream fin(filepath);
    std::string temp;

    while (fin >> temp)
    {
        std::cout << temp << "\n";
    }
}

int main()
{

    const int wWidth = 1280;
    const int wHeight = 720;

    sf::RenderWindow window(sf::VideoMode({ wWidth, wHeight }), "GameScreen");
    window.setFramerateLimit(60);

    //LoadFromFile("assets/config.txt");


    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    ImGui::GetStyle().ScaleAllSizes(1.0f);
    ImGui::GetIO().FontGlobalScale = 1.0f;

    float c[3] = { 0.0f, 1.0f, 1.0f };

    //circle parameters
    float cirlceRadius = 50;
    int circleSegments = 32;
    float circleSpeedX = 1.0;
    float circleSpeedY = 0.5;
    bool drawCircle = true;
    bool drawText = true;

    sf::CircleShape circle(cirlceRadius, circleSegments);
    circle.setPosition({10,10});

    sf::Font myFont;
    sf::Texture textyre;

    if (!textyre.loadFromFile("Solid_white_bordered.png"))
    {
        std::cerr << "Could not load texture! \n";
        exit(0);
    }

    if (!myFont.openFromFile("./assets/fonts/Geo-Regular.ttf"))
    {
        std::cerr << "Could not load font! \n";
        exit(0);
    }

    sf::Text textSample(myFont, "Sample Text", 24);
    textSample.setPosition({ 0, wHeight - (float)textSample.getCharacterSize() });

    char displayString[255] = "AnotherSampleText";


    //vector testing
    Vector2<float> vec1(-5.0f, 6.2f);
    Vector2<float> vec2(3.0f, 5.0f);


    std::cout << vec1 + vec2 << '\n';
    std::cout << vec1 - vec2 << '\n';
    std::cout << vec1 * 2 << '\n';
    std::cout << vec1 / 2 << '\n';
    std::cout << vec1.magnitude() << '\n';
    std::cout << vec1.normalized() << '\n';
    std::cout << vec1.distance(vec2) << '\n';
    std::cout << vec1.sqrdDistance(vec2) << '\n';
    Vector2 scaled(vec1.normalized());
    std::cout << scaled;
    std::cout << scaled.magnitude();

    //the game loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                std::cout << "Key pressed with code = " << static_cast<int>(keyPressed->scancode);
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());
        //draw imgui UI
        ImGui::Begin("Window title");
        ImGui::Text("Window Text");
        ImGui::Checkbox("DrawCircle", &drawCircle);
        ImGui::SameLine();
        ImGui::Checkbox("DrawText", &drawText);
        ImGui::SliderFloat("Radius", &cirlceRadius, 0.0f, 300.0f);
        ImGui::SliderInt("Slides", &circleSegments, 3, 64);
        ImGui::ColorEdit3("Circle Color", c);
        ImGui::InputText("Text", displayString, 255);
        if (ImGui::Button("Set Text"))
        {
            textSample.setString(displayString);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Circle"))
        {
            circle.setPosition({0,0});
        }
        ImGui::End();


        circle.setPointCount(circleSegments);
        circle.setRadius(cirlceRadius);

        circle.setFillColor(sf::Color(int(c[0] * 255), int(c[1] * 255), int(c[2] * 255)));
        circle.setPosition({ circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY });

        window.clear();
        if (drawCircle)
        {
            window.draw(circle);
        }
        if (drawText)
        {
            window.draw(textSample);
        }
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

}