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
    map<string, Stop> routesForBus;
    map<string, Stop> routesForTrolleybus;
    map<string, Stop> routesForTram;

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
            buff_class.view = (string) tool.child("type_of_vehicle").child_value();
            //-------------------------------------------------------------------

            if (buff_class.view == "Автобус") {
                // парсинг координат ------------------------------------------------
                tmp_coordinates = (string) tool.child("coordinates").child_value();
                buff_class.coord.emplace_back(stod(tmp_coordinates.substr(0, 9)),
                                              stod(tmp_coordinates.substr(10, 9)));
                // ------------------------------------------------------------------

                const auto found = routesForBus.find(i);
                if (found != routesForBus.cend()) {
                    routesForBus[i].max_route++;
                    routesForBus[i].coord.emplace_back(stod(tmp_coordinates.substr(0, 9)),
                                                       stod(tmp_coordinates.substr(10, 9)));
                }
                routesForBus.insert(make_pair(i, buff_class));

                buff_class.clear();
            }

            if (buff_class.view == "Трамвай") {
                // парсинг координат ------------------------------------------------
                tmp_coordinates = (string) tool.child("coordinates").child_value();
                buff_class.coord.emplace_back(stod(tmp_coordinates.substr(0, 9)),
                                              stod(tmp_coordinates.substr(10, 9)));
                // ------------------------------------------------------------------

                const auto found = routesForTram.find(i);
                if (found != routesForTram.cend()) {
                    routesForTram[i].max_route++;
                    routesForTram[i].coord.emplace_back(stod(tmp_coordinates.substr(0, 9)),
                                                        stod(tmp_coordinates.substr(10, 9)));
                }
                routesForTram.insert(make_pair(i, buff_class));

                buff_class.clear();
            }

            if (buff_class.view == "Троллейбус") {
                // парсинг координат ------------------------------------------------
                tmp_coordinates = (string) tool.child("coordinates").child_value();
                buff_class.coord.emplace_back(stod(tmp_coordinates.substr(0, 9)),
                                              stod(tmp_coordinates.substr(10, 9)));
                // ------------------------------------------------------------------

                const auto found = routesForTrolleybus.find(i);
                if (found != routesForTrolleybus.cend()) {
                    routesForTrolleybus[i].max_route++;
                    routesForTrolleybus[i].coord.emplace_back(stod(tmp_coordinates.substr(0, 9)),
                                                              stod(tmp_coordinates.substr(10, 9)));
                }
                routesForTrolleybus.insert(make_pair(i, buff_class));

                buff_class.clear();
            }
        }

        buff.clear();
    }


    cout << setprecision(9);

    int max_bus = 0;
    string max_bus_rout;
    for (auto it = routesForBus.begin(); it != routesForBus.end(); it++) {
        if (it->second.max_route > max_bus) {
            max_bus = it->second.max_route;
            max_bus_rout = it->first;
        }
    }
    int max_tram = 0;
    string max_tram_rout;
    for (auto it = routesForTram.begin(); it != routesForTram.end(); it++) {
        if (it->second.max_route > max_tram) {
            max_tram = it->second.max_route;
            max_tram_rout = it->first;
        }

    }
    int max_trolleybus = 0;
    string max_trolleybus_rout;
    for (auto it = routesForTram.begin(); it != routesForTram.end(); it++) {
        if (it->second.max_route > max_trolleybus) {
            max_trolleybus = it->second.max_route;
            max_trolleybus_rout = it->first;
        }
    }

    cout << max_bus << "\n";
    cout << "Маршрут: <" << max_bus_rout << "> Транспорт: " << routesForBus[max_bus_rout].view << " Координаты: \n";
    for (int i = 0; i < routesForBus[max_bus_rout].coord.size(); i++)
        cout << routesForBus[max_bus_rout].coord[i].first << " " << routesForBus[max_bus_rout].coord[i].second << "\n";

    cout << max_trolleybus<< "\n";
    cout << "Маршрут: <" << max_trolleybus_rout << "> Транспорт: " << routesForTrolleybus[max_trolleybus_rout].view
         << " Координаты: \n";
    for (int i = 0; i < routesForTrolleybus[max_trolleybus_rout].coord.size(); i++)
        cout << routesForTrolleybus[max_trolleybus_rout].coord[i].first << " "
             << routesForTrolleybus[max_trolleybus_rout].coord[i].second << "\n";

    cout << max_tram<< "\n";
    cout << "Маршрут: <" << max_tram_rout << "> Транспорт: " << routesForTram[max_tram_rout].view << " Координаты: \n";
    for (int i = 0; i < routesForTram[max_tram_rout].coord.size(); i++)
        cout << routesForTram[max_tram_rout].coord[i].first << " " << routesForTram[max_tram_rout].coord[i].second
             << "\n";

    return 0;
}
