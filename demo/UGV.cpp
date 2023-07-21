#include "UGV.h"


UGV::UGV(string _camp, string _id, Position_ECEF _pos, double _speed, double _detect_range, Weapon _weapon) :
	camp(_camp), id(_id), pos(_pos), speed(_speed), detect_range(_detect_range), weapon(_weapon)
{
	blood = 100.0;
	time_step = 1.0;
	state = State::Ready;
	state_msg = new StateMsg;
}

void UGV::delta_int()
{
	if (state == State::Move) {
		/*
		*	当已知机动目标点时
		*	计算下一时刻的位置坐标
		*	next_pos = cur_pos + ((v * delta_T) / distance) * (aim_pos - cur_pos)
		*	distance为起点到终点坐标的直线距离，LLH坐标无法直接求解两点的直线距离，需要转换为ECEF坐标计算直线距离
		*/
		pos = pos_start + (pos_end - pos_start) * ((speed * (move_step ++ ) * time_step) / distance);
		double tmp_distance = pos.calc_distance(pos_end);
		// 判断是否到达目标点(距离小于等于一个步长的里程)
		if (tmp_distance <= (speed * time_step)) {
			// 到达
			pos = pos_end;
			// 状态更新
			state = State::Ready;
		}
	}
}

void UGV::delta_ext(double e, const adevs::Bag<IO_Type>& xb)
{
	if (this->state == State::Dead) return;

	adevs::Bag<IO_Type>::const_iterator iter = xb.begin();
	for (; iter != xb.end(); iter++)
	{
		std::regex reg(",");
		std::string  msg = (*iter).value;
		std::sregex_token_iterator it(msg.begin(), msg.end(), reg, -1);
		decltype(it) end;
		auto type = it->str().c_str();		// 消息事件类型
		
		if ((*iter).port == this->in_order && strcmp(type, "order") == 0)  // 收到机动指令
		{
			// 机动指令消息格式"order, agent_id, x, y, z"
			auto agent_id = (++it)->str().c_str();
			if (strcmp(agent_id, this->id.c_str()))
			{
				double x = atof((++it)->str().c_str());
				double y = atof((++it)->str().c_str());
				double z = atof((++it)->str().c_str());
				this->pos_start = Position_ECEF(pos.get_x(), pos.get_y(), pos.get_z());
				this->pos_end = Position_ECEF(x, y, z);
				this->distance = pos_start.calc_distance(pos_end);
				this->move_step = 1;
				this->state = State::Move;
			}
		}
		else if ((*iter).port == this->in_friend && strcmp(type, "pos") == 0)  // 收到友方信息
		{
			
		}
		else if ((*iter).port == this->in_enemy)  // 收到敌方信息
		{
			
			if (strcmp(type, "pos") == 0)  // 敌方位置
			{

			}
			else if (strcmp(type, "attack") == 0)  // 敌方攻击
			{

			}

		}

	}

	


}

void UGV::delta_conf(const adevs::Bag<IO_Type>& xb)
{
	delta_int();
	delta_ext(0.0, xb);
}

void UGV::output_func(adevs::Bag<IO_Type>& yb)
{
	string msg = getState();
	IO_Type y_friend(out_friend, msg);
	IO_Type y_enemy(out_enemy, msg);
	yb.insert(y_friend);
	yb.insert(y_enemy);
}

double UGV::ta()
{
	return this->state == State::Dead ? INFINITY : time_step;
}

void UGV::gc_output(adevs::Bag<IO_Type>& g)
{

}

string UGV::getState()
{
	return camp + "," + pos.toString();
}

StateMsg* UGV::getCurrentStateMsg()
{
	state_msg->camp = this->camp;
	state_msg->id = this->id;
	state_msg->cur_pos = this->pos;
	state_msg->speed = this->speed;
	state_msg->aim_pos = this->pos_end;
	state_msg->distance = this->distance;

	state_msg->detect_range = this->detect_range;
	state_msg->isDetect = true;
	state_msg->detect_agent.emplace_back(Agent("camp", "id", Position_ECEF(0, 0, 0)));
	
	state_msg->weapon = this->weapon;

	return state_msg;
}
