#include "Settings.h"

void Settings::load()
{

	std::ifstream i("settings.json");
	if (i.is_open()) {
		nlohmann::json j;
		i >> j;
		bg_color.SetRGB(j["bg_color"]);
		last_access_to_archive_count = j["last_access_to_archive_count"];
		last_modify_to_archive_count = j["last_modify_to_archive_count"];
		create_to_archive_count = j["create_to_archive_count"];
		last_access_to_archive_mod = j["last_access_to_archive_mod"];
		last_modify_to_archive_mod = j["last_modify_to_archive_mod"];
		create_to_archive_mod = j["create_to_archive_mod"];
	}
	else {
		bg_color = wxColor(200, 200, 255);
		last_access_to_archive_count = 2;
		last_modify_to_archive_count = 3;
		create_to_archive_count = 4;
		last_access_to_archive_mod = DAY;
		last_modify_to_archive_mod = WEEK;
		create_to_archive_mod = MONTH;
	}

	
}

void Settings::save()
{
	nlohmann::json j;
	j["bg_color"] = bg_color.GetRGB();
	j["last_access_to_archive_count"] = last_access_to_archive_count;
	j["last_modify_to_archive_count"] = last_modify_to_archive_count;
	j["create_to_archive_count"] = create_to_archive_count;
	j["last_access_to_archive_mod"] = last_access_to_archive_mod;
	j["last_modify_to_archive_mod"] = last_modify_to_archive_mod;
	j["create_to_archive_mod"] = create_to_archive_mod;
	
	std::ofstream o("settings.json");
	o << j << std::endl;
}


wxColour Settings::bg_color = wxColor(0, 0, 0);
int Settings::last_access_to_archive_count = 0;
int Settings::last_modify_to_archive_count = 0;
int Settings::create_to_archive_count = 0;
int Settings::last_access_to_archive_mod = DAY;
int Settings::last_modify_to_archive_mod = DAY;
int Settings::create_to_archive_mod = DAY;