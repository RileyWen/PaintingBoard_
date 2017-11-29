#include "stdafx.h"
#include "ShapeClass.h"

POINT TranslatePos(_In_ LPARAM& lParam) {
	POINT pos;
	pos.x = LOWORD(lParam);
	pos.y = HIWORD(lParam);
	return pos;
}

void Rect::Paint(HDC& hdc) const
{
	RECT rect;
	if (this->init_pos.x > this->last_pos.x) {
		rect.left = this->last_pos.x;
		rect.right = this->init_pos.x;
	}
	else {
		rect.left = this->init_pos.x;
		rect.right = this->last_pos.x;
	}
	if (this->init_pos.y < this->last_pos.y) {
		rect.top = this->init_pos.y;
		rect.bottom = this->last_pos.y;
	}
	else {
		rect.top = this->last_pos.y;
		rect.bottom = this->init_pos.y;
	}
	MoveToEx(hdc, this->init_pos.x, this->init_pos.y, NULL);
	FrameRect(hdc,&rect, CreateSolidBrush(RGB(0, 0, 0)));
}

bool Rect::isAboveShape(POINT& pos) const
{
	if (((abs(pos.x - this->init_pos.x) <= 3 || abs(pos.x - this->last_pos.x) <= 3) && pos.y >= init_pos.y && pos.y <= last_pos.y)
		|| ((abs(pos.y - this->init_pos.y) <= 3 || abs(pos.y - this->last_pos.y) <= 3) && pos.x >= init_pos.x && pos.x <= last_pos.x))
		return true;
	else
		return false;
}

void Line::Paint(HDC& hdc) const
{
	MoveToEx(hdc, this->init_pos.x, this->init_pos.y, NULL);
	LineTo(hdc, this->last_pos.x, this->last_pos.y);
}

bool Line::isAboveShape(POINT& current_pos) const
{
	double x1 = this->init_pos.x, y1 = this->init_pos.y;
	double x2 = this->last_pos.x, y2 = this->last_pos.y;
	double x3 = current_pos.x, y3 = current_pos.y;
	double d = fabs(((x2 - x1)*(y3 - y1) - (x3 - x1)*(y2 - y1))
				/ sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)));
	if (((x3 - x1)*(x2 - x1) + (y3 - y1)*(y2 - y1)) < 0
		|| ((x3 - x2)*(x2 - x1) + (y3 - y2)*(y2 - y1)) > 0)
		return false;
	if (d < 3.0f)
		return true;
	else
		return false;
}

void Circle::Paint(HDC& hdc) const
{
	HBRUSH brush = (HBRUSH)GetStockObject(NULL_BRUSH);
	SelectObject(hdc, brush);
	MoveToEx(hdc, this->init_pos.x, this->init_pos.y, NULL);
	Ellipse(hdc, this->init_pos.x, this->init_pos.y, this->last_pos.x, this->last_pos.y);
}

bool Circle::isAboveShape(POINT& pos) const
{
	double rx = (this->init_pos.x + this->last_pos.x) / 2;	//Բ��x
	double ry = (this->init_pos.y + this->last_pos.y) / 2;	//Բ��y
	double x1 = pos.x;
	double y1 = pos.y;
	double d = sqrt((x1 - rx)*(x1 - rx) + (y1 - ry)*(y1 - ry));
	if (fabs(this->r - d) < 3.0f)
		return true;
	else
		return false;
}

void Circle::SetLastPos(LPARAM &lParam)
{
	POINT pos = TranslatePos(lParam);
	int d = (abs(this->init_pos.x - pos.x) > abs(init_pos.y - pos.y))
		? abs(this->init_pos.x - pos.x)
		: abs(init_pos.y - pos.y);
	this->r = double(d) / 2.0f;
	pos.x = (this->init_pos.x - pos.x) > 0 ? init_pos.x - d : init_pos.x + d;
	pos.y = (this->init_pos.y - pos.y) > 0 ? init_pos.y - d : init_pos.y + d;
	this->last_pos = pos;
}

Base_shape::Base_shape(LPARAM& lParam)
{
	this->init_pos = TranslatePos(lParam);
}

Base_shape::Base_shape(POINT& pos1, POINT& pos2)
{
	init_pos = pos1;
	last_pos = pos2;
}

void Base_shape::GetPos(POINT &pos1, POINT &pos2) const
{
	pos1 = this->init_pos;
	pos2 = this->last_pos;
}

void Base_shape::SetShiftAmount(POINT &shift, POINT& pos1, POINT& pos2)
{
	this->init_pos.x = shift.x + pos1.x;
	this->init_pos.y = shift.y + pos1.y;
	this->last_pos.x = shift.x + pos2.x;
	this->last_pos.y = shift.y + pos2.y;
}

void Base_shape::SetLastPos(LPARAM& lParam)
{
	this->last_pos = TranslatePos(lParam);
}

void OpenPaintingBoard(_In_ HWND hWnd, _Inout_ Base_shape*& head)
{
	TCHAR szFilePath[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������  
	ofn.lpstrFilter = L"All files\0*.*\0Painting Board File(*.ptb)\0*.ptb\0\0";//���ù���  
	ofn.nFilterIndex = 2;//����������  
	ofn.lpstrFile = szFilePath;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL  
	ofn.nMaxFile = sizeof(szFilePath);;//����������  
	ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ�� 
	ofn.lpstrTitle = TEXT("Please select a PaitingBoard File");//ʹ��ϵͳĬ�ϱ������ռ���  
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
	if (GetOpenFileName(&ofn))
	{
		FILE* fin;
		_wfopen_s(&fin, szFilePath, L"r,ccs=UNICODE");
		POINT init_pos, last_pos;
		double r;
		Shape shape;
		while (fwscanf_s(fin, L"%d %ld %ld %ld %ld",
			&shape,
			&init_pos.x, &init_pos.y,
			&last_pos.x, &last_pos.y) == 5) {
			if (!head) {
				switch (shape) {
				case LINE_:
					head = new Line(init_pos, last_pos);
					break;
				case RECT_:
					head = new Rect(init_pos, last_pos);
					break;
				case CIRCLE_:
					fwscanf_s(fin, L"%lf", &r);
					head = new Circle(init_pos, last_pos);
					((Circle *)head)->r = r;
					break;
				case POLYGON_: {
					head = new cPolygon(init_pos, last_pos);
					int EdgeNum;
					fwscanf_s(fin, L"%d", &EdgeNum);
					((cPolygon*)head)->EdgeNum = EdgeNum;
					fwscanf_s(fin, L"%ld%ld%ld%ld", &init_pos.x, &init_pos.y, &last_pos.x, &last_pos.y);
					((cPolygon*)head)->head = ((cPolygon*)head)->tail = new Line(init_pos, last_pos);
					for (int i = ((cPolygon*)head)->EdgeNum - 1; i > 0; i--) {
						fwscanf_s(fin, L"%ld%ld%ld%ld", &init_pos.x, &init_pos.y, &last_pos.x, &last_pos.y);
						((cPolygon*)head)->tail->Next = new Line(init_pos, last_pos);
						((cPolygon*)head)->tail = (Line *)(((cPolygon*)head)->tail->Next);
					}
				}
				}
			}
			else {
				Base_shape* cursor = head;
				while (cursor->Next)
					cursor = cursor->Next;

				switch (shape) {
				case LINE_:
					cursor->Next = new Line(init_pos, last_pos);
					break;
				case RECT_:
					cursor->Next = new Rect(init_pos, last_pos);
					break;
				case CIRCLE_:
					fwscanf_s(fin, L"%lf", &r);
					cursor->Next = new Circle(init_pos, last_pos);
					((Circle *)cursor->Next)->r = r;
					break;
				case POLYGON_: {
					cursor->Next = new cPolygon(init_pos, last_pos);
					cursor = cursor->Next;
					int EdgeNum;
					fwscanf_s(fin, L"%d", &EdgeNum);
					((cPolygon*)cursor)->EdgeNum = EdgeNum;
					fwscanf_s(fin, L"%ld%ld%ld%ld", &init_pos.x, &init_pos.y, &last_pos.x, &last_pos.y);
					((cPolygon*)cursor)->head=((cPolygon*)cursor)->tail=new Line(init_pos, last_pos);
					Line *line_cursor = ((cPolygon*)cursor)->head;
					for (int i = EdgeNum-1; i > 0; i--) {
						fwscanf_s(fin, L"%ld%ld%ld%ld", &init_pos.x, &init_pos.y, &last_pos.x, &last_pos.y);
						((cPolygon*)cursor)->tail->Next = new Line(init_pos, last_pos);
						((cPolygon*)cursor)->tail = (Line *)(((cPolygon*)cursor)->tail->Next);

						line_cursor = (Line *)line_cursor->Next;
					}
				}

				}
			}
		}
		fclose(fin);
	}
	else {
		MessageBox(hWnd, TEXT("File Unvalid"), NULL, MB_ICONERROR);
	}

}

void SavePaitingBoard(HWND hWnd, Base_shape * head)
{
	TCHAR szFilePath[MAX_PATH] = {0};
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������  
	ofn.lpstrFilter = L"All files\0*.*\0Painting Board File(*.ptb)\0*.ptb\0\0";//���ù���  
	ofn.nFilterIndex = 2;//����������  
	ofn.lpstrFile = szFilePath;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL  
	ofn.nMaxFile = sizeof(szFilePath);//����������  
	ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ�� 
	ofn.lpstrTitle = TEXT("Please select a PaitingBoard File to save");//ʹ��ϵͳĬ�ϱ������ռ���  
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;;//Ŀ¼������ڣ�����ֻ��ѡ��,��д����
	ofn.lpstrDefExt = L"ptb";
	if (GetSaveFileName(&ofn))
	{
		//MessageBox(hWnd, szFilePath, L"Sucess", MB_OK);
		
		if (head) {
			FILE* fout;
			_wfopen_s(&fout, szFilePath, L"w+,ccs=UNICODE");
			Base_shape *tmp_paint = head;
			POINT init_pos, last_pos;
			do {
				tmp_paint->GetPos(init_pos, last_pos);
				fwprintf(fout, L"%d %ld %ld %ld %ld ",tmp_paint->type, init_pos.x, init_pos.y, last_pos.x, last_pos.y);
				if (tmp_paint->type == CIRCLE_)
					fwprintf(fout, L"%lf ", ((Circle *)tmp_paint)->r);
				if (tmp_paint->type == POLYGON_) {
					fwprintf(fout, L"%d ", ((cPolygon*)tmp_paint)->EdgeNum);
					Line *line_cursor = ((cPolygon*)tmp_paint)->head;
					for (int i = ((cPolygon*)tmp_paint)->EdgeNum; i > 0; i--) {
						line_cursor->GetPos(init_pos, last_pos);
						fwprintf(fout, L"%ld %ld %ld %ld ", init_pos.x, init_pos.y, last_pos.x, last_pos.y);
						line_cursor = (Line *)line_cursor->Next;
					}
				}
				fwprintf(fout, L"\n");
			} while (tmp_paint = tmp_paint->Next);
			fclose(fout);
		}
		else
			MessageBox(NULL, TEXT("No shape is drawn!"), NULL, MB_ICONERROR);
	}
	else {
		MessageBox(NULL, TEXT("File Unvalid"), NULL, MB_ICONERROR);
	}
	
}

void cPolygon::AddVertice(LPARAM & lParam)
{
	this->EdgeNum++;
	this->tail->SetLastPos(lParam);
	POINT pos1, pos2;
	this->tail->GetPos(pos2, pos1);
	pos2 = TranslatePos(lParam);
	this->tail->Next = new Line(pos1, pos2);
	this->tail = (Line *)this->tail->Next;
}

void cPolygon::AddLastVertice(LPARAM & lParam)
{
	this->EdgeNum+=2;
	POINT pos1, pos2, unused;
	this->tail->GetPos(unused, pos1);
	this->head->GetPos(pos2, unused);
	this->tail->Next = new Line(pos1, pos2);
	this->tail = (Line *)this->tail->Next;
}

void cPolygon::Paint(HDC &hdc) const
{
	Line *cursor = this->head;
	while (cursor) {
		cursor->Paint(hdc);
		cursor = (Line *)cursor->Next;
	}
}

bool cPolygon::isAboveShape(POINT &pos) const
{
	Line *cursor = this->head;
	bool isAboveLine = false;
	while (cursor && !(isAboveLine = cursor->isAboveShape(pos)))
		cursor = (Line *)cursor->Next;
	return isAboveLine;
}

void cPolygon::SetLastPos(LPARAM &lParam)
{
	this->tail->SetLastPos(lParam);
}
