#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <numeric>
#include <ctime>

using namespace std;

enum class VehicleType { Car, Bike };

class Vehicle {
    string plate;
    string type;
public:
    Vehicle(string p, string t) {
        this->plate = p;
        this->type = t;
    }
    string getPlate() { return plate; }
    string getType() { return type; }
};

class Car : public Vehicle {
public:
    Car(const string& p) : Vehicle(p, "Car") {}
};

class Bike : public Vehicle {
public:
    Bike(const string& p) : Vehicle(p, "Bike") {}
};

class ParkingSpot {
    int id;
    bool free;
    Vehicle* vehicle;
public:
    ParkingSpot(int id) {
        this->id = id;
        this->free = true;
        this->vehicle = nullptr;
    }
    bool isFree() { return free; }
    void Park(Vehicle* v) {
        vehicle = v;
        free = false;
    }
    void freeSpot() {
        vehicle = nullptr;
        free = true;
    }
    int getId() { return id; }
};

class Ticket {
    int id;
    Vehicle* vehicle;
    ParkingSpot* spot;
    time_t entrytime;
public:
    Ticket(int i, ParkingSpot* s, Vehicle* v) {
        this->id = i;
        this->spot = s;
        this->vehicle = v;
        entrytime = time(nullptr);
    }
    int getid() { return id; }
    time_t getentry() { return entrytime; }
    Vehicle* getvehicle() { return vehicle; }
    ParkingSpot* getspot() { return spot; }
};

class HourlyPricing {
public:
    int calculatefare(time_t entry, time_t exit) {
        int hours = max(1, int(difftime(exit, entry) / 3600));
        return hours * 60;
    }
};

class ParkingLot {
    vector<ParkingSpot> spots;
    vector<Ticket> parkingtickets;
    HourlyPricing pricing;
    int ticket_counter = 0;
public:
    ParkingLot(int n) {
        spots.reserve(n);
        for (int i = 1; i <= n; i++) {
            spots.emplace_back(i);
        }
        parkingtickets.reserve(n);
    }

    Ticket* ParkVehicle(Vehicle* v) {
        for (auto& s : spots) {
            if (s.isFree()) {
                s.Park(v);
                parkingtickets.emplace_back(++ticket_counter, &s, v);
                return &parkingtickets.back();
            }
        }
        return nullptr;
    }

    int UnparkVehicle(int ticketId) {
        for (auto it = parkingtickets.begin(); it != parkingtickets.end(); ++it) {
            if (it->getid() == ticketId) {
                time_t exitTime = time(nullptr);
                int fare = pricing.calculatefare(it->getentry(), exitTime);
                it->getspot()->freeSpot();
                parkingtickets.erase(it);
                return fare;
            }
        }
        return -1;
    }
};

int main() {
    ParkingLot lot(6);

    Car c1("ABC123");
    Bike b1("XYZ789");

    Ticket* t1 = lot.ParkVehicle(&c1);
    if (t1) cout << "Car parked, Ticket ID: " << t1->getid() << endl;

    Ticket* t2 = lot.ParkVehicle(&b1);
    if (t2) cout << "Bike parked, Ticket ID: " << t2->getid() << endl;


    int fare1 = lot.UnparkVehicle(t1->getid());
    cout << "Car unparked, Fare: " << fare1 << endl;

    int fare2 = lot.UnparkVehicle(t2->getid());
    cout << "Bike unparked, Fare: " << fare2 << endl;

    return 0;
}
