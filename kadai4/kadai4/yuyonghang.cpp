#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include<cmath>
using namespace std;

/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
* go around the circle

**/

int corners[4][2] = {
	{ 0,0 },{ 0,19 },{ 34,19 },{ 34,0 }
};
int corner_hit = 0; // test if we have visited all four corners
int stat_index = 0; //0 means hit all the corners, 1 means finding the target, 2 means going to the target

class Point
{
public:
	int x, y;
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
	Point()
	{
		x = 0; y = 0;
	}
};

int next(int target[2], int now)
{
	int  stage = 0;
	for (; stage < 4; stage++)
	{
		if (target[0] == corners[stage][0] && target[1] == corners[stage][1])
		{
			corner_hit += 1;
			return (now + 1) % 4;
		}
	}
	return now;

}


int get_distance(Point x1, Point x2)
{

	return abs(x1.x - x2.x) + abs(x1.y - x2.y);
}
int get_area(Point x1, Point x2)
{
	return (1 + abs(x1.x - x2.x)) * (1 + abs(x1.y - x2.y));
}
int get_distance(int x, int y, int x1, int y1)
{

	return abs(x - x1) + abs(y - y1);
}
int get_area(int x, int y, int x1, int y1)
{
	return (1 + abs(x - x1)) * (1 + abs(y - y1));
}

void exchange(int &x, int&y)
{
	int temp;
	temp = x;
	x = y;
	y = temp;
}



int corner_area(Point target, Point corner, char  map[35][20])
{
	int start_x = target.x, end_x = corner.x;
	int start_y = target.y, end_y = corner.y;
	if (start_x > end_x) exchange(start_x, end_x);
	if (start_y > end_y) exchange(start_y, end_y);
	char free = '.';
	int area = get_area(start_x, start_y, end_x, end_y);
	for (int x = start_x; x <= end_x; x++)
	{
		for (int y = start_y; y <= end_y; y++)
		{
			char cur = map[x][y];
			if (cur == '.') continue;
			else if (cur == '0') area -= 1; // has already taken
			else {
				return 0; // if cannot form a  rectangle with the corner
			}
		}
	}
	return area;
}

int get_max_area_to_corner(Point target, char  map[35][20])
{
	int max_area = 0;
	int target_x = target.x;
	int target_y = target.y;
	for (int corner_index = 0; corner_index < 4; corner_index++)
	{
		int * cur_corner = corners[corner_index];
		int c_x = cur_corner[0], c_y = cur_corner[1];// get corner x and y
		if (!(map[c_x][c_y] == '0')) continue;
		int area = corner_area(target, Point(c_x, c_y), map);
		if (area > max_area)
		{
			max_area = area;
		}
	}
	return max_area;
}

Point find_new_target(char map[35][20], Point self_pos, vector<Point> opponents)
{
	// find new target with the objective funtion
	int objective[35][20];
	int max_x = 0, max_y = 0, max_value = -10000;
	for (int x = 0; x< 35; x++)
		for (int y = 0; y<20; y++)
		{
			char cur_label = map[x][y];
			if (!(cur_label == '.')) continue;
			int obj_value = 0;
			Point cur_point = Point(x, y);
			obj_value -= get_distance(cur_point, self_pos);
			for (vector<Point>::iterator it = opponents.begin(); it != opponents.end(); ++it)
			{
				Point cur_op = *it;
				obj_value += get_distance(cur_point, cur_op);
			}
			obj_value += get_max_area_to_corner(cur_point, map);

			if (obj_value > max_value)
			{
				max_value = obj_value;
				max_x = x;
				max_y = y;
			}
		}
	return Point(max_x, max_y);
}

int main()
{
	int opponentCount; // Opponent count
	cin >> opponentCount; cin.ignore();

	// game loop

	int stage = 0;
	Point target_point;
	while (1) {
		int gameRound;
		cin >> gameRound; cin.ignore();
		int x; // Your x position
		int y; // Your y position
		int backInTimeLeft; // Remaining back in time
		cin >> x >> y >> backInTimeLeft; cin.ignore();

		vector<Point> opponents;
		Point self_pos = Point(x, y);
		for (int i = 0; i < opponentCount; i++) {
			int opponentX; // X position of the opponent
			int opponentY; // Y position of the opponent
			int opponentBackInTimeLeft; // Remaining back in time of the opponent
			cin >> opponentX >> opponentY >> opponentBackInTimeLeft; cin.ignore();
			opponents.push_back(Point(opponentX, opponentY));

		}
		char cur_map[34][20];
		for (int i = 0; i < 20; i++) {
			string line; // One line of the map ('.' = free, '0' = you, otherwise the id of the opponent)
			cin >> line; cin.ignore();
			for (int j = 0; j<line.length(); j++)
			{
				cur_map[j][i] = line[j];

			}
		}
		if (stat_index == 0) {
			int target[2] = { x, y };
			stage = next(target, stage);
			cout << corners[stage][0] << " " << corners[stage][1] << endl;
			if (corner_hit == 5) stat_index = 1;
		}

		if (stat_index == 2)
		{
			// go to the target
			char target_label = cur_map[target_point.x][target_point.y];
			if (target_label == '.')
				cout << target_point.x << " " << target_point.y << endl;
			else
			{
				// ocupied by others or reached 
				stat_index = 1;
			}

		}

		if (stat_index == 1)
		{
			//find the new target
			target_point = find_new_target(cur_map, self_pos, opponents);
			cout << target_point.x << " " << target_point.y << endl;
			stat_index = 2;
		}

	}
}