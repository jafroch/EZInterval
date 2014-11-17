#include "ez/interval.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(basic_interval)

BOOST_AUTO_TEST_CASE(Constructor) {
	auto const A = ez::make_interval[0][0];
	BOOST_CHECK_EQUAL(A.lower(), 0);
	BOOST_CHECK_EQUAL(A.upper(), 0);
	BOOST_CHECK_EQUAL(A.left_open(), false);
	BOOST_CHECK_EQUAL(A.left_closed(), true);
	BOOST_CHECK_EQUAL(A.right_open(), false);
	BOOST_CHECK_EQUAL(A.right_closed(), true);

	auto const B = ez::make_interval[3](4);
	BOOST_CHECK_EQUAL(B.lower(), 3);
	BOOST_CHECK_EQUAL(B.upper(), 4);
	BOOST_CHECK_EQUAL(B.left_open(), false);
	BOOST_CHECK_EQUAL(B.left_closed(), true);
	BOOST_CHECK_EQUAL(B.right_open(), true);
	BOOST_CHECK_EQUAL(B.right_closed(), false);

	auto const C = ez::make_interval(-2)[7];
	BOOST_CHECK_EQUAL(C.lower(), -2);
	BOOST_CHECK_EQUAL(C.upper(), 7);
	BOOST_CHECK_EQUAL(C.left_open(), true);
	BOOST_CHECK_EQUAL(C.left_closed(), false);
	BOOST_CHECK_EQUAL(C.right_open(), false);
	BOOST_CHECK_EQUAL(C.right_closed(), true);

	auto const D = ez::make_interval(1.0)(5.5);
	BOOST_CHECK_EQUAL(D.lower(), 1.0);
	BOOST_CHECK_EQUAL(D.upper(), 5.5);
	BOOST_CHECK_EQUAL(D.left_open(), true);
	BOOST_CHECK_EQUAL(D.left_closed(), false);
	BOOST_CHECK_EQUAL(D.right_open(), true);
	BOOST_CHECK_EQUAL(D.right_closed(), false);
}

BOOST_AUTO_TEST_CASE(EmptyTest) {
	BOOST_CHECK(!empty(ez::make_interval[0][0]));
	BOOST_CHECK(empty(ez::make_interval[0](0)));
	BOOST_CHECK(empty(ez::make_interval(0)[0]));
	BOOST_CHECK(empty(ez::make_interval(0)(0)));

	BOOST_CHECK(!empty(ez::make_interval[0][1]));
	BOOST_CHECK(!empty(ez::make_interval[0](1)));
	BOOST_CHECK(!empty(ez::make_interval(0)[1]));
	BOOST_CHECK(!empty(ez::make_interval(0)(1)));

	BOOST_CHECK(empty(ez::make_interval[1][0]));
	BOOST_CHECK(empty(ez::make_interval[1](0)));
	BOOST_CHECK(empty(ez::make_interval(1)[0]));
	BOOST_CHECK(empty(ez::make_interval(1)(0)));

	BOOST_CHECK(static_cast<bool>(ez::make_interval[0][0]));
	BOOST_CHECK(static_cast<bool>(ez::make_interval[0][1]));
	BOOST_CHECK(!static_cast<bool>(ez::make_interval[1][0]));
}

BOOST_AUTO_TEST_CASE(EqualityTest) {
	BOOST_CHECK_EQUAL(ez::make_interval[0][3], ez::make_interval[0][3]);
	BOOST_CHECK_EQUAL(ez::make_interval[0.0][3.0], ez::make_interval[0.0][3.0]);

	BOOST_CHECK(ez::make_interval[0][1] != ez::make_interval[0](1));
	BOOST_CHECK(ez::make_interval[0][1] != ez::make_interval(0)[1]);
	BOOST_CHECK(ez::make_interval[0][1] != ez::make_interval(0)(1));
	BOOST_CHECK(ez::make_interval[0](1) != ez::make_interval(0)[1]);
	BOOST_CHECK(ez::make_interval[0](1) != ez::make_interval(0)(1));
	BOOST_CHECK(ez::make_interval(0)[1] != ez::make_interval(0)(1));

	BOOST_CHECK(ez::make_interval[1][0] != ez::make_interval[5](-3));
}

BOOST_AUTO_TEST_CASE(Assign) {

}

BOOST_AUTO_TEST_CASE(Singleton) {
	BOOST_CHECK(singleton(ez::make_interval[0][0]));
	BOOST_CHECK(!singleton(ez::make_interval(0)[0]));
	BOOST_CHECK(!singleton(ez::make_interval[0](0)));
	BOOST_CHECK(!singleton(ez::make_interval(0)(0)));

	BOOST_CHECK(singleton(ez::make_interval[1][1]));
	BOOST_CHECK(!singleton(ez::make_interval[0][1]));
}

BOOST_AUTO_TEST_CASE(CommonPropertiesTest) {
	auto const int_interval = ez::make_interval(-6)[9];
	BOOST_CHECK_EQUAL(15, diameter(int_interval));
	BOOST_CHECK_EQUAL(1, midpoint(int_interval));
	BOOST_CHECK_EQUAL(7, radius(int_interval));

	BOOST_CHECK_EQUAL(1.5, ez::midpoint<double>(int_interval));
	BOOST_CHECK_EQUAL(7.5, ez::radius<double>(int_interval));

	auto const double_interval = ez::make_interval(-6.0)[9.0];
	BOOST_CHECK_EQUAL(15.0, diameter(double_interval));
	BOOST_CHECK_EQUAL(1.5, midpoint(double_interval));
	BOOST_CHECK_EQUAL(7.5, radius(double_interval));

	auto const bad_interval = ez::make_interval(1)[0];
	BOOST_CHECK_THROW(diameter(bad_interval), ez::empty_interval);
	BOOST_CHECK_THROW(midpoint(bad_interval), ez::empty_interval);
	BOOST_CHECK_THROW(radius(bad_interval),   ez::empty_interval);

	BOOST_CHECK_EQUAL(diameter(bad_interval, -1), -1);
	BOOST_CHECK_EQUAL(midpoint(bad_interval, -1), -1);
	BOOST_CHECK_EQUAL(radius(bad_interval, -1), -1);

	BOOST_CHECK_EQUAL(diameter(bad_interval, -1.5), -1.5);
	BOOST_CHECK_EQUAL(midpoint(bad_interval, -1.5), -1.5);
	BOOST_CHECK_EQUAL(radius(bad_interval, -1.5), -1.5);
}

BOOST_AUTO_TEST_CASE(Contains) {
	auto const A = ez::make_interval[0][2];
	BOOST_CHECK(!contains(A, -1));
	BOOST_CHECK(contains(A, 0));
	BOOST_CHECK(contains(A, 1));
	BOOST_CHECK(contains(A, 2));
	BOOST_CHECK(!contains(A, 3));

	auto const B = ez::make_interval[0](2);
	BOOST_CHECK(!contains(B, -1));
	BOOST_CHECK(contains(B, 0));
	BOOST_CHECK(contains(B, 1));
	BOOST_CHECK(!contains(B, 2));
	BOOST_CHECK(!contains(B, 3));

	auto const C = ez::make_interval(0)[2];
	BOOST_CHECK(!contains(C, -1));
	BOOST_CHECK(!contains(C, 0));
	BOOST_CHECK(contains(C, 1));
	BOOST_CHECK(contains(C, 2));
	BOOST_CHECK(!contains(C, 3));

	auto const D = ez::make_interval(0)(2);
	BOOST_CHECK(!contains(D, -1));
	BOOST_CHECK(!contains(D, 0));
	BOOST_CHECK(contains(D, 1));
	BOOST_CHECK(!contains(D, 2));
	BOOST_CHECK(!contains(D, 3));
}

BOOST_AUTO_TEST_CASE(Intersection) {
	{
		auto const interval1 = ez::make_interval[1][3];
		auto const interval2 = ez::make_interval[0][2];
		BOOST_CHECK_EQUAL(intersection(interval1, interval2), ez::make_interval[1][2]);
	}

	{
		auto const interval1 = ez::make_interval(1)[3];
		auto const interval2 = ez::make_interval(0)[2];
		BOOST_CHECK_EQUAL(intersection(interval1, interval2), ez::make_interval(1)[2]);
	}

	{
		auto const interval1 = ez::make_interval[1](3);
		auto const interval2 = ez::make_interval[0](2);
		BOOST_CHECK_EQUAL(intersection(interval1, interval2), ez::make_interval[1](2));
	}

	{
		auto const interval1 = ez::make_interval(1)(3);
		auto const interval2 = ez::make_interval(0)(2);
		BOOST_CHECK_EQUAL(intersection(interval1, interval2), ez::make_interval(1)(2));
	}

	auto const empty_interval = intersection(ez::make_interval[0][1], ez::make_interval[3][4]);
	BOOST_CHECK(empty(empty_interval));
}

BOOST_AUTO_TEST_CASE(TypeDefEquality) {
	auto const closed = ez::closed_interval<int>{1, 3};
	BOOST_CHECK_EQUAL(closed, ez::make_interval[1][3]);

	auto const ropen = ez::ropen_interval<int>{1, 3};
	BOOST_CHECK_EQUAL(ropen, ez::make_interval[1](3));

	auto const lopen = ez::lopen_interval<int>{1, 3};
	BOOST_CHECK_EQUAL(lopen, ez::make_interval(1)[3]);

	auto const open = ez::open_interval<int>{1, 3};
	BOOST_CHECK_EQUAL(open, ez::make_interval(1)(3));
}

BOOST_AUTO_TEST_CASE(Subsets) {
	{
		auto const A = ez::make_interval[1][2];
		auto const B = ez::make_interval[0][2];

		BOOST_CHECK(subset(A,  B));
		BOOST_CHECK(superset(B, A));
		BOOST_CHECK(!proper_subset(A, B));
		BOOST_CHECK(!proper_superset(B, A));
	}

	{
		auto const A = ez::make_interval[1][2];
		auto const B = ez::make_interval[0][3];

		BOOST_CHECK(subset(A, B));
		BOOST_CHECK(superset(B, A));
		BOOST_CHECK(proper_subset(A, B));
		BOOST_CHECK(proper_superset(B, A));
	}

	{
		auto const A = ez::make_interval[1](2);
		auto const B = ez::make_interval[0][2];

		BOOST_CHECK(subset(A, B));
		BOOST_CHECK(superset(B, A));
		BOOST_CHECK(proper_subset(A, B));
		BOOST_CHECK(proper_superset(B, A));
	}

	{
		auto const A = ez::make_interval(1)(2);
		auto const B = ez::make_interval(0)(2);

		BOOST_CHECK(subset(A, B));
		BOOST_CHECK(superset(B, A));
		BOOST_CHECK(!(proper_subset(A, B)));
		BOOST_CHECK(!(proper_superset(B, A)));
	}

	{
		auto const A = ez::make_interval(1)(2);
		auto const B = ez::make_interval[1][2];

		BOOST_CHECK(subset(A, B));
		BOOST_CHECK(superset(B, A));
		BOOST_CHECK(proper_subset(A, B));
		BOOST_CHECK(proper_superset(B, A));
	}

	{
		auto const A = ez::make_interval(0)(0);
		auto const B = ez::make_interval[1](2);

		BOOST_CHECK(subset(A, B));
		BOOST_CHECK(superset(B, A));
		BOOST_CHECK(proper_subset(A, B));
		BOOST_CHECK(proper_superset(B, A));
	}

	{
		auto const A = ez::make_interval(0)[1];
		auto const B = ez::make_interval(2)(2);

		BOOST_CHECK(!subset(A, B));
		BOOST_CHECK(!superset(B, A));
		BOOST_CHECK(!proper_subset(A, B));
		BOOST_CHECK(!proper_superset(B, A));
	}

	{
		auto const A = ez::make_interval(0)(0);
		auto const B = ez::make_interval(2)(2);

		BOOST_CHECK(subset(A, B));
		BOOST_CHECK(superset(B, A));
		BOOST_CHECK(!proper_subset(A, B));
		BOOST_CHECK(!proper_superset(B, A));
	}
}

BOOST_AUTO_TEST_CASE(Arithmetic) {
	auto const A = ez::make_interval[0][4];
	auto const B = ez::make_interval(2)(3);
	auto const C = ez::make_interval[-2](2);
	auto const D = ez::make_interval(0)[3];

	// Addition
	BOOST_CHECK_EQUAL(A + A, ez::make_interval[0][8]);
	BOOST_CHECK_EQUAL(A + B, ez::make_interval(2)(7));
	BOOST_CHECK_EQUAL(A + C, ez::make_interval[-2](6));
	BOOST_CHECK_EQUAL(A + D, ez::make_interval(0)[7]);

	BOOST_CHECK_EQUAL(B + B, ez::make_interval(4)(6));
	BOOST_CHECK_EQUAL(B + C, ez::make_interval(0)(5));
	BOOST_CHECK_EQUAL(B + D, ez::make_interval(2)(6));

	BOOST_CHECK_EQUAL(C + C, ez::make_interval[-4](4));
	BOOST_CHECK_EQUAL(C + D, ez::make_interval(-2)(5));

	BOOST_CHECK_EQUAL(D + D, ez::make_interval(0)[6]);

	BOOST_CHECK_EQUAL(A + B, B + A);
	BOOST_CHECK_EQUAL(A + C, C + A);
	BOOST_CHECK_EQUAL(A + D, D + A);
	BOOST_CHECK_EQUAL(B + C, C + B);
	BOOST_CHECK_EQUAL(B + D, D + B);
	BOOST_CHECK_EQUAL(C + D, D + C);

	// Subtraction
	BOOST_CHECK_EQUAL(A - A, ez::make_interval[-4][4]);
	BOOST_CHECK_EQUAL(A - B, ez::make_interval(-3)(2));
	BOOST_CHECK_EQUAL(A - C, ez::make_interval(-2)[6]);
	BOOST_CHECK_EQUAL(A - D, ez::make_interval[-3](4));

	BOOST_CHECK_EQUAL(B - A, ez::make_interval(-2)(3));
	BOOST_CHECK_EQUAL(B - B, ez::make_interval(-1)(1));
	BOOST_CHECK_EQUAL(B - C, ez::make_interval(0)(5));
	BOOST_CHECK_EQUAL(B - D, ez::make_interval(-1)(3));

	BOOST_CHECK_EQUAL(C - A, ez::make_interval[-6](2));
	BOOST_CHECK_EQUAL(C - B, ez::make_interval(-5)(0));
	BOOST_CHECK_EQUAL(C - C, ez::make_interval(-4)(4));
	BOOST_CHECK_EQUAL(C - D, ez::make_interval[-5](2));

	BOOST_CHECK_EQUAL(D - A, ez::make_interval(-4)[3]);
	BOOST_CHECK_EQUAL(D - B, ez::make_interval(-3)(1));
	BOOST_CHECK_EQUAL(D - C, ez::make_interval(-2)[5]);
	BOOST_CHECK_EQUAL(D - D, ez::make_interval(-3)(3));

	// Division
	BOOST_CHECK_THROW(A / A, ez::divide_by_zero);
	BOOST_CHECK_EQUAL(A / B, ez::make_interval[0](2));
	BOOST_CHECK_THROW(A / C, ez::divide_by_zero);
	BOOST_CHECK_THROW(A / D, ez::divide_by_zero);

	BOOST_CHECK_THROW(B / A, ez::divide_by_zero);
	BOOST_CHECK_EQUAL(B / B, ez::make_interval(0)(1));
	BOOST_CHECK_THROW(B / C, ez::divide_by_zero);
	BOOST_CHECK_THROW(B / D, ez::divide_by_zero);

	BOOST_CHECK_THROW(C / A, ez::divide_by_zero);
	BOOST_CHECK_EQUAL(C / B, ez::make_interval(-1)(1));
	BOOST_CHECK_THROW(C / C, ez::divide_by_zero);
	BOOST_CHECK_THROW(C / D, ez::divide_by_zero);

	BOOST_CHECK_THROW(D / A, ez::divide_by_zero);
	BOOST_CHECK_EQUAL(D / B, ez::make_interval[0](1));
	BOOST_CHECK_THROW(D / C, ez::divide_by_zero);
	BOOST_CHECK_THROW(D / D, ez::divide_by_zero);

	// Multiplication
	BOOST_CHECK_EQUAL(A * A, ez::make_interval[0][16]);
	BOOST_CHECK_EQUAL(A * B, ez::make_interval[0](12));
	BOOST_CHECK_EQUAL(A * C, ez::make_interval[-8](8));
	BOOST_CHECK_EQUAL(A * D, ez::make_interval[0][12]);

	BOOST_CHECK_EQUAL(B * B, ez::make_interval(4)(9));
	BOOST_CHECK_EQUAL(B * C, ez::make_interval(-6)(6));
	BOOST_CHECK_EQUAL(B * D, ez::make_interval(0)(9));

	BOOST_CHECK_EQUAL(C * C, ez::make_interval(-4)[4]);
	BOOST_CHECK_EQUAL(C * D, ez::make_interval[-6](6));

	BOOST_CHECK_EQUAL(D * D, ez::make_interval(0)[9]);

	BOOST_CHECK_EQUAL(A * B, B * A);
	BOOST_CHECK_EQUAL(A * C, C * A);
	BOOST_CHECK_EQUAL(A * D, D * A);
	BOOST_CHECK_EQUAL(B * C, C * B);
	BOOST_CHECK_EQUAL(B * D, D * B);
	BOOST_CHECK_EQUAL(C * D, D * C);
}

BOOST_AUTO_TEST_SUITE_END()
