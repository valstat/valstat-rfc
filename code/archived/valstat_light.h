#pragma once
/*
(c) 2020 by dbj@dbj.org CC BY SA 3.0

light version is for  restricted environemnts
it is not using std lib
*/

namespace dbj::light
{
	/*
	standard C++ valstat simplest shape
	*/
	template< typename T, typename S>
	struct [[nodiscard]] valstat final
	{
		using value_type = T;
		using status_type = S;

		using type = valstat;

		value_type* value{};
		status_type* status{};

		// convenience function not in a proposal
		// decode the state    
		constexpr static meta_state meta_state(type const& vstat_) noexcept
		{
			if (vstat_.value && vstat_.status) return meta_state::info;
			if (vstat_.value && !vstat_.status) return meta_state::ok;
			if (!vstat_.value && !vstat_.status) return meta_state::empty;
			/*if (!vstat_.value && vstat_.status)*/ return meta_state::error;
		}

	}; // valstat

} // ns dbj::light

