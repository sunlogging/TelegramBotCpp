#define CURL_STATICLIB
#include <curl/curl.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>



const std::string BASE_URL_TG = "https://api.telegram.org/bot";
const std::string TOKEN = "5924412397:AAG5pCTLiBH5_Edz9lwkUYbuyA-VNOE_KKs";
const std::string ID = "5976859038";

const std::string COIN_MAIN = "USD";
const std::string BASE_URL_API = "https://www.google.com/finance/quote/" + COIN_MAIN + "-";

std::vector<std::string> coins = {"BTC", "TRX", "RUB"};


void sendMessage(std::string text) {
    CURL* curl = curl_easy_init();
    CURLcode response;

    std::string command = "/sendMessage";
    std::string request = BASE_URL_TG + TOKEN + command + "?chat_id="+ ID + "&text=" + text + "&parse_mode=HTML";
    std::cout << request;
    curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
    response = curl_easy_perform(curl);
    std::cout << response;
    curl_easy_cleanup(curl);
}

auto split(std::string s, std::string del = " ")
{
    int start, end = -1 * del.size();
    std::vector<std::string> spliters;
    do {
        start = end + del.size();
        end = s.find(del, start);
        spliters.push_back(s.substr(start, end - start));
    } while (end != -1);
    return spliters;
}

double searthCode(std::string code) {
    return std::stold(split(split(code, "YMlKec fxKbKc\">")[1], "</div>")[0]);
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

auto getCurrency(std::string coin) {

    CURL* curl = curl_easy_init();
    CURLcode res;
    std::string response;

    std::string request = BASE_URL_API + coin + "?sa=X&ved=2ahUKEwj8ssuPmbb8AhUIif0HHemIBvIQmY0JegQICBAc";

    curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    return searthCode(response);


}
void getAllCurrency() {

    for (std::string coin : coins) {
        std::string msg = coin + "-" + std::to_string(getCurrency(coin));
        std::cout << msg;
        sendMessage(msg);
    }
}

int main()
{
    while (!false) {
        
        getAllCurrency();
        int time = 60 * 60 * 30;
        Sleep(time);
    }

    return 0;
}
