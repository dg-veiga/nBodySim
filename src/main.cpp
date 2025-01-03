#include <SFML/Graphics.hpp>
#include <complex>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define N_BODIES 15
#define TRAIL_SIZE 50
#define BODY_SIZE 10.0
#define G 10000.0

class Body
{
private:
    float mass_ = 50.0;
    float state_[6];
    std::complex<float> vel_ = {static_cast<float>(rand() % 100), static_cast<float>(rand() % 100)};
    sf::CircleShape shape_;
    sf::Time dt_ = sf::milliseconds(16/10);
    sf::RenderWindow &window_;
    std::vector<sf::CircleShape> trail_;

public:
    float loc[2];

    Body(sf::RenderWindow &window) : window_(window), trail_(TRAIL_SIZE)
    {
        loc[0] = static_cast<float>(rand() % WINDOW_WIDTH);
        loc[1] = static_cast<float>(rand() % WINDOW_HEIGHT);

        shape_ = sf::CircleShape(BODY_SIZE);
        shape_.setFillColor(sf::Color::Magenta);
    };

    ~Body() {};

    void update(std::vector<Body> bodies)
    {
        sf::sleep(dt_);
        float dt = dt_.asSeconds();

        std::complex<float> acc = getGravityPullAcc(bodies);
        vel_ += acc * dt;

        trail_.erase(trail_.begin());
        trail_.push_back(shape_);
        trail_.back().setPosition(loc[0], loc[1]);
        
        loc[0] += vel_.real() * dt;
        loc[1] += vel_.imag() * dt;
        this->shape_.setPosition(loc[0], loc[1]);
    };

    std::complex<float> getGravityPullAcc(std::vector<Body> bodies)
    {
        std::complex<float> total_acc = {0.0, 0.0};
        for (const auto &body : bodies)
        {
            float dx = body.loc[0] - loc[0];
            float dy = body.loc[1] - loc[1];
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance > 0)
            {
                float force = static_cast<float>(G * mass_ * body.mass_ / (distance * distance));
                total_acc += std::complex<float>(force * dx / distance, force * dy / distance);
            }
        }
        return total_acc;
    };

    void drawTrail(sf::RenderWindow &window){
        for (int i = 0; i < TRAIL_SIZE; ++i)
        {
            trail_[i].setFillColor(sf::Color::Color(255,0,0,255 - 255 * (TRAIL_SIZE-i) / TRAIL_SIZE));
            trail_[i].setRadius(float(BODY_SIZE * i / TRAIL_SIZE));
            window.draw(trail_[i]);
        }  
    };

    void render()
    {
        this->drawTrail(this->window_);
        this->window_.draw(this->shape_);
    };
};

void updateUniverse(sf::RenderWindow &window, std::vector<Body> &bodies)
{
    for (auto &body : bodies)
    {
        body.update(bodies);
        body.render();
    }
};

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML window");

    std::vector<Body> bodies;
    for (int i = 0; i < N_BODIES; ++i)
    {
        bodies.emplace_back(window);
    }

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;

        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear screen
        window.clear();

        updateUniverse(window, bodies);

        // Update the window
        window.display();
    }

    return 0;
}