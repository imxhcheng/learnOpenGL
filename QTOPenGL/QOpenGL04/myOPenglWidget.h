#pragma once

#include <QOpenGLWidget>
#include "Nv12Render.h"

const int g_width = 3264;
const int g_height = 2448;

class myOPenglWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	myOPenglWidget(QWidget *parent);
	~myOPenglWidget();

	void initializeGL(); //����������Ⱦ��ĳ�ʼ������
	void paintGL();//����������Ⱦ�����Ⱦ����

	uchar* readYuv(char* path);

private:
	Nv12Render	m_Nv12Render;

	uchar*		m_pNv12Ptr;
};
