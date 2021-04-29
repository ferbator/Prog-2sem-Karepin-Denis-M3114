#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <iomanip>
#include "pugixml.hpp"

using namespace std;

using namespace pugi;

double calc_dist(pair<double, double> &a, pair<double, double> &b) {

    double lat1 = a.first / 180 * M_PI;
    double long1 = a.second / 180 * M_PI;
    double lat2 = b.first / 180 * M_PI;
    double long2 = b.second / 180 * M_PI;

    double delta = long2 - long1;

    double y = sqrt(pow(cos(lat2) * sin(delta), 2) +
                    pow(cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(delta), 2));
    double x = sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(delta);

    double ad = atan2(y, x);
    double dist = ad * 6372795;

    return dist;
}

double primFindMST(vector<pair<double, double>> coordinates) {
    int n = coordinates.size();
    vector<vector<double>> graph(n, vector<double>(n));
    vector<double> distant(n, 6372795);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            graph[i][j] = calc_dist(coordinates[i], coordinates[j]);

    vector<bool> passed(n);
    distant[0] = 0;
    for (int i = 0; i < n; i++) {
        int min_ = -1;

        for (int j = 0; j < n; j++)
            if (!passed[j] && (min_ == -1 || distant[j] < distant[min_]))
                min_ = j;

        passed[min_] = true;

        for (int j = 0; j < n; j++)
            if (!passed[j] && graph[min_][j] < distant[j] && min_ != j)
                distant[j] = graph[min_][j];
    }

    double result = 0;

    for (int i = 0; i < n; i++)
        result += distant[i];

    return result;

}

class Stop {
public:
    string view;
    vector<pair<double, double>> coord;
    int max_route = 0;
    double long_route = 0.0;

    void clear() {
        view = "";
        coord.clear();
        max_route = 0;
    }
};

void print_map(map<string, Stop> a) {
    for (auto it = a.begin(); it != a.end(); it++) {
        cout << "<" << it->second.view << "> ==" << it->first << "== " << it->second.max_route << "\n";
//        for (auto i:it->second.coord)
//            cout << i.first << " " << i.second << "\n";
    }
}

void route_with_max_stops(map<string, Stop> routes) {
    int max = 0;
    string key_max_rout;
    for (auto it = routes.begin(); it != routes.end(); it++) {
        if (it->second.max_route > max) {
            max = it->second.max_route;
            key_max_rout = it->first;
        }
    }
    cout << "\nМаршрут: <" << key_max_rout << "> Транспорт: " << routes[key_max_rout].view
         << " Координаты остановок и их количество: \n" << "*" << max + 1 << "*\n\n";;
    for (int i = 0; i < routes[key_max_rout].coord.size(); i++)
        cout << "<" << i + 1 << "> (" << routes[key_max_rout].coord[i].first << ", "
             << routes[key_max_rout].coord[i].second << ") ";
    cout << "\n";
}

void longest_route(map<string, Stop> a) {
    for (auto it = a.begin(); it != a.end(); it++)
        it->second.long_route = primFindMST(it->second.coord);
    double max = 0.0;
    string key_long_route;
    for (auto it = a.begin(); it != a.end(); it++) {
        if (it->second.long_route > max) {
            max = it->second.long_route;
            key_long_route = it->first;
        }
    }

    cout << "Маршрут: <" << key_long_route << "> Транспорт: " << a[key_long_route].view
         << " Наиболее длинный маршрут: \n" << "*" << max + 1 << "*\n";;
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
    map<string, int> AllStreet;

    string tmp_routes;
    string tmp_coordinates;
    string tmp_street;
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
        buff.push_back(tmp_routes.substr(pos, tmp_routes.size()));

//        for (auto &i:buff)
//            cout << i << " ";
//        cout << "\n";
        // ------------------------------------------------------------------

        // парсинг улицы ----------------------------------------------------
        tmp_street = (string) tool.child("location").child_value();
        /* bool flag = false;
         for (int i = 0; i < tmp_street.size(); i++) {
             if (tmp_street[i] == ',') {
                 flag = true;

                 string tmp_street_first = tmp_street.substr(0, i);
                 string tmp_street_second = tmp_street.substr(i + 2, tmp_street.size() - i - 2);

                 if (AllStreet.find(tmp_street_first) != AllStreet.end()) {
                     AllStreet[tmp_street_first]++;
                 } else
                     AllStreet.emplace(tmp_street_first, 1);

                 if (AllStreet.find(tmp_street_second) != AllStreet.end()) {
                     AllStreet[tmp_street_second]++;
                 }
                 AllStreet.emplace(tmp_street_second, 1);

                 break;
             }
         }
         if (!flag) {
             if (AllStreet.find(tmp_street) != AllStreet.end()) {
                 AllStreet[tmp_street]++;
             } else
                 AllStreet.emplace(tmp_street, 1);
         }*/
        string tmp_value;

        for (int i = 0; i < tmp_street.size(); i++) {
            if (tmp_street[i] != ',')
                tmp_value = tmp_value + tmp_street[i];
            else {
                AllStreet[tmp_value]++;
                tmp_value = "";
            }
        }
        if (tmp_value != "") {
            tmp_value.erase(0);
            AllStreet[tmp_value]++;
        }
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

                if (routesForBus.find(i) != routesForBus.cend()) {
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

                if (routesForTram.find(i) != routesForTram.cend()) {
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

                if (routesForTrolleybus.find(i) != routesForTrolleybus.cend()) {
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

    cout << "------------------1-----------------";
    // Первое задание
    route_with_max_stops(routesForBus);
    route_with_max_stops(routesForTram);
    route_with_max_stops(routesForTrolleybus);
    cout << "------------------------------------\n\n";

    cout << "------------------2-----------------\n";
    // Второе задание
    longest_route(routesForBus);
    longest_route(routesForTram);
    longest_route(routesForTrolleybus);
    cout << "------------------------------------\n\n";

    cout << "------------------3-----------------\n";
    // Третье задание
    int max_street = 0;
    string key_max_street;
    for (auto it = AllStreet.begin(); it != AllStreet.end(); it++)
        if (it->second > max_street && it->first != "") {
            max_street = it->second;
            key_max_street = it->first;
        }
    cout << "Улица: <" << key_max_street << "> Остановки: <" << AllStreet[key_max_street] << ">\n";
    cout << "------------------------------------\n";


    // print_map(routesForBus);

    return 0;
}
