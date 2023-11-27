#ifndef OBJREADER
#define OBJREADER

#include "objdata.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

class FileReader{
	
	// data handling
	static bool parse(std::ifstream &file, OBJData &data);
	static bool parse(std::ifstream &file, MTLData &data);
	
	// internal operations
protected:
	static bool checkTriple(bool a, bool b, bool c){
		if(!a || !b || !c){
			std::cout << "File read error: float triple defined incorrectly\n";
			return false;
		}
		return true;
	}
	static int readInt(std::string str){
		int num = 0;
		bool isNum = false;
		for(int c = 0; str[c] != '\0'; c++){
			if(str[c] >= '0' && str[c] <= '9') num = str[c] - '0' + num * 10;
			else break;
		}
		return num;
	}
	
	// main functions
public:
	template <typename D>
	static bool read(const char *fileName, D &data);
};

template <typename D>
bool FileReader::read(const char *fileName, D &data){
	
	// access
	std::ifstream file(fileName);
	if(!file){
		std::cout << "File read error: " << fileName << " file not opened\n";
		return false;
	}
	
	// reading
	parse(file, data);
	
	file.close();
	return true;
}

bool FileReader::parse(std::ifstream &file, OBJData &data){ // eventually a function to parse individual commands ... ?
	data.objects.push_back(ObjectData(data.objects.size()));
	bool objectDeclared = false;
	std::string line;
	while(std::getline(file, line)){
		std::stringstream stream(line);
		std::string keyword;
		stream >> keyword;
		
		// vertices
		if(keyword == "v"){
			float x, y, z, w;
			checkTriple(bool(stream >> x), bool(stream >> y), bool(stream >> z));
			if(!(stream >> w)) w = 1.0;
			data.vertices.push_back(x);
			data.vertices.push_back(y);
			data.vertices.push_back(z);
			data.vertices.push_back(w);
		}
		else if(keyword == "vt"){
			float u, v;
			if(!(stream >> u)) std::cout << "File read error: texture coordinate defined incorrectly\n";
			if(stream.peek() == EOF) v = 0; else stream >> v;
			data.textureCoords.push_back(u);
			data.textureCoords.push_back(v);
		}
		else if(keyword == "vn"){
			float x, y, z;
			checkTriple(bool(stream >> x), bool(stream >> y), bool(stream >> z));
			data.normals.push_back(x);
			data.normals.push_back(y);
			data.normals.push_back(z);
		}
		
		// elements
		else if(keyword == "f"){
			std::string face;
			int tUsed = -1, nUsed = -1;
			std::vector<int> indices;
			int vertices;
			for(vertices = 0; stream >> face; vertices++){
				int v, t, n, length;
				bool isIndex = false;
				length = face.find("/");
				v = readInt(face.substr(0, length));
				face.erase(0, length + 1);
				length = face.find("/");
				t = readInt(face.substr(0, length));
				face.erase(0, length + 1);
				n = readInt(face);
				if(v == 0 || tUsed != -1 && bool(t) != bool(tUsed) || nUsed != -1 && bool(n) != bool(nUsed)) std::cout << "File read error: face term defined incorrectly\n";
				tUsed = t;
				nUsed = n;
				indices.push_back(v);
				indices.push_back(t);
				indices.push_back(n);
			}
			switch(vertices){
				case 3:
					data.objects.back().tris.insert(data.objects.back().tris.end(), indices.begin(), indices.end());
					break;
				case 4:
					data.objects.back().quads.insert(data.objects.back().tris.end(), indices.begin(), indices.end());
					break;
			}
		}
		
		// grouping
		else if(keyword == "s"){
			std::string s;
			stream >> s;
			if(!objectDeclared) objectDeclared = true;
			else{
				data.objects.push_back(ObjectData(data.objects.size()));
				data.objects.back() = data.objects.back();
			}
			data.objects.back().smoothingId = s == "off" ? 0 : readInt(s);
		}
		else if(keyword == "usemtl"){
			std::string m;
			stream >> m;
			data.objects.back().material = m;
		}
		
		// files
		else if(keyword == "mtllib"){
			std::string l;
			stream >> l;
			FileReader::read(l.c_str(), data.materials);
		}
	}
	return true;
}

bool FileReader::parse(std::ifstream &file, MTLData &data){
	data.materials.push_back(MaterialData(""));
	bool materialDeclared = false;
	
	// reading
	std::string line;
	while(std::getline(file, line)){
		std::stringstream stream(line);
		std::string keyword;
		stream >> keyword;
		
		// colour
		if(keyword[0] == 'K'){
			float r, g, b;
			checkTriple(bool(stream >> r), bool(stream >> g), bool(stream >> b));
			switch(keyword[1]){
				case 'a':
					data.materials.back().ambientColour = std::tuple<float,float,float>(r, g, b);
					break;
				case 'd':
					data.materials.back().diffuseColour = std::tuple<float,float,float>(r, g, b);
					break;
				case 's':
					data.materials.back().specularColour = std::tuple<float,float,float>(r, g, b);
					break;
				case 'e':
					data.materials.back().emissiveColour = std::tuple<float,float,float>(r, g, b);
					break;
			}
		}
		
		// textures
		else if(keyword.substr(0, 4) == "map_"){
			std::string src, type;
			stream >> src;
			type = keyword.substr(4);
			if(type == "Kd") data.materials.back().diffuseTexture.extract(src.c_str());
		}
		
		// grouping
		else if(keyword == "newmtl"){
			std::string m;
			stream >> m;
			if(!materialDeclared){
				materialDeclared = true;
				data.materials.back().name = m;
			}
			else data.materials.push_back(MaterialData(m));
		}
	}
	return true;
}

#endif