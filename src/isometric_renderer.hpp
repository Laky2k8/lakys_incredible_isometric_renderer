#pragma once

#include <iostream>
#include <algorithm>
#include <tuple>
#include <random>
#include <cmath>
#include <array>
#include <cfloat>

#include "vec3.h"
#include "vertex.h"
#include "rgb.h"

#include "renderer_config.h"

#include "obj_loader.h"



using namespace std;

namespace iso
{


	class Camera
	{
	public:
		vec3 position;
		vec3 rotation;
		float zoom;

		Camera(vec3 pos, vec3 rot, float zoom)
		{
			this->position = pos;
			this->rotation = rot;
			this->zoom = zoom;
		}
	};

	struct TexCoord {
		float u, v;
		TexCoord(float u = 0.0f, float v = 0.0f) : u(u), v(v) {}
	};

	struct Face
	{
		array<int, 3> indices;
		array<TexCoord, 3> uvs;
		float avgZ;

		Face(array<int, 3> idx, array<TexCoord, 3> uv_coords, float z) : indices(idx), uvs(uv_coords), avgZ(z) {}
	};


	float lerp(float a, float b, float f) 
	{
		return (a * (1.0 - f)) + (b * f);
	}

	class Model
	{
	private:
		vector<Vertex> vertices;
		vector<Face> faces;
		string objPath;
		string texturePath;

		float calculateShading(float depth)
		{
			float shade = 200.0f - (depth * 2.0f);
			shade = max(50.0f, min(255.0f, shade)); // Clamp to [50, 255]

			// Normalize to [0,1]
			return shade / 255.0f;
		}

	public:

		/*Model(string objPath) : objPath(objPath)
		{

			objl::Loader objLoader;

			bool loadout = objLoader.LoadFile(objPath);

			if(!loadout)
			{
				cerr << "Failed to load OBJ file from " << objPath << endl;
				return;
			}

			objl::Mesh mesh = objLoader.LoadedMeshes[0];

			cout << "Loaded OBJ file from" << objPath << endl;
			cout << "	- Object name: " << mesh.MeshName << endl;
			cout << endl;

			cout << "	- Vertices count: " << mesh.Vertices.size() << endl;
			cout << "	- Vertices: " << endl;
			for (int i = 0; i < mesh.Vertices.size(); ++i)
			{
				cout << "		" << i << ". X:" << mesh.Vertices[i].Position.X << ", Y: " << mesh.Vertices[i].Position.Y << ", Z: " << mesh.Vertices[i].Position.Z << endl;
			}
			cout << endl;

			cout << "	- Texture coordinates: " << endl;
			for (int i = 0; i < mesh.Vertices.size(); ++i)
			{
				cout << "		" << i << ". X:" << mesh.Vertices[i].TextureCoordinate.X << ", Y: " << mesh.Vertices[i].TextureCoordinate.Y  << endl;
			}
			cout << endl;


			// Assign vertices
			
			float minX = FLT_MAX, maxX = -FLT_MAX;
			float minY = FLT_MAX, maxY = -FLT_MAX;
			float minZ = FLT_MAX, maxZ = -FLT_MAX;

			// Find min and max
			for (auto& v : mesh.Vertices)
			{
				minX = std::min(minX, v.Position.X);
				maxX = std::max(maxX, v.Position.X);
				minY = std::min(minY, v.Position.Y);
				maxY = std::max(maxY, v.Position.Y);
				minZ = std::min(minZ, v.Position.Z);
				maxZ = std::max(maxZ, v.Position.Z);
			}

			// Normalize and scale to [-50, 50]
			for (auto& v : mesh.Vertices)
			{
				float newX = ((v.Position.X - minX) / (maxX - minX)) * 100.0f - 50.0f;
				float newY = ((v.Position.Y - minY) / (maxY - minY)) * 100.0f - 50.0f;
				float newZ = ((v.Position.Z - minZ) / (maxZ - minZ)) * 100.0f - 50.0f;

				vertices.emplace_back(Vertex(newX, newY, newZ));
			}

			for (int i = 0; i < mesh.Vertices.size(); ++i)
			{
				vertices.emplace_back(Vertex(
					mesh.Vertices[i].Position.X * 2,
					mesh.Vertices[i].Position.Y * 2,
					mesh.Vertices[i].Position.Z * 2
				));
			}

			// Save texture coordinates
			vector<TexCoord> texCoords;

			for (int i = 0; i < mesh.Vertices.size(); i++)
			{
				texCoords.emplace_back(TexCoord(
					mesh.Vertices[i].TextureCoordinate.X,
					mesh.Vertices[i].TextureCoordinate.Y
				));
			}

			// Assign faces (triangles)
			for (int i = 0; i < mesh.Indices.size(); i += 3)
			{
				int idx1 = mesh.Indices[i];
				int idx2 = mesh.Indices[i + 1];
				int idx3 = mesh.Indices[i + 2];

				faces.emplace_back(Face(
					{ idx1, idx2, idx3 },
					{ 
						TexCoord(mesh.Vertices[idx1].TextureCoordinate.X, mesh.Vertices[idx1].TextureCoordinate.Y),
						TexCoord(mesh.Vertices[idx2].TextureCoordinate.X, mesh.Vertices[idx2].TextureCoordinate.Y),
						TexCoord(mesh.Vertices[idx3].TextureCoordinate.X, mesh.Vertices[idx3].TextureCoordinate.Y)
					},
					0.0f
				));
			}


			vertices = {
				Vertex(-50.0f,-50.0f,-50.0f),
				Vertex( 50.0f,-50.0f,-50.0f),
				Vertex( 50.0f, 50.0f,-50.0f),
				Vertex(-50.0f, 50.0f,-50.0f),
				Vertex(-50.0f,-50.0f, 50.0f),
				Vertex( 50.0f,-50.0f, 50.0f),
				Vertex( 50.0f, 50.0f, 50.0f),
				Vertex(-50.0f, 50.0f, 50.0f)
			};

			faces = {
				Face({0,1,2}, {TexCoord(0.0f, 0.0f), TexCoord(1.0f, 0.0f), TexCoord(1.0f, 1.0f)}, 0), Face({0,2,3}, {TexCoord(0.0f, 0.0f), TexCoord(1.0f, 1.0f), TexCoord(0.0f, 1.0f)}, 0),
				Face({4,6,5}, {TexCoord(0.0f, 0.0f), TexCoord(1.0f, 1.0f), TexCoord(1.0f, 0.0f)}, 0), Face({4,7,6}, {TexCoord(0.0f, 0.0f), TexCoord(0.0f, 1.0f), TexCoord(1.0f, 1.0f)}, 0),
				Face({3,2,6}, {TexCoord(0.0f, 1.0f), TexCoord(1.0f, 1.0f), TexCoord(1.0f, 0.0f)}, 0), Face({3,6,7}, {TexCoord(0.0f, 1.0f), TexCoord(1.0f, 0.0f), TexCoord(0.0f, 0.0f)}, 0),
				Face({0,5,1}, {TexCoord(0.0f, 1.0f), TexCoord(1.0f, 0.0f), TexCoord(1.0f, 1.0f)}, 0), Face({0,4,5}, {TexCoord(0.0f, 1.0f), TexCoord(0.0f, 0.0f), TexCoord(1.0f, 0.0f)}, 0),
				Face({1,5,6}, {TexCoord(0.0f, 0.0f), TexCoord(1.0f, 0.0f), TexCoord(1.0f, 1.0f)}, 0), Face({1,6,2}, {TexCoord(0.0f, 0.0f), TexCoord(1.0f, 1.0f), TexCoord(0.0f, 1.0f)}, 0),
				Face({0,3,7}, {TexCoord(0.0f, 0.0f), TexCoord(0.0f, 1.0f), TexCoord(1.0f, 1.0f)}, 0), Face({0,7,4}, {TexCoord(0.0f, 0.0f), TexCoord(1.0f, 1.0f), TexCoord(1.0f, 0.0f)}, 0)
			};

		}*/

		Model(string objPath) : objPath(objPath)
		{
			loadModel(objPath);
		}


		void loadModel(string objPath)
		{
			// Clear previous data
			vertices.clear();
			faces.clear();

			this->objPath = objPath;

			// Load new model
			objl::Loader objLoader;

			bool loadout = objLoader.LoadFile(objPath);

			if(!loadout)
			{
				cerr << "Failed to load OBJ file from " << objPath << endl;
				return;
			}

			objl::Mesh mesh = objLoader.LoadedMeshes[0];

			cout << "Loaded OBJ file from" << objPath << endl;
			cout << "	- Object name: " << mesh.MeshName << endl;
			cout << endl;

			// Assign vertices
			
			float minX = FLT_MAX, maxX = -FLT_MAX;
			float minY = FLT_MAX, maxY = -FLT_MAX;
			float minZ = FLT_MAX, maxZ = -FLT_MAX;

			// Find min and max
			for (auto& v : mesh.Vertices)
			{
				minX = std::min(minX, v.Position.X);
				maxX = std::max(maxX, v.Position.X);
				minY = std::min(minY, v.Position.Y);
				maxY = std::max(maxY, v.Position.Y);
				minZ = std::min(minZ, v.Position.Z);
				maxZ = std::max(maxZ, v.Position.Z);
			}

			// Calculate dimensions
			float width = maxX - minX;
			float height = maxY - minY;
			float depth = maxZ - minZ;

			// Find the largest dimension
			float maxDimension = std::max({width, height, depth});

			// Define target size (adjust this value to your needs)
			float targetSize = 200.0f;

			// Calculate scale factor
			float scale = targetSize / maxDimension;

			// Calculate center of the bounding box
			float centerX = (minX + maxX) / 2.0f;
			float centerY = (minY + maxY) / 2.0f - 30.0f;
			float centerZ = (minZ + maxZ) / 2.0f;

			// Scale and center the model
			for (int i = 0; i < mesh.Vertices.size(); ++i)
			{
				vertices.emplace_back(Vertex(
					(mesh.Vertices[i].Position.X - centerX) * scale,
					(mesh.Vertices[i].Position.Y - centerY) * scale,
					(mesh.Vertices[i].Position.Z - centerZ) * scale
				));
			}

			// Save texture coordinates
			vector<TexCoord> texCoords;

			// Assign faces (triangles)
			for (int i = 0; i < mesh.Indices.size(); i += 3)
			{
				int idx1 = mesh.Indices[i];
				int idx2 = mesh.Indices[i + 1];
				int idx3 = mesh.Indices[i + 2];

				faces.emplace_back(Face(
					{ idx1, idx2, idx3 },
					{ 
						TexCoord(mesh.Vertices[idx1].TextureCoordinate.X, mesh.Vertices[idx1].TextureCoordinate.Y),
						TexCoord(mesh.Vertices[idx2].TextureCoordinate.X, mesh.Vertices[idx2].TextureCoordinate.Y),
						TexCoord(mesh.Vertices[idx3].TextureCoordinate.X, mesh.Vertices[idx3].TextureCoordinate.Y)
					},
					0.0f
				));
			}

			cout << "Material: " << mesh.MeshMaterial.name << "\n";
			cout << "Ambient Color: " << mesh.MeshMaterial.Ka.X << ", " << mesh.MeshMaterial.Ka.Y << ", " << mesh.MeshMaterial.Ka.Z << "\n";
			cout << "Diffuse Color: " << mesh.MeshMaterial.Kd.X << ", " << mesh.MeshMaterial.Kd.Y << ", " << mesh.MeshMaterial.Kd.Z << "\n";
			cout << "Specular Color: " << mesh.MeshMaterial.Ks.X << ", " << mesh.MeshMaterial.Ks.Y << ", " << mesh.MeshMaterial.Ks.Z << "\n";
			cout << "Specular Exponent: " << mesh.MeshMaterial.Ns << "\n";
			cout << "Optical Density: " << mesh.MeshMaterial.Ni << "\n";
			cout << "Dissolve: " << mesh.MeshMaterial.d << "\n";
			cout << "Illumination: " << mesh.MeshMaterial.illum << "\n";
			cout << "Ambient Texture Map: " << mesh.MeshMaterial.map_Ka << "\n";
			cout << "Diffuse Texture Map: " << mesh.MeshMaterial.map_Kd << "\n";
			cout << "Specular Texture Map: " << mesh.MeshMaterial.map_Ks << "\n";
			cout << "Alpha Texture Map: " << mesh.MeshMaterial.map_d << "\n";
			cout << "Bump Map: " << mesh.MeshMaterial.map_bump << "\n";
			if(!mesh.MeshMaterial.map_Kd.empty())
			{
				// Texture path is the same as the OBJ file but with the texture filename
				size_t lastSlash = objPath.find_last_of("/\\");
				texturePath = objPath.substr(0, lastSlash + 1) + mesh.MeshMaterial.map_Kd;
				cout << "	- Texture path: " << texturePath << endl;
			}
			else
			{
				texturePath = "/assets/textures/grass.png"; // Default texture
			}
			
		}

		string getName()
		{
			// return model name
			size_t lastSlash = objPath.find_last_of("/\\");
			size_t lastDot = objPath.find_last_of(".");
			if (lastDot == string::npos || lastDot < lastSlash) lastDot = objPath.length();
			return objPath.substr(lastSlash + 1, lastDot - lastSlash - 1);
		}

		string getTexturePath()
		{
			return texturePath;
		}

		void render(Camera cam, Texture modelTex, void(*draw_textured_triangle)(Vertex, Vertex, Vertex, Texture, TexCoord, TexCoord, TexCoord, float), float centerX, float centerY)
		{
				vector<Face> sorted_faces;

			// Calculate averager depth
			for(int face_idx = 0; face_idx < faces.size(); face_idx++)
			{

				const auto& face = faces[face_idx].indices;

				// Transform to world then camera space
				vec3 vert1_worldSpace = vec3(vertices[face[0]].getX(), vertices[face[0]].getY(), vertices[face[0]].getZ());
				vec3 vert2_worldSpace = vec3(vertices[face[1]].getX(), vertices[face[1]].getY(), vertices[face[1]].getZ());
				vec3 vert3_worldSpace = vec3(vertices[face[2]].getX(), vertices[face[2]].getY(), vertices[face[2]].getZ());

				vec3 vert1_camSpace = cameraSpace(vert1_worldSpace, cam.position, cam.rotation);
				vec3 vert2_camSpace = cameraSpace(vert2_worldSpace, cam.position, cam.rotation);
				vec3 vert3_camSpace = cameraSpace(vert3_worldSpace, cam.position, cam.rotation);

				float avgZ = (vert1_camSpace.getZ() + vert2_camSpace.getZ() + vert3_camSpace.getZ()) / 3.0f;

				array<TexCoord, 3> face_uvs = faces[face_idx].uvs;
				
				
				// Add to list of sorted faces
				sorted_faces.emplace_back(face, face_uvs, avgZ);
			}

			// Sort faces by depth
			sort(sorted_faces.begin(), sorted_faces.end(), 
			[](const Face& a, const Face& b)
			{
				return a.avgZ > b.avgZ; 
			});


			// And now we can finally render the faces without any weirdness yippee :D
			for(int i = 0; i < sorted_faces.size(); i++)
			{
				Face face = sorted_faces[i];
				const auto& indices = face.indices;
				const auto& uvs = face.uvs;

				// Project the vertices to screen space
				Vertex vert1_projected = vertices[indices[0]].getProjection(cam.position, cam.rotation, centerX, centerY, cam.zoom);
				Vertex vert2_projected = vertices[indices[1]].getProjection(cam.position, cam.rotation, centerX, centerY, cam.zoom);
				Vertex vert3_projected = vertices[indices[2]].getProjection(cam.position, cam.rotation, centerX, centerY, cam.zoom);

				// Check if back-facing (we can skip those USELESS, PATHETIC triangles smh my head)
				/*if(backFacing(vert1_projected, vert2_projected, vert3_projected))
				{
					continue;
				}*/

				float shade = calculateShading(face.avgZ);

				// And finally, draw the triangle ^^
				draw_textured_triangle(vert1_projected, vert2_projected, vert3_projected, modelTex, uvs[0], uvs[1], uvs[2], shade);

			}
		}

		void draw_grid(Camera cam, float centerX, float centerY, rgb color, void(*draw_line)(int, int, int, int, rgb), int gridSize = 500)
		{
			int spacing = 10; // Spacing between grid lines

			// Draw lines parallel to X-axis (going left-right in world space)
			for (int z = -gridSize; z <= gridSize; z += spacing) 
			{
				vec3 startWorld(-gridSize, 0, z);
				vec3 endWorld(gridSize, 0, z);

				// Create vertices directly from world coordinates
				Vertex startVertex(startWorld.getX(), startWorld.getY(), startWorld.getZ() -50.0f);
				Vertex endVertex(endWorld.getX(), endWorld.getY(), endWorld.getZ() -50.0f);

				Vertex startScreen = startVertex.getProjection(cam.position, cam.rotation, centerX, centerY, cam.zoom);
				Vertex endScreen = endVertex.getProjection(cam.position, cam.rotation, centerX, centerY, cam.zoom);

				draw_line((int)startScreen.getX(), (int)startScreen.getY(), (int)endScreen.getX(), (int)endScreen.getY(), color);
			}

			// Draw lines parallel to Z-axis (going forward-backward in world space)
			for (int x = -gridSize; x <= gridSize; x += spacing) 
			{
				vec3 startWorld(x, 0, -gridSize);
				vec3 endWorld(x, 0, gridSize);

				// Create vertices directly from world coordinates
				Vertex startVertex(startWorld.getX(), startWorld.getY(), startWorld.getZ());
				Vertex endVertex(endWorld.getX(), endWorld.getY(), endWorld.getZ());

				Vertex startScreen = startVertex.getProjection(cam.position, cam.rotation, centerX, centerY, cam.zoom);
				Vertex endScreen = endVertex.getProjection(cam.position, cam.rotation, centerX, centerY, cam.zoom);

				draw_line((int)startScreen.getX(), (int)startScreen.getY(), (int)endScreen.getX(), (int)endScreen.getY(), color);
			}
		}

	};
}

