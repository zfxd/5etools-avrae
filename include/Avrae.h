//
// Avrae.h
//

#pragma once

#include <iostream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Avrae
{
	public:
	// Members
		std::string name;
		int level;
		std::string school;
		std::vector < std::string > automation;
		std::string classes;
		std::string subclasses = "";
		std::string casttime;
		std::string range;
		json components;
		std::string duration;
		bool ritual = false;
		std::string description;
		std::string higherlevels = "";
		bool concentration = false;

		
	// Shouldn't need to explicitly declare these...
	//	Avrae();
	// ~Avrae();

		std::string to_string() {
			// to string
			std::string str = "{\"name\":\"" + name + "\",";
			str += "\"level\":" + std::to_string(level) + ",";
			str += "\"school\":\"" + school + "\",";
			str += "\"automation\":[],"; // Maybe we add automation support next time lol
			str += "\"classes\":\"" + classes + "\",";
			str += "\"subclasses\":\"" + subclasses + "\",";
			str += "\"casttime\":\"" + casttime + "\",";
			str += "\"range\":\"" + range + "\",";
			str += "\"components\":" + components.dump() + ",";
			str += "\"duration\":\"" + duration + "\",";
			str += "\"ritual\":" + std::to_string(ritual) + ",";
			str += "\"description\":\"" + description + "\",";
			str += "\"higherlevels\":\"" + higherlevels + "\",";
			str += "\"concentration\":" + std::to_string(concentration) + "}";
			return str;

		}

	

};

/*	AVRAE FORMAT
	{"name":"Gate Seal",
	"level" : 4,
	"school" : "A",
	"automation" : [] ,
	"classes" : "Sorcerer, Warlock, Wizard",
	"subclasses" : "",
	"casttime" : "1 minute",
	"range" : "60ft",
	"components" :
	{"verbal":true,
	"somatic" : true,
	"material" : "a broken portal key, which the spell consumes"},
	"duration" : "24 hours",
	"ritual" : false,
	"description" : "You fortify the fabric of the planes within a 30-foot cube you can see within range. Within that area, portals close and can't be opened for the duration. 
					Spells and other effects that allow planar travel or open portals such as gate or plane shift, fail if used to enter or leave the area. The cube is stationary.",
	"higherlevels" : "When you cast this spell using a spell slot of 6th level or higher, the spell lasts until dispelled.",
	"concentration" : false}
	}	*/

// Should be just a [list] of these