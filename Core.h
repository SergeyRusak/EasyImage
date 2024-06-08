#pragma once
#include <wx/string.h>
#include "Settings.h"
#include "SortComposition.h"
#include "Core.h"
#include <wx/log.h>

class Core
{
public:
	Core();
	~Core();
	int move(std::filesystem::path origin, std::filesystem::path destination, bool overwrite);
	wxArrayString getFiles(std::string path, bool recursive);
	int createDir(std::filesystem::path path);
	std::string sort(std::filesystem::path file, wxString startDir);
	SortComposition* sortSystem;



};
