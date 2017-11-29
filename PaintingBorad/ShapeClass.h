#pragma once

#ifndef Shape_Class_H
#define Shape_Class_H

//enum of 3 type of shapes: LINE, CIRCLE, RECT
enum Shape { LINE_, CIRCLE_, RECT_, POLYGON_ };


class Base_shape {
protected:
	//���Խ������˵�����
	POINT init_pos, last_pos;

public:
	//ָ��ͼ����������һ��ͼ��
	Base_shape *Next = NULL;

	//���ͼ�ε���״
	Shape type;

	//�������Ϣ�к�������Ϣ��lParam��ʼ��init_pos
	Base_shape(LPARAM&);

	//���������꣬��ͼ�����ͳ�ʼ��
	Base_shape(POINT&, POINT&);

	//��ȡ���Խ������˵�����
	void GetPos(POINT&, POINT&) const;

	//����ƫ����
	virtual void SetShiftAmount(POINT& shift_amount, POINT& pos1_before_move, POINT& pos2_before_move);

	//�������Ϣ�к�������Ϣ��lParam����last_pos
	virtual void SetLastPos(LPARAM&);

	//��ͼ�ӿڣ���ͬͼ�β�ͬʵ��
	virtual void Paint(HDC&) const = 0;

	//�ж�����Ƿ����ڵ�ǰͼ����
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

//�򿪻����ļ�
void OpenPaintingBoard(_In_ HWND hWnd, _Inout_ Base_shape*& head);

//���滭���ļ�
void SavePaitingBoard(_In_ HWND hWnd, _In_ Base_shape* head);

#endif // !Shape_Class_H

