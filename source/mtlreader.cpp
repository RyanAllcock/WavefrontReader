#include "wavefrontreader.hpp"

#include <stdio.h> // print read content

// read method

void MTLReader::read(FileReader &reader, MTLBuilder &builder){
	
	// lines
	while(!reader.isFinished()){
		int offset = 0;
		int index;
		
		// whitespace
		reader.peek(offset, ' ');
		
		// material
		if(reader.check(offset, "newmtl ")){
			offset += 6;
			if(reader.race(offset, std::vector<char>{' '}, std::vector<char>{'\n'})){
				std::string name = reader.seek(offset, std::vector<char>{' ', '\n'});
				builder.material(name);
			}
			else builder.material();
		}
		
		// colours
		else if((index = 0, 
				index += 1 * reader.check(offset, "Ka "), 
				index += 2 * reader.check(offset, "Kd "), 
				index += 3 * reader.check(offset, "Ks "), 
				index += 4 * reader.check(offset, "Ke "), index)){
			const std::vector<int> paramOffsets{2, 2, 2, 2};
			const std::vector<int> paramTypes{MaterialAmbient, MaterialDiffuse, MaterialSpecular, MaterialEmissive};
			offset += paramOffsets[index - 1];
			int type = paramTypes[index - 1];
			reader.peek(offset, ' ');
			std::vector<std::string> numbers = reader.peekFloats(offset, std::vector<char>{' '}, std::vector<char>{'\n'});
			builder.colour(type, numbers);
		}
		
		// textures
		else if((index = 0, 
				index += 1 * reader.check(offset, "map_Ka "), 
				index += 2 * reader.check(offset, "map_Kd "), 
				index += 3 * reader.check(offset, "map_Ks "), 
				index += 4 * reader.check(offset, "map_Ns "), 
				index += 5 * reader.check(offset, "map_d "), 
				index += 6 * reader.check(offset, "map_bump "), 
				index += 7 * reader.check(offset, "bump "), 
				index += 8 * reader.check(offset, "map_disp "), 
				index += 9 * reader.check(offset, "disp "), 
				index += 10 * reader.check(offset, "decal "), 
				index += 11 * reader.check(offset, "refl "), index)){
			const std::vector<int> paramOffsets{6, 6, 6, 6, 5, 8, 4, 8, 4, 5, 4};
			const std::vector<int> paramTypes{
				TextureAmbient, TextureDiffuse, TextureSpecular, TextureSpecularHighlight, 
				TextureUntracked, TextureBump, TextureBump, TextureDisplacement, TextureDisplacement, 
				TextureUntracked, TextureUntracked
			};
			int type = paramTypes[index - 1];
			offset += paramOffsets[index - 1];
			reader.peek(offset, ' ');
			std::string file = reader.seek(offset, std::vector<char>{' ', '\n'});
			if(file.size() > 0) builder.texture(type, file);
			else builder.texture(type);
		}
		
		// whole parameters
		else if((index = 0, index += reader.check(offset, "illum "), index)){
			const std::vector<int> paramOffsets{5};
			const std::vector<int> paramTypes{MaterialIllumination};
			offset += paramOffsets[index - 1];
			int type = paramTypes[index - 1];
			reader.peek(offset, ' ');
			std::string sign = reader.peek(offset, '-');
			std::string integer = reader.peekDigits(offset);
			std::string value = sign + integer;
			builder.iparam(type, value);
		}
		
		// fractional parameters
		else if((index = 0, 
				index += reader.check(offset, "d "), 
				index += 2 * reader.check(offset, "Ns "), 
				index += 3 * reader.check(offset, "Ni "), 
				index += 4 * reader.check(offset, "Tr "), index)){
			const std::vector<int> paramOffsets{1, 2, 2, 2};
			const std::vector<int> paramTypes{MaterialOpaqueness, MaterialSpecularExponent, MaterialOpticalDensity, MaterialTransparency};
			offset += paramOffsets[index - 1];
			int type = paramTypes[index - 1];
			reader.peek(offset, ' ');
			std::string value = reader.peekFloat(offset);
			builder.fparam(type, value);
		}
		
		// next line
		reader.seek(offset, std::vector<char>{'\n'});
		offset++;
		reader.advance(offset);
	}
}

// printing methods

void MTLPrinter::comment(std::string const &comment){
	printf("comment: %sh\n", comment.c_str());
}

void MTLPrinter::material(std::string const &name){
	printf("material: %s\n", name.c_str());
}

void MTLPrinter::material(){
	printf("material (empty name)\n");
}

void MTLPrinter::colour(int type, std::vector<std::string> const &numbers){
	const std::vector<std::string> names{"ambient", "diffuse", "specular", "emissive"};
	std::string const &name = names[type];
	printf("colour|%s:", name.c_str());
	for(std::string number : numbers) printf(" %s", number.c_str());
	printf("\n");
}

void MTLPrinter::texture(int type, std::string const &file){
	const std::vector<std::string> names{
		"untracked", 
		"ambient", "diffuse", "specular", "specular-highlight", 
		"bump", "displacement"
	};
	std::string const &name = names[type];
	printf("map|%s: %s\n", name.c_str(), file.c_str());
}

void MTLPrinter::texture(int type){
	const std::vector<std::string> names{
		"untracked", 
		"ambient", "diffuse", "specular", "specular-highlight", 
		"bump", "displacement"
	};
	std::string const &name = names[type];
	printf("map|%s (empty map)\n", name.c_str());
}

void MTLPrinter::iparam(int type, std::string const &value){
	const std::vector<std::string> names{"illumination-model"};
	std::string const &name = names[type];
	printf("iparam|%s: %s\n", name.c_str(), value.c_str());
}

void MTLPrinter::fparam(int type, std::string const &value){
	const std::vector<std::string> names{"opaqueness", "specular-exponent", "optical-density", "transparency"};
	std::string const &name = names[type];
	printf("fparam|%s: %s\n", name.c_str(), value.c_str());
}

// context methods

MTLContext::MTLContext(std::vector<Material> &_materials, std::set<std::string> &_textures) : materials{_materials}, textures{_textures} {}

void MTLContext::comment(std::string const &comment){}

void MTLContext::material(std::string const &name){
	materials.push_back(Material());
	materials.back().name = name;
}

void MTLContext::material(){}

void MTLContext::colour(int type, std::vector<std::string> const &numbers){
	if(!materials.empty()){
		std::vector<float> data;
		for(std::string const &number : numbers) data.push_back(atof(number.c_str()));
		materials.back().colours.insert(std::pair<int,std::vector<float>>{type, data});
	}
}

void MTLContext::texture(int type, std::string const &file){
	if(!materials.empty()){
		materials.back().textures.insert(std::pair<int,std::string>{type, file});
		textures.insert(file);
	}
}

void MTLContext::texture(int type){}

void MTLContext::iparam(int type, std::string const &value){
	if(!materials.empty()){
		int data = atoi(value.c_str());
		materials.back().iextra.insert(std::pair<int,float>{type, data});
	}
}

void MTLContext::fparam(int type, std::string const &value){
	if(!materials.empty()){
		float data = atof(value.c_str());
		materials.back().fextra.insert(std::pair<int,float>{type, data});
	}
}
