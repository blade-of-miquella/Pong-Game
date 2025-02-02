#include "raylib.h"

Color green = Color{38, 185, 154, 255};
Color dark_green = Color{ 20, 160, 133, 255 };
Color light_green = Color{ 129, 204, 184, 255 };
Color yellow = Color{ 243, 213, 91, 255 };

int player_score = 0;
int cpu_score = 0;

class Ball {
public:
	float x, y;
	int speedX, speedY;
	int radius;

	void Draw() {
		DrawCircle(x, y, radius, yellow);
	}

	void Update() {
		x += speedX; y += speedY;
		if (y + radius >= GetScreenHeight() || y - radius <= 0) {
			speedY *= -1;
		}
		if (x + radius >= GetScreenWidth()) {
			cpu_score++;
			ResetBall();
		}
		if (x - radius <= 0) {
			player_score++;
			ResetBall();
		}
	}

	void ResetBall() {
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		int speed_choices[2] = { -1, 1 };
		speedX *= speed_choices[GetRandomValue(0, 1)];
		speedY *= speed_choices[GetRandomValue(0, 1)];
	}
};

class Paddle {
public:
	float x, y, width, height;
	int speedY;

	void Draw() {
		DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
	}

	void Update() {
		if (IsKeyDown(KEY_W)) {
			y = y - speedY;
		}
		if (IsKeyDown(KEY_S)) {
			y = y + speedY;
		}
		
	}
	void LimitMove() {
		if (y < 0) {
			y = 0;
		}
		if (y + height > GetScreenHeight()) {
			y = GetScreenHeight() - height;
		}
	}
};

class CPUPaddle : public Paddle {
public:
	void Update(int ball_y) {
		if (y + height / 2 > ball_y) {
			y = y - speedY;
		}
		if (y + height / 2 < ball_y) {
			y = y + speedY;
		}
	}
};

Ball ball;
Paddle player;
CPUPaddle cpu;

int main() {

	const int screen_width = 1280;
	const int screen_height = 800;
	InitWindow(screen_width, screen_height, "Pong Game");
	SetTargetFPS(60);

	ball.radius = 20;
	ball.x = screen_width / 2; ball.y = screen_height / 2;
	ball.speedX = 7; ball.speedY = 7;

	player.width = 25; player.height = 120;
	player.x = screen_width - player.width - 10; player.y = screen_height / 2 - player.height / 2;
	player.speedY = 6;

	cpu.width = 25; cpu.height = 120;
	cpu.x = 10; cpu.y = screen_height / 2 - cpu.height / 2;
	cpu.speedY = 6;

	while (!WindowShouldClose()) {
		BeginDrawing();

		ball.Update();
		player.Update();
		player.LimitMove();
		cpu.Update(ball.y);
		cpu.LimitMove();

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height })) {
			ball.speedX *= -1;
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height })) {
			ball.speedX *= -1;
		}

		ClearBackground(dark_green);
		DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, green);
		DrawCircle(screen_width / 2, screen_height / 2, 150, light_green);
		DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
		ball.Draw();
		player.Draw();
		cpu.Draw();
		DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
		DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

		EndDrawing();
	}

	CloseWindow();
}