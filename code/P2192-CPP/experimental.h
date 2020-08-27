#ifndef DBJ_REF_WRAP_INC_
#define DBJ_REF_WRAP_INC_

#include <utility>
#include <cassert>
#include "../common/local_type_traits.h"
#include "dbj_valstat.h"

/**
 * (c) 2019/2020 by Dusan B. Jovanovic, https://dusanjovanovic.org
 *
 * Licence CC BY SA 4.0
 * dbj reference wrapper with occupancy empty semantics
 *
 * Any instance of dbj::ref_wrap<T> at any given point in time
 * either represents a ref to a value or does not have a value
 * it is said to be empty
 *
 * default constructed ref wrap contains a nullptr
 * also has boolean implicit casting, to check if value is there
 * also has '*' unary operator to get the value stored
 *
 * so what's the purpose? basically providing 'empty' reference semantics.
 * for example:
 *
 * dbj::ref_wrap<T> operator [] (unsigned idx_ ) {
 *      if (idx_ < data.size())
 *             return data[idx_];
 *      return dbj::ref_wrap<T>{} ; // empty reference
 * }
 *
 * usage:
 *
 * auto slot42 = fun(42);
 *
 * // CAUTION! app will exit() if nullptr inside
 * if ( ! slot42 ){
 *     perror("slot 42 does not exist!");
 * } else {
 *      auto value = *slot42 ; 
 * }
 *
 * other than above the behavior is exactly the same as C++20 std::reference_wrapper
 *
 * POLICY: upon trying to dereference the nullptr of the default constructed ref_wrap
 * application will exit.
*/


namespace dbj::experimental {

	template <class T>
	class ref_wrap
	{
	public:
		// types
		using type = ref_wrap;
		using value_type = T;

		// construct/copy/destroy
		template <
			class U, class = decltype(
				detail::FUN<T>(detail::declval<U>()),
				detail::enable_if_t<
				!detail::is_same_v<
				ref_wrap, detail::remove_cvref_t<U>
				>
				>()
				)
		>
			constexpr ref_wrap(U&& u)
			noexcept(
				noexcept(detail::FUN<T>(detail::forward<U>(u)))
				)
			: ptr_(detail::addressof(detail::FUN<T>(detail::forward<U>(u))))
		{
			/* */
		}

		constexpr ref_wrap() noexcept : ptr_(nullptr) {}

		// check the occupancy state: value or empty, true of false
		constexpr operator bool() const noexcept { return (ptr_ != nullptr); }

		constexpr ref_wrap(const ref_wrap&) noexcept = default;

		// no assignment to ref.
		ref_wrap& operator=(const ref_wrap& x) noexcept = default;

		// here we deliver the reference to the value
		// asserts if empty
		constexpr T& operator*() const noexcept
		{
			if (ptr_ == nullptr)
			{
				// POLICY is to exit()
				// if instance is empty
				assert(ptr_);
				exit(1);
			}
			return *ptr_;
		}

		// implicit casting operator
		// asserts if empty
		constexpr operator T& () const noexcept
		{
			if (ptr_ == nullptr)
			{
				// POLICY is to exit()
				// if instance is empty
				assert(ptr_);
				exit(1);
			}
			return *ptr_;
		}

		[[deprecated]]
		constexpr T& get() const noexcept
		{
			if (ptr_ == nullptr)
			{
				assert(ptr_);
				exit(1);
			}
			return *ptr_;
		}

		template <class... ArgTypes>
		constexpr std::invoke_result_t<T&, ArgTypes...>
			operator()(ArgTypes&&... args) const
		{
			if (ptr_ == nullptr)
			{
				// POLICY is to exit()
				// if instance is empty
				assert(ptr_);
				exit(1);
			}

			return std::invoke(*ptr_, std::forward<ArgTypes>(args)...);
		}

	private:
		// BIG warning:
		// there is no way of knowing if this is a dangling pointer
		T* ptr_{};
	};

	// deduction guides
	template <class T>
	ref_wrap(T&)->ref_wrap<T>;

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////

	/// sam as above but using metastate 
	/// basically same as std::reference_wrapper<T> or native references 
	/// this allows for dangling references or pointers
	/// WARNING: this is inferior to std::option<T> based solutions

	////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	class reference_with_state final
	{
		// nullptr by default
		T* ptr_{};
	public:

		enum class status_code {
			empty_instance = 100
		};

		constexpr friend
			const char* const status_message(status_code sc_)
			noexcept
		{
			switch (sc_) {
			case status_code::empty_instance: return "Empty instance";
			default: return "Unknown status code";
			}
		}

		// types
		using type = reference_with_state;
		using value_type = T;

		// embeded, self contained, metastate carrier
		using vstat = dbj::valstat<T, status_code>;

		// construct/copy/destroy
		template <
			class U, class = decltype(
				detail::FUN<T>(detail::declval<U>()),
				detail::enable_if_t<
				!detail::is_same_v<
				reference_with_state, detail::remove_cvref_t<U>
				>
				>()
				)
		>
			constexpr reference_with_state(U&& u)
			noexcept(
				noexcept(detail::FUN<T>(detail::forward<U>(u)))
				)
			: ptr_(detail::addressof(detail::FUN<T>(detail::forward<U>(u))))
		{
			/* */
		}

		constexpr reference_with_state() : ptr_(nullptr) {}

		constexpr operator bool() const noexcept { return (ptr_ != nullptr); }

		reference_with_state(const reference_with_state&) noexcept = default;

		// no assignment to ref.
		reference_with_state& operator=(const reference_with_state& x) noexcept = default;

		// metastate return, no exit() necessary
		constexpr vstat operator * () const noexcept
		{
			if (ptr_ == nullptr)
			{
				return { {}, status_code::empty_instance };
			}
			return { ptr_, {} };
		}

		// metastate return, no exit() necessary
		[[deprecated]]
		constexpr operator T& () const noexcept
		{
			if (ptr_ == nullptr)
			{
				return { {}, status_code::empty_instance };
			}
			return *ptr_;
		}

		// metastate return, no exit() necessary
		constexpr vstat get() noexcept
		{
			if (ptr_ == nullptr)
			{
				return { {}, status_code::empty_instance };
			}
			return { *ptr_, {} };
		}


		template <	class... ArgTypes>
		struct rezult_type {
			using type = std::invoke_result_t<T&, ArgTypes...>;
			using valstat = dbj::valstat< rezult_type::type, status_code >;
		};

		// will not compile if *ptr_ is not Invocable
		// metastate return, no exit() necessary
		template <	class... ArgTypes >
		/*constexpr*/ 
		auto
			operator()(ArgTypes&&... args) /*const*/
				-> typename	rezult_type< ArgTypes...>::valstat
		{
			if ( this->ptr_ == nullptr)
			{
				// ERROR metastate
				return { {}, status_code::empty_instance };
			}

			// OK metastate
			return  
			{
				std::invoke(*ptr_, detail::forward<ArgTypes>(args)...)
				, {}
			};
		}

	};

	// deduction guide
	template <class T>
	reference_with_state(T&)->reference_with_state<T>;

	//////////////////////////////////////////////////////////////////////////
	// testing

	namespace {
		using rfw = reference_with_state<int>;

		inline auto doubler (int arg_) { return 2 * arg_; }
	}

	inline void test_reference_with_state() {

	auto driver = [](auto vstat)
		{
		std::cout << "\n";
			if (vstat.value)
				SX(*vstat.value);
			else
				std::cout << " empty value";

		std::cout << "\n";
			if (vstat.status)
				SX(
					status_message( *vstat.status )
				);
			else
				std::cout << " empty status";
	};

		auto i42_ = 42;
		auto& irf_ = i42_;
		rfw rf_1 = irf_;

		rfw rf_2;
		driver(rf_2.get());

		// will not compile as it is expected --> driver(rf_2());

		rf_2 = rf_1;
		driver(rf_2.get());

		// storing ref to function
		reference_with_state rff = doubler;

		// calling the function
		// seeing the rezult
		driver( rff(42) );
	}
}

#endif // DBJ_REF_WRAP_INC_