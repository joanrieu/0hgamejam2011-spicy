#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define BLOCK 10

sf::FloatRect V2R(const sf::Vector2f& pos) {
        return sf::FloatRect(pos, sf::Vector2f(BLOCK, BLOCK));
}

sf::RectangleShape V2R(const sf::Vector2f& pos, const sf::Color& color) {
        sf::RectangleShape rect(sf::Vector2f(BLOCK, BLOCK));
        rect.setPosition(pos);
        rect.setFillColor(color);
        return rect;
}

template<typename T>
struct cmp {
bool operator()(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
        if (a.x < b.x)
                return true;
        else
                return a.y < b.y;
}
};

sf::Vector2f pos, spd;

typedef std::set<sf::Vector2f, cmp<float> > BlockSet;
BlockSet blocks;

void updatePhysics(unsigned steps) {

        while (steps --> 0) {

                pos += spd / 3.f;

                for (BlockSet::const_iterator it = blocks.begin(); it != blocks.end(); ++it)
                        if (V2R(*it).intersects(V2R(pos)))
                                blocks.erase(it);

        }

}

int main() {

        sf::RenderWindow win(sf::VideoMode(800, 600), "Spicy", sf::Style::Default &~ sf::Style::Resize);

        win.setMouseCursorVisible(false);
        win.setVerticalSyncEnabled(true);

        sf::Mouse::setPosition(sf::Vector2i(win.getSize().x / 2, win.getSize().y / 2), win);

        blocks.insert(sf::Vector2f(50, 50));
        blocks.insert(sf::Vector2f(150, 50));
        blocks.insert(sf::Vector2f(50, 250));
        blocks.insert(sf::Vector2f(0, 550));

        sf::Clock clock;

        while (win.isOpen()) {

                sf::Event event;
                while (win.pollEvent(event)) {

                        if (event.type == sf::Event::Closed or (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape))
                                win.close();

                        else if (event.type == sf::Event::MouseMoved)
                                spd = 2.f * sf::Vector2f(static_cast<float>(event.mouseMove.x) / win.getSize().x, static_cast<float>(event.mouseMove.y) / win.getSize().y) - sf::Vector2f(1.f, 1.f);

                }

                {
                        const int mx = sf::Mouse::getPosition(win).x;
                        const int my = sf::Mouse::getPosition(win).y;

                        const int nmx = mx < 0 ? 0 : (mx >= win.getSize().x ? win.getSize().x : mx);
                        const int nmy = my < 0 ? 0 : (my >= win.getSize().y ? win.getSize().y : my);

                        sf::Mouse::setPosition(sf::Vector2i(nmx, nmy), win);

                }

                win.clear(sf::Color(100, 100, 100));

                const sf::Uint32 delta = clock.restart().asMilliseconds();
                updatePhysics(delta);

                for (BlockSet::const_iterator it = blocks.begin(); it != blocks.end(); ++it)
                        win.draw(V2R(*it - pos + sf::Vector2f(win.getSize().x / 2, win.getSize().y / 2), sf::Color(20, 20, 20)));

                win.draw(V2R(sf::Vector2f(win.getSize().x / 2, win.getSize().y / 2), sf::Color(100, 0, 0)));

                win.display();

        }

        return EXIT_SUCCESS;

}

