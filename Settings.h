#pragma once
#include <wx/colour.h>
#include <json.hpp>
#include <fstream>
#include <wx/gdicmn.h>


//static enum TIME
//{
//	DAY = 86400,
//	WEEK = 604800,
//	MONTH = 2592000,
//	YEAR = 31536000
//};

class Settings
{
private:
	


public:
	static enum TIME
	{
		DAY = 86400,
		WEEK = 604800,
		MONTH = 2592000,
		YEAR = 31536000
	};
	
	static wxColour bg_color;
	static int last_access_to_archive_count;
	static int last_modify_to_archive_count;
	static int create_to_archive_count;
	static int last_access_to_archive_mod;
	static int last_modify_to_archive_mod;
	static int create_to_archive_mod;
	
	


	static void load();
	static void save();

};

static const int All[] = { Settings::TIME::DAY,Settings::TIME::WEEK,Settings::TIME::MONTH,Settings::TIME::YEAR };
