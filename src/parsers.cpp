// parsers.cpp

#include "parsers.h"

/** Takes a time(Casting time) in 5et format and returns it in Avrae format
 *		5et format is a list of casting times, each a:
 *		- "number" + "unit" (e.g. "1 action, bonus, minute, hour, reaction, free action") 
 *		+ optional "condition" (for reaction and free action)
 * 
 *		Avrae format is a string in order number + unit + condition
 */
std::string fet_time(json fet_t) {
	std::string ret = "";
	int i = 0;
	for (json t : fet_t) {
		if (i++ > 0) {
			ret += " or ";
		}
		int n = t["number"].template get<int>();
		std::string u = t["unit"].template get<std::string>();
		ret += std::to_string(n) + " " + u;


		if (!t["condition"].is_null()) {
			ret += ", " + t["condition"].template get<std::string>();
		}
	}

	return ret;
}

/** Takes a range in 5et format and returns it in Avrae format
 * 5et is a json with:
 *		- "distance" (which is a json with a "type" and "amount")
 *		
 *		- "type" (range type)
 *			- if "special - output "Special"
 *			- if "point"
				*"self" -> "Self"
				*"touch"-> "Touch"
				* "sight" -> "Sight"
				* "unlimited" -> "Unlimited"
				* "plane" -> "Unlimited on the same plane"
				* else - target depends entirely on "distance" [amount] [type]
 *			- Anything else - output "Self" plus, in parentheses,
 *				* [amount]-[type] [shape]
 *				* types in singular
 * 				* Special shapes: "sphere" -> " radius",
 * 								 "hemisphere" or "cylinder" -> "-radius [shape]"
 * 
 * 
 * Avrae format is a string. Check the above rules.
 */
std::string fet_range(json fet_r) {
	// Maybe add errors instead of default fall through...
	std::string ret = "";
	std::string range_type = fet_r["type"].template get<std::string>();
	
	if (range_type == "special") {
		ret += "Special";
	}
	else if (range_type == "point") {
		json dist = fet_r["distance"];
		std::string dist_type = dist["type"].template get<std::string>();
		if (dist_type == "self") {
			ret += "Self";
		}
		else if (dist_type == "touch") {
			ret += "Touch";
		}
		else if (dist_type == "sight") {
			ret += "Sight";
		}
		else if (dist_type == "unlimited") {
			ret += "Unlimited";
		}
		else if (dist_type == "plane") {
			ret += "Unlimited on the same plane";
		}
		else {
			int dist_amount = dist["amount"].template get<int>();
			if(dist_amount == 1){

				// Singularize
				if (dist_type == "feet") {
					dist_type = "foot";
				}
				else if (dist_type == "yards") {
					dist_type = "yard";
				}
				else if (dist_type == "miles") {
					dist_type = "mile";
				}
			}
			ret += std::to_string(dist_amount) + " " + dist_type;
		}	
	}
	else {
		std::string shape = range_type;
		ret += "Self (";
		json dist = fet_r["distance"];
		std::string dist_type = dist["type"].template get<std::string>();
		int dist_amount = dist["amount"].template get<int>();

		// Singularize
		if (dist_type == "feet") {
			dist_type = "foot";
		}
		else if (dist_type == "yards") {
			dist_type = "yard";
		}
		else if (dist_type == "miles") {
			dist_type = "mile";
		}

		// Special shapes
		ret += std::to_string(dist_amount) + "-" + dist_type;
		if (shape == "sphere") {
			ret +=" radius";
		}
		else if (shape == "hemisphere" || shape == "cylinder") {
			ret +="-radius " + shape;
		}
		else {
			ret +=" " + shape;
		}

		ret +=")";
	}

	return ret;
}

/** Takes a duration in 5et format and returns it in Avrae format
 * 5et format is a list of json with
 *		- type
 *			* "instant" -> "Instantaneous"
 *			* "special" -> "Special"
 *			* "permanent": -> check optional "ends"
 *				- "ends" is null -> "Permanent"
 *				- List of "ends" -> "Until x, y or z"
 *					- "dispel" -> "dispelled"
 * 					- "trigger" -> "triggered"
 *					- "discharge" -> "discharged"
 *			* "timed" -> check "Durations"
 *		- Durations (each a json with):
 *			* type (dtype) -> "turn", "round", "minute", "hour", "day", "week", "year"
 *			* amount
 *			* optional upTo (bool)
 *		- "Concentration" (bool) - determines "upTo"
 *	
 * 
 * Avrae format is a string. Check the above rules.
 */
std::string fet_duration(json fet_d) {
	std::string ret = "";
	int i = 0;
	for (json curr_d : fet_d) {
		if (i++ > 0) {
			ret += " or ";
			// Not bothering with the semicolon nonsense for >2 durations
			// Doubt there are spells with those
		}

		std::string type = curr_d["type"];
		if (type == "instant") {
			return "Instantaneous";
		}
		else if (type == "special") {
			return "Special";
		}
		else if (type == "permanent") {
			if (curr_d["ends"].is_null()) {
				return "Permanent";
			}
			else {
				ret += "Until ";
				int max = curr_d["ends"].size();
				for (int j = 0; j < max; j++) {
					if (j > 1) {
						ret += " or ";
					}

					std::string curr = curr_d["ends"][j].template get<std::string>();
					if (curr == "dispel") {
						ret += "dispelled";
					}
					else if (curr == "trigger") {
						ret += "triggered";
					}
					else if (curr == "discharge") {
						ret += "discharged";
					}

					// Anything >2 is ugly but I'm pretty sure no spell has more than 2 ends
				}
			}
		}
		else if (type == "timed") {
			// Check durations
			json d = curr_d["duration"];
			std::string dtype = d["type"].template get<std::string>();
			int amount = d["amount"].template get<int>();
			bool upTo = false;
			bool conc = false;


			// Check upTo
			if (!d["upTo"].is_null()) {
				// Same deal as Conc. If it's set, it SHOULD be true
				upTo = d["upTo"].template get<bool>();
			}
			// Check concentration
			if (!curr_d["concentration"].is_null()) {
				// Conc, if is set it SHOULD be true

				conc = curr_d["concentration"].template get<bool>();
				upTo = conc;
			}

			std::string concto = "";

			if (conc) {
				concto += "Concentration, ";
			}

			if (upTo) {
				if (concto == "") {
					concto += "Up to ";
				}
				else {
					concto += "up to ";
				}
			}

			ret += concto + std::to_string(amount) + " ";

			// Pluralize if needed
			if (amount > 1) {
				dtype += "s";
			}
			ret += dtype;
		}
		else {
			std::cerr << "Error: unknown duration type" << std::endl;
			return "Error";
		}
	}
	return ret;
}

/** Takes classes in 5etools format and returns it in Avrae
  * format. Classes is a json of:
  * "fromClassList" - a list of json with:
  * - "name" (class name)
  * - "source" (sourcebook)
  * 
  * 5eTools also has other sources e.g. subclass, race, feat but those
  * aren't supported yet
  * 
  * Avrae format is a string with the class names separated by commas
  */
std::string fet_classes(json fet_c) {
	std::string ret = "";
	json classes = fet_c["fromClassList"];
	for (int i = 0; i < classes.size(); i++) {
		if (i > 0) {
			ret += ", ";
		}

		ret += classes[i]["name"].template get<std::string>();
	}

	// TODO - subclasses? Races? Background? Probably another function

	return ret;
}

/**
  * Finds and replaces 5etools code?markdown?
  * e.g. {@dice xdy} -> xdy
  *		{@condition cond} -> cond
  *		{@status stat} -> stat
  *		
  */
std::string fet_sanitizer(std::string in) {
	// This feels horrendous performance wise
	std::string::iterator p = in.begin();
	while (p != in.end()) {
		if (*p == '{') {
			std::string::iterator start = p;
			while (*p != ' ') {
				p++;
			}
			p++;

			p = in.erase(start, p);
		}
		else if (*p == '}') {
			p = in.erase(p);
		}
		else {
			p++;
		}
	}
	return in;
}

/**
 * Takes a list of entries in 5et format and returns it in Avrae format
 * 5et only contains a list of strings. Each one is separated by a new line.
 * Works for description, and also for higherLevels
 */
std::string fet_entries(std::vector<std::string> entries) {
	std::string ret = "";

	for (std::string entry : entries) {
		ret += fet_sanitizer(entry) + "\n";
	}

	return ret;
}

/**
 * Takes a json of entries in 5et format and returns it in Avrae format
 * Level is used for Royalty component (1gp per spell level)
 *		e.g. M (Royalty Component: 1 gp)
 * 5et format is a json with:
 * - bool "v" (verbal)
 * - bool "s" (somatic)
 * - bool "r" (royalty)
 * - "m" that can be:
 *		- a string for "Has Material Component" OR
 *		- a json {	* string "text" - the material component
 *					* optional bool "consume" = true
 *					* optional int "cost" in cp}
 *			for "Has Consumable/Costed Material Component" OR
 *		- bool = true for "Has Generic Material Component"
 * 
 * 
 * Avrae format is a json with:
 * - bool "verbal"
 * - bool "somatic"
 * - string "material" (Royalty components go here)
 * 
 */
json fet_components(json fet_c, int level) {
	json ret;
	if (!fet_c["v"].is_null()) {
		ret["verbal"] = true;
	}

	if (!fet_c["s"].is_null()) {
		ret["somatic"] = true;
	}

	// Build "material" string
	std::string m = "";
	if (fet_c["m"].is_null()) {
		// No M - keep as empty string
	}
	else if (fet_c["m"].type() == json::value_t::boolean) {
		// Generic Material Component
		m += "-";
	}
	else if (fet_c["m"].type() == json::value_t::string) {
		m += fet_c["m"].template get<std::string>();
	}
	else {
		// Has Consumable/Costed Material Component
		// "cost" and "consumed" only for filtering - all are included in "text"
		m += fet_c["m"]["text"].template get<std::string>();
	}


	// Royalty comes last
	if (!fet_c["r"].is_null()) {
		if (m != "") m += ", ";
		m += "Royalty Component: " + std::to_string(level) + " gp)";
	}

	ret["material"] = m;

	return ret;
}