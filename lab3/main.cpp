#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <iomanip>
#include "pugixml.hpp"

using namespace std;

using namespace pugi;

#define M_PI 3.1415926535;

class Stop {
public:
    string view;
    vector<pair<double, double>> coord;
    int max_route = 0;

    void clear() {
        view = "";
        coord.clear();
        max_route = 0;
    }
};

double calc_dist(double latitude_first, double longitude_first, double latitude_second, double longitude_second) {

    double lat1 = latitude_first / 180 * M_PI;
    double long1 = longitude_first / 180 * M_PI;
    double lat2 = latitude_second / 180 * M_PI;
    double long2 = longitude_second / 180 * M_PI;

    double delta = long2 - long1;

    double y = sqrt(pow(cos(lat2) * sin(delta), 2) +
                    pow(cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(delta), 2));
    double x = sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(delta);

    double ad = atan2(y, x);
    double dist = ad * 6372795;

    return dist;
}

int main() {
    system("chcp 65001");

    xml_document doc;
    xml_parse_result result = doc.load_file("data.xml");
    if (!result) {
        cout << "The file can't be opened" << endl;
    }
    map<string, Stop> routes;

    string tmp_routes;
    string tmp_coordinates;
    vector<string> buff;
    Stop buff_class;
    for (xml_node tool = doc.first_child().first_child(); tool; tool = tool.next_sibling()) {
        // парсинг номера маршрута ------------------------------------------
        tmp_routes = (string) tool.child("routes").child_value();
        int pos = 0;
        for (int i = 0; i < tmp_routes.size(); i++) {
            if (tmp_routes[i] == ',' || tmp_routes[i] == '.') {
                buff.push_back(tmp_routes.substr(pos, i - pos));
                pos = i + 1;
            }
        }
        if (tmp_routes[tmp_routes.size()] == '.' || tmp_routes[tmp_routes.size()] == ',')
            buff.push_back(tmp_routes.substr(pos, tmp_routes.size()));
        // ------------------------------------------------------------------

        for (auto &i : buff) {

            // парсинг вида транспорта ------------------------------------------
            buff_class.view = tool.child("type_of_vehicle").child_value();
            //-------------------------------------------------------------------

            // парсинг координат ------------------------------------------------
            tmp_coordinates = (string) tool.child("coordinates").child_value();
            buff_class.coord.emplace_back(stod(tmp_coordinates.substr(0, 9)),
                                          stod(tmp_coordinates.substr(10, 9)));
            // ------------------------------------------------------------------

            const auto found = routes.find(i);
            if (found != routes.cend()) {
                routes[i].max_route++;
                routes[i].coord.emplace_back(stod(tmp_coordinates.substr(0, 9)),
                                             stod(tmp_coordinates.substr(10, 9)));
            }
            routes.insert(make_pair(i, buff_class));

            buff_class.clear();
        }

        buff.clear();
    }


    cout << setprecision(9);
    int max_bus = 0;
    int max_trolleybus = 0;
    int max_tram = 0;
    for (auto it = routes.begin(); it != routes.end(); it++) {
        if (it->second.view == "Автобус")
            if (it->second.max_route > max_bus)
                max_bus = it->second.max_route;
        if (it->second.view == "Трамвай")
            if (it->second.max_route > max_tram)
                max_tram = it->second.max_route;
        if (it->second.view == "Троллейбус")
            if (it->second.max_route > max_trolleybus)
                max_trolleybus = it->second.max_route;

        cout << "Маршрут: <" << it->first << "> Транспорт: " << it->second.view << " Координаты: \n";
        for (int i = 0; i < it->second.coord.size(); i++)
            cout << it->second.coord[i].first << " " << it->second.coord[i].second << "\n";

        cout << it->second.max_route << "\n";
    }
    cout << max_bus << " " << max_trolleybus << " " << max_tram;
    return 0;
}
