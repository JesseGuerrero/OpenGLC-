#include <GL/glew.h>
#include <vector>
#include <iostream>

class PongItem {
public:
    GLuint GL_Pointer;
    float y;
    float x;
    float halfWidth;
    float halfHeight;

    virtual void UpdateItem() = 0;
};

class Paddle : public PongItem {
public:
    Paddle(float initialX, float initialY) {
        this->x = initialX;
        this->y = initialY;
        this->halfWidth = 0.03;
        this->halfHeight = 0.1f;
        Vector3f Vertices[4];
        Vertices[0] = Vector3f(x, y - halfHeight, 0.0f);   // bottom left
        Vertices[1] = Vector3f(closerToZero(x, halfWidth), y - halfHeight, 0.0f);    // bottom right
        Vertices[2] = Vector3f(closerToZero(x, halfWidth), y + halfHeight, 0.0f);     // top right
        Vertices[3] = Vector3f(x, y + halfHeight, 0.0f);     // top left

        glGenBuffers(1, &GL_Pointer);
        glBindBuffer(GL_ARRAY_BUFFER, GL_Pointer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    }



    void MoveUp(float amount) {
        if(y < 1.0 - 0.1f)
            y += amount; // Move the paddle up by the specified amount
    }

    void MoveDown(float amount) {
        if(y > -1.0 + 0.1f)
            y -= amount; // Move the paddle down by the specified amount
    }

    float closerToZero(float num, float diff) {
        if (num > 0)
            num -= diff;
        else if (num < 0)
            num += diff;
        return num;
    }

    void UpdateItem() override
    {
        Vector3f Vertices[4];
        Vertices[0] = Vector3f(x, y - halfHeight, 0.0f);   // bottom left
        Vertices[1] = Vector3f(closerToZero(x, halfWidth), y - halfHeight, 0.0f);    // bottom right
        Vertices[2] = Vector3f(closerToZero(x, halfWidth), y + halfHeight, 0.0f);     // top right
        Vertices[3] = Vector3f(x, y + halfHeight, 0.0f);     // top left

        glBindBuffer(GL_ARRAY_BUFFER, GL_Pointer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    }
};

vector<PongItem*> items;
int leftScore = 0;
int rightScore = 0;
class Ball : public PongItem {
public:
    float ball_speed = 0.007f;
    float ball_dir_x = 1.0f;
    float ball_dir_y = 0.0f;
    Ball() {
        this->x = 0;
        this->y = 0;
        this->halfWidth = 0.015;
        this->halfHeight = 0.015;
        Vector3f Vertices[4];
        Vertices[0] = Vector3f(x + halfWidth, y - halfHeight, 0.0f);   // bottom right
        Vertices[1] = Vector3f(x - halfWidth, y - halfHeight, 0.0f);    // bottom left
        Vertices[2] = Vector3f(x - halfWidth, y + halfHeight, 0.0f);     // top left
        Vertices[3] = Vector3f(x + halfWidth, y + halfHeight, 0.0f);     // top right

        glGenBuffers(1, &GL_Pointer);
        glBindBuffer(GL_ARRAY_BUFFER, GL_Pointer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    }

    void vec2_norm(float& x, float &y) {
        // sets a vectors length to 1 (which means that x + y == 1)
        float length = sqrt((x * x) + (y * y));
        if (length != 0.0f) {
            length = 1.0f / length;
            x *= length;
            y *= length;
        }
    }

    void UpdateItem() override
    {
        // fly a bit
        x += ball_dir_x * ball_speed;
        y += ball_dir_y * ball_speed;
        float racket_width = items[0]->halfWidth * 2;
        float racket_height = items[0]->halfHeight * 2;

        // hit by left racket?
        float racket_left_x = items[0]->x - items[0]->halfWidth;
        float racket_left_y = items[0]->y - items[0]->halfHeight;
        if (x < racket_left_x + racket_width &&
            x > racket_left_x &&
            y < racket_left_y + racket_height &&
            y > racket_left_y) {
            // set fly direction depending on where it hit the racket
            // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
            float t = ((y - racket_left_y) / racket_height) - 0.5f;
            ball_dir_x = fabs(ball_dir_x); // force it to be positive
            ball_dir_y = t;
        }

        float racket_right_x = items[1]->x - items[1]->halfWidth;
        float racket_right_y = items[1]->y - items[1]->halfHeight;
        // hit by right racket?
        if (x > racket_right_x &&
            x < racket_right_x + racket_width &&
            y < racket_right_y + racket_height &&
            y > racket_right_y) {
            // set fly direction depending on where it hit the racket
            // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
            float t = ((y - racket_right_y) / racket_height) - 0.5f;
            ball_dir_x = -fabs(ball_dir_x); // force it to be negative
            ball_dir_y = t;
        }

        // hit left wall?
        if (x <= -1) {
            ++rightScore;
            x = 0;
            y = 0;
            ball_dir_x = fabs(ball_dir_x); // force it to be positive
            ball_dir_y = 0;
        }

        // hit right wall?
        if (x >= 1) {
            ++leftScore;
            x = 0;
            y = 0;
            ball_dir_x = -fabs(ball_dir_x); // force it to be negative
            ball_dir_y = 0;
        }

        // hit top wall?
        if (y >= 1) {
            ball_dir_y = -fabs(ball_dir_y); // force it to be negative
        }

        // hit bottom wall?
        if (y <= -1) {
            ball_dir_y = fabs(ball_dir_y); // force it to be positive
        }

        // make sure that length of dir stays at 1
        vec2_norm(ball_dir_x, ball_dir_y);

        Vector3f Vertices[4];
        Vertices[0] = Vector3f(x + halfWidth, y - halfHeight, 0.0f);   // bottom left
        Vertices[1] = Vector3f(x - halfWidth, y - halfHeight, 0.0f);    // bottom right
        Vertices[2] = Vector3f(x - halfWidth, y + halfHeight, 0.0f);     // top right
        Vertices[3] = Vector3f(x + halfWidth, y + halfHeight, 0.0f);     // top left

        glBindBuffer(GL_ARRAY_BUFFER, GL_Pointer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    }
};

static void Render(Shader &shader)
{
    shader.use();
    for(PongItem* item : items)
        item->UpdateItem();

    for(const PongItem* item : items) {
        glBindBuffer(GL_ARRAY_BUFFER, item->GL_Pointer);

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_POLYGON, 0, 4);

        glDisableVertexAttribArray(0);
    }
}

#include <unordered_map>
std::unordered_map<char, bool> keyState;
static void PollInputEvents(SDL_Event* event, bool* quit) {
    while (SDL_PollEvent(event)) {
        switch(event->type) {
            case SDL_QUIT:
                *quit = true;
                break;
            case SDL_KEYDOWN:
                keyState[event->key.keysym.sym] = true;
                break;
            case SDL_KEYUP:
                keyState[event->key.keysym.sym] = false;
                break;
        }
    }
}

static bool isKeyDown(char key) {
    return keyState[key];
}

static void HandleUserInput() {
    if(isKeyDown('w'))
        dynamic_cast<Paddle*>(items[0])->MoveUp(0.01f);
    if(isKeyDown('s'))
        dynamic_cast<Paddle*>(items[0])->MoveDown(0.01f);
    if(isKeyDown(SDLK_UP))
        dynamic_cast<Paddle*>(items[1])->MoveUp(0.01f);
    if(isKeyDown(SDLK_DOWN))
        dynamic_cast<Paddle*>(items[1])->MoveDown(0.01f);
}
