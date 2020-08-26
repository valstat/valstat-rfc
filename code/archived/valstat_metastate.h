#pragma once
/*
(c) 2020 by dbj@dbj.org CC BY SA 3.0

As far as metstate paradigm usability is concerned this is completely redundant effort.
But her is a proposal for adopter requiring metastate
to be materialized as C++ abstraction and values


valstat is a vesel for a meta state
"meta state" is "state of states"
valstat meta-state = value state AND status state

here we show the metastate paradigm in C++ code, without std::valstat

*/

namespace dbj {
	/*
	 Core Logic:

	 Single instance can be in two states: empty or has value, false or true

	 Oberving the states of two instances there are four possible metastates

	 Meta state thus means "state of the states"

	 It can be simply made from:

	 two std::optional occupancy states: "is empty" or "has value"
	 two native pointer states: "null" or "not null" to mean "empty" or "has value"

	Value and Status are the names of the roles of two instances we compare.
	Roles are important because they convey the names of the meta states of Value and Status
	The logic of naming the meta states is a consequence of
	the roles of Values and Status in a program.

	Meta States Tags

	 +-------------------------+
	 |      S  | true  | false |   V, S  -- Value and Status, two roles for two instances
	 | V       |       |       |   true  -- present or "has value"
	 |---------+-------+-------|   false -- not present or "has no value"
	 | true    | info  | ok    |
	 |---------+-------+-------|
	 | false   | error | empty |
	 +-------------------------+


	English tag names of metastates do not mandate the logic of using them. Although they are a strong
	hint.

	*/

	enum class meta_state {
		ok, error, info, empty
	};

	constexpr inline auto metastate_string(meta_state state_) noexcept
		-> const char* const
	{
		switch (state_) {
		case(meta_state::info):		return "info";
		case(meta_state::ok):		return "ok";
		case(meta_state::empty):	return "empty";
		case(meta_state::error):	return "error";
		default:
			return "unknown";
		}
	}

	// standard C++ meta state decoding function
	// Note: Value and Status are roles dictating the logic
	// Context is a function return
	// type requirement for both T and S is to be able to be interogated
	// for the state of occpuancy ... se the code bellow
	template<typename T, typename S >
	constexpr inline meta_state meta_state_capture(T const & value, S const & status) noexcept
	{
		if (value && status) return meta_state::info;
		if (value && !status) return meta_state::ok;
		if (!value && !status) return meta_state::empty;
		/* if (!value && status) */ return meta_state::error;
	}

} // dbj 

// probably should go in another header
#include <optional>

namespace dbj {

	//	valstat is a vesel for a meta state

		// compare two optionals to decide the state
		// without valstat instance
		// Context is function return
	//template< typename T, typename S>
	//constexpr inline dbj::meta_state meta_state_capture
	//(std::optional<T> value, std::optional<S> status) noexcept
	//{
	//	if (value && status) return dbj::meta_state::info;
	//	if (value && !status) return dbj::meta_state::ok;
	//	if (!value && !status) return dbj::meta_state::empty;
	//	/* if (!value && status) */ return dbj::meta_state::error;
	//}
}

// there is no valstat here, yet ... testing the applicability

namespace {

	inline void test_using_two_pointers()
	{
		using dbj::meta_state;
		constexpr auto* just_a_state = "I am a status";
		constexpr int i1{ 42 };

		// nullptr_t's are forbidden
		// null pointer must have a type
		constexpr int* ptr_int_empty{};

		static_assert(meta_state::empty
			== dbj::meta_state_capture(ptr_int_empty, ptr_int_empty)
			);
		static_assert(meta_state::error
			== dbj::meta_state_capture(ptr_int_empty, just_a_state)
			);
		static_assert(meta_state::info
			== dbj::meta_state_capture(&i1, just_a_state)
			);
		static_assert(meta_state::ok
			== dbj::meta_state_capture(&i1, ptr_int_empty)
			);
	}

	inline void test_using_two_optionals()
	{
		using std::optional;
		using dbj::meta_state;

		constexpr optional just_a_status = "I am a status";
		constexpr optional opt42 = 42;
		constexpr optional<int> opt_int_empty;

		static_assert(
			meta_state::empty ==
			dbj::meta_state_capture(opt_int_empty, opt_int_empty)
			);
		static_assert(
			meta_state::error ==
			dbj::meta_state_capture(opt_int_empty, just_a_status)
			);
		static_assert(
			meta_state::info ==
			dbj::meta_state_capture(opt42, just_a_status)
			);
		static_assert(
			meta_state::ok ==
			dbj::meta_state_capture(opt42, opt_int_empty)
			);
	}
}