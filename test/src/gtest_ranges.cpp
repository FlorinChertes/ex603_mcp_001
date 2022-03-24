#include "../../lib/inc/ranges_func.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(VectorTest, test_initialize_from_iota_001)
{
	auto vi = std::views::iota(5, 20);

	std::vector ve = to_vector(vi);

	EXPECT_EQ(ve.size(), 15);
}



