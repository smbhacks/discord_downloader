#include <iostream>
#include <vector>
#include <filesystem>
#include <format>
#include <Urlmon.h>
#include <fstream>
#include <algorithm>
#pragma comment(lib, "urlmon.lib") // make URLDownloadToFile work lol
namespace fs = std::filesystem;
std::vector<std::string> used_file_names;
std::vector<std::string> extensions;
std::string output_path;

//whatever
LPCWSTR StringToLPCWSTR(const std::string& str)
{
    std::wstring temp = std::wstring(str.begin(), str.end());
    wchar_t* result = new wchar_t[temp.length() + 1];
    wcscpy_s(result, temp.length() + 1, temp.c_str());
    return result;
}

std::string make_path(std::string link, int c, std::string extension)
{
    std::string temp = output_path;
    if (!fs::exists(output_path)) fs::create_directory(output_path);
    std::string filename = link.substr(link.rfind('/') + 1, c + extension.length() - 1 - link.rfind('/'));
    temp += filename;
    int occurences = std::count(used_file_names.begin(), used_file_names.end(), filename);
    if(occurences != 0) temp.insert(temp.length() - 4, std::format("_({})", occurences+1));
    used_file_names.push_back(filename);
    return temp;
}

void check_download(std::string link, std::string site, std::string extension)
{
    link += '?'; //add a question mark at the end to make the validation easier
    unsigned int c = link.rfind(extension);
    if (c < link.length() && link[c + extension.length()] == '?')
    {
        link.pop_back(); //remove ? at the end
        std::cout << std::format("\nDownloading from {}{}", site, link.substr(link.rfind('/') + 1, c + extension.length() - 1 - link.rfind('/')));

        LPCWSTR download_link = StringToLPCWSTR(link);
        LPCWSTR path = StringToLPCWSTR(make_path(link, c, extension));
        URLDownloadToFile(NULL, download_link, path, BINDF_GETNEWESTVERSION, NULL);
    }
}

void handle_link(std::string link, std::string site)
{
    for (int i = 0; i < extensions.size(); i++) check_download(link, site, extensions[i]);
}

void handle_text(std::string text)
{
    unsigned int urlstart = text.find("https://cdn.discordapp.com/attachments/");
    if (urlstart < text.length())
    {
        handle_link(text, "cdn.discordapp.com:   ");
        return;
    }
    urlstart = text.find("https://media.discordapp.net/attachments/");
    if (urlstart < text.length()) handle_link(text, "media.discordapp.net: ");
}

void handle_channel(fs::path path)
{
    std::ifstream input(path);
    while (!input.eof())
    {
        std::string text; input >> text;
        handle_text(text);
    }
}

int main(int argc, char* argv[])
{
    std::string program_path = argv[0];
    std::string program_name = program_path.substr(program_path.find_last_of("/\\") + 1);
    if (argc < 3)
    {
        std::cout << std::format("\nDiscord Files Downloader by StudsX 2023\n---------------------------------------\nUse: {} [folder] [extensions]\n[folder]: A folder that contains txt files of archived Discord channels or DMs.\n[extensions]: Files with these extensions will be downloaded. Seperate extensions with spaces. Don't include periods.\n---------------------------------------\nExample: {} my_discord_server jpg png mp4\n", program_name, program_name);
        return 0;
    }
    std::vector<fs::path> file_paths;
    std::string folder_path = argv[1];
    output_path = folder_path + "_output\\";
    for (const auto& entry : fs::directory_iterator(folder_path))
        file_paths.push_back(entry.path());

    for (int i = argc; i > 2; i--)
    {
        extensions.push_back(std::format(".{}",argv[i - 1]));
    }
    for (int x = 0; x < file_paths.size(); x++)
        handle_channel(file_paths[x]);

    std::cout << "\nDownload complete.\n";
}