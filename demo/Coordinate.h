#ifndef _COORDINATE_H
#define _COORDINATE_H

#pragma once

#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>

const int R = 8500;		                      // km 地球的等效半径
const double G = 9.81;	                      // N 重力加速度
const double PI = acos(-1.0);                 // pi 3.1415926...

const double a = 6378137;                     // 长半轴 m
const double b = 6356752.3142;                // 短半轴 m
const double f = (a - b) / a;                 // f为极扁率
const double e_sq = f * (2 - f);              // e_sp为e^2 e为椭球偏心率


inline double toRadians(double degree) {
	return (degree * PI) / 180.0;
}

inline double toAngle(double rad) {
	return (rad * 180.0) / PI;
}

// LLH坐标系 经纬高 大地坐标
class Position_LLH {

public:
	Position_LLH() {}
	Position_LLH(double _lon, double _lat, double _hgt) :
		lon(_lon), lat(_lat), hgt(_hgt) {}
	~Position_LLH() {}

	// 运算符重载
	Position_LLH operator+(const Position_LLH& pos) {
		return Position_LLH(this->lon + pos.lon, this->lat + pos.lat, this->hgt + pos.hgt);
	}
	Position_LLH operator-(const Position_LLH& pos) {
		return Position_LLH(this->lon - pos.lon, this->lat - pos.lat, this->hgt - pos.hgt);
	}
	Position_LLH operator*(const double factor) {
		return Position_LLH(factor * this->lon, factor * this->lat, factor * this->hgt);
	}

	void print() {
		using namespace std;
		cout << "position_llh = (" << setiosflags(ios::fixed) << setprecision(3)
			<< this->lon << "," << this->lat << "," << this->hgt << ")" << endl;
	}

	double get_lon() { return lon; }
	double get_lat() { return lat; }
	double get_hgt() { return hgt; }

private:
	double		lon;		// longitude
	double		lat;		// latitude
	double		hgt;		// height
};


// ECEF坐标系 地心地固坐标系
class Position_ECEF {

public:
	Position_ECEF() {}
	Position_ECEF(double _x, double _y, double _z) :
		x(_x), y(_y), z(_z) {}
	~Position_ECEF() {}

	// 求解两点间距离
	double calc_distance(const Position_ECEF& pos) {
		return sqrt(pow(this->x - pos.x, 2) + pow(this->y - pos.y, 2) + pow(this->z - pos.z, 2));
	}

	void print() {
		using namespace std;
		cout << "position_ecef = (" << setiosflags(ios::fixed) << setprecision(3)
			<< this->x << "," << this->y << "," << this->z << ")" << endl;
	}

	Position_ECEF operator-(const Position_ECEF& pos) {
		return Position_ECEF(this->x - pos.x, this->y - pos.y, this->z - pos.z);
	}

	Position_ECEF operator+(const Position_ECEF& pos) {
		return Position_ECEF(this->x + pos.x, this->y + pos.y, this->z + pos.z);
	}

	Position_ECEF operator*(const double factor) {
		return  Position_ECEF(factor * this->x, factor * this->y, factor * this->z);
	}

	double get_x() { return x; }
	double get_y() { return y; }
	double get_z() { return z; }

	string toString() {
		return  to_string(x) + "," + to_string(y) + "," + to_string(z);
	}

private:
	double	x, y, z;       // km
};

// LLH -> ECEF
inline Position_ECEF convertCoordinate(Position_LLH& pos) {
	double sin_lat = sin(toRadians(pos.get_lat()));
	double cos_lat = cos(toRadians(pos.get_lat()));
	double sin_lon = sin(toRadians(pos.get_lon()));
	double cos_lon = cos(toRadians(pos.get_lon()));
	double N = a / sqrt(1 - e_sq * sin_lat * sin_lat);      // 基准椭球体的曲率半径

	double x = (pos.get_hgt() + N) * cos_lat * cos_lon;
	double y = (pos.get_hgt() + N) * cos_lat * sin_lon;
	double z = (pos.get_hgt() + (1 - e_sq) * N) * sin_lat;
	return Position_ECEF(x / 1000, y / 1000, z / 1000); // m->km
}

// ECEF -> LLH
inline Position_LLH convertCoordinate(Position_ECEF& pos) {
	double x = pos.get_x();
	double y = pos.get_y();
	double z = pos.get_z();
	double oe = sqrt(x * x + y * y);
	double lon = atan(y / x);   // 经度角
	double p = z;
	double lat = atan(p / oe);
	double sin_lat = sin(toRadians(lat));
	double N = a / sqrt(1 - e_sq * sin_lat * sin_lat);
	double p_update = z + N * e_sq * sin(toRadians(lat));
	while (abs(p_update - p) >= 1e6) {
		p = p_update;
		lat = atan(p / oe);
		sin_lat = sin(toRadians(lat));
		N = a / sqrt(1 - e_sq * sin_lat * sin_lat);
		p_update = z + N * e_sq * sin(toRadians(lat));
	}
	double hgt = sqrt(x * x + y * y + p * p) - N;
	return Position_LLH(toAngle(lon), toAngle(lat), hgt);
}



#endif // _COORDINATE_H