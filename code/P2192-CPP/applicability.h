#pragma once
/*
valstat
usability feasibility and applicability

dbj is an imaginary company using std::valstat
*/

#include "../common/common.h"
#include "dbj_valstat.h" // dbj::valstat<T,S>

#include "experimental.h" // ref wrapper with empty state

#include <string_view>

namespace dbj::valstat_testing {

	// dbj::stl has the std::valstat 
	// status type is determined as std::string

	template<typename T>
	using valstat = dbj::valstat<T, std::string_view >;

	constexpr valstat<int> add(int lhs, int rhs) noexcept
#ifdef __clang__
// pure function
		__attribute__((const));
#endif
		;
		namespace {
			constexpr inline valstat<int> add(int lhs, int rhs)
				// valstat enabled API does not throw exceptions
				noexcept
			{
				if (lhs >= 0) {
					if (INT_MAX - lhs < rhs) {
						// creating valstat instance
						// with empty value option 
						// status option contains a string
						return { {}, "integer overflow" };
						// signal the ERROR state
					}
				}
				else {
					if (rhs < INT_MIN - lhs) {
						// creating valstat instance
						// with empty value option 
						// status option contains a string
						return { {}, "integer overflow" };
						// signal the ERROR state
					}
				}
				// creating valstat instance
				// with value option set
				// status option remains empty
				return { lhs + rhs, {} };
				// signal the OK state
			}

			inline void test_shiny_foot_gun()
			{
				constexpr int int_1 = 42, int_2 = 13;
				if (constexpr auto v_ = add(int_1, int_2).value; v_)
				{
					// v_ aka value is not empty
					SX(*v_);
				}
			}
		
		} // nspace

	///////////////////////////////////////////////////////////////////////
	// metastate in the context of constructors
	// also inside non trivial value type
	// added twist: space instrument app, can not use std lib

	template<typename T>
	struct vortex_data {

		using  type = vortex_data;
		using  value_type = T ;

/*	
    we do not need std::valstat here
    the added twist: we pass the data out through OK or INFO metastates 
	only

	paradigm is respected: valstat is metastate carrier
*/
		struct valstat final{
			value_type* value {};  // dangerous ad-hoc design
			const char* message{};// status is string literal or nullptr
		} ;

		// passing the valstat as a ref argument
		explicit constexpr vortex_data
			(type::valstat& valstat, T data_)	noexcept
				: the_data_( new T(data_) )
			{	valstat = metastate();		}

		// getting the data through metastate
		// capture the current metastate of the instance
		// pass the data out if metstate is INFO or OK
		// metastate ERROR never happens
		constexpr type::valstat metastate() noexcept
		{
			if ( the_data_ == nullptr )
				return { {}, {} }; // EMPTY state

			if (*the_data_ == sentinel_ )
				return { the_data_,  "the data == int(42)" }; // INFO state

			return { the_data_, {} }; // OK state
		}

		 vortex_data(vortex_data const&) = delete;
		 vortex_data& operator = (vortex_data const&) = delete;

		 vortex_data(vortex_data&& other_)
		 {
			 using std::swap ;
			 std::swap( this->the_data_, other_.the_data_ );
		 }
		 vortex_data& operator = (vortex_data&& other_)
		 {
			 using std::swap ;
			 std::swap(this->the_data_, other_.the_data_);
		 }

		~vortex_data() {
			if (the_data_) delete the_data_;
			the_data_ = nullptr;
		}

	private:
		constexpr static const int sentinel_ = 42; 
		T * the_data_{};
	}; // vortex_data

	static inline void test_constructor_usage() {

		using int_data_type = vortex_data<int>;
		using valstat_type = typename int_data_type::valstat ;

		// also test moveability and copyability
		auto driver = [&](typename int_data_type::valstat vstat_, auto val_ ) {

			std::cout << "\n\nconstructing with:" << val_ ;
			
			int_data_type the_data(vstat_, val_);

			// pass the data out with OK or INFO metastates
			// if any ot the two is sent back
			auto [value, status_message ] = the_data.metastate();
// tests
			assert (*vstat_.value   == *value);

			if (vstat_.message && status_message )
			assert ( 0 == strcmp(vstat_.message, status_message) );

#ifndef NDEBUG
			using std::cout;

		cout << "\nValue: ";
		if (value )
			cout << (*value);
		else
			cout << "empty";

		cout << "\nStatus: ";
		if (status_message)
			cout << status_message ;
		else
			cout << "empty";
#endif // !NDEBUG
		};

		std::cout << "\n\ntesting constructor metastates";

		driver(valstat_type{}, 42);
		driver(valstat_type{}, int{});
		driver(valstat_type{}, 13);
	}

} // dbj::valstat_testing


