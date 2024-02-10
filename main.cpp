//
// main.cpp
//

#include "main.h"

using namespace std;


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
		std::cout << "Name: " << curr.name << std::endl;

		if (spell["level"].is_null()) {
			std::cerr << "Error: level is null" << std::endl;
			continue;
		}
		curr.level = spell["level"];
		//std::cout << "Level: " << curr.level << std::endl;

		if (spell["school"].is_null()) {
			std::cerr << "Error: school is null" << std::endl;
			continue;
		}
		curr.school = spell["school"];
		//std::cout << "School: " << curr.school << std::endl;

		if (spell["time"].is_null()) {
			std::cerr << "Error: time is null" << std::endl;
			continue;
		}
		curr.casttime = fet_time(spell["time"]);
		//std::cout << "Time: " << curr.casttime << std::endl;

		if (spell["range"].is_null()) {
			std::cerr << "Error: range is null" << std::endl;
			continue;
		}
		curr.range = fet_range(spell["range"]);
		std::cout << "Range: " << curr.range << std::endl;





		// VVVV TODO


		if (spell["duration"].is_null()) {
			std::cerr << "Error: duration is null" << std::endl;
			continue;
		}
		json duration = spell["duration"];
		std::cout << "Spell duration: " << to_string(duration) << std::endl;

		//if (spell["classes"].is_null()) {
		//	std::cerr << "Error: classes is null" << std::endl;
		//	continue;
		//}
		//json classes = spell["classes"];
		//std::cout << "Spell classes: " << to_string(classes) << std::endl;

		//if (spell["entries"].is_null()) {
		//	std::cerr << "Error: entries is null" << std::endl;
		//	continue;
		//}
		//json entries = spell["entries"];
		//std::cout << "Spell entries: " << to_string(entries) << std::endl;


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
	