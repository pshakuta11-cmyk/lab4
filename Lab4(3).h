#pragma once
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;

class Service {
protected:
    int id;
    string name;
    double price;
    int duration;
    string type;

public:
    Service() : id(0), price(0), duration(0) {}
    Service(int i, string n, double p, int d, string t) : id(i), name(n), price(p), duration(d), type(t) {}
    virtual ~Service() {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getDuration() const { return duration; }
    string getType() const { return type; }

    void setId(int v) { id = v; }
    void setName(string v) { name = v; }
    void setPrice(double v) { price = v; }
    void setDuration(int v) { duration = v; }
    void setType(string v) { type = v; }

    bool operator<(const Service& o) const { return id < o.id; }
    bool operator==(const Service& o) const { return id == o.id; }

    virtual void printHeader() const;
    virtual void printRow() const;
    virtual void input();
};

struct ServiceHash {
    size_t operator()(const Service& s) const { return hash<int>()(s.getId()); }
};

class Salon {
public:
    map<int, Service> mapData;
    set<Service> setData;
    multimap<string, Service> mmData;
    multiset<Service> msData;
    unordered_map<int, Service> umapData;
    unordered_set<Service, ServiceHash> usetData;
    unordered_multimap<int, Service> ummData;
    unordered_multiset<Service, ServiceHash> umsData;

    void run();
};

int inputInt(string prompt);
double inputDouble(string prompt);
string toLower(string s);
string padRight(string str, int width);
string formatPrice(double price);
Service inputService();
void editService(Service& s);

void runLab4();