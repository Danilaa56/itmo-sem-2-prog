#include <iostream>
#include <string>
#include <cmath>

#include "lib56/files.h"
#include "lib56/xml/XMLParser.h"

//String stuff
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::list<std::string> split(std::string const& str, std::string const& delimeters){
    size_t delimetersLength = delimeters.length();

    std::list<std::string> list;
    size_t length = str.length();
    size_t startPosition = 0;
    for(size_t i = 0; i < length; i++) {
        for(int j = 0; j < delimetersLength; j++){
            if(str[i] == delimeters[j]) {
                list.emplace_back(str, startPosition, i - startPosition);
                startPosition = i + 1;
                break;
            }
        }
    }
    list.emplace_back(str, startPosition, length - startPosition);
    return list;
}

//Xml-problem stuff
std::list<std::string> getStreets(XMLObject* transport_station) {
    XMLObject* location = transport_station->getChildByTagName("location");
    if(location == nullptr || location->getChildren().empty())
        return std::list<std::string>();

    std::string locationString = transport_station->getChildByTagName("location")->getChildren().begin().operator*()->name;
    std::list<std::string> streets = split(locationString, ",");
    for(std::string& street : streets) {
        trim(street);
    }
    return streets;
}

std::list<std::string> getRoutes(XMLObject* transport_station) {
    XMLObject* location = transport_station->getChildByTagName("routes");
    if(location == nullptr || location->getChildren().empty())
        return std::list<std::string>();

    std::string& routesString = transport_station->getChildByTagName("routes")->getChildren().begin().operator*()->name;
    std::list<std::string> streets = split(routesString, ".,");
    for(std::string& street : streets) {
        trim(street);
    }
    return streets;
}

std::pair<float, float> getPosition(XMLObject* transport_station) {
    XMLObject* node = transport_station->getChildByTagName("coordinates");

    std::string& nodeContent = node->getChildren().begin().operator*()->name;
    std::list<std::string> streets = split(nodeContent, ",");
    return std::pair<float, float> (std::stof(streets.front()), std::stof(streets.front()));
}

//Math stuff
double distanceOnSphere(double lat1, double long1, double lat2, double long2) {
    double delta = long2 - long1;
    double y = sqrt(pow(cos(lat2)*sin(delta), 2) + pow(cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(delta), 2));
    double x = sin(lat1) * sin(lat2) + cos(lat1)*cos(lat2)*cos(delta);
    return atan2(y,x);
}

double getMST(std::list<std::pair<float, float>> const& points){
    size_t pointsCount = points.size();
    double result = 0;

    std::map<std::pair<float, float> const*, double> distances;

    for(auto const& point : points)
        distances[&point] = 1E30;

    distances[&points.front()] = -1;

    auto const* currentVertex = &points.front();
    std::pair<float, float> const* leastIndex;

    for(int ready = 1; ready < pointsCount; ready++){
        double leastDistance = 1E30;
        for(auto const& point : points) {
            if(distances[&point] == -1)
                continue;
            double distance = distanceOnSphere(currentVertex->first / 180 * M_PI, currentVertex->second / 180 * M_PI,
                                               point.first / 180 * M_PI, point.second / 180 * M_PI);
            if(distance < distances[&point]) {
                distances[&point] = distance;
            }
            if(distances[&point] < leastDistance){
                leastIndex = &point;
                leastDistance = distances[&point];
            }
        }
        result += leastDistance;
        distances[leastIndex] = -1;
        currentVertex = leastIndex;
    }
    return result;
}

int main(){
    setlocale(LC_ALL, "Russian");
    setlocale(LC_NUMERIC, "English");

    std::string fileName = "data-20210322T085457-structure-20190416T084545.xml";

    int length;
    char* xmlData = readFile(fileName.c_str(), &length);

    if(!xmlData){
        std::cout << "Failed to read file: " << fileName << std::endl;
        exit(1);
    }

    XMLObject* root = XMLParser56::parse(xmlData);

    std::map<std::string, std::map<std::string, std::list<std::pair<float, float>>>> routeStops;
    std::map<std::string, int> streetCount;

    for(auto transport_station : root->children) {
        std::string& vehicleType = transport_station->getChildByTagName("type_of_vehicle")->getChildren().begin().operator*()->name;

        auto iter = routeStops.find(vehicleType);
        if(iter == routeStops.end()) {
            routeStops[vehicleType] = std::map<std::string, std::list<std::pair<float, float>>>();
        }

        std::list<std::string> routes = getRoutes(transport_station);
        std::pair<float, float> position = getPosition(transport_station);
        for(std::string const& route : routes) {
            auto iter = routeStops[vehicleType].find(route);
            if(iter == routeStops[vehicleType].end()) {
                routeStops[vehicleType][route] = std::list<std::pair<float, float>>();
            }
            routeStops[vehicleType][route].push_back(position);
        }

        std::list<std::string> streets = getStreets(transport_station);
        for(std::string const& street : streets) {
            auto iter = streetCount.find(street);
            if(iter == streetCount.end()) {
                streetCount[street] = 1;
            } else {
                streetCount[street] += 1;
            }
        }
    }

    for(auto const& routesPair : routeStops) {
        std::string const& vehicleType = routesPair.first;

        std::string mostDistanceName;
        double mostDistance = 0;

        std::string mostStopsCountName;
        size_t mostStopsCount = 0;
        for(auto const& pair : routeStops[vehicleType]) {
            double distance = getMST(pair.second) * 6372.795;
            if (distance > mostDistance) {
                mostDistanceName = pair.first;
                mostDistance = distance;
            }

            if (pair.second.size() > mostStopsCount) {
                mostStopsCountName = pair.first;
                mostStopsCount = pair.second.size();
            }
        }
        std::cout << "Наиболее длинный маршрут типа " << vehicleType << ": " << mostDistanceName << ". Примерная длина: " << mostDistance << " км" << std::endl;
        std::cout << "Маршрут типа " << vehicleType << " с наибольшим числом остановок: " << mostStopsCountName << ". Остановок: " << mostStopsCount << std::endl;
    }

    std::string longestKey;
    int longestCount = 0;
    for(auto const& pair : streetCount) {
        if (pair.second > longestCount) {
            longestKey = pair.first;
            longestCount = pair.second;
        }
    }

    std::cout << "Улица с наибольшим числом остановок: " << longestKey << ". Остановок: " << longestCount << std::endl;

    delete root;
    return 0;
}