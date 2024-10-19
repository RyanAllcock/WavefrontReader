#ifndef HEADER_OBJREADER
#define HEADER_OBJREADER

#include "filereader/filereader.hpp"

#include <map> // context optional data
#include <set> // context lookup data

// properties

enum MaterialColour{
	MaterialAmbient, MaterialDiffuse, MaterialSpecular, MaterialEmissive
};

enum MaterialTexture{
	TextureUntracked, 
	TextureAmbient, TextureDiffuse, TextureSpecular, TextureSpecularHighlight, 
	TextureBump, TextureDisplacement
};

enum MaterialExtraInt{
	MaterialIllumination
};
	
enum MaterialExtraFloat{
	MaterialOpaqueness, MaterialSpecularExponent, MaterialOpticalDensity, MaterialTransparency
};

// context

struct ObjectData{
	std::vector<float> vertex, uvcoord, normal;
	std::vector<std::string> libraries;
};

struct ObjectGroup{
	std::string name;
	std::vector<std::vector<int>> indices;
	std::map<int,std::string> materials;
};

struct Material{
	std::string name;
	std::map<int,std::vector<float>> colours;
	std::map<int,std::string> textures;
	std::map<int,int> iextra;
	std::map<int,float> fextra;
};

// builders

struct OBJBuilder{
	virtual void comment(std::string const &comment) = 0;
	virtual void object(std::string const &name) = 0;
	virtual void object() = 0;
	virtual void group(std::vector<std::string> const &names) = 0;
	virtual void group() = 0;
	virtual void vertex(std::vector<std::string> const &numbers) = 0;
	virtual void texture(std::vector<std::string> const &numbers) = 0;
	virtual void normal(std::vector<std::string> const &numbers) = 0;
	virtual void face(std::vector<std::vector<std::string>> const &indices) = 0;
	virtual void material(std::string const &name) = 0;
	virtual void material() = 0;
	virtual void library(std::string const &name) = 0;
	virtual void library() = 0;
};

struct OBJPrinter : OBJBuilder{
	void comment(std::string const &comment);
	void object(std::string const &name);
	void object();
	void group(std::vector<std::string> const &names);
	void group();
	void vertex(std::vector<std::string> const &numbers);
	void texture(std::vector<std::string> const &numbers);
	void normal(std::vector<std::string> const &numbers);
	void face(std::vector<std::vector<std::string>> const &indices);
	void material(std::string const &name);
	void material();
	void library(std::string const &name);
	void library();
};

struct OBJContext : OBJBuilder{
	ObjectData &data;
	std::vector<ObjectGroup> &groups;
	std::string currentMaterial;
	std::vector<ObjectGroup*> currentGroups;
	OBJContext(ObjectData &_data, std::vector<ObjectGroup> &_groups);
	void indexMaterial(std::string name);
	void finalise();
	void comment(std::string const &comment);
	void object(std::string const &name);
	void object();
	void group(std::vector<std::string> const &names);
	void group();
	void vertex(std::vector<std::string> const &numbers);
	void texture(std::vector<std::string> const &numbers);
	void normal(std::vector<std::string> const &numbers);
	void face(std::vector<std::vector<std::string>> const &indices);
	void material(std::string const &name);
	void material();
	void library(std::string const &name);
	void library();
};

struct MTLBuilder{
	virtual void comment(std::string const &comment) = 0;
	virtual void material(std::string const &name) = 0;
	virtual void material() = 0;
	virtual void colour(int type, std::vector<std::string> const &numbers) = 0;
	virtual void texture(int type, std::string const &file) = 0;
	virtual void texture(int type) = 0;
	virtual void iparam(int type, std::string const &value) = 0;
	virtual void fparam(int type, std::string const &value) = 0;
};

struct MTLPrinter : MTLBuilder{
	void comment(std::string const &comment);
	void material(std::string const &name);
	void material();
	void colour(int type, std::vector<std::string> const &numbers);
	void texture(int type, std::string const &file);
	void texture(int type);
	void iparam(int type, std::string const &value);
	void fparam(int type, std::string const &value);
};

struct MTLContext : MTLBuilder{
	std::vector<Material> &materials;
	std::set<std::string> &textures;
	MTLContext(std::vector<Material> &_materials, std::set<std::string> &_textures);
	void comment(std::string const &comment);
	void material(std::string const &name);
	void material();
	void colour(int type, std::vector<std::string> const &numbers);
	void texture(int type, std::string const &file);
	void texture(int type);
	void iparam(int type, std::string const &value);
	void fparam(int type, std::string const &value);
};

// readers

struct OBJReader{
	static void read(FileReader &reader, OBJBuilder &builder);
};

struct MTLReader{
	static void read(FileReader &reader, MTLBuilder &builder);
};

#endif