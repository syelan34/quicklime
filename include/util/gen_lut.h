#pragma once

#include <array>
#include <cstddef>
namespace ql {
    /**
    * @brief Generate a lookup table
    *
    * @tparam S Size of the lookup table
    * @tparam F Function to generate the table
    * @tparam R Return type of the function
    * @param func Function to generate the table
    * @return R The generated table
    */
	template <size_t S	 = 0, std::invocable<double> F,
			  typename R = std::array<std::invoke_result_t<F, double>, S>>
	constexpr R generateTable(F &&func) {
		static_assert(S > 0, "LUT size must be greater than zero");
		R table{};
		for (size_t i = 0; i < S; i++)
			table[i] = func(static_cast<double>(i) / S);
		return table;
	}
} // namespace ql
