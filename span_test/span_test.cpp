#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "../Span/span.h"

// constructor

TEST_CASE("constructor of span<int> creates non-empty span with upper and lower bounds")
{
	span<int> span(1, 2);
	CHECK(span.upper() == 2);
	CHECK(span.lower() == 1);
	CHECK(!span.is_empty());
}

TEST_CASE("constructor of span<int> creates empty span if upper and lower bounds are equal")
{
	span<int> span(1, 1);
	CHECK(span.is_empty());
}

TEST_CASE("constructor of span<int> throws exception if lower bound is larger than upper bound")
{
	CHECK_THROWS_AS(span<int>(5, 2), std::invalid_argument);
}

// is_inside for a value

TEST_CASE("span<int>::is_inside returns true for an integer inside bounds")
{
	span<int> span(1, 5);
	CHECK(span.is_inside(2));
}

TEST_CASE("span<int>::is_inside returns true for an integer equal to lower bound")
{
	span<int> span(1, 5);
	CHECK(span.is_inside(1));
}

TEST_CASE("span<int>::is_inside returns false for an integer equal to upper bound")
{
	span<int> span(1, 5);
	CHECK(!span.is_inside(5));
}

TEST_CASE("span<int>::is_inside returns false for an integer smaller than lower bound")
{
	span<int> span(1, 5);
	CHECK(!span.is_inside(0));
}

TEST_CASE("span<int>::is_inside returns false for an integer larger than upper bound")
{
	span<int> span(1, 5);
	CHECK(!span.is_inside(6));
}

// is_inside_inclusive for a value

TEST_CASE("span<int>::is_inside_inclusive returns true for an integer inside bounds")
{
	span<int> span(1, 5);
	CHECK(span.is_inside_inclusive(2));
}

TEST_CASE("span<int>::is_inside_inclusive returns true for an integer equal to lower bound")
{
	span<int> span(1, 5);
	CHECK(span.is_inside_inclusive(1));
}

TEST_CASE("span<int>::is_inside_inclusive returns true for an integer equal to upper bound")
{
	span<int> span(1, 5);
	CHECK(span.is_inside_inclusive(5));
}

TEST_CASE("span<int>::is_inside_inclusive returns false for an integer smaller than lower bound")
{
	span<int> span(1, 5);
	CHECK(!span.is_inside_inclusive(0));
}

TEST_CASE("span<int>::is_inside_inclusive returns false for an integer larger than upper bound")
{
	span<int> span(1, 5);
	CHECK(!span.is_inside_inclusive(6));
}

// is_inside for another span

TEST_CASE("span<int>::is_inside returns true for another span with larger lower and smaller upper bound")
{
	span<int> s{ 1, 5 };
	CHECK(s.is_inside(span<int>{ 2, 4 }));
}

TEST_CASE("span<int>::is_inside returns true for another span with same bounds")
{
	span<int> s{ 1, 5 };
	CHECK(s.is_inside(span<int>{ 1, 5 }));
}

TEST_CASE("span<int>::is_inside returns true for another narrower span with common boundary")
{
	span<int> s{ 1, 5 };
	CHECK(s.is_inside(span<int>{ 1, 4 }));
	CHECK(s.is_inside(span<int>{ 2, 5 }));
}

TEST_CASE("span<int>::is_inside returns false for another span with smaller lower bound and larger upper bound")
{
	span<int> s{ 1, 5 };
	CHECK(!s.is_inside(span<int>{ 0, 6 }));
}

TEST_CASE("span<int>::is_inside returns false for another span with smaller lower bound and smaller upper bound")
{
	span<int> s{ 1, 5 };
	CHECK(!s.is_inside(span<int>{ 0, 4 }));
}

TEST_CASE("span<int>::is_inside returns false for another span with larger lower bound and larger upper bound")
{
	span<int> s{ 1, 5 };
	CHECK(!s.is_inside(span<int>{ 2, 6 }));
}

// intersection

TEST_CASE("span<int>::intersection returns new span corresponding to the intersection with span provided")
{
	span<int> s1{ 1, 5 };
	span<int> s2{ 2, 6 };
	auto intersection = s1.intersection(s2);
	CHECK(intersection.lower() == 2);
	CHECK(intersection.upper() == 5);
}

TEST_CASE("span<int>::intersection of non-empty with empty span returns empty span")
{
	span<int> s{ 1, 5 };
	CHECK(s.intersection(span<int>{ 2, 2 }).is_empty());
	CHECK(s.intersection(span<int>{ -1, -1 }).is_empty());
	CHECK(s.intersection(span<int>{ 6, 6 }).is_empty());
}

TEST_CASE("span<int>::intersection of empty span with non-empty returns empty span")
{
	span<int> s{ 2, 2 };
	CHECK(s.intersection(span<int>{ 1, 3 }).is_empty());
	CHECK(s.intersection(span<int>{ 2, 5 }).is_empty());
	CHECK(s.intersection(span<int>{ 1, 5 }).is_empty());
}

TEST_CASE("span<int>::intersection of empty span with empty returns empty span")
{
	span<int> s{ 2, 2 };
	CHECK(s.intersection(span<int>{ 1, 1 }).is_empty());
	CHECK(s.intersection(span<int>{ 2, 2 }).is_empty());
	CHECK(s.intersection(span<int>{ 5, 5 }).is_empty());
}


struct char_less
{
	bool operator()(const char* left, const char* right) const { return strcmp(left, right) < 0; }
};

struct char_equal
{
	bool operator()(const char* left, const char* right) const { return strcmp(left, right) == 0; }
};

using char_span = span<const char*, char_less, char_equal>;

TEST_CASE("span<const char*>::is_inside returns true for a string alphabetically inside bounds")
{
	char_span s{ "abc", "xyz" };
	CHECK(s.is_inside("bcd"));
	CHECK(s.is_inside("abcd"));
	CHECK(s.is_inside("xy"));
}

TEST_CASE("span<const char*>::is_inside returns false for a string alphabetically outside bounds")
{
	char_span s{ "abc", "bcd" };
	CHECK(!s.is_inside("xyz"));
	CHECK(!s.is_inside("bcda"));
}

TEST_CASE("span<const char*>::is_inside returns true for a string equal to lower bound")
{
	char_span s{ "abc", "bcd" };
	CHECK(s.is_inside("abc"));
}

TEST_CASE("span<const char*>::is_inside returns false for a string equal to upper bound")
{
	char_span s{ "abc", "bcd" };
	CHECK(!s.is_inside("bcd"));
}

TEST_CASE("span<const char*>::is_inside_inclusive returns true for a string equal to upper bound")
{
	char_span s{ "abc", "bcd" };
	CHECK(s.is_inside_inclusive("bcd"));
}

TEST_CASE("span<const char*>::is_inside returns true for another span with bounds alphabetically inside current bounds")
{
	char_span s{ "abc", "bcd" };
	CHECK(s.is_inside(char_span{ "abd", "bcd" }));
}

TEST_CASE("span<const char*>::is_inside returns true for another span with same bounds")
{
	char_span s{ "abc", "bcd" };
	CHECK(s.is_inside(char_span{ "abc", "bcd" }));
}

TEST_CASE("span<const char*>::is_inside returns false for another span with bounds alphabetically outside current bounds")
{
	char_span s{ "abc", "bcd" };
	CHECK(!s.is_inside(char_span{ "aba", "bce" }));
	CHECK(!s.is_inside(char_span{ "aba", "bcd" }));
	CHECK(!s.is_inside(char_span{ "abc", "bce" }));
	CHECK(!s.is_inside(char_span{ "ab", "bcd" }));
	CHECK(!s.is_inside(char_span{ "abc", "bcda" }));
}
