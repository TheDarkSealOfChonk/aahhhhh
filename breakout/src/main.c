#include <stdio.h>
#include <string.h>

#include "render.h"

typedef struct Ball {
  float x, y;
  float xVelocity, yVelocity;
} Ball;

char bricks[6][6];
float paddleX = 0.0f;
float oldPaddleX = 0.0f;
Ball ball = {0};
int score = 3;

typedef struct AABB {
  float xMin, yMin; // Minimum x and y coordinates
  float xMax, yMax; // Maximum x and y coordinates
} AABB;

char isColliding(AABB a, AABB b) {
  // Check if there's no overlap on the x-axis
  if (a.xMax < b.xMin || a.xMin > b.xMax) {
    return 0;
  }

  // Check if there's no overlap on the y-axis
  if (a.yMax < b.yMin || a.yMin > b.yMax) {
    return 0;
  }

  // If both tests fail, there's an overlap
  return 1;
}

void drawQuad(float x1, float y1, float x2, float y2) {}

void drawBrick(float x, float y) {
  // x += 0.5;
  // y += 0.5;
  drawQuad(x + -0.1f, y + 0.05f, x + 0.1f, y + -0.05f);
}

void drawBricks() {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      if (bricks[j][i]) {
        drawBrick((-0.85f + (0.3f * j)), (-0.1f + (0.2f * i)));
      }
    }
  }
}

void drawPaddle() {
  drawBrick(paddleX, -0.9f); // draw a brick
}

void drawBall(float x, float y) {
  drawQuad(x + -0.05f, y + 0.05f, x + 0.05f, y + -0.05f);
}

void drawBalls() {
  drawBall(ball.x, ball.y); // Draw the ball
}

char checkBallWithBrick(int j, int i, float ballX, float ballY) {
  if (bricks[i][j]) {
    float brickX = (-0.85f + (0.3f * j));
    float brickY = (-0.1f + (0.2f * i));

    AABB ballBox;
    ballBox.xMin = ballX + -0.05f;
    ballBox.xMax = ballX + 0.05f;
    ballBox.yMin = ballY + -0.05f;
    ballBox.yMax = ballY + 0.05f;

    AABB brickBox;
    brickBox.xMin = brickX + -0.1f;
    brickBox.xMax = brickX + 0.1f;
    brickBox.yMin = brickY + -0.05f;
    brickBox.yMax = brickY + 0.05f;

    return isColliding(ballBox, brickBox);
  }

  return 0;
}

char checkBallWithPaddle(float ballX, float ballY) {
  AABB ballBox;
  ballBox.xMin = ballX + -0.05f;
  ballBox.xMax = ballX + 0.05f;
  ballBox.yMin = ballY + -0.05f;
  ballBox.yMax = ballY + 0.05f;

  AABB paddleBox;
  paddleBox.xMin = paddleX + -0.1f;
  paddleBox.xMax = paddleX + 0.1f;
  paddleBox.yMin = -0.9f + -0.05f;
  paddleBox.yMax = -0.9f + 0.05f;

  return isColliding(ballBox, paddleBox);
}

void moveBalls() {
  float newBallX = ball.x + ball.xVelocity;
  float newBallY = ball.y + ball.yVelocity;

  char collided = 0;

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      if (checkBallWithBrick(j, i, newBallX, newBallY)) {
        collided = 1;
        bricks[j][i] -= 1;
        if (bricks[j][i] == 0) {
          score++;
          printf("new score: %i\n", score);
        }
      }
    }
  }

  if (checkBallWithPaddle(newBallX, newBallY)) {
    collided = 1;
    ball.xVelocity += (paddleX - oldPaddleX) * 0.2f;
  }

  if (newBallY >= 0.95f) {
    collided = 1;
  }

  if (collided) {
    ball.xVelocity = ball.xVelocity;
    ball.yVelocity = -ball.yVelocity;
  }

  ball.x = newBallX;
  ball.y = newBallY;
}

void movePaddle() {
  float newPaddleX = paddleX;

  if (glfwGetKey(getWindow(), GLFW_KEY_A)) {
    newPaddleX += -0.01f;
  }

  if (glfwGetKey(getWindow(), GLFW_KEY_D)) {
    newPaddleX += 0.01f;
  }

  char wasTouchingBall = checkBallWithPaddle(ball.x, ball.y);

  oldPaddleX = paddleX;
  paddleX = newPaddleX;

  if (checkBallWithPaddle(ball.x, ball.y) && (!wasTouchingBall)) {
    ball.xVelocity = paddleX - oldPaddleX;
  }
}

void resetBalls() {
  if ((ball.y <= -1.5f) || (ball.x <= -1.5f) || (ball.x >= 1.5f)) {
    ball.x = 0.0f;
    ball.y = -0.7f;
    ball.yVelocity = 0.01f;
    ball.xVelocity = 0.001f;
    score -= 2;
    printf("new score: %i\n", score);
  }
}

void endGame() {
  if (score <= 0) {
    glfwSetWindowShouldClose(getWindow(), 1);
  }
}

void clearScreen() { glClear(GL_COLOR_BUFFER_BIT); }

void frameEnd() {
  glfwSwapBuffers(getWindow());
  glfwPollEvents();
}

int main() {
  initRenderer();

  for (int i = 0; i < 6; i++) {
    memset(bricks[i], 2, 6);
  }

  ball.y = -0.7f;
  ball.yVelocity = 0.01f;
  ball.xVelocity = 0.001f;

  // Main loop
  while (!glfwWindowShouldClose(getWindow())) {
    clearScreen();
    drawBricks();
    drawPaddle();
    drawBalls();
    movePaddle();
    moveBalls();
    resetBalls();
    endGame();

    drawFrame();

    frameEnd();
  }

  endRenderer();

  return 0;
}
