[![License:MIT](https://img.shields.io/badge/License-MIT-blue?style=plastic)](LICENSE)
[![C++ CI build](../../actions/workflows/build.yml/badge.svg)](../../actions/workflows/build.yml)
### property_cpp

Swift-style computed properties for C++ - a small, header-only study project.

A `Property<T>` wraps a backing variable and exposes it through an optional
getter and setter. It reads like a plain value (implicit conversion to `T`) and
is assigned to like one, but every read can run a getter and every write a
setter - so you get computed, validated or read-only members with ordinary
member syntax.

### <sub>Note</sub>

Header only - just `#include "property.hpp"`, no library to link. This is mostly
a learning project: there is little it does that plain accessor methods would
not, but it is a compact, self-contained demonstration of the idea.

### <sub>Property kinds</sub>

```cpp
#include "property.hpp"

class Sensor {

	public:

		// plain read/write (default getter and setter)
		Property<int> celsius { this->_celsius };

		// computed, read-only (a getter, no setter -> throws on assignment)
		Property<int> fahrenheit {
			this->_celsius,
			[](const int& c) { return c * 9 / 5 + 32; },
			nullptr
		};

		// read/write with a validating setter
		Property<int> level {
			this->_level,
			[](const int& v) { return v; },
			[](int& v, const int& n) { v = n < 0 ? 0 : ( n > 100 ? 100 : n ); }
		};

		// read-only enforced at compile time (assignment does not compile)
		Property<int>::ReadOnly id { this->_id };

		Sensor() : _celsius(20), _level(50), _id(42) {}

	private:
		int _celsius, _level, _id;
};

Sensor s;
int  c = s.celsius;     // read     -> 20
s.celsius = 30;         // write    -> setter stores 30
int  f = s.fahrenheit;  // computed -> 86
s.level = 250;          // setter clamps -> 100
// s.id = 7;            // compile error: read-only
```

A property without a setter throws `std::runtime_error("property is read-only")`
on assignment, while `Property<T>::ReadOnly` rejects assignment already at
compile time. `Property<T>` works for any type, `int` and `std::string` alike.

### <sub>Examples</sub>

See [main.cpp](main.cpp) for a runnable demonstration of every property kind.
