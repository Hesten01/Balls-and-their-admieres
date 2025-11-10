#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define WIDTH 400
#define HEIGHT 400
#define TITLE "Balls and their admirers"
#define BALL_COUNT 10
#define FPS 60
#define VEL_MAX 50
#define RADIUS_MAX 20
#define RADIUS_MIN 5


Color COLORS[] = {
    LIGHTGRAY, GRAY,   DARKGRAY, YELLOW,     GOLD,      ORANGE,  PINK,
    RED,       MAROON, GREEN,    LIME,       DARKGREEN, SKYBLUE, BLUE,
    DARKBLUE,  PURPLE, VIOLET,   DARKPURPLE, BEIGE,     BROWN,   DARKBROWN,
};

// Definition of Ball
// Ball stores state and other properties
// YOUR CODE HERE
struct Ball {
    int posx, posy, velx, vely, radius;
    struct Ball *follows;    // Changed from int* to struct Ball*
    Color color;
};

int leader;



struct Ball balls[BALL_COUNT];

// Initializes a ball with random values
struct Ball *init_ball_random(struct Ball *p) {
  // Randomly initialize state and properties
  p->posx = rand() % WIDTH;
  p->posy = rand() % HEIGHT;
  p->velx = rand() % VEL_MAX;
  p->vely = rand() % VEL_MAX;
  p->radius = (rand() % (RADIUS_MAX / RADIUS_MIN + 1)) + RADIUS_MIN;
  p->color = COLORS[rand() % (sizeof(COLORS) / sizeof(COLORS[0]))];
  p->follows = &balls[0];

  if (p == &balls[0]) {
    // First ball is the leader - moves in a more interesting pattern
    p->velx = 2;  // Give it a constant velocity for smooth movement
    p->vely = 2;
    p->follows = p;  // Leader follows itself
    p->color = RED;  // Make leader red to distinguish it
    p->radius = RADIUS_MAX;  // Make leader bigger
  }
  return p;
}

// Initialize all `balls`
void init_balls_random() {
  // YOUR CODE HERE
  for (size_t i = 0; i < BALL_COUNT; i++) {
    init_ball_random(&balls[i]);
  }
 
}

struct Ball *draw_ball(struct Ball *p) {
  DrawCircle(p->posx, p->posy, p->radius, p->color);
  return p;
}

// Updates the positions of balls according to their velocities
struct Ball *update_pos(struct Ball *p) {
  p->posx = (WIDTH + p->posx + p->velx) %
            WIDTH; // `WIDTH +` because C uses truncated division
  p->posy = (HEIGHT + p->posy + p->vely) % HEIGHT;
  return p;
}

// Updates the velocity of a ball so that it follows the leading ball
struct Ball *update_vel_for_following(struct Ball *p) {
  int errx = p->follows->posx - p->posx;
  int erry = p->follows->posy - p->posy;
  p->velx = errx > 0 ? 1 : -1;
  p->vely = erry > 0 ? 1 : -1;
  return p;
}

// Update all elements
void update_elements() {
  for (size_t i = 0; i < BALL_COUNT; ++i) {
    draw_ball(update_pos(update_vel_for_following(&balls[i])));
  }
}

int main() {
  srand(time(NULL));  // Initialize random seed
  InitWindow(WIDTH, HEIGHT, TITLE);
  SetTargetFPS(FPS);

  init_balls_random();

  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    BeginDrawing();
    update_elements();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }
}