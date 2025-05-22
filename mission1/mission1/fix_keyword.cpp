#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#define INPUTFILE "keyword_weekday_500.txt"
#define OUTPUTFILE "result.txt"

#define MAX_NUM 2100000000
#define BEST_TWO 2

enum WEEK {
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	SUNDAY,
	MAX_DAY
};

struct SearchKeywordAndDayoftheweek {
	string keyword;
	string week;
};

struct SearchItem {
	string name;
	double point;

	bool operator<(const SearchItem& other) const {
		return point < other.point;
	}
};

vector<SearchItem> weekBest[MAX_DAY]; //월 ~ 금
vector<SearchItem> twoBest[BEST_TWO]; //평일, 주말
int UZ = 9;

// 레벤슈타인 거리 계산 알고리즘 (문자열 유사도 검사)
int levenshtein(const std::string& a, const std::string& b) {
	const size_t len_a = a.size();
	const size_t len_b = b.size();

	std::vector<std::vector<int>> d(len_a + 1, std::vector<int>(len_b + 1));

	for (size_t i = 0; i <= len_a; ++i) d[i][0] = i;
	for (size_t j = 0; j <= len_b; ++j) d[0][j] = j;

	for (size_t i = 1; i <= len_a; ++i) {
		for (size_t j = 1; j <= len_b; ++j) {
			if (a[i - 1] == b[j - 1])
				d[i][j] = d[i - 1][j - 1];
			else
				d[i][j] = 1 + std::min({ d[i - 1][j], d[i][j - 1], d[i - 1][j - 1] });
		}
	}
	return d[len_a][len_b];
}

// 점수 환산
bool similer(const std::string& a, const std::string& b) {
	if (a.empty() && b.empty()) return true; // 둘 다 empty면 100%임. true.
	if (a.empty() || b.empty()) return false; // 둘 중 하나만 empty면, 0%임. false로 판단.

	int dist = levenshtein(a, b);
	int max_len = std::max(a.length(), b.length());
	// 유사도 비율 (1.0: 완전히 같음, 0.0: 전혀 다름)
	double similarity = 1.0 - (double)dist / max_len;

	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= 80) return true;
	return false;
}

void sortTwoBest(int day_of_the_week)
{
	std::sort(twoBest[day_of_the_week].begin(), twoBest[day_of_the_week].end());
}

void sortWeekBest(int day_of_the_week)
{
	std::sort(weekBest[day_of_the_week].begin(), weekBest[day_of_the_week].end());
}

int setDayOfTheWeek(std::string& wk)
{
	if (wk == "monday") return MONDAY;
	if (wk == "tuesday") return TUESDAY;
	if (wk == "wednesday") return WEDNESDAY;
	if (wk == "thursday") return THURSDAY;
	if (wk == "friday") return FRIDAY;
	if (wk == "saturday") return SATURDAY;
	if (wk == "sunday") return SUNDAY;
}

void reAlignment(double max_weekBest, double max_twoBest) {
	//재정렬 작업
	if (UZ >= MAX_NUM || max_weekBest >= MAX_NUM || max_twoBest >= MAX_NUM) {
		UZ = 9;
		for (int i = 0; i < MAX_DAY; i++) {
			int num = 1;
			for (auto it = weekBest[i].rbegin(); it != weekBest[i].rend(); ++it) {
				SearchItem& node = *it;
				node.point = num;
				num++;
			}
		}
		for (int i = 0; i < BEST_TWO; i++) {
			int num = 1;
			// rbegin()과 rend()를 사용한 역순 순회
			// vector<SearchItem> twoBest[BEST_TWO];
			for (auto it = twoBest[i].rbegin(); it != twoBest[i].rend(); ++it) {
				SearchItem& node = *it;
				node.point = num;
				num++;
			}
		}
	}
}

int perfectHit(int day_of_the_week, string keyword, int is_weekend) {
	//관리 목록에 존재하는지 확인
	//관리되는 키워드이면 점수가 증가

	double max_weekBest = 0;
	double max_twoBest = 0;

	int flag = 0;
	for (SearchItem& node : weekBest[day_of_the_week]) {
		if (node.name == keyword) {
			max_weekBest = node.point + (node.point * 0.1);
			node.point += (node.point * 0.1);
			flag = 1;
			break;
		}
	}

	for (SearchItem& node : twoBest[is_weekend]) {
		if (node.name == keyword) {
			max_twoBest = node.point + (node.point * 0.1);
			node.point += (node.point * 0.1);
			flag = 1; // 누락되어 있어서 추가함
			break;
		}
	}

	reAlignment(max_weekBest, max_twoBest);

	return flag;
}

int bestHit(int day_of_the_week, string keyword) {
	//찰떡 HIT
	int flag = 0;
	for (SearchItem& node : weekBest[day_of_the_week]) {
		if (similer(node.name, keyword)) {
			flag = 1;
		}
	}

	for (SearchItem& node : twoBest[day_of_the_week]) {
		if (similer(node.name, keyword)) {
			flag = 1;
		}
	}
	return flag;
}

void notPerfectNotBestHit(int day_of_the_week, string keyword) {
	//완벽 HIT / 찰떡 HIT 둘다 아닌경우
	double point = UZ;
	if (weekBest[day_of_the_week].size() < 10) {
		weekBest[day_of_the_week].push_back({ keyword, point });
		sortWeekBest(day_of_the_week);
	}

	if (twoBest[day_of_the_week].size() < 10) {
		twoBest[day_of_the_week].push_back({ keyword, point });
		sortTwoBest(day_of_the_week);
	}

	if (weekBest[day_of_the_week].size() >= 10) {
		if (weekBest[day_of_the_week].back().point < point) {
			weekBest[day_of_the_week].pop_back();
			weekBest[day_of_the_week].push_back({ keyword, point });
			sortWeekBest(day_of_the_week);
		}
	}

	if (twoBest[day_of_the_week].size() >= 10) {
		if (twoBest[day_of_the_week].back().point < point) {
			twoBest[day_of_the_week].pop_back();
			twoBest[day_of_the_week].push_back({ keyword, point });
			sortTwoBest(day_of_the_week);
		}
	}
}

string input2(string keyword, string wk) {
	UZ++;

	int day_of_the_week = setDayOfTheWeek(wk);

	//평일 / 주말
	int is_weekend = 0;
	if (day_of_the_week >= MONDAY && day_of_the_week <= FRIDAY) is_weekend = 0;
	else is_weekend = 1;

	if (perfectHit(day_of_the_week, keyword, is_weekend)) {
		return keyword;
	}

	if (bestHit(day_of_the_week, keyword)) {
		return keyword;
	}

	notPerfectNotBestHit(day_of_the_week, keyword);

	return keyword;
}

void inputfromFile() {
	ifstream inputfile{ INPUTFILE }; //500개 데이터 입력
	if (!inputfile.is_open())
		return;

	for (int i = 0; i < 500; i++) {
		SearchKeywordAndDayoftheweek searched_data;
		inputfile >> searched_data.keyword >> searched_data.week;
		string ret = input2(searched_data.keyword, searched_data.week);
		std::cout << ret << std::endl;
	}
	inputfile.close();
}

int main() {
	inputfromFile();
}
