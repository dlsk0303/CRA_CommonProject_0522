// 무엇을 TC로 만들어야할지 감이 잘 안와요 ㅠ.ㅠ
#if 0
#include "gmock/gmock.h"
#include <vector>
#include <string>
#include "fix_keyword.cpp"

using namespace testing;
using std::vector;
using std::string;

TEST(KeyWord, TC1) {
	//vector<string> strs = { "water", "monday" };
	SearchKeywordAndDayoftheweek strs = { "water", "monday" };

	//PerfectSearchKeywordCorrectionService app;
	//string actual = app.input(strs);
	//EXPECT_EQ(strs.keyword, actual);
}
#endif