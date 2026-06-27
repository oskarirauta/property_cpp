#include "property.hpp"
#include <iostream>
#include <string>

// A small demo type. Its public interface is made of computed properties; each
// one wraps a private backing variable and adds get/set behaviour on top.
class Sensor {

	public:

		// 1. plain read/write property - default getter and setter, so reading
		//    and assigning go straight to the backing variable.
		Property<int> celsius { this->_celsius };

		// 2. computed read-only property - the getter derives the value from the
		//    backing variable; there is no setter, so assigning throws at run time.
		Property<int> fahrenheit {
			this->_celsius,
			[](const int& c) { return c * 9 / 5 + 32; },
			nullptr
		};

		// 3. read/write property with a validating setter - the setter clamps the
		//    stored value to 0..100.
		Property<int> level {
			this->_level,
			[](const int& v) { return v; },
			[](int& v, const int& n) { v = n < 0 ? 0 : ( n > 100 ? 100 : n ); }
		};

		// 4. a std::string property - shows the template works for non-int types.
		Property<std::string> name { this->_name };

		// 5. compile-time read-only property - assigning to it does not compile.
		Property<int>::ReadOnly id { this->_id };

		Sensor() : _celsius(20), _level(50), _name("sensor"), _id(42) {}

	private:
		int _celsius;
		int _level;
		std::string _name;
		int _id;
};

int main() {

	Sensor s;

	std::cout << "property_cpp - computed properties\n\n";
	std::cout << "celsius    : " << s.celsius    << "   (plain read/write)\n";
	std::cout << "fahrenheit : " << s.fahrenheit << "   (computed c*9/5+32, read-only)\n";
	std::cout << "level      : " << s.level      << "   (read/write, setter clamps 0..100)\n";
	std::cout << "name       : " << s.name       << "   (std::string property)\n";
	std::cout << "id         : " << s.id         << "   (compile-time read-only)\n\n";

	std::cout << "set celsius = 30, the computed fahrenheit follows:\n";
	s.celsius = 30;
	std::cout << "   celsius=" << s.celsius << " fahrenheit=" << s.fahrenheit << "\n\n";

	std::cout << "set level = 250, the setter clamps it:\n";
	s.level = 250;
	std::cout << "   level=" << s.level << "\n\n";

	std::cout << "set name = \"thermostat\":\n";
	s.name = std::string("thermostat");
	std::cout << "   name=" << s.name << "\n\n";

	std::cout << "assigning to the read-only fahrenheit throws at run time:\n";
	try {
		s.fahrenheit = 100;
	} catch ( const std::exception& e ) {
		std::cout << "   caught: " << e.what() << "\n";
	}

	// Assigning to the compile-time read-only 'id' would not compile:
	//    s.id = 7;   // error: use of deleted function 'operator='

	return 0;
}
