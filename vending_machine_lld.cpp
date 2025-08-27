#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

enum class ProductType { Soda, Water, Wafers };
enum class PaymentType { Cash, Digital };
enum class PaymentStatus { Failed, Successful };

class Product {
    int id;
    ProductType type;
    string name;
    int price;
    int quantity;
public:
    Product(int id, ProductType type, string name, int price, int qty)
        : id(id), type(type), name(name), price(price), quantity(qty) {}
    int getId() const { return id; }
    ProductType getType() const { return type; }
    string getName() const { return name; }
    int getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    void reduceQuantity() { if (quantity > 0) quantity--; }
};

class Inventory {
    unordered_map<int, Product> items;
public:
    void addProduct(const Product& p) { items.emplace(p.getId(), p); }
    Product* getProduct(int id) {
        auto it = items.find(id);
        if (it != items.end() && it->second.getQuantity() > 0) return &it->second;
        return nullptr;
    }
};

class Payment {
public:
    virtual PaymentStatus pay(int amount) = 0;
    virtual ~Payment() = default;
};

class CashPayment : public Payment {
public:
    PaymentStatus pay(int amount) override {
        cout << "Paid " << amount << " using Cash\n";
        return PaymentStatus::Successful;
    }
};

class DigitalPayment : public Payment {
public:
    PaymentStatus pay(int amount) override {
        cout << "Paid " << amount << " using Digital method\n";
        return PaymentStatus::Successful;
    }
};

class VendingMachine {
    Inventory inventory;
public:
    VendingMachine() {
        inventory.addProduct(Product(1, ProductType::Soda, "Soda", 50, 5));
        inventory.addProduct(Product(2, ProductType::Water, "Water", 30, 5));
        inventory.addProduct(Product(3, ProductType::Wafers, "Wafers", 40, 5));
    }
    void buyProduct(int id, Payment& payment) {
        Product* product = inventory.getProduct(id);
        if (!product) {
            cout << "Product not available!\n";
            return;
        }
        PaymentStatus status = payment.pay(product->getPrice());
        if (status == PaymentStatus::Successful) {
            product->reduceQuantity();
            cout << "Dispensed: " << product->getName() << "\n";
        } else {
            cout << "Payment failed!\n";
        }
    }
};

int main() {
    VendingMachine vm;
    CashPayment cash;
    DigitalPayment digital;
    vm.buyProduct(1, cash);
    vm.buyProduct(2, digital);
    vm.buyProduct(3, cash);
    return 0;
}
