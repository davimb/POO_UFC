#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

void setSize(sf::Sprite& sprite, int width, int height) {

    auto dim = sprite.getLocalBounds();
    sprite.setScale((float) width / dim.width, (float) height / dim.height);

}

struct Entity {

    int x {0};
    int y {0};
    int step {0};
    sf::Sprite sprite;

    Entity(int x, int y, int step, sf::Texture& texture):
        x {x}, y {y}, step {step}, sprite(texture) {
    }

    void draw(sf::RenderWindow& window) {
        this->sprite.setPosition(x * step, y * step);
        setSize(this->sprite, step, step);
        window.draw(this->sprite);
    }

};

struct Board {

    int nc {0};
    int nl {0};
    int step {0};
    sf::Sprite sprite;
    sf::RectangleShape rect;

    Board(int nc, int nl, int step, sf::Texture& texture) {
        this->nc = nc;
        this->nl = nl;
        this->step = step;
        this->sprite.setTexture(texture);
        setSize(this->sprite, nc * step, nl * step);
        this->sprite.setPosition(0, 0);
        this->rect.setSize(sf::Vector2f(step, step));
        this->rect.setFillColor(sf::Color::Transparent);
        this->rect.setOutlineColor(sf::Color::Black);
        this->rect.setOutlineThickness(2);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(this->sprite);
        for (int i = 0; i < nc; i++) {
            for (int j = 0; j < nl; j++) {
                rect.setPosition(i * step, j * step);
                window.draw(rect);
            }
        }
    }

};

void moveEntity(sf::Keyboard::Key key, Entity& entity, std::vector<sf::Keyboard::Key> move_keys) {

    if (key == move_keys[0])
        entity.x--;
    else if (key == move_keys[1])
        entity.y--;
    else if (key == move_keys[2])
        entity.x++;
    else if (key == move_keys[3])
        entity.y++;

}

sf::Texture loadTexture(std::string path) {

    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cout << "Error loading texture" << std::endl;
        exit(1);
    }
    return texture;

}

int main() {

    sf::Texture rosquinha_tex { loadTexture("rosquinha.png") };
    sf::Texture homer_tex { loadTexture("homer.png") };
    sf::Texture spring_tex { loadTexture("springfield.jpg") };
    sf::Texture rosmer_tex { loadTexture("rosmer.png") };
    bool aux=0;

    const int STEP {100};

    Entity rosquinha(2, 2, STEP, rosquinha_tex);
    Entity homer(0, 0, STEP, homer_tex);
    Board board(7, 5, STEP, spring_tex);
    Entity rosmer(0, 0, STEP, rosmer_tex);

    sf::RenderWindow window(sf::VideoMode(board.nc * STEP, board.nl * STEP), "SFML works!");

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed and aux==0) {
                moveEntity(event.key.code, rosquinha, {sf::Keyboard::Left, sf::Keyboard::Up, sf::Keyboard::Right, sf::Keyboard::Down});

                if (homer.x == rosquinha.x and homer.y == rosquinha.y) {
                    rosmer.x = homer.x;
                    rosmer.y = homer.y;
                    aux=1;
                }

                moveEntity(event.key.code, homer, {sf::Keyboard::A, sf::Keyboard::W, sf::Keyboard::D, sf::Keyboard::S});

                if (homer.x == rosquinha.x and homer.y == rosquinha.y) {
                    rosmer.x = homer.x;
                    rosmer.y = homer.y;
                    aux=1;
                }
            }
            else if (event.type == sf::Event::KeyPressed and aux==1) {
                moveEntity(event.key.code, rosmer, {sf::Keyboard::Left, sf::Keyboard::Up, sf::Keyboard::Right, sf::Keyboard::Down});
            }
        }
        window.clear();
        board.draw(window);
        if(aux==0) {
        rosquinha.draw(window);
        homer.draw(window);
        }
        else {
        rosmer.draw(window);
        }
        window.display();
    }
    return 0;

}
