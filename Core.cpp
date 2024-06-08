#include "Core.h"



int Core::move(std::filesystem::path origin, std::filesystem::path destination, bool overwrite)
{
    try {

    if (std::filesystem::exists(destination)) {
        if (overwrite) {
            std::filesystem::remove(destination);
        }
        else {
            int i = 0;
            while (std::filesystem::exists(destination.parent_path().string() +
                "\\" +
                destination.stem().string() + ((i > 0) ? (
                    "(" + std::to_string(i) + ")") : ("")) +
                destination.extension().string())) {
                i++;
            }
            destination = std::filesystem::path(destination.parent_path().string() +
                "\\" +
                destination.stem().string() + ((i>0)?(
                "(" + std::to_string(i) + ")"):("") )+
                destination.extension().string());
        }
    }
        std::filesystem::copy(origin, destination);
        std::filesystem::remove(origin);
    }
    catch (std::filesystem::filesystem_error& e) {
        wxLogError(e.what());
        return -1;
    }
    return 0;
}

wxArrayString Core::getFiles(std::string path, bool recursive)
{
    wxArrayString files;
    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (recursive && entry.is_directory()) {
                wxArrayString temp = Core::getFiles(entry.path().string(), recursive);
                for (int i = 0; i < temp.size(); i++)
                {
                    files.Add(temp[i]);
                }
            }
            if (entry.is_regular_file()) {
                files.Add(entry.path().string());
            }

        }
    }
    catch (std::filesystem::filesystem_error& e) {
        wxLogError(e.what());
        return files;
    }
    catch (std::system_error & e) {
        wxLogError(e.what());
        return files;
    }
    return files;
}

int Core::createDir(std::filesystem::path path)
{
    if (!std::filesystem::exists(path.parent_path()))
        std::filesystem::create_directories(path.parent_path());

    return 0;
}

std::string Core::sort(std::filesystem::path file, wxString startDir)
{
    return (startDir + std::filesystem::path::preferred_separator + sortSystem->Sort(file) + std::filesystem::path::preferred_separator + file.filename().string()).ToStdString();
}

Core::Core()
{
    Core::sortSystem = new SortComposition();

}
