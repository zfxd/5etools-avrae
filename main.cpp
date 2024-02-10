//
// main.cpp
//

#include "main.h"
#include "Avrae.h"

using namespace std;


#include <fstream>
//#include <Windows.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main()
{
	std::ifstream f;

	f.open("../../../5et.json");

	// Check if open was successful
	if (f.fail()) {
		std::cerr << "Error opening file" << std::endl;
		return 1;
	}

	json j;

	try
	{
		j = json::parse(f);
	}
	catch (json::parse_error& ex)
	{
		std::cerr << "parse error at byte " << ex.byte << std::endl;
	}

	json spells = j["spell"];

	std::vector<Avrae> avrae_spells;

	for (json spell : spells) {
		std::cout << "---------------------------" << std::endl;

		// Do I want to store it in 5et format?
		//
		
		Avrae curr;

		//std::cout << to_string(spell) << std::endl;
		if (spell["name"].is_null()) {
			std::cerr << "Error: name is null" << std::endl;
			continue;
		}
		curr.name = spell["name"];
		std::cout << "Spell name: " << spell["name"] << std::endl;

		if (spell["level"].is_null()) {
			std::cerr << "Error: level is null" << std::endl;
			continue;
		}
		curr.level = spell["level"];
		std::cout << "Spell level: " << spell["level"] << std::endl;

		if (spell["school"].is_null()) {
			std::cerr << "Error: school is null" << std::endl;
			continue;
		}
		curr.school = spell["school"];
		std::cout << "Spell school: " << spell["school"] << std::endl;


		// Parse into avrae format
	}

	return 0;
}

template <typename BasicJsonType>
std::string to_string(const BasicJsonType& j)
{
	return j.dump();
}



// 5ET FORMAT
	//{
	//	"name": "Diamond Edge",
	//		"level" : 0,
	//		"school" : "T",
	//		"time" : [
	//	{
	//		"number": 1,
	//			"unit" : "bonus"
	//	}
	//		] ,
	//		"range": {
	//		"type": "point",
	//			"distance" : {
	//			"type": "touch"
	//		}
	//	},
	//		"duration": [
	//	{
	//		"type": "timed",
	//			"duration" : {
	//			"type": "minute",
	//				"amount" : 1
	//		}
	//	}
	//		] ,
	//		"classes": {
	//		"fromClassList": [
	//		{
	//			"name": "Druid",
	//				"source" : "PHB"
	//		},
	//					{
	//						"name": "Shugenja",
	//						"source" : "FeyFolio"
	//					},
	//					{
	//						"name": "Sorcerer",
	//						"source" : "PHB"
	//					},
	//					{
	//						"name": "Warmage",
	//						"source" : "FeyFolio"
	//					}
	//		]
	//	},
	//		"entries": [
	//			"The edge of a slashing or piercing weapon you are holding is magically sharpened. For the duration, you can use your spellcasting ability instead of 
	//			Strength or Dexterity for the attack and damage rolls of melee attacks using that weapon, and the weapon deals double damage to objects andstructures.
	//			The weapon also becomes magical, if it isn’t already. The spell ends if you cast it again or if you let go of the weapon."
	//		] ,
	//		"source" : "FeyFolio",
	//		"page" : 87,
	//		"components" : {
	//		"v": true,
	//			"s" : true,
	//			"m" : "a tiny diamond worth at least 1gp"
	//	}
	