#include "repeating_stream.hpp"

#include "CppUnitTest.h"

#include <istream>
#include <vector>
#include <format>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void fn(std::istream& is)
{

	auto s = std::string();
	std::getline(is, s);
}

namespace testrepeatingstream
{
	TEST_CLASS(SourceTests)
	{
	public:
		
		TEST_METHOD(RepeatCharacters5Times)
		{
			using Buffer_t = std::vector<char>;
			RepeatingInputStream<Buffer_t> stream(RepeatingInputSource<Buffer_t>( Buffer_t{ 'a', 'b', 'c' }, 5));

			auto s = std::string();
			stream >> s;
			
			Assert::AreEqual(std::string("abcabcabcabcabc"), s);
		}

		TEST_METHOD(RepeatCharacters1Times)
		{
			using Buffer_t = std::vector<char>;
			RepeatingInputStream<Buffer_t> stream(RepeatingInputSource<Buffer_t>(Buffer_t{ 'a', 'b', 'c' }, 1));

			auto s = std::string();
			stream >> s;

			Assert::AreEqual(std::string("abc"), s);
		}

		TEST_METHOD(RepeatCharacters0Times)
		{
			using Buffer_t = std::vector<char>;
			RepeatingInputStream<Buffer_t> stream(RepeatingInputSource<Buffer_t>(Buffer_t{ 'a', 'b', 'c' }, 0));

			auto s = std::string();
			stream >> s;

			Assert::AreEqual(std::string(""), s);
		}

		// This test is successful if it even builds.
		TEST_METHOD(RepeatingInputStreamCanBePassedAsIstream)
		{
			using Buffer_t = std::vector<char>;
			RepeatingInputStream<Buffer_t> stream(RepeatingInputSource<Buffer_t>(Buffer_t{ 'a', 'b', 'c' }, 5));

			auto s = std::string();
			stream >> s;

			fn(stream);
		}
	};
}
