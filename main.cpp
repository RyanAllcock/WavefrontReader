#include "source/wavefrontreader.hpp"

#include <stdio.h> // print data

void printObjectData(ObjectData const &objectData){
	printf("data:\n");
	printf("\tvertices:\n");
	for(int i = 0; i < objectData.vertex.size(); i += 3)
		printf("\t\t%f %f %f\n", objectData.vertex[i], objectData.vertex[i + 1], objectData.vertex[i + 2]);
	printf("\tuv coordinates:\n");
	for(int i = 0; i < objectData.uvcoord.size(); i += 2)
		printf("\t\t%f %f\n", objectData.uvcoord[i], objectData.uvcoord[i + 1], objectData.uvcoord[i + 2]);
	printf("\tnormals:\n");
	for(int i = 0; i < objectData.normal.size(); i += 3)
		printf("\t\t%f %f %f\n", objectData.normal[i], objectData.normal[i + 1], objectData.normal[i + 2]);
	printf("\tlibraries:\n");
	for(std::string const &library : objectData.libraries) printf("\t\t%s\n", library.c_str());
}

void printObjectGroups(std::vector<ObjectGroup> const &objectGroups, bool combineMaterialIndices){
	printf("groups:\n");
	for(ObjectGroup const &group : objectGroups){
		printf("\t%s:\n", group.name.c_str());
		if(combineMaterialIndices){
			printf("\t\tmaterial indices:\n");
			std::vector<std::vector<int>>::const_iterator face = group.indices.begin();
			for(std::pair<int,std::string> const &material : group.materials){
				while(face - group.indices.begin() < material.first){
					printf("\t\t\t%s:", material.second.c_str());
					for(int i = 0; i < face->size(); i += 3) printf(" %i|%i|%i", (*face)[i + 0], (*face)[i + 1], (*face)[i + 2]);
					printf("\n");
					face++;
				}
			}
		}
		else{
			printf("\t\tindices:\n");
			for(std::vector<int> const &indices : group.indices){
				printf("\t\t\t");
				for(int i = 0; i < indices.size(); i += 3) printf("%i|%i|%i ", indices[i + 0], indices[i + 1], indices[i + 2]);
				printf("\n");
			}
			printf("\t\tmaterials:\n");
			for(std::pair<int,std::string> const &material : group.materials)
				printf("\t\t\t%s until index %i\n", material.second.c_str(), material.first);
		}
	}
}

void printMaterialProperties(std::vector<Material> const &materialProperties){
	printf("materials:\n");
	for(Material const &material : materialProperties){
		printf("\t%s:\n", material.name.c_str());
		for(std::pair<int,std::vector<float>> const &colour : material.colours){
			const std::vector<std::string> names{"ambient", "diffuse", "specular", "emissive"};
			std::string const &name = names[colour.first];
			printf("\t\tcolour|%s:", name.c_str());
			for(float const value : colour.second) printf(" %f", value);
			printf("\n");
		}
		for(std::pair<int,std::string> const &texture : material.textures){
			const std::vector<std::string> names{
				"untracked", 
				"ambient", "diffuse", "specular", "specular-highlight", 
				"bump", "displacement"
			};
			std::string const &name = names[texture.first];
			printf("\t\ttexture|%s: %s\n", name.c_str(), texture.second.c_str());
		}
		for(std::pair<int,int> const &iextra : material.iextra){
			const std::vector<std::string> names{"illumination-model"};
			std::string const &name = names[iextra.first];
			printf("\t\tiparam|%s: %i\n", name.c_str(), iextra.second);
		}
		for(std::pair<int,int> const &fextra : material.fextra){
			const std::vector<std::string> names{"opaqueness", "specular-exponent", "optical-density", "transparency"};
			std::string const &name = names[fextra.first];
			printf("\t\tfparam|%s: %i\n", name.c_str(), fextra.second);
		}
	}
}

void printMaterialTextures(std::set<std::string> &materialTextures){
	printf("textures:\n");
	for(std::string const &texture : materialTextures) printf("\t%s\n", texture.c_str());
}

int main(int argc, char *argv[]){
	
	// default
	const char *objSourceDefault = "box.obj";
	const char *mtlSourceDefault = "box.mtl";
	
	// input
	std::string objSource = objSourceDefault;
	std::string mtlSource = mtlSourceDefault;
	if(argc > 1) objSource = argv[1];
	if(argc > 2) mtlSource = argv[2];
	
	// reader
	FileReader objReader(objSource.c_str());
	FileReader mtlReader(mtlSource.c_str());
	
	// data
	ObjectData objectData;
	std::vector<ObjectGroup> objectGroups;
	std::vector<Material> materialProperties;
	std::set<std::string> materialTextures;
	
	// builder
	OBJPrinter objPrinter;
	OBJContext objContext(objectData, objectGroups);
	MTLPrinter mtlPrinter;
	MTLContext mtlContext(materialProperties, materialTextures);
	
	// read
	// objReader.reset();
	// OBJReader::read(objReader, objPrinter);
	// mtlReader.reset();
	// MTLReader::read(mtlReader, mtlPrinter);
	
	// extraction
	objReader.reset();
	OBJReader::read(objReader, objContext);
	objContext.finalise();
	mtlReader.reset();
	MTLReader::read(mtlReader, mtlContext);
	
	// print
	printObjectData(objectData);
	printObjectGroups(objectGroups, true);
	printMaterialProperties(materialProperties);
	printMaterialTextures(materialTextures);
	
	return 0;
}