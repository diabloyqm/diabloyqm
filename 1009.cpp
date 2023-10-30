#include <iostream>
#include <sstream>
#include <string>
#include <map>

int main() {
    std::string message = "8=FIX.4.2|9=118|35=D|34=2|49=DONALD|52=20160613-22:52:37.227|56=VENUE|11=1|21=3|38=111|40=2|44=50|54=1|55=MQ|60=20160613-22:52:37.227|10=058|";
    
    std::stringstream ss(message);
    std::string token;
    std::map<std::string, std::string> keyValuePairs;

    // Tokenize on '|'
    while (std::getline(ss, token, '|')) {
        std::size_t pos = token.find('=');
        if (pos != std::string::npos) {
            std::string key = token.substr(0, pos);
            std::string value = token.substr(pos + 1);
            keyValuePairs[key] = value;
        }
    }

    // Print extracted key-value pairs
    for (const auto& pair : keyValuePairs) {
        std::cout << pair.first << " -> " << pair.second << std::endl;
    }

    return 0;
}
