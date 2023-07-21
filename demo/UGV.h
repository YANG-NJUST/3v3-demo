#ifndef _UGV_H
#define _UGV_H

#include <adevs.h>
#include <regex>

#include "Coordinate.h"
#include "Data.h"

typedef adevs::PortValue<string> IO_Type;

struct StateMsg
{
	string		   camp;		// 阵营
	string		   id;			// id

	// 机动
	Position_ECEF  cur_pos;		 // 位置
	double		   speed;		 // 速度
	Position_ECEF  aim_pos;		 // 目标点
	double         distance;	 // 距目标点的距离

	// 探测
	double		   detect_range;	// 探测范围
	bool		   isDetect;		// 探测设备状态（1：开启，0：关闭）
	vector<Agent>  detect_agent;	// 探测到的平台

	// 武器
	Weapon         weapon;		// 武器系统

};

class UGV : public adevs::Atomic<IO_Type>
{

public:

	UGV(string _camp, string _id, Position_ECEF _pos, double _speed, double _detect_range, Weapon _weapon);

	// 通过 Atomic 继承
	virtual void delta_int() override;
	virtual void delta_ext(double e, const adevs::Bag<IO_Type>& xb) override;
	virtual void delta_conf(const adevs::Bag<IO_Type>& xb) override;
	virtual void output_func(adevs::Bag<IO_Type>& yb) override;
	virtual double ta() override;
	virtual void gc_output(adevs::Bag<IO_Type>& g) override;

	// inport
	static const int in_friend = 0;
	static const int in_enemy = 1;
	static const int in_order = 2;
	// outport
	static const int out_friend = 3;
	static const int out_enemy = 4;

	string getState();

	StateMsg* getCurrentStateMsg();

private:
	
	enum State { Ready, Move, Dead } state;

	Position_ECEF    pos;				// 位置
	Position_ECEF	 pos_start;		    // 机动起点
	Position_ECEF    pos_end;			// 机动终点 
	double			  distance;			// 机动 起点->目标点 距离
	double			  move_step;		// 机动步数

	double			 blood;				// 血量
	double			 speed;				// 机动速度
	double			 detect_range;		// 探测范围
	string			 camp;				// 阵营
	string			 id;				// 编号

	Weapon			 weapon;			// 武器

	double			 time_step;

	StateMsg*		 state_msg;
};

#endif // !_UGV_H
