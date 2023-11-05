/**
 * @file main.cpp
 * @brief This program is a game that is played on the OLED display. 
 * The game is a simple game where the player uses a potentimeter to
 * move a platform to catch a ball.
 * Rules: 
 * The player has 3 lives. If the player misses the ball, the player loses a life
 * If the player catches the ball, the player gains a point
 * If the player loses all 3 lives, the game is over
 * Every 5 points, game level increases by 1 and the ball moves faster
 * Button is used to start the game, restart in any moment, and restart when game is over
 * 
 * @details This program is deployed on the ESP32 microcontroller.
*/

// Libraries that are used in this program
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Declaration of the OLED display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // This display does not have a reset pin accessible
#define SCREEN_ADDRESS 0x3C // Address obtained from the I2C Scanner

// Declaration of the OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Resolution of the analog to digital converter, 3.3V/4096
#define ADC_RESOLUTION (3.3 / 4096)

// Declaration of external variables
#define POTENTIOMETER_PIN 34
#define BUTTON_PIN 18

// We don't need define getters because we don't need to get the values of the platform

/** @class Platform
 * @brief This class is used to create a platform that is used to catch the ball,
 * platform must stay within the limits of the OLED display 
 * @param x: x coordinate of the platform
 * @param y: y coordinate of the platform
 * @param width: width of the platform
 * @param height: height of the platform
*/

class Platform {
  public:
    Platform(int x, int y, int width, int height);
    void draw();
    void move(int x);
    int getX() { return x; }
    int getY() { return y; }
  private:
    int x;
    int y;
    int width;
    int height;
};

// Platform parameters
#define PLATFORM_WIDTH 30
#define PLATFORM_HEIGHT 3

// Implement the constructor and methods of the Platform class
Platform::Platform(int x, int y, int width, int height) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
}
void Platform::draw() {
  display.fillRect(x, y, width, height, WHITE);
}
void Platform::move(int x) {
  this->x = x;
}

// Initialize the platform and save it in a variable to use it in the game
Platform platform((SCREEN_WIDTH - PLATFORM_WIDTH) / 2, SCREEN_HEIGHT - PLATFORM_HEIGHT, PLATFORM_WIDTH, PLATFORM_HEIGHT);

/** @class Ball
 * @brief This class is used to create a ball that is used to be caught by the platform,
 * ball must stay within the limits of the OLED display 
 * @param x: x coordinate of the ball
 * @param y: y coordinate of the ball
 * @param radius: radius is just a pixel
 * @param xSpeed: speed of the ball in the x direction
 * @param ySpeed: speed of the ball in the y direction
 * The ball never touches the sides of the screen, that is, SCREEN_WIDTH and 0. So when the next position 
 * of the ball is greater than SCREEN_WIDTH or less than 0, the ball move on the opposite direction to evade the sides of the screen.
 * The ball has a downward movement and moves randomly by 1 pixel in the x direction when falling.
*/
class Ball {
  public:
    Ball(int x, int y, int radius, int xSpeed, int ySpeed);
    void draw();
    void move();
    void reset();
    int getX() { return x; }
    int getY() { return y; }
    void setYSpeed(int ySpeed) { this->ySpeed = ySpeed; }
  private:
    int x;
    int y;
    int radius;
    int xSpeed;
    int ySpeed;
};

// Ball parameters
#define BALL_RADIUS 1
#define BALL_X_SPEED 1
#define BALL_Y_SPEED 1
#define MIN_Y_POSITION 13 // The ball starts at the top of the screen

// Implement the constructor and methods of the Ball class
Ball::Ball(int x, int y, int radius, int xSpeed, int ySpeed) {
  this->x = x;
  this->y = y;
  this->radius = radius;
  this->xSpeed = xSpeed;
  this->ySpeed = ySpeed;
}
void Ball::draw() {
  display.fillCircle(x, y, radius, WHITE);
}
void Ball::move() {
  x += random(-1, 1);
  // We need reduce the speed of the ball because the ball moves too fast
  y += ySpeed;

  // No touch the sides of the screen
  if (x >= SCREEN_WIDTH - BALL_RADIUS) {
    x = SCREEN_WIDTH - BALL_RADIUS;
  }
  if (x <= BALL_RADIUS) {
    x = BALL_RADIUS;
  }

  if (y > SCREEN_HEIGHT) {
    reset();
  }
}
void Ball::reset() {
  x = random(SCREEN_WIDTH);
  y = MIN_Y_POSITION;
}

// Initialize the ball, the ball starts at the top and any x position
Ball ball(random(SCREEN_WIDTH), MIN_Y_POSITION, BALL_RADIUS, BALL_X_SPEED, BALL_Y_SPEED);

/** @class Game
 * @brief This class is used to create a game that is played on the OLED display,
 * game is played by moving the platform to catch the ball
 * @param platform: platform that is used to catch the ball
 * @param ball: ball that is used to be caught by the platform
 * @param score: score of the game
 * @param lives: lives of the game
 * @param level: level of the game
 * @param gameStart: boolean that is used to start the game
 * @param gameOver: boolean that is used to end the game
 * @param gameRestart: boolean that is used to restart the game
*/

class Game {
  public:
    Game(Platform& platform, Ball& ball, int score, int lives, int level, bool gameStart, bool gameOver, bool gameRestart);
    void draw(); // Draw the game on the OLED display, including the platform, ball, score, lives, and level
    void start(); // Start the game
    void restart(); // Restart the game
    void update(); // Update the game, including the platform, ball, score, lives, and level
    void checkCollision(); // Check if the ball is caught by the platform, if the ball is caught, the score increases by 1
    void checkGameOver(); // Check if the game is over, if the game is over, the game is restarted
    void checkLevel(); // Check if the level increases, if the level increases, the ball moves faster
    void checkButton(); // Check if the button is pressed, if the button is pressed, the game starts, restarts, or continues
    void setLevelIncrease(bool levelIncrease) { this->levelIncrease = levelIncrease; }
  private:
    Platform platform;
    Ball ball;
    int score;
    int lives;
    int level;
    bool gameStart;
    bool gameOver;
    bool gameRestart;
    bool levelIncrease;
};

// Game parameters
#define GAME_LIVES 3
#define GAME_LEVEL 1

// Implement the constructor and methods of the Game class
Game::Game(Platform& platform, Ball& ball, int score, int lives, int level, bool gameStart, bool gameOver, bool gameRestart) : platform(platform), ball(ball) {
  this->score = score;
  this->lives = lives;
  this->level = level;
  this->gameStart = gameStart;
  this->gameOver = gameOver;
  this->gameRestart = gameRestart;
  this->levelIncrease = false;
}
// Implement the methods of the Game class
void Game::draw() {
  display.clearDisplay();
  platform.draw();
  ball.draw();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("pts: ");
  display.print(score);
  display.setCursor(50, 0);
  display.print("H: ");
  display.print(lives);
  display.setCursor(90, 0);
  display.print("lvl: ");
  display.print(level);
  display.drawLine(0, 9, SCREEN_WIDTH, 9, WHITE);

  display.display();
}
void Game::start() { // Start the game by pressing the button when the game is not started show the message "Press to start"
  if (!gameStart) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Press to start");
    display.display();
    checkButton();
  } else {
    update();
  }
}
void Game::restart() { // Restart the game by pressing the button when the game is over show the message "Press to restart"
  if (gameOver) {
    gameOver = false;
    checkButton();
  } else {
    update();
  }
}
void Game::update() { // Update the game, including the platform, ball, score, lives, and level
  draw();
  platform.move(analogRead(POTENTIOMETER_PIN) * ADC_RESOLUTION * (SCREEN_WIDTH - PLATFORM_WIDTH));
  ball.move();
  checkCollision();
  checkGameOver();
  checkLevel();
}
void Game::checkCollision() { // Check if the ball is caught by the platform, if the ball is caught, the score increases by 1
  if (ball.getY() + BALL_RADIUS >= platform.getY() && ball.getX() >= platform.getX() && ball.getX() <= platform.getX() + PLATFORM_WIDTH) {
    score++;
    ball.reset();
  }
}
void Game::checkGameOver() { // Check if the game is over, if the game is over, the game is restarted pressing the button
  if (ball.getY() + BALL_RADIUS >= SCREEN_HEIGHT) {
    lives--;
    ball.reset();
  }
  if (lives == 0) {
    gameOver = true;
    gameStart = false;
    score = 0;
    lives = GAME_LIVES;
    level = GAME_LEVEL;

    ball = Ball(random(SCREEN_WIDTH), MIN_Y_POSITION, BALL_RADIUS, BALL_X_SPEED, BALL_Y_SPEED);
    ball.reset();
    restart();
  }
}
void Game::checkLevel() { // Check if the level increases, if the level increases, the ball moves faster
  if (score % 5 == 0 && score != 0 && !levelIncrease) {
    level++;
    setLevelIncrease(true);
    // Increase the speed of the ball
    ball.setYSpeed(level);
    Serial.println("Level up!");
    // Apply changes to the ball 
    ball.reset();

  } else if (score % 5 != 0) {
    setLevelIncrease(false);
  }
}
void Game::checkButton() { // Check if the button is pressed, if the button is pressed, the game starts, restarts, or continues
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (!gameStart) {
      gameStart = true;
    } else if (gameOver) {
      gameOver = false;
      gameRestart = true;
    }
  }
}

// Initialize the game
Game game(platform, ball, 0, GAME_LIVES, GAME_LEVEL, false, false, false);

void setup() {
  Serial.begin(9600);

  // Initialize the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  // Initialize the potentiometer
  pinMode(POTENTIOMETER_PIN, INPUT);

  // Initialize the button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  game.start();
}