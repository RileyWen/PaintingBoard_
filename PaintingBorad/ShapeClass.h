#pragma once

#ifndef Shape_Class_H
#define Shape_Class_H

//enum of 3 type of shapes: LINE, CIRCLE, RECT
enum Shape { LINE_, CIRCLE_, RECT_, POLYGON_ };


class Base_shape {
protected:
	//主对角线两端点坐标
	POINT init_pos, last_pos;

public:
	//指向图形链表中下一个图形
	Base_shape *Next = NULL;

	//这个图形的形状
	Shape type;

	//以鼠标消息中含坐标信息的lParam初始化init_pos
	Base_shape(LPARAM&);

	//以两点坐标，及图形类型初始化
	Base_shape(POINT&, POINT&);

	//获取主对角线两端点坐标
	void GetPos(POINT&, POINT&) const;

	//设置偏移量
	virtual void SetShiftAmount(POINT& shift_amount, POINT& pos1_before_move, POINT& pos2_before_move);

	//以鼠标消息中含坐标信息的lParam设置last_pos
	virtual void SetLastPos(LPARAM&);

	//绘图接口，不同图形不同实现
	virtual void Paint(HDC&) const = 0;

	//判断鼠标是否落在当前图形上
	virtual bool isAboveShape(POINT&) const = 0;
};

class Line :public Base_shape {
public:
	Line(POINT& pos1, POINT& pos2) :Base_shape(pos1, pos2) {}
	Line(LPARAM& lParam) :Base_shape(lParam) { this->type = LINE_; }
	void Paint(HDC&) const;
	bool isAboveShape(POINT&) const;
};

class Rect :public Base_shape {
public:
	Rect(POINT& pos1, POINT& pos2) :Base_shape(pos1, pos2) {}
	Rect(LPARAM& lParam) :Base_shape(lParam) { this->type = RECT_; };
	void Paint(HDC&) const;
	bool isAboveShape(POINT&) const;
};

class Circle :public Base_shape {
public:
	double r = 0;
	Circle(POINT& pos1, POINT& pos2) :Base_shape(pos1, pos2) {}
	Circle(LPARAM& lParam) :Base_shape(lParam) { this->type = CIRCLE_; };
	void Paint(HDC&) const;
	bool isAboveShape(POINT&) const;
	void SetLastPos(LPARAM&);
};

class cPolygon :public Base_shape {
public:
	Line *head, *tail;
	int EdgeNum;

	cPolygon(POINT& pos1, POINT& pos2) :Base_shape(pos1, pos2) {
		this->type = POLYGON_;
	}
	cPolygon(LPARAM& lParam) :Base_shape(lParam) {
		this->init_pos.x = this->init_pos.y = this->last_pos.x = this->last_pos.y = 0;
		this->head = this->tail = new Line(lParam);
		this->type = POLYGON_;
		this->EdgeNum = 0;
	};
	void AddVertice(LPARAM& lParam);
	void AddLastVertice(LPARAM& lParam);
	void Paint(HDC&) const;
	bool isAboveShape(POINT&) const;
	void SetLastPos(LPARAM&);
};

//Convert LPARAM to POINT
POINT TranslatePos(_In_ LPARAM&);

//打开画板文件
void OpenPaintingBoard(_In_ HWND hWnd, _Inout_ Base_shape*& head);

//保存画板文件
void SavePaitingBoard(_In_ HWND hWnd, _In_ Base_shape* head);

#endif // !Shape_Class_H

