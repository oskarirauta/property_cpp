#pragma once

#include <functional>
#include <ostream>
#include <stdexcept>
#include <string>

// Swift-style computed properties for C++.
//
// A Property<T> wraps a reference to a backing variable and gives it an
// optional getter and setter:
//
//   - reading the property converts implicitly to T, running the getter if one
//     was supplied (otherwise the backing variable is returned unchanged);
//   - assigning to the property runs the setter (the default setter just stores
//     the value).
//
// A property with no setter is read-only and throws std::runtime_error on
// assignment. For read-only that is enforced already at compile time, use the
// nested Property<T>::ReadOnly.
//
// The getter and setter are stored as std::function so that every Property<T>
// has the same type and can live as a class member; the small price is the
// usual std::function indirection. Values are passed by const reference to
// avoid copies for larger T.
template<typename T>
class Property {

	public:

		// derive the exposed value from the backing variable
		using Getter = std::function<T(const T&)>;
		// store a new value into the backing variable
		using Setter = std::function<void(T&, const T&)>;

		// A read-only property: it reads just like a Property, but assigning to
		// it is rejected by the compiler (the assignment operator is deleted).
		class ReadOnly {

			public:

				ReadOnly(T& ref) : ref(ref) {}

				operator T() const { return this->ref; }

				ReadOnly& operator =(const T&) = delete;	// read-only: assignment does not compile

				friend std::ostream& operator <<(std::ostream& os, const ReadOnly& p) {
					return os << T(p);
				}

			private:
				T& ref;
		};

		Property(
			T& ref,
			Getter get = nullptr,
			Setter set = [](T& var, const T& value) { var = value; }
		) : ref(ref), get(get), set(set) {}

		operator T() const {
			return this->get == nullptr ? this->ref : this->get(this->ref);
		}

		Property& operator =(const T& value) {
			if ( this->set == nullptr )
				throw std::runtime_error("property is read-only");
			this->set(this->ref, value);
			return *this;
		}

		friend std::ostream& operator <<(std::ostream& os, const Property& p) {
			return os << T(p);
		}

	private:
		T& ref;
		Getter get;
		Setter set;
};
