#include "Lab4(3).h"
#include <sstream>

int inputInt(string prompt) {
    int value;
    while (true) {
        cout << prompt; cin >> value;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); cout << "Ошибка! Введите целое число.\n"; continue; }
        cin.ignore(1000, '\n'); return value;
    }
}

double inputDouble(string prompt) {
    double value;
    while (true) {
        cout << prompt; cin >> value;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); cout << "Ошибка! Введите число.\n"; continue; }
        cin.ignore(1000, '\n'); return value;
    }
}

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
    return s;
}

string padRight(string str, int width) {
    int visible_length = 0;
    for (char c : str) if ((c & 0xC0) != 0x80) visible_length++;
    if (visible_length < width) str.append(width - visible_length, ' ');
    return str;
}

string formatPrice(double price) {
    stringstream ss;
    if (price == (int)price) ss << (int)price;
    else ss << fixed << setprecision(2) << price;
    ss << "р";
    return ss.str();
}

void Service::printHeader() const {
    cout << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << "Тип\n";
    cout << string(85, '-') << "\n";
}

void Service::printRow() const {
    cout << padRight(to_string(id), 5) << padRight(name, 30) << padRight(formatPrice(price), 12) << padRight(to_string(duration) + " мин", 12) << type << "\n";
}

void Service::input() {
    id = inputInt("ID: ");
    cout << "Название процедуры: "; getline(cin, name);
    price = inputDouble("Цена (руб): ");
    duration = inputInt("Длительность (мин): ");
    cout << "Тип процедуры: "; getline(cin, type);
}

Service inputService() {
    Service s;
    s.input();
    return s;
}

void editService(Service& s) {
    cout << "\nЧто изменить?\n";
    cout << "1. Название\n2. Цена\n3. Длительность\n4. Тип\n5. ВСЕ ПОЛЯ\n";
    int ch = inputInt("Выбор: ");

    switch (ch) {
    case 1: {
        string n;
        cout << "Новое название: ";
        getline(cin, n);
        s.setName(n);
        break;
    }
    case 2: {
        double p = inputDouble("Новая цена: ");
        s.setPrice(p);
        break;
    }
    case 3: {
        int d = inputInt("Новая длительность: ");
        s.setDuration(d);
        break;
    }
    case 4: {
        string t;
        cout << "Новый тип: ";
        getline(cin, t);
        s.setType(t);
        break;
    }
    case 5: {
        s.input();
        break;
    }
    default:
        cout << "Неверный выбор\n";
    }
}

void Salon::run() {
    int choice;
    do {
        cout << "\n--- ЛР4 (Вариант 13: Косметический салон) ---\n";
        cout << "1. Map (ID -> Услуга)\n";
        cout << "2. Set (Уникальные услуги)\n";
        cout << "3. Multimap (Тип процедуры -> Услуга)\n";
        cout << "4. Multiset (Услуги)\n";
        cout << "5. Unordered_map (ID -> Услуга)\n";
        cout << "6. Unordered_set (Услуги)\n";
        cout << "7. Unordered_multimap (Длительность -> Услуга)\n";
        cout << "8. Unordered_multiset (Услуги)\n";
        cout << "0. Выход\n";
        choice = inputInt("Выбор контейнера: ");

        if (choice == 0) break;

        int act;
        do {
            cout << "\n1. Добавить\n2. Показать\n3. Удалить\n4. Изменить\n5. Поиск (по названию)\n6. Сортировать (по цене)\n7. В файл\n0. Назад\n";
            act = inputInt("Выбор действия: ");
            if (act == 0) break;

            // ================= MAP =================
            if (choice == 1) {
                if (act == 1) {
                    Service s = inputService();
                    if (mapData.find(s.getId()) != mapData.end()) {
                        cout << "Услуга с таким ID уже существует\n";
                    }
                    else {
                        mapData[s.getId()] = s;
                        cout << "Услуга добавлена\n";
                    }
                }
                else if (act == 2) {
                    if (mapData.empty()) cout << "Список пуст\n";
                    else { mapData.begin()->second.printHeader(); for (auto& p : mapData) p.second.printRow(); }
                }
                else if (act == 3) {
                    int id = inputInt("ID: ");
                    if (mapData.erase(id)) cout << "Услуга удалена\n";
                    else cout << "Услуга с таким ID не найдена\n";
                }
                else if (act == 4) {
                    int id = inputInt("ID: ");
                    auto it = mapData.find(id);
                    if (it != mapData.end()) { editService(it->second); cout << "Данные изменены\n"; }
                    else cout << "Услуга с таким ID не найдена\n";
                }
                else if (act == 5) {
                    string n; cout << "Название для поиска: "; getline(cin, n);
                    string lt = toLower(n); bool found = false;
                    for (auto& p : mapData) {
                        if (toLower(p.second.getName()) == lt) {
                            if (!found) p.second.printHeader();
                            p.second.printRow(); found = true;
                        }
                    }
                    if (!found) cout << "Не найдено\n";
                }
                else if (act == 6) {
                    vector<Service> v; for (auto& p : mapData) v.push_back(p.second);
                    sort(v.begin(), v.end(), [](const Service& a, const Service& b) { return a.getPrice() > b.getPrice(); });
                    if (!v.empty()) { v[0].printHeader(); for (auto& s : v) s.printRow(); }
                    else cout << "Список пуст\n";
                }
                else if (act == 7) {
                    ofstream f("map.txt");
                    f << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << "Тип\n" << string(85, '-') << "\n";
                    for (auto& p : mapData) f << padRight(to_string(p.second.getId()), 5) << padRight(p.second.getName(), 30) << padRight(formatPrice(p.second.getPrice()), 12) << padRight(to_string(p.second.getDuration()) + " мин", 12) << p.second.getType() << "\n";
                    cout << "Данные сохранены в файл map.txt\n";
                }
            }

            // ================= SET =================
            else if (choice == 2) {
                if (act == 1) {
                    Service s = inputService();
                    if (setData.find(s) != setData.end()) cout << "Такая услуга уже существует\n";
                    else { setData.insert(s); cout << "Услуга добавлена\n"; }
                }
                else if (act == 2) {
                    if (setData.empty()) cout << "Список пуст\n";
                    else { setData.begin()->printHeader(); for (auto& s : setData) s.printRow(); }
                }
                else if (act == 3) {
                    int id = inputInt("ID: ");
                    for (auto it = setData.begin(); it != setData.end(); ++it) if (it->getId() == id) { setData.erase(it); cout << "Услуга удалена\n"; break; }
                }
                else if (act == 4) {
                    int id = inputInt("ID: ");
                    for (auto it = setData.begin(); it != setData.end(); ++it) {
                        if (it->getId() == id) {
                            Service s = *it; setData.erase(it); editService(s); setData.insert(s);
                            cout << "Данные изменены\n"; break;
                        }
                    }
                }
                else if (act == 5) {
                    string n; cout << "Название для поиска: "; getline(cin, n);
                    string lt = toLower(n); bool found = false;
                    for (auto& s : setData) {
                        if (toLower(s.getName()) == lt) {
                            if (!found) s.printHeader();
                            s.printRow(); found = true;
                        }
                    }
                    if (!found) cout << "Не найдено\n";
                }
                else if (act == 6) {
                    vector<Service> v(setData.begin(), setData.end());
                    sort(v.begin(), v.end(), [](const Service& a, const Service& b) { return a.getPrice() > b.getPrice(); });
                    if (!v.empty()) { v[0].printHeader(); for (auto& s : v) s.printRow(); }
                    else cout << "Список пуст\n";
                }
                else if (act == 7) {
                    ofstream f("set.txt");
                    f << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << "Тип\n" << string(85, '-') << "\n";
                    for (auto& s : setData) f << padRight(to_string(s.getId()), 5) << padRight(s.getName(), 30) << padRight(formatPrice(s.getPrice()), 12) << padRight(to_string(s.getDuration()) + " мин", 12) << s.getType() << "\n";
                    cout << "Данные сохранены в файл set.txt\n";
                }
            }

            // ================= MULTIMAP =================
            else if (choice == 3) {
                if (act == 1) {
                    string t; cout << "Тип процедуры (ключ): "; getline(cin, t);
                    Service s = inputService();
                    s.setType(t);
                    mmData.insert({ t, s });
                    cout << "Услуга добавлена\n";
                }
                else if (act == 2) {
                    if (mmData.empty()) cout << "Список пуст\n";
                    else { mmData.begin()->second.printHeader(); for (auto& p : mmData) p.second.printRow(); }
                }
                else if (act == 3) {
                    string t; cout << "Тип процедуры: "; getline(cin, t);
                    auto range = mmData.equal_range(t);
                    if (range.first != range.second) { mmData.erase(range.first, range.second); cout << "Услуги удалены\n"; }
                    else cout << "Не найдено\n";
                }
                else if (act == 4) {
                    int id = inputInt("ID: ");
                    for (auto& p : mmData) {
                        if (p.second.getId() == id) { editService(p.second); cout << "Данные изменены\n"; break; }
                    }
                }
                else if (act == 5) {
                    string n; cout << "Название для поиска: "; getline(cin, n);
                    string lt = toLower(n); bool found = false;
                    for (auto& p : mmData) {
                        if (toLower(p.second.getName()) == lt) {
                            if (!found) p.second.printHeader();
                            p.second.printRow(); found = true;
                        }
                    }
                    if (!found) cout << "Не найдено\n";
                }
                else if (act == 6) {
                    vector<Service> v; for (auto& p : mmData) v.push_back(p.second);
                    sort(v.begin(), v.end(), [](const Service& a, const Service& b) { return a.getPrice() > b.getPrice(); });
                    if (!v.empty()) { v[0].printHeader(); for (auto& s : v) s.printRow(); }
                    else cout << "Список пуст\n";
                }
                else if (act == 7) {
                    ofstream f("multimap.txt");
                    f << padRight("Тип процедуры", 20) << padRight("ID", 5) << padRight("Название", 30) << padRight("Цена", 12) << "Длит.\n" << string(85, '-') << "\n";
                    for (auto& p : mmData) f << padRight(p.first, 20) << padRight(to_string(p.second.getId()), 5) << padRight(p.second.getName(), 30) << padRight(formatPrice(p.second.getPrice()), 12) << p.second.getDuration() << " мин\n";
                    cout << "Данные сохранены в файл multimap.txt\n";
                }
            }

            // ================= MULTISET =================
            else if (choice == 4) {
                if (act == 1) { Service s = inputService(); msData.insert(s); cout << "Услуга добавлена\n"; }
                else if (act == 2) {
                    if (msData.empty()) cout << "Список пуст\n";
                    else { msData.begin()->printHeader(); for (auto& s : msData) s.printRow(); }
                }
                else if (act == 3) {
                    int id = inputInt("ID: ");
                    for (auto it = msData.begin(); it != msData.end(); ++it) if (it->getId() == id) { msData.erase(it); cout << "Услуга удалена\n"; break; }
                }
                else if (act == 4) {
                    int id = inputInt("ID: ");
                    for (auto it = msData.begin(); it != msData.end(); ++it) {
                        if (it->getId() == id) {
                            Service s = *it; msData.erase(it); editService(s); msData.insert(s);
                            cout << "Данные изменены\n"; break;
                        }
                    }
                }
                else if (act == 5) {
                    string n; cout << "Название для поиска: "; getline(cin, n);
                    string lt = toLower(n); bool found = false;
                    for (auto& s : msData) {
                        if (toLower(s.getName()) == lt) {
                            if (!found) s.printHeader();
                            s.printRow(); found = true;
                        }
                    }
                    if (!found) cout << "Не найдено\n";
                }
                else if (act == 6) {
                    vector<Service> v(msData.begin(), msData.end());
                    sort(v.begin(), v.end(), [](const Service& a, const Service& b) { return a.getPrice() > b.getPrice(); });
                    if (!v.empty()) { v[0].printHeader(); for (auto& s : v) s.printRow(); }
                    else cout << "Список пуст\n";
                }
                else if (act == 7) {
                    ofstream f("multiset.txt");
                    f << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << "Тип\n" << string(85, '-') << "\n";
                    for (auto& s : msData) f << padRight(to_string(s.getId()), 5) << padRight(s.getName(), 30) << padRight(formatPrice(s.getPrice()), 12) << padRight(to_string(s.getDuration()) + " мин", 12) << s.getType() << "\n";
                    cout << "Данные сохранены в файл multiset.txt\n";
                }
            }

            // ================= UNORDERED_MAP =================
            else if (choice == 5) {
                if (act == 1) {
                    Service s = inputService();
                    if (umapData.find(s.getId()) != umapData.end()) cout << "Услуга с таким ID уже существует\n";
                    else { umapData[s.getId()] = s; cout << "Услуга добавлена\n"; }
                }
                else if (act == 2) {
                    if (umapData.empty()) cout << "Список пуст\n";
                    else { umapData.begin()->second.printHeader(); for (auto& p : umapData) p.second.printRow(); }
                }
                else if (act == 3) {
                    int id = inputInt("ID: ");
                    if (umapData.erase(id)) cout << "Услуга удалена\n"; else cout << "Услуга с таким ID не найдена\n";
                }
                else if (act == 4) {
                    int id = inputInt("ID: ");
                    auto it = umapData.find(id);
                    if (it != umapData.end()) { editService(it->second); cout << "Данные изменены\n"; }
                    else cout << "Услуга с таким ID не найдена\n";
                }
                else if (act == 5) {
                    string n; cout << "Название для поиска: "; getline(cin, n);
                    string lt = toLower(n); bool found = false;
                    for (auto& p : umapData) {
                        if (toLower(p.second.getName()) == lt) {
                            if (!found) p.second.printHeader();
                            p.second.printRow(); found = true;
                        }
                    }
                    if (!found) cout << "Не найдено\n";
                }
                else if (act == 6) {
                    vector<Service> v; for (auto& p : umapData) v.push_back(p.second);
                    sort(v.begin(), v.end(), [](const Service& a, const Service& b) { return a.getPrice() > b.getPrice(); });
                    if (!v.empty()) { v[0].printHeader(); for (auto& s : v) s.printRow(); }
                    else cout << "Список пуст\n";
                }
                else if (act == 7) {
                    ofstream f("umap.txt");
                    f << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << "Тип\n" << string(85, '-') << "\n";
                    for (auto& p : umapData) f << padRight(to_string(p.second.getId()), 5) << padRight(p.second.getName(), 30) << padRight(formatPrice(p.second.getPrice()), 12) << padRight(to_string(p.second.getDuration()) + " мин", 12) << p.second.getType() << "\n";
                    cout << "Данные сохранены в файл umap.txt\n";
                }
            }

            // ================= UNORDERED_SET =================
            else if (choice == 6) {
                if (act == 1) {
                    Service s = inputService();
                    if (usetData.find(s) != usetData.end()) cout << "Такая услуга уже существует\n";
                    else { usetData.insert(s); cout << "Услуга добавлена\n"; }
                }
                else if (act == 2) {
                    if (usetData.empty()) cout << "Список пуст\n";
                    else { usetData.begin()->printHeader(); for (auto& s : usetData) s.printRow(); }
                }
                else if (act == 3) {
                    int id = inputInt("ID: ");
                    for (auto it = usetData.begin(); it != usetData.end(); ++it) if (it->getId() == id) { usetData.erase(it); cout << "Услуга удалена\n"; break; }
                }
                else if (act == 4) {
                    int id = inputInt("ID: ");
                    for (auto it = usetData.begin(); it != usetData.end(); ++it) {
                        if (it->getId() == id) {
                            Service s = *it; usetData.erase(it); editService(s); usetData.insert(s);
                            cout << "Данные изменены\n"; break;
                        }
                    }
                }
                else if (act == 5) {
                    string n; cout << "Название для поиска: "; getline(cin, n);
                    string lt = toLower(n); bool found = false;
                    for (auto& s : usetData) {
                        if (toLower(s.getName()) == lt) {
                            if (!found) s.printHeader();
                            s.printRow(); found = true;
                        }
                    }
                    if (!found) cout << "Не найдено\n";
                }
                else if (act == 6) {
                    vector<Service> v(usetData.begin(), usetData.end());
                    sort(v.begin(), v.end(), [](const Service& a, const Service& b) { return a.getPrice() > b.getPrice(); });
                    if (!v.empty()) { v[0].printHeader(); for (auto& s : v) s.printRow(); }
                    else cout << "Список пуст\n";
                }
                else if (act == 7) {
                    ofstream f("uset.txt");
                    f << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << "Тип\n" << string(85, '-') << "\n";
                    for (auto& s : usetData) f << padRight(to_string(s.getId()), 5) << padRight(s.getName(), 30) << padRight(formatPrice(s.getPrice()), 12) << padRight(to_string(s.getDuration()) + " мин", 12) << s.getType() << "\n";
                    cout << "Данные сохранены в файл uset.txt\n";
                }
            }

            // ================= UNORDERED_MULTIMAP =================
            else if (choice == 7) {
                if (act == 1) {
                    int d = inputInt("Длительность (ключ): ");
                    Service s = inputService();
                    ummData.insert({ d, s });
                    cout << "Услуга добавлена\n";
                }
                else if (act == 2) {
                    if (ummData.empty()) cout << "Список пуст\n";
                    else { ummData.begin()->second.printHeader(); for (auto& p : ummData) p.second.printRow(); }
                }
                else if (act == 3) {
                    int d = inputInt("Длительность: ");
                    auto range = ummData.equal_range(d);
                    if (range.first != range.second) { ummData.erase(range.first, range.second); cout << "Услуги удалены\n"; }
                    else cout << "Не найдено\n";
                }
                else if (act == 4) {
                    int id = inputInt("ID: ");
                    for (auto& p : ummData) {
                        if (p.second.getId() == id) { editService(p.second); cout << "Данные изменены\n"; break; }
                    }
                }
                else if (act == 5) {
                    string n; cout << "Название для поиска: "; getline(cin, n);
                    string lt = toLower(n); bool found = false;
                    for (auto& p : ummData) {
                        if (toLower(p.second.getName()) == lt) {
                            if (!found) p.second.printHeader();
                            p.second.printRow(); found = true;
                        }
                    }
                    if (!found) cout << "Не найдено\n";
                }
                else if (act == 6) {
                    vector<Service> v; for (auto& p : ummData) v.push_back(p.second);
                    sort(v.begin(), v.end(), [](const Service& a, const Service& b) { return a.getPrice() > b.getPrice(); });
                    if (!v.empty()) { v[0].printHeader(); for (auto& s : v) s.printRow(); }
                    else cout << "Список пуст\n";
                }
                else if (act == 7) {
                    ofstream f("umm.txt");
                    f << padRight("Длит.", 10) << padRight("ID", 5) << padRight("Название", 30) << padRight("Цена", 12) << "\n" << string(70, '-') << "\n";
                    for (auto& p : ummData) f << padRight(to_string(p.first) + " мин", 10) << padRight(to_string(p.second.getId()), 5) << padRight(p.second.getName(), 30) << padRight(formatPrice(p.second.getPrice()), 12) << "\n";
                    cout << "Данные сохранены в файл umm.txt\n";
                }
            }

            // ================= UNORDERED_MULTISET =================
            else if (choice == 8) {
                if (act == 1) { Service s = inputService(); umsData.insert(s); cout << "Услуга добавлена\n"; }
                else if (act == 2) {
                    if (umsData.empty()) cout << "Список пуст\n";
                    else { umsData.begin()->printHeader(); for (auto& s : umsData) s.printRow(); }
                }
                else if (act == 3) {
                    int id = inputInt("ID: ");
                    for (auto it = umsData.begin(); it != umsData.end(); ++it) if (it->getId() == id) { umsData.erase(it); cout << "Услуга удалена\n"; break; }
                }
                else if (act == 4) {
                    int id = inputInt("ID: ");
                    for (auto it = umsData.begin(); it != umsData.end(); ++it) {
                        if (it->getId() == id) {
                            Service s = *it; umsData.erase(it); editService(s); umsData.insert(s);
                            cout << "Данные изменены\n"; break;
                        }
                    }
                }
                else if (act == 5) {
                    string n; cout << "Название для поиска: "; getline(cin, n);
                    string lt = toLower(n); bool found = false;
                    for (auto& s : umsData) {
                        if (toLower(s.getName()) == lt) {
                            if (!found) s.printHeader();
                            s.printRow(); found = true;
                        }
                    }
                    if (!found) cout << "Не найдено\n";
                }
                else if (act == 6) {
                    vector<Service> v(umsData.begin(), umsData.end());
                    sort(v.begin(), v.end(), [](const Service& a, const Service& b) { return a.getPrice() > b.getPrice(); });
                    if (!v.empty()) { v[0].printHeader(); for (auto& s : v) s.printRow(); }
                    else cout << "Список пуст\n";
                }
                else if (act == 7) {
                    ofstream f("ums.txt");
                    f << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << "Тип\n" << string(85, '-') << "\n";
                    for (auto& s : umsData) f << padRight(to_string(s.getId()), 5) << padRight(s.getName(), 30) << padRight(formatPrice(s.getPrice()), 12) << padRight(to_string(s.getDuration()) + " мин", 12) << s.getType() << "\n";
                    cout << "Данные сохранены в файл ums.txt\n";
                }
            }
        } while (act != 0);
    } while (choice != 0);
}

void runLab4() {
    Salon s;
    s.run();
}