#pragma once

#include <functional>
#include <stdexcept>

template<typename T>
class Property {

	public:

		class ReadOnly {

			public:

				ReadOnly(T &ref) : ref(ref) {};
				operator T() const { return ref; }
				T& operator = (T value) {
					this -> assert_on_readonly<true>();
					return ref;
				}

			private:

				template <bool readonly>
				void assert_on_readonly(void) {
					static_assert(!readonly,
						"property is read-only");
				}

				T& ref;
		};

		using Getter = std::function<const T(T&)>;
		using Setter = std::function<void(T&, T)>;

		Property(
			T &ref,
			Getter get = nullptr,
			Setter set = [](int &var, int n) { var = n; }
		) : ref(ref), get(get), set(set) {};

		operator T() const {
			return this -> get == nullptr ? this -> ref : this -> get(ref);
		};

		T& operator = (T value) {
			if ( this -> set == nullptr )
				throw std::runtime_error("property is read-only");
			this -> set(ref, value);
			return ref;
		}

	private:

		T& ref;
		Getter get;
		Setter set;
};

inline std::ostream &operator<<(std::ostream &os, Property<std::string>::ReadOnly const &m) {
	return os << std::string(m);
}

inline std::ostream &operator<<(std::ostream &os, Property<std::string> const &m) {
	return os << std::string(m);
}

