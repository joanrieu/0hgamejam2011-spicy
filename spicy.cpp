#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define BLOCK 10
#define V2R(v) sf::FloatRect(v, sf::Vector2f(BLOCK, BLOCK))

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
                        if (V2R(*it).Intersects(V2R(pos)))
                                exit(1);

        }

}

int main() {

        sf::RenderWindow win(sf::VideoMode(800, 600), "Spicy", sf::Style::Default &~ sf::Style::Resize);

        win.ShowMouseCursor(false);
        win.EnableVerticalSync(true);

        sf::Mouse::SetPosition(sf::Vector2i(win.GetWidth() / 2, win.GetHeight() / 2), win);

        blocks.insert(sf::Vector2f(50, 50));

        while (win.IsOpened()) {

                sf::Event event;
                while (win.PollEvent(event)) {

                        if (event.Type == sf::Event::Closed or (event.Type == sf::Event::KeyPressed and event.Key.Code == sf::Keyboard::Escape))
                                win.Close();

                        else if (event.Type == sf::Event::MouseMoved)
                                spd = 2.f * sf::Vector2f(static_cast<float>(event.MouseMove.X) / win.GetWidth(), static_cast<float>(event.MouseMove.Y) / win.GetHeight()) - sf::Vector2f(1.f, 1.f);

                }

                {
                        const int mx = sf::Mouse::GetPosition(win).x;
                        const int my = sf::Mouse::GetPosition(win).y;

                        const int nmx = mx < 0 ? 0 : (mx >= win.GetWidth() ? win.GetWidth() : mx);
                        const int nmy = my < 0 ? 0 : (my >= win.GetHeight() ? win.GetHeight() : my);

                        sf::Mouse::SetPosition(sf::Vector2i(nmx, nmy), win);

                }

                win.Clear(sf::Color(100, 100, 100));

                const sf::Uint32 delta = win.GetFrameTime();
                updatePhysics(delta);

                for (BlockSet::const_iterator it = blocks.begin(); it != blocks.end(); ++it)
                        win.Draw(sf::Shape::Rectangle(V2R(*it - pos), sf::Color(20, 20, 20)));

                win.Draw(sf::Shape::Rectangle(V2R(sf::Vector2f(win.GetWidth() / 2, win.GetHeight() / 2)), sf::Color(100, 0, 0)));

                win.Display();

        }

        return EXIT_SUCCESS;

}

