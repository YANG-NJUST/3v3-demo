#ifndef _UGV_H
#define _UGV_H

#include <adevs.h>
#include <regex>

#include "Coordinate.h"
#include "Data.h"

typedef adevs::PortValue<string> IO_Type;

struct StateMsg
{
	string		   camp;		// ��Ӫ
	string		   id;			// id

	// ����
	Position_ECEF  cur_pos;		 // λ��
	double		   speed;		 // �ٶ�
	Position_ECEF  aim_pos;		 // Ŀ���
	double         distance;	 // ��Ŀ���ľ���

	// ̽��
	double		   detect_range;	// ̽�ⷶΧ
	bool		   isDetect;		// ̽���豸״̬��1��������0���رգ�
	vector<Agent>  detect_agent;	// ̽�⵽��ƽ̨

	// ����
	Weapon         weapon;		// ����ϵͳ

};

class UGV : public adevs::Atomic<IO_Type>
{

public:

	UGV(string _camp, string _id, Position_ECEF _pos, double _speed, double _detect_range, Weapon _weapon);

	// ͨ�� Atomic �̳�
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

	Position_ECEF    pos;				// λ��
	Position_ECEF	 pos_start;		    // �������
	Position_ECEF    pos_end;			// �����յ� 
	double			  distance;			// ���� ���->Ŀ��� ����
	double			  move_step;		// ��������

	double			 blood;				// Ѫ��
	double			 speed;				// �����ٶ�
	double			 detect_range;		// ̽�ⷶΧ
	string			 camp;				// ��Ӫ
	string			 id;				// ���

	Weapon			 weapon;			// ����

	double			 time_step;

	StateMsg*		 state_msg;
};

#endif // !_UGV_H
