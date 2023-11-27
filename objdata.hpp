#ifndef OBJDATA
#define OBJDATA

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "libraries/stb_image.h"
#endif

#include <vector>
#include <tuple>

// primitive data
struct Image;

// data
struct ObjectData;
struct MaterialData;

// files
struct OBJData;
struct MTLData;

// handlers
struct DataHandler;

// classes

struct Image{
	unsigned char *data;
	int width, height;
	int channels;
	Image(){ data = NULL; }
	~Image(){ if(data) stbi_image_free(data); }
	void extract(const char *fileName){
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load(fileName, &width, &height, &channels, 0);
	}
};

struct ObjectData{
	int id;
	std::vector<int> tris;
	std::vector<int> quads;
	int smoothingId;
	std::string material;
	ObjectData(int i){
		id = i;
		smoothingId = 0;
		material = "";
	}
};

struct MaterialData{
	std::string name;
	std::tuple<float,float,float> ambientColour;
	std::tuple<float,float,float> diffuseColour;
	std::tuple<float,float,float> specularColour;
	std::tuple<float,float,float> emissiveColour;
	Image diffuseTexture;
	MaterialData(std::string m) : name(m) {
		ambientColour = std::tuple<float,float,float>(0, 0, 0);
		diffuseColour = std::tuple<float,float,float>(0, 0, 0);
		specularColour = std::tuple<float,float,float>(0, 0, 0);
		emissiveColour = std::tuple<float,float,float>(0, 0, 0);
	}
};

struct MTLData{
	std::vector<MaterialData> materials;
};

struct OBJData{
	std::vector<float> vertices;
	std::vector<float> textureCoords;
	std::vector<float> normals;
	std::vector<ObjectData> objects;
	MTLData materials;
};

struct DataHandler{
	static void display(OBJData &data);
	static void display(MTLData &data);
};

// functions

void DataHandler::display(OBJData &data){
	
	// vertices
	std::cout << "vertex positions:\n";
	for(int v = 0; v < data.vertices.size(); v += 4)
		std::cout << "[" << data.vertices[v] << "," << data.vertices[v+1] << "," << data.vertices[v+2] << "," << data.vertices[v+3] << "]\n";
	std::cout << "texture coordinates:\n";
	for(int t = 0; t < data.textureCoords.size(); t += 2)
		std::cout << "[" << data.textureCoords[t] << "," << data.textureCoords[t+1] << "]\n";
	std::cout << "normal vectors:\n";
	for(int n = 0; n < data.normals.size(); n += 3)
		std::cout << "[" << data.normals[n] << "," << data.normals[n+1] << "," << data.normals[n+2] << "]\n";
	
	// objects
	std::cout << "objects:\n";
	for(ObjectData o : data.objects){
		std::cout << "object with material " << o.material << " smoothing " << o.smoothingId << "{\n";
		for(int f = 0; f < o.tris.size(); f += 9)
			std::cout << "[" << o.tris[f] << "," << o.tris[f+1] << "," << o.tris[f+2] << "] " << 
				"[" << o.tris[f+3] << "," << o.tris[f+4] << "," << o.tris[f+5] << "] " << 
				"[" << o.tris[f+6] << "," << o.tris[f+7] << "," << o.tris[f+8] << "]\n";
		for(int f = 0; f < o.quads.size(); f += 3)
			std::cout << "[" << o.quads[f] << "," << o.quads[f+1] << "," << o.quads[f+2] << "] " << 
				"[" << o.quads[f+3] << "," << o.quads[f+4] << "," << o.quads[f+5] << "] " << 
				"[" << o.quads[f+6] << "," << o.quads[f+7] << "," << o.quads[f+8] << "] " << 
				"[" << o.quads[f+9] << "," << o.quads[f+10] << "," << o.quads[f+11] << "]\n";
		std::cout << "}\n";
	}
	
	// materials
	DataHandler::display(data.materials);
}

void DataHandler::display(MTLData &data){
	std::cout << "materials:\n";
	for(int m = 0; m < data.materials.size(); m++){
		std::cout << "material " << m << " " << data.materials[m].name << "{\n";
		std::cout << "ambient colour [" << std::get<0>(data.materials[m].ambientColour) << "," << std::get<1>(data.materials[m].ambientColour) << "," << std::get<2>(data.materials[m].ambientColour) << "]\n";
		std::cout << "diffuse colour [" << std::get<0>(data.materials[m].diffuseColour) << "," << std::get<1>(data.materials[m].diffuseColour) << "," << std::get<2>(data.materials[m].diffuseColour) << "]\n";
		std::cout << "specular colour [" << std::get<0>(data.materials[m].specularColour) << "," << std::get<1>(data.materials[m].specularColour) << "," << std::get<2>(data.materials[m].specularColour) << "]\n";
		std::cout << "emissive colour [" << std::get<0>(data.materials[m].emissiveColour) << "," << std::get<1>(data.materials[m].emissiveColour) << "," << std::get<2>(data.materials[m].emissiveColour) << "]\n";
		std::cout << "diffuse texture: " << (data.materials[m].diffuseTexture.data ? "yes" : "no") << "\n";
		std::cout << "}\n";
	}
}

#endif