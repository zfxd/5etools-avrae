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

			// TODO - Deal with singular as required maybe...
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
 * 5et format is a list of durations, each a json with:
 *		- "type" (duration type)
 *			* "instant" -> "Instantaneous"
 *			* "special" -> "Special"
 *			* "permanent":
 *				- "ends" is null -> "Permanent"
 *				- List of "ends" -> "Until x, y or z"
 *					- "dispel" -> "dispelled"
 * 					- "trigger" -> "triggered"
 *					- "discharge" -> "discharged"
 *			* "
 * 
 * Avrae format is a string. Check the above rules.
 */
std::string fet_duration(json fet_d) {
	return "TODO";
}