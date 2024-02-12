//
// main.cpp
//

#include "main.h"

using namespace std;


int main()
{
	std::ifstream f;

	f.open("../../../epb.json");

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
		//std::cout << "Range: " << curr.range << std::endl;

		if (spell["duration"].is_null()) {
			std::cerr << "Error: duration is null" << std::endl;
			continue;
		}
		curr.duration = fet_duration(spell["duration"]);
		std::cout << "Duration: " << curr.duration << std::endl;
		
		// CONCENTRATION
		std::vector<json> durs = spell["duration"].template get<std::vector<json>>();
		for (json d : durs) {
			if (!d["concentration"].is_null()) {
				curr.concentration = d["concentration"].template get<bool>();
				std::cout << "Concentration" << std::endl;
				break;
			}
		}

		if (spell["classes"].is_null()) {
			std::cerr << "Error: classes is null" << std::endl;
			continue;
		}
		curr.classes = fet_classes(spell["classes"]);
		//std::cout << "Classes: " << curr.classes << std::endl;

		// TODO - subclasses? Avrae has a field for this.
		// What about background? Race? ... etc etc - 5etools has a TON of sources
		// Low priority for now

		// Entries = spell description
		if (spell["entries"].is_null()) {
			std::cerr << "Error: entries is null" << std::endl;
			continue;
		}
		curr.description = fet_entries(spell["entries"].template get<std::vector<std::string>>());
		//std::cout << "Description: " << curr.description << std::endl;

		// Higher Levels
		if (!spell["entriesHigherLevel"].is_null()) {
			std::vector<json> ehl= spell["entriesHigherLevel"].template get<std::vector< json >>();
			
			// Why would we ever have more???
			json e = ehl[0];

			if (e["entries"].is_null()) {
				std::cerr << "Error: Has upcast but spell.entriesHigherLevel.entries is null" << std::endl;
				return 1;
			}
			else {
				curr.higherlevels = fet_entries(e["entries"].template get<std::vector<std::string>>());
				//std::cout << "Higher Levels: " << curr.higherlevels << std::endl;
			}
		}
		else {
			std::cout << "No upcast" << std::endl;
		}



		if (spell["components"].is_null()) {
			std::cerr << "Error: components is null" << std::endl;
			continue;
		}
		curr.components = fet_components(spell["components"], curr.level);
		std::cout << "Components: " << curr.components << std::endl;

		// Ritual
		if (!spell["meta"].is_null()) {
			// Ritual
			if(!spell["meta"]["ritual"].is_null()) {
				curr.ritual = spell["meta"]["ritual"].template get<bool>();
				std::cout << "Ritual" << std::endl;
			}

			// Technomagic goes here too??
		}


		// School
		if(spell["school"].is_null()) {
			std::cerr << "Error: school is null" << std::endl;
			continue;
		}
		curr.school = fet_school(spell["school"]);

		// Subschool
		// Avrae appends to schoolstring
		if(!spell["subschools"].is_null()) {
			// Subschool exists
			if(!add_subschools(curr, spell["subschools"])) {
				std::cerr << "Error: subschools is null" << std::endl;
				continue;
			}
		}

		std::cout << "School: " << curr.school << std::endl;

		avrae_spells.push_back(curr);
	}

	// Output to file
	std::ofstream out;
	out.open("../../../out.json");
	out << "[";
	for (int i = 0; i < avrae_spells.size(); i++) {
		if (i != 0) {
			out << ",";
		}

		out << avrae_spells[i].to_string();
	}
	out << "]";

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
	