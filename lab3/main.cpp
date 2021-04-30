#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <iomanip>
#include "pugixml.hpp"

using namespace std;

class CStop {
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

    double primFindMST() {
        int n = coord.size();
        vector<vector<double>> graph(n, vector<double>(n));
        vector<double> distant(n, 6372795);

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                graph[i][j] = calc_dist(coord[i], coord[j]);

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

    ~CStop() = default;

};

void ParsXmlRoutes(map<string, CStop> &routes, const string &type_transport) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("data.xml");
    if (!result) {
        cout << "The file can't be opened" << endl;
    }


    string tmp_routes;
    string tmp_coordinates;
    vector<string> buff;
    CStop buff_class;
    for (pugi::xml_node tool = doc.first_child().first_child(); tool; tool = tool.next_sibling()) {
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

        // ------------------------------------------------------------------

        for (auto &i : buff) {
            // парсинг вида транспорта ------------------------------------------
            buff_class.view = (string) tool.child("type_of_vehicle").child_value();
            //-------------------------------------------------------------------
            if (buff_class.view == type_transport) {
                // парсинг координат ------------------------------------------------
                tmp_coordinates = (string) tool.child("coordinates").child_value();
                buff_class.coord.emplace_back(stod(tmp_coordinates.substr(0, 9)),
                                              stod(tmp_coordinates.substr(10, 9)));
                // ------------------------------------------------------------------

                if (routes.find(i) != routes.cend()) {
                    routes[i].max_route++;
                    routes[i].coord.emplace_back(stod(tmp_coordinates.substr(0, 9)),
                                                 stod(tmp_coordinates.substr(10, 9)));
                }
                routes.insert(make_pair(i, buff_class));

                buff_class.clear();
            }
        }

        buff.clear();
    }
}

void smartPars(string &str) {
    for (int i = 0; i < str.size(); i++) {

    }
}

void ParsXmlStreet(map<string, int> &street) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("data.xml");

    if (!result) {
        cout << "The file can't be opened" << endl;
    }

    string tmp_street;

    for (pugi::xml_node tool = doc.first_child().first_child(); tool; tool = tool.next_sibling()) {
        // парсинг улицы ----------------------------------------------------
        tmp_street = (string) tool.child("location").child_value();

        int indexOfCommon;
        for (indexOfCommon = 0; indexOfCommon < tmp_street.size(); indexOfCommon++) {
            if (tmp_street[indexOfCommon] == ',')
                break;
        }

        if (indexOfCommon == tmp_street.size()) {
            auto its = street.find(tmp_street);
            if (!tmp_street.empty()) {
                if (its != street.end())
                    its->second++;
                else
                    street.insert(make_pair(tmp_street, 1));

            }
        } else {
            string tmp_street_first = tmp_street.substr(0, indexOfCommon);
            string tmp_street_second = tmp_street.substr(indexOfCommon + 1);

            auto its = street.find(tmp_street_first);
            if (!tmp_street_first.empty()) {
                if (its != street.end())
                    its->second++;
                else
                    street.insert(make_pair(tmp_street_first, 1));
            }


            if (!tmp_street_second.empty()) {
                its = street.find(tmp_street_second);
                if (its != street.end())
                    its->second++;
                else
                    street.insert(make_pair(tmp_street_second, 1));
            }

        }
    }
    // ------------------------------------------------------------------
}

void print_map(map<string, CStop> a) {
    for (auto it = a.begin(); it != a.end(); it++) {
        cout << "<" << it->second.view << "> ==" << it->first << "== " << it->second.max_route << "\n";
//        for (auto i:it->second.coord)
//            cout << i.first << " " << i.second << "\n";
    }
}

void route_with_max_stops(map<string, CStop> routes) {
    int max = 0;
    string key_max_rout;
    for (auto it = routes.begin(); it != routes.end(); it++) {
        if (it->second.max_route > max) {
            max = it->second.max_route;
            key_max_rout = it->first;
        }
    }
    cout << "\nМаршрут: <" << key_max_rout << "> Транспорт: " << routes[key_max_rout].view
         << " Координаты остановок и их количество: \n" << "*" << max + 1 << "*\n\n";
    for (int i = 0; i < routes[key_max_rout].coord.size(); i++)
        cout << "<" << i + 1 << "> (" << routes[key_max_rout].coord[i].first << ", "
             << routes[key_max_rout].coord[i].second << ") ";
    cout << "\n";
}


void longest_route(map<string, CStop> a) {
    for (auto it = a.begin(); it != a.end(); it++)
        it->second.long_route = it->second.primFindMST();
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

    map<string, CStop> routesForBus;
    map<string, CStop> routesForTrolleybus;
    map<string, CStop> routesForTram;

    string bus = "Автобус";
    string troll = "Троллейбус";
    string tram = "Трамвай";

    ParsXmlRoutes(routesForBus, bus);
    ParsXmlRoutes(routesForTram, tram);
    ParsXmlRoutes(routesForTrolleybus, troll);

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

    map<string, int> AllStreet;
    ParsXmlStreet(AllStreet);

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

    return 0;
}
