#include "raylib.h"
#include "rlgl.h"
#include <iostream>

#include "isometric_renderer.hpp"
#include "vec3.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#undef RAYGUI_IMPLEMENTATION            // Avoid including raygui implementation again

#define GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "gui_window_file_dialog.h"

char modelFile[512] = "assets/models/teapot/teapot.obj";
string textureFile = "assets/models/teapot/default.png";


#define TITLE "Laky's Incredible Isometric Renderer"
#define VERSION_NUM "0.2.5"

void textured_triangle(Vertex vert1, Vertex vert2, Vertex vert3, Texture texture, iso::TexCoord uv1, iso::TexCoord uv2, iso::TexCoord uv3, float shade);
void draw_line(int x1, int y1, int x2, int y2, rgb color);



int main() 
{
	const int screenWidth = 800;
	const int screenHeight = 600;
	InitWindow(screenWidth, screenHeight, (string(TITLE) + " " + string(VERSION_NUM)).c_str());
	SetTargetFPS(144);

	// Model and texture loading
	GuiWindowFileDialogState fileDialogState = InitGuiWindowFileDialog(GetWorkingDirectory());

	iso::Camera cam(vec3(0, 0, 0), vec3(30, 45, 0), 2);

	iso::Model objModel(modelFile);
	Texture objTex = LoadTexture(textureFile.c_str());
	//iso::Model objModel("assets/models/objTex/Grass_Block.obj");
	//Texture objTex = LoadTexture("assets/models/objTex/Grass_Block_TEX.png");

	if (objTex.id == 0) {
		std::cout << "Failed to load texture!" << std::endl;
	}

	bool gridVisible = false;

	while (!WindowShouldClose())
	{
		SetWindowTitle((string(TITLE) + " " + string(VERSION_NUM) + " - " + to_string(GetFPS()) + " FPS").c_str());


        if (fileDialogState.SelectFilePressed)
        {
            // Load image file (if supported extension)
            if (IsFileExtension(fileDialogState.fileNameText, ".obj"))
            {
                strcpy(modelFile, TextFormat("%s" PATH_SEPERATOR "%s", fileDialogState.dirPathText, fileDialogState.fileNameText));
				objModel.loadModel(modelFile);
				textureFile = objModel.getTexturePath();
				UnloadTexture(objTex); // Unload previous texture
				objTex = LoadTexture(textureFile.c_str());

				if (objTex.id == 0) {
					std::cout << "Failed to load texture!" << std::endl;
				}
            }
			else
			{
				int result = GuiMessageBox((Rectangle){ 0, 0, 250, 100 }, "#191#Message Box", "The engine only support .OBJ models!", "OK");
			}

            fileDialogState.SelectFilePressed = false;
        }

		
		// Update
		float cameraSpeed = 100.0f * GetFrameTime();

		// Forward and backward
		if(!fileDialogState.windowActive) // Disable camera movement when file dialog is active
		{
			if(IsKeyDown(KEY_S))
			{
				cerr << "Moving forward" << endl;
				cam.position.setX(cam.position.getX() - cameraSpeed);
			}
			if(IsKeyDown(KEY_W))
			{
				cerr << "Moving backward" << endl;
				cam.position.setX(cam.position.getX() + cameraSpeed);
			}

			// Left and right
			if(IsKeyDown(KEY_D))
			{
				cerr << "Moving left" << endl;
				cam.position.setZ(cam.position.getZ() - cameraSpeed);
			}
			if(IsKeyDown(KEY_A))
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
				cam.rotation.setX(cam.rotation.getX() + cameraSpeed);
			}
			if(IsKeyDown(KEY_DOWN))
			{
				cam.rotation.setX(cam.rotation.getX() - cameraSpeed);
			}
			if(IsKeyDown(KEY_RIGHT))
			{
				cam.rotation.setY(cam.rotation.getY() - cameraSpeed);
			}
			if(IsKeyDown(KEY_LEFT))
			{
				cam.rotation.setY(cam.rotation.getY() + cameraSpeed);
			}
			if(IsKeyDown(KEY_DELETE))
			{
				cam.rotation.setZ(cam.rotation.getZ() + cameraSpeed);
			}
			if(IsKeyDown(KEY_PAGE_DOWN))
			{
				cam.rotation.setZ(cam.rotation.getZ() - cameraSpeed);
			}


			if(IsKeyPressed(KEY_SPACE))
			{
				gridVisible = !gridVisible;
			}

			// Zoom
			cam.zoom = expf(logf(cam.zoom) + ((float)GetMouseWheelMove()*0.1f));

			if (cam.zoom > 100.0f) cam.zoom = 100.0f;
			else if (cam.zoom < 0.1f) cam.zoom = 0.1f;

		}




		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText(("Loaded model: " + objModel.getName()).c_str(), 10, screenHeight - 50, 20, DARKGRAY);
		DrawText("SPACE to toggle grid", 10, screenHeight - 30, 20, DARKGRAY);

		/*textured_triangle(
			Vertex(100, 100, 0), Vertex(200, 100, 0), Vertex(150, 200, 0),
			objTex,
			iso::TexCoord(0.0f, 0.0f), iso::TexCoord(1.0f, 0.0f), iso::TexCoord(0.5f, 1.0f),
			1.0f
		);*/
		
		if(gridVisible)
		{
			objModel.draw_grid(cam, screenWidth / 2.0f, screenHeight / 2.0f, rgb(45, 45, 45), draw_line, 500);
		}
		
		objModel.render(cam, objTex, &textured_triangle, screenWidth / 2.0f, screenHeight / 2.0f);

		// File loading GUI
		if (fileDialogState.windowActive) GuiLock();

		if (GuiButton((Rectangle){ 20, 20, 140, 30 }, GuiIconText(ICON_FILE_OPEN, "Open Model"))) fileDialogState.windowActive = true;

		GuiUnlock();

		// GUI: Dialog Window
		//--------------------------------------------------------------------------------
		GuiWindowFileDialog(&fileDialogState);
		//--------------------------------------------------------------------------------

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

void draw_line(int x1, int y1, int x2, int y2, rgb color)
{
	rlBegin(RL_LINES);
		rlColor4ub(color.getR(), color.getG(), color.getB(), color.getA());
		rlVertex2f((float)x1, (float)y1);
		rlVertex2f((float)x2, (float)y2);
	rlEnd();
}