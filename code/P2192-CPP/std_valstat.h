#pragma once
/*
(c) 2020 by dbj@dbj.org CC BY SA 3.0

valstat STL version
*/

namespace std
{

	/*
	standard C++ STL simplest shape
	*/
	template< typename T, typename S>
	struct [[nodiscard]] valstat final
	{
		using value_type = T;
		using status_type = S;

		using type = valstat;

		T value{};
		S status{};

	}; // valstat
} // ns dbj