#pragma once

#ifndef __cplusplus
#error this header is C++ only
#endif // __cplusplus

#include "config.h"

namespace dbj {

	// local version of type_traits
// valstat light must not depend on STL
	namespace detail
	{

	// integral_constant
	template <class Ty_, Ty_ _Val>
	struct integral_constant {
		static constexpr Ty_ value = _Val;

		using valueTy_pe = Ty_;
		using type = integral_constant;

		constexpr operator valueTy_pe() const noexcept {
			return value;
		}

		[[nodiscard]] constexpr valueTy_pe operator()() const noexcept {
			return value;
		}
};

	// bool_constant
	template <bool _Val>
	using bool_constant = integral_constant<bool, _Val>;

	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

#ifdef COMPLEXITY_IS_NOT_A_VIRTUE
	//  is_same
#ifdef __clang__
	template <class Ty_1, class Ty_2>
	inline constexpr bool is_same_v = __is_same(Ty_1, Ty_2);

	template <class Ty_1, class Ty_2>
	struct is_same : bool_constant<__is_same(Ty_1, Ty_2)> {};

#else // not clang 
	
	template <class, class>
	inline constexpr bool is_same_v = false; // determine whether arguments are the same type

	template <class Ty_>
	inline constexpr bool is_same_v<Ty_, Ty_> = true;

	template <class Ty_1, class Ty_2>
	struct is_same : bool_constant<is_same_v<Ty_1, Ty_2>> {};
#endif // __clang__

#else

	template<class T, class U>
	struct is_same : false_type {};

	template<class T>
	struct is_same<T, T> : true_type {};

	template< class T, class U >
	inline constexpr bool is_same_v = is_same<T, U>::value;

#endif // COMPLEXITY_IS_NOT_A_VIRTUE


		// cl.exe compatible only
		template <class Ty_>
		[[nodiscard]] constexpr Ty_* addressof(Ty_& Val) noexcept {
			return __builtin_addressof(Val);
		}

		template <class Ty_>
		const Ty_* addressof(const Ty_&&) = delete;

		//Add_reference_
		template <class Ty_, class = void>
		struct Add_reference_ { // add reference (non-referenceable type)
			using _Lvalue = Ty_;
			using _Rvalue = Ty_;
		};

		// ALIAS TEMPLATE void_t
		template <class... Ty_pes>
		using void_t = void;

		template <class Ty_>
		struct Add_reference_<Ty_, void_t<Ty_&>> { // (referenceable type)
			using _Lvalue = Ty_&;
			using _Rvalue = Ty_&&;
		};

		// add_lvalue_reference
		template <class Ty_>
		struct add_lvalue_reference {
			using type = typename Add_reference_<Ty_>::_Lvalue;
		};

		template <class Ty_>
		using add_lvalue_reference_t = typename Add_reference_<Ty_>::_Lvalue;

		// add_rvalue_reference
		template <class Ty_>
		struct add_rvalue_reference {
			using type = typename Add_reference_<Ty_>::_Rvalue;
		};

		template <class Ty_>
		using add_rvalue_reference_t = typename Add_reference_<Ty_>::_Rvalue;

		// declval
		template <class Ty_>
		add_rvalue_reference_t<Ty_> declval() noexcept;


		// enable_if
		template <bool Test_, class Ty_ = void>
		struct enable_if {}; // no member "type" when !Test_

		template <class Ty_>
		struct enable_if<true, Ty_> { // type is Ty_ for Test_
			using type = Ty_;
		};

		template <bool Test_, class Ty_ = void>
		using enable_if_t = typename enable_if<Test_, Ty_>::type;

		// is_lvalue_reference
		template <class>
		inline constexpr bool is_lvalue_reference_v = false; // determine whether type argument is an lvalue reference

		template <class Ty_>
		inline constexpr bool is_lvalue_reference_v<Ty_&> = true;

		template <class Ty_>
		struct is_lvalue_reference : bool_constant<is_lvalue_reference_v<Ty_>> {};

		// is_rvalue_reference
		template <class>
		inline constexpr bool is_rvalue_reference_v = false; // determine whether type argument is an rvalue reference

		template <class Ty_>
		inline constexpr bool is_rvalue_reference_v<Ty_&&> = true;

		template <class Ty_>
		struct is_rvalue_reference : bool_constant<is_rvalue_reference_v<Ty_>> {};

		// remove_cv
		template <class T_>
		struct remove_cv
		{ // remove top-level const and volatile qualifiers
			using type = T_;
		};

		template <class T_>
		struct remove_cv<const T_>
		{
			using type = T_;
		};

		template <class T_>
		struct remove_cv<volatile T_>
		{
			using type = T_;
		};

		template <class T_>
		struct remove_cv<const volatile T_>
		{
			using type = T_;
		};

		template <class T_>
		using remove_cv_t = typename remove_cv<T_>::type;

		// remove_reference
		template <class T_>
		struct remove_reference
		{
			using type = T_;
		};

		template <class T_>
		struct remove_reference<T_&>
		{
			using type = T_;
		};

		template <class T_>
		struct remove_reference<T_&&>
		{
			using type = T_;
		};

		template <class T_>
		using remove_reference_t = typename remove_reference<T_>::type;

		template <class T_>
		using _remove_cvref_t = remove_cv_t<remove_reference_t<T_>>;

		template <class T_>
		using remove_cvref_t = _remove_cvref_t<T_>;

		template <class T_>
		struct remove_cvref
		{
			using type = remove_cvref_t<T_>;
		};

		template <class T>
		T& FUN(T& t) noexcept
		{
			return t;
		}
		template <class T>
		void FUN(T&&) = delete;

		// forward
		template <class Ty_>
		[[nodiscard]] constexpr Ty_&& forward(
			remove_reference_t<Ty_>& _Arg) noexcept { // forward an lvalue as either an lvalue or an rvalue
			return static_cast<Ty_&&>(_Arg);
		}

		template <class Ty_>
		[[nodiscard]] constexpr Ty_&& forward(remove_reference_t<Ty_>&& _Arg) noexcept { // forward an rvalue as an rvalue
			static_assert(!is_lvalue_reference_v<Ty_>, "bad forward call");
			return static_cast<Ty_&&>(_Arg);
		}

	} // namespace detail


} // dbj