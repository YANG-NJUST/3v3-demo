#ifndef _DATA_H
#define _DATA_H

struct Weapon
{
	string		name;
	double		min_range;
	double		max_range;
	double		kill;
	int			num;
	Weapon(string _name, double _min_range, double _max_range, double _kill, int _num):
		name(_name), min_range(_min_range), max_range(_max_range), kill(_kill), num(_num){}
	Weapon() {}
};

struct Agent
{
	string				camp;
	string				id;
	Position_ECEF		pos;
	Agent(string _camp, string _id, Position_ECEF _pos) :
		camp(_camp), id(_id), pos(_pos) {}
	Agent() {}
};

struct MoveOrder
{

};




#endif // !_DATA_H

