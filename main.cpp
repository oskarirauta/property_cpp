#include "property.hpp"
#include <iostream>

const int _value1 = 0;
const int _value2 = 100;
const int _value3 = 200;
const int _value4 = 250;

const int _value5 = 500;

class TestClass {

	public:

		// Default property with default getter and setter
		Property<int> dynamicDefault { this -> value1 };

		// Getter of dynamicReadWrite property returns always value1 + 1.
		Property<int> dynamicReadWrite {
				this -> value1,
				[&](int &r) { return r + 1; },
				[&](int &r, int t) { r = t; },
		};

		// dynamicReadOnly does not have setter at all.
		Property<int> dynamicReadOnly {
				this -> value2,
				[&](int &r) { return r; },
				nullptr
		};

		// staticReadOnly returns value3, but does not allow setting.
		Property<int>::ReadOnly staticReadOnly { this -> value3 };

		Property<int> dynamicCalculated {
				this -> value2, // not used but must be introduced..
				[&](int &r) { return this -> value5 == 500 ? 9 : 0; },
				nullptr
		};

		TestClass() {
			this -> value1 = _value1;
			this -> value2 = _value2;
			this -> value3 = _value3;
			this -> value4 = _value4;
			this -> value5 = _value5;
		}

	private:
		int value1, value2, value3, value4, value5;

};

int main() {

	TestClass test;

	std::cout << "Property test\n\n" <<
		"Properties:\n" <<
		"   value1 = dynamicDefault ( default getter and setter ), initial: " <<
		_value1 << "\n" <<
		"   value2 = dynamicReadWrite ( getter returns actual value + 1 ), initial: " <<
		_value2 << "\n" <<
		"   value3 = dynamicReadOnly ( getter returns actual value, no setter), initial: " <<
		_value3 << "\n" <<
		"   value4 = staticReadOnly ( static getter only ), initial: " <<
		_value4 << "\n" <<
		"   value5 = dynamicCalculated ( calculated property ), value used in calculation: " <<
		_value5 << "\n" << std::endl;

	std::cout << "value1: " << test.dynamicDefault << std::endl;
	std::cout << "value2: " << test.dynamicReadWrite << std::endl;
	std::cout << "value3: " << test.dynamicReadOnly << std::endl;
	std::cout << "value4: " << test.staticReadOnly << std::endl;
	std::cout << "value5: " << test.dynamicCalculated << std::endl;

	std::cout << "\nSetting value1 to integer value " << _value1 + 14  << "." << std::endl;
	test.dynamicDefault = _value1 + 14;
	std::cout << "result= value1: " << test.dynamicDefault << std::endl;

	std::cout << "\nSetting value2 to integer value " << _value2 + 14 << "." << std::endl;
	test.dynamicReadWrite = _value2 + 14;
	std::cout << "result= value2: " << test.dynamicReadWrite <<
		" ( actual value + 1 )" << std::endl;

	std::cout << "\nSetting value3 to integer value " << _value3 + 14 << ". Results assert." << std::endl;
	test.dynamicReadOnly = _value3 + 14;
	std::cout << "result= value3:  " << test.dynamicReadOnly << std::endl;

	/*
	std::cout << "\nSetting value4 to integer value " << _value4 + 14 << ". Results assert." << std::endl;
	test.staticReadOnly = _value4 + 14;
	std::cout << "result= value3: " << test.staticReadOnly << std::endl;
	*/

	return 0;

}
