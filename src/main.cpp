#include "raylib.h"
#include "rlgl.h"
#include <iostream>

#include "isometric_renderer.hpp"
#include "vec3.h"

#define TITLE "Laky's Incredible Isometric Renderer"
#define VERSION_NUM "0.2.0"

void textured_triangle(Vertex vert1, Vertex vert2, Vertex vert3, Texture texture, iso::TexCoord uv1, iso::TexCoord uv2, iso::TexCoord uv3, float shade);

int main() 
{
	const int screenWidth = 800;
	const int screenHeight = 600;
	InitWindow(screenWidth, screenHeight, (string(TITLE) + " " + string(VERSION_NUM)).c_str());
	SetTargetFPS(144);

	iso::Camera cam(vec3(0, 0, 0), vec3(30, 45, 0), 2);

	iso::Model cubeModel("assets/models/teapot/teapot.obj");
	Texture grass = LoadTexture("assets/models/teapot/default.png");
	//iso::Model cubeModel("assets/models/grass/Grass_Block.obj");
	//Texture grass = LoadTexture("assets/models/grass/Grass_Block_TEX.png");

	if (grass.id == 0) {
		std::cout << "Failed to load texture!" << std::endl;
	}

	while (!WindowShouldClose())
	{
		SetWindowTitle((string(TITLE) + " " + string(VERSION_NUM) + " - " + to_string(GetFPS()) + " FPS").c_str());
		
		// Update
		float cameraSpeed = 100.0f * GetFrameTime(); // Adjust accordingly

		// Forward and backward
		if(IsKeyDown(KEY_W))
		{
			cerr << "Moving forward" << endl;
			cam.position.setX(cam.position.getX() - cameraSpeed);
		}
		if(IsKeyDown(KEY_S))
		{
			cerr << "Moving backward" << endl;
			cam.position.setX(cam.position.getX() + cameraSpeed);
		}

		// Left and right
		if(IsKeyDown(KEY_A))
		{
			cerr << "Moving left" << endl;
			cam.position.setZ(cam.position.getZ() - cameraSpeed);
		}
		if(IsKeyDown(KEY_D))
		{
			cerr << "Moving right" << endl;
			cam.position.setZ(cam.position.getZ() + cameraSpeed);
		}

		// Up and down
		if(IsKeyDown(KEY_Q))
		{
			cerr << "Moving up" << endl;
			cam.position.setY(cam.position.getY() + cameraSpeed);
		}
		if(IsKeyDown(KEY_E))
		{
			cerr << "Moving down" << endl;
			cam.position.setY(cam.position.getY() - cameraSpeed);
		}

		// Rotation
		if(IsKeyDown(KEY_UP))
		{
			cam.rotation.setY(cam.rotation.getY() - cameraSpeed);
		}
		if(IsKeyDown(KEY_DOWN))
		{
			cam.rotation.setY(cam.rotation.getY() + cameraSpeed);
		}
		if(IsKeyDown(KEY_LEFT))
		{
			cam.rotation.setX(cam.rotation.getX() - cameraSpeed);
		}
		if(IsKeyDown(KEY_RIGHT))
		{
			cam.rotation.setX(cam.rotation.getX() + cameraSpeed);
		}

		// Zoom
		cam.zoom = expf(logf(cam.zoom) + ((float)GetMouseWheelMove()*0.1f));

		if (cam.zoom > 100.0f) cam.zoom = 100.0f;
		else if (cam.zoom < 0.1f) cam.zoom = 0.1f;


		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("It works!", 20, 20, 20, BLACK);

		/*textured_triangle(
			Vertex(100, 100, 0), Vertex(200, 100, 0), Vertex(150, 200, 0),
			grass,
			iso::TexCoord(0.0f, 0.0f), iso::TexCoord(1.0f, 0.0f), iso::TexCoord(0.5f, 1.0f),
			1.0f
		);*/

		DrawTriangle(
			(Vector2){ 350, 200 },  // Bottom vertex first
			(Vector2){ 400, 100 },  // Top-right
			(Vector2){ 300, 100 },   // Top-left
			RED
		);
		
		cubeModel.render(cam, grass, &textured_triangle, screenWidth / 2.0f, screenHeight / 2.0f);

		EndDrawing();
	}
	CloseWindow();
	return 0;
}


void textured_triangle(Vertex vert1, Vertex vert2, Vertex vert3, Texture texture, iso::TexCoord uv1, iso::TexCoord uv2, iso::TexCoord uv3, float shade) 
{
    unsigned char shadeValue = (unsigned char)(255 * shade);

    rlDisableBackfaceCulling();
    
    rlBegin(RL_TRIANGLES);
        rlSetTexture(texture.id); 
        
        // Vertex 3
        rlColor4ub(shadeValue, shadeValue, shadeValue, 255);
        rlTexCoord2f(uv3.u, uv3.v);
        rlVertex2f(vert3.getX(), vert3.getY());

        // Vertex 2
        rlColor4ub(shadeValue, shadeValue, shadeValue, 255);
        rlTexCoord2f(uv2.u, uv2.v);
        rlVertex2f(vert2.getX(), vert2.getY());

        // Vertex 1
        rlColor4ub(shadeValue, shadeValue, shadeValue, 255);
        rlTexCoord2f(uv1.u, uv1.v);
        rlVertex2f(vert1.getX(), vert1.getY());
    rlEnd();
    
    rlSetTexture(0);
    
    rlEnableBackfaceCulling();
}