
#include <iostream>

#include <numeric>
#include <iterator>
#include <algorithm>
#include <queue>
#include <map>
#include <vector>
#include <string>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
 class Product {
 public:
	 Product() {}

	 explicit Product(int id, const std::string& name)
	 : id_{id}
	 , name_{name}
	 {
		std::cout << "Product(): " << id_ << ", " << name_ << '\n';
	 }

	 explicit Product(const std::string& name)
		 : Product{ 0, name }
	 {
	 }

	 // copy constructor
	 Product(const Product& other)
	 : id_{other.id_}
	 , name_{other.name_}
	 {
		 std::cout << "Product(copy): " << id_ << ", " << name_ << '\n';
	 }

	 Product(Product&& other) noexcept
		 : id_{ other.id_ }
		 , name_{ std::move(other.name_) }
	 {
		 std::cout << "Product(move): " << id_ << ", " << name_ << '\n';
	 }

	 Product& operator=(const Product& other)
	 {
		 if (this == &other)
			 return *this;

		 id_ = other.id_;
		 name_ = other.name_;

		 std::cout << "operator=(copy): " << id_ << ", " << name_ << '\n';
		 return *this;
	 }

	 Product& operator=(Product&& other) noexcept
	 {
		 id_ = other.id_;
		 name_ = std::move(other.name_);

		 std::cout << "operator=(move): " << id_ << ", " << name_ << '\n';
		 return *this;
	 }

	 int Id() const { return id_; }
	 const std::string& name() const { return name_; }

  private:
	  int id_ {-1};
	  std::string name_ {"none"};
 };

 void foo(Product )
 {
	 std::cout << "inside foo()\n";
 }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
 struct MyType0 {
	 int x;
 };
 struct MyType1 {
	 MyType1() {}
	 int x {};
 };
 struct MyType2 {
	 MyType2() = default;
	 int x;
 };
 struct MyType3 {
	 MyType3();
	 int x {};
 };
 MyType3::MyType3() = default;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
 constexpr double ToWattsRatio{ 745.699872 };
 class HorsePower;
 class WattPower {
 public:
	 WattPower() = default;
	 explicit WattPower(double p) : power_{ p } {}
	 explicit WattPower(const HorsePower& h);
	 double getValue() const { return power_; }
 private:
	 double power_{ 0. };
 };
 class HorsePower {
 public:
	 HorsePower() = default;
	 explicit HorsePower(double p) : power_{ p } {}
	 explicit HorsePower(const WattPower& w);
	 double getValue() const { return power_; }
 private:
	 double power_{ 0. };
 };

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

