#include "wavefrontreader.hpp"

#include <stdio.h> // print read content

// read method

void OBJReader::read(FileReader &reader, OBJBuilder &builder){
	
	// line-based content
	while(!reader.isFinished()){
		int offset = 0;
		
		// whitespace
		reader.peek(offset, ' ');
		
		// comment
		if(reader.check(offset, "#")){
			offset++;
			reader.peek(offset, ' ');
			std::string comment = reader.seek(offset, '\n');
			builder.comment(comment);
		}
		
		// object
		else if(reader.check(offset, "o ")){
			offset++;
			if(reader.race(offset, std::vector<char>{' '}, std::vector<char>{'\n'})){
				std::string name = reader.seek(offset, std::vector<char>{' ', '\n'});
				builder.object(name);
			}
			else builder.object();
		}
		
		// group
		else if(reader.check(offset, "g ")){
			offset++;
			std::vector<std::string> groups;
			while(!reader.isFinished()){
				if(!reader.race(offset, std::vector<char>{' '}, std::vector<char>{'\n'})) break;
				std::string name = reader.seek(offset, std::vector<char>{' ', '\n'});
				groups.push_back(name);
			}
			if(!groups.empty()) builder.group(groups);
			else builder.group();
		}
		
		// vertex
		else if(reader.check(offset, "v ")){
			offset++;
			reader.peek(offset, ' ');
			std::vector<std::string> numbers = reader.peekFloats(offset, std::vector<char>{' '}, std::vector<char>{'\n'});
			builder.vertex(numbers);
		}
		
		// texture coordinate
		else if(reader.check(offset, "vt ")){
			offset += 2;
			reader.peek(offset, ' ');
			std::vector<std::string> numbers = reader.peekFloats(offset, std::vector<char>{' '}, std::vector<char>{'\n'});
			builder.texture(numbers);
		}
		
		// normal
		else if(reader.check(offset, "vn ")){
			offset += 2;
			reader.peek(offset, ' ');
			std::vector<std::string> numbers = reader.peekFloats(offset, std::vector<char>{' '}, std::vector<char>{'\n'});
			builder.normal(numbers);
		}
		
		// face
		else if(reader.check(offset, "f ")){
			offset++;
			reader.peek(offset, ' ');
			std::vector<std::vector<std::string>> indices;
			while(!reader.isFinished()){
				std::vector<std::string> pairing;
				for(int i = 0; !reader.isFinished(); i++){
					std::string pair;
					pair += reader.peek(offset, '-');
					pair += reader.peekDigits(offset);
					pairing.push_back(pair);
					if(!reader.check(offset, "/")) break;
					offset++;
				}
				indices.push_back(pairing);
				if(!reader.race(offset, std::vector<char>{' '}, std::vector<char>{'\n'})) break;
			}
			builder.face(indices);
		}
		
		// material
		else if(reader.check(offset, "usemtl ")){
			offset += 6;
			if(reader.race(offset, std::vector<char>{' '}, std::vector<char>{'\n'})){
				std::string name = reader.seek(offset, std::vector<char>{' ', '\n'});
				builder.material(name);
			}
			else builder.material();
		}
		
		// library
		else if(reader.check(offset, "mtllib ")){
			offset += 6;
			if(reader.race(offset, std::vector<char>{' '}, std::vector<char>{'\n'})){
				std::string name = reader.seek(offset, std::vector<char>{' ', '\n'});
				builder.library(name);
			}
			else builder.library();
		}
		
		// next line
		reader.seek(offset, std::vector<char>{'\n'});
		offset++;
		reader.advance(offset);
	}
}

// printing methods

void OBJPrinter::comment(std::string const &comment){
	printf("comment: %s\n", comment.c_str());
}

void OBJPrinter::object(std::string const &name){
	printf("object: %s\n", name.c_str());
}

void OBJPrinter::object(){
	printf("object (empty name)\n");
}

void OBJPrinter::group(std::vector<std::string> const &names){
	printf("groups:");
	for(std::string const &name : names) printf(" %s", name.c_str());
	printf("\n");
}

void OBJPrinter::group(){
	printf("group (empty name)\n");
}

void OBJPrinter::vertex(std::vector<std::string> const &numbers){
	printf("vertex:");
	for(std::string number : numbers) printf(" %s", number.c_str());
	printf("\n");
}

void OBJPrinter::texture(std::vector<std::string> const &numbers){
	printf("texture:");
	for(std::string number : numbers) printf(" %s", number.c_str());
	printf("\n");
}

void OBJPrinter::normal(std::vector<std::string> const &numbers){
	printf("normal:");
	for(std::string number : numbers) printf(" %s", number.c_str());
	printf("\n");
}

void OBJPrinter::face(std::vector<std::vector<std::string>> const &indices){
	printf("face:");
	for(std::vector<std::string> const &pair : indices){
		printf(" ");
		std::vector<std::string>::const_iterator index = pair.begin();
		while(true){
			printf("%s", index->c_str());
			index++;
			if(index != pair.end()) printf("|");
			else break;
		}
	}
	printf("\n");
}

void OBJPrinter::material(std::string const &name){
	printf("material: %s\n", name.c_str());
}

void OBJPrinter::material(){
	printf("material (empty name)\n");
}

void OBJPrinter::library(std::string const &name){
	printf("library: %s\n", name.c_str());
}

void OBJPrinter::library(){
	printf("library (empty name)\n");
}

// context methods

OBJContext::OBJContext(ObjectData &_data, std::vector<ObjectGroup> &_groups) : data{_data}, groups{_groups}, currentMaterial(".default"), currentGroups{} {
	std::vector<ObjectGroup>::iterator defaultGroup = std::find_if(groups.begin(), groups.end(), [](ObjectGroup const &group){ return group.name == ".default"; });
	if(defaultGroup != groups.end()) currentGroups.push_back(&(*defaultGroup));
	else{
		groups.push_back(ObjectGroup());
		groups.back().name = ".default";
		currentGroups.push_back(&groups.back());
	}
}

void OBJContext::indexMaterial(std::string next){
	if(currentMaterial != ""){
		for(ObjectGroup *group : currentGroups) 
			group->materials.insert(std::pair<int,std::string>{
				group->indices.size(), currentMaterial});
	}
	currentMaterial = next;
}

void OBJContext::finalise(){
	indexMaterial(""); // place final material indices
}

void OBJContext::comment(std::string const &comment){}

void OBJContext::object(std::string const &name){}

void OBJContext::object(){}

void OBJContext::group(std::vector<std::string> const &names){
	indexMaterial(currentMaterial);
	currentGroups.clear();
	for(std::string const &name : names){
		std::vector<ObjectGroup>::iterator group = std::find_if(groups.begin(), groups.end(), [name](ObjectGroup const &group){ return group.name == name; });
		if(group != groups.end()) currentGroups.push_back(&(*group));
		else{
			groups.push_back(ObjectGroup());
			groups.back().name = name;
			currentGroups.push_back(&groups.back());
		}
	}
	if(currentGroups.empty()) currentGroups.push_back(&(*(std::find_if(groups.begin(), groups.end(), [](ObjectGroup const &group){ return group.name == ".default"; }))));
}

void OBJContext::group(){}

void OBJContext::vertex(std::vector<std::string> const &numbers){
	for(std::string const &number : numbers) data.vertex.push_back(atof(number.c_str()));
}

void OBJContext::texture(std::vector<std::string> const &numbers){
	for(std::string const &number : numbers) data.uvcoord.push_back(atof(number.c_str()));
}

void OBJContext::normal(std::vector<std::string> const &numbers){
	for(std::string const &number : numbers) data.normal.push_back(atof(number.c_str()));
}

void OBJContext::face(std::vector<std::vector<std::string>> const &indices){
	std::vector<int> face;
	for(std::vector<std::string> const &pair : indices){
		std::vector<int> output;
		int pairIndex = 0;
		for(std::string const &index : pair){
			int value;
			if(index == "") value = 0;
			else{
				value = atoi(index.c_str());
				if(value < 0){
					switch(pairIndex){
						case 0:
							value = data.vertex.size() / 3 + value;
							break;
						case 1:
							value = data.uvcoord.size() / 2 + value;
							break;
						case 2:
							value = data.normal.size() / 3 + value;
							break;
					}
					if(value < 0) value = 0;
				}
			}
			face.push_back(value);
			pairIndex++;
		}
		while(pairIndex < 3) face.push_back(0);
	}
	for(ObjectGroup *group : currentGroups) group->indices.push_back(face);
}

void OBJContext::material(std::string const &name){
	indexMaterial(name);
}

void OBJContext::material(){}

void OBJContext::library(std::string const &name){
	data.libraries.push_back(name);
}

void OBJContext::library(){}