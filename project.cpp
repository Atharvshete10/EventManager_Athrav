#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <ctime>
using namespace std;

struct Event {
    int id;
    string name, date, time, type, location;
};

class EventManager {
private:
    vector<Event> events;
    int nextId;
    bool isAdmin;

public:
    EventManager() {
        nextId = 1;
        isAdmin = false;
        loadEvents();
    }

    void saveEvents() {
        ofstream out("events.txt");
        for (auto &e : events) {
            out << e.id << " " << e.name << " " << e.date << " " << e.time
                << " " << e.type << " " << e.location << "\n";
        }
    }

    void loadEvents() {
        ifstream in("events.txt");
        Event e;
        while (in >> e.id >> e.name >> e.date >> e.time >> e.type >> e.location) {
            events.push_back(e);
            nextId = max(nextId, e.id + 1);
        }
    }

    bool conflict(string date, string time) {
        for (auto &e : events) {
            if (e.date == date && e.time == time) return true;
        }
        return false;
    }

    void adminLogin() {
        string pass;
        cout << "Enter Admin Password: ";
        cin >> pass;
        if (pass == "atharv10") {
            isAdmin = true;
            cout << "Admin login successful.\n";
        } else {
            cout << "Wrong password.\n";
        }
    }

    void addEvent() {
        if (!isAdmin) {
            cout << "Only Admin can add events.\n";
            return; 
        }
        Event e;
        e.id = nextId++;
        cout << "Enter Name: "; cin >> e.name;
        cout << "Enter Date (DD-MM-YYYY): "; cin >> e.date;
        cout << "Enter Time (HH:MM): "; cin >> e.time;
        cout << "Enter Type: "; cin >> e.type;
        cout << "Enter Location: "; cin >> e.location;

        if (conflict(e.date, e.time)) {
            cout << "Conflict detected! Another event exists at this time.\n";
            return;
        }
        events.push_back(e);
        saveEvents();
        cout << "Event added successfully.\n";
    }

    void viewEvents() {
        for (auto &e : events) {
            cout << e.id << ". " << e.name << " | " << e.date << " " << e.time
                 << " | " << e.type << " | " << e.location << "\n";
        }
    }

    void deleteEvent() {
        if (!isAdmin) { cout << "Only Admin can delete events.\n";
            return;
        }
        int id; cout << "Enter Event ID to delete: "; cin >> id;
        auto it = remove_if(events.begin(), events.end(), [&](Event &e){ return e.id == id; });
        if (it != events.end()) {
            events.erase(it, events.end());
            saveEvents();
            cout << "Event deleted successfully.\n";
        } else {
            cout << "Event not found.\n";
        }
    }

    void editEvent() {
        if (!isAdmin) {
            cout << "Only Admin can edit events.\n";
            return; 
        }
        int id; cout << "Enter Event ID to edit: "; cin >> id;
        for (auto &e : events) {
            if (e.id == id) {
                cout << "Editing Event: " << e.name << "\n";
                cout << "Enter new Name: "; cin >> e.name;
                cout << "Enter new Date (DD-MM-YYYY): "; cin >> e.date;
                cout << "Enter new Time (HH:MM): "; cin >> e.time;
                cout << "Enter new Type: "; cin >> e.type;
                cout << "Enter new Location: "; cin >> e.location;
                saveEvents();
                cout << "Event updated successfully.\n";
                return;
            }
        }
        cout << "Event not found.\n";
    }

    void searchEvents() {
        string key;
        cout << "Enter keyword to search (name/type): ";
        cin >> key;
        for (auto &e : events) {
            if (e.name.find(key) != string::npos || e.type.find(key) != string::npos) {
                cout << e.id << ". " << e.name << " | " << e.date << " " << e.time
                     << " | " << e.type << " | " << e.location << "\n";
            }
        }
    }

    void sendReminders() {
        if (!isAdmin) { 
            cout << "Only Admin can send reminders.\n"; 
            return; 
        }

        ifstream infile("attendees.csv");
        if (!infile) {
            cout << "attendees.csv not found! Create a CSV file with emails.\n";
            return;
        }

        cout << "Sending reminders for upcoming events...\n";
        string email;
        while (getline(infile, email)) {
            if (!email.empty()) {
                cout << "Reminder sent to: " << email << "\n";
                // In real system: use libcurl / SMTP to actually send mail
            }
        }
    }
};

int main() {
    EventManager manager;
    int choice;
    do {
        cout << "\n--- Smart Event Manager ---\n";
        cout << "1. Admin Login\n";
        cout << "2. Add Event (Admin)\n";
        cout << "3. View All Events\n";
        cout << "4. Delete Event (Admin)\n";
        cout << "5. Edit Event (Admin)\n";
        cout << "6. Search Events\n";
        cout << "7. Send Event Reminders (Admin)\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: manager.adminLogin(); break;
            case 2: manager.addEvent(); break;
            case 3: manager.viewEvents(); break;
            case 4: manager.deleteEvent(); break;
            case 5: manager.editEvent(); break;
            case 6: manager.searchEvents(); break;
            case 7: manager.sendReminders(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while(choice != 0);

    return 0;
}
