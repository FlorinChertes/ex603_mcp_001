#include <iostream>
#include <string>
#include <vector>
#include <variant>

template<class... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
};

class Wheel {
public:
    Wheel(const std::string& n) : name(n) { }
    std::string getName() const {
        return name;
    }
private:
    std::string name;
};

class Body {};

class Engine {};

class Car;

using CarElement = std::variant<Wheel, Body, Engine, Car>;

class Car {
public:
    Car(std::initializer_list<CarElement*> carElements) :
        elements{ carElements } {}

    template<typename T>
    void visitCarElements(T&& visitor) const {
        for (auto elem : elements) {
            std::visit(visitor, *elem);
        }
    }
private:
    std::vector<CarElement*> elements;
};

overloaded carElementPrintVisitor{                                            // (2)
    [](const Body& body) {  std::cout << "Visiting body" << '\n'; },
    [](this auto const& self, const Car& car) {  car.visitCarElements(self);  // (4)
                                                  std::cout << "Visiting car" << '\n'; },
    [](const Wheel& wheel) {  std::cout << "Visiting "
                                          << wheel.getName() << " wheel" << '\n'; },
    [](const Engine& engine) {  std::cout << "Visiting engine" << '\n'; }
};

overloaded carElementDoVisitor{                                               // (3)
    [](const Body& body) {  std::cout << "Moving my body" << '\n'; },
    [](this auto const& self, const Car& car) {  car.visitCarElements(self);   // (5)
                                                std::cout << "Starting my car" << '\n'; },
    [](const Wheel& wheel) {  std::cout << "Kicking my "
                                          << wheel.getName() << " wheel" << '\n'; },
    [](const Engine& engine) {  std::cout << "Starting my engine" << '\n'; }
};


void test_064()
{
    std::cout << "*** test 064 ***" << std::endl;
    std::cout << '\n';


    std::cout << '\n';

    CarElement wheelFrontLeft = Wheel("front left");
    CarElement wheelFrontRight = Wheel("front right");
    CarElement wheelBackLeft = Wheel("back left");
    CarElement wheelBackRight = Wheel("back right");
    CarElement body = Body{};
    CarElement engine = Engine{};

    CarElement car = Car{ &wheelFrontLeft, &wheelFrontRight,                    // (1)
             &wheelBackLeft, &wheelBackRight,
             &body, &engine };

    std::visit(carElementPrintVisitor, engine);
    std::visit(carElementPrintVisitor, car);
    std::cout << '\n';

    std::visit(carElementDoVisitor, engine);
    std::visit(carElementDoVisitor, car);
    std::cout << '\n';

}


template <typename actual_type>
class CRTPExample {
public:
    void placeOrder() {
        static_cast<actual_type*>(this)->actualPlaceOrder();
    }
    void actualPlaceOrder() {
        std::cout << "CRTPExample::actualPlaceOrder()\n";
    }
};

class SpecificCRTPExample : public CRTPExample<SpecificCRTPExample> {
public:
    void actualPlaceOrder() {
        std::cout << "SpecificCRTPExample::actualPlaceOrder()\n";
    }
};

void test_CRTP_064_01()
{
    std::cout << "*** test CRTP 064 01 ***" << std::endl;
    std::cout << '\n';

    CRTPExample<SpecificCRTPExample>* crtp_example = new SpecificCRTPExample;
    crtp_example->placeOrder();

    delete crtp_example;
    crtp_example = nullptr;

}