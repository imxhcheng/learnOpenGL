#include "myglwidget02.h"
#include <QtDebug>


//GLSL3.0�汾��,������attribute�ؼ��֣��Լ�varying�ؼ��֣������Ա���ͳһ��in/out��Ϊǰ�ùؼ���
#define GL_VERSION  "#version 330 core\n"
#define GLCHA(x)  #@x           //�ӵ�����,��x��Ϊ�ַ�
#define GLSTR(x)  #x            //��˫���ţ���x��Ϊ�ַ���
#define GET_GLSTR(x) GL_VERSION#x


const char *vsrc = GET_GLSTR(

layout(location = 0) in vec3 aPos;		//λ�ñ���������λ��ֵΪ0
layout(location = 1) in vec3 aColor;	//��ɫ����������λ��ֵΪ1
out vec3 ourColor;		//��Ƭ����ɫ�����һ����ɫ

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
}
);

const char *fsrc = GET_GLSTR(

out vec4 FragColor;
in vec3 ourColor;
void main(void)
{
	FragColor = vec4(ourColor, 1.0f);
}
);


myGlWidget::myGlWidget(QWidget *parent) :QOpenGLWidget(parent)
{

}


void myGlWidget::paintGL()
{
	// ����
   // glViewport(0, 0, width(), height());

	glClear(GL_COLOR_BUFFER_BIT);

	// ��ȾShader
	vao.bind();      //�󶨼���vao
	glDrawArrays(GL_TRIANGLES, 0, 3);    //����3������,��ʽΪ������
	//glDrawArrays(GL_LINE_LOOP, 0, 3);    //����3������,��ʽΪ������

	vao.release();       //���

}
void myGlWidget::initializeGL()
{
	//0. Ϊ��ǰ������ʼ��OpenGL����
	initializeOpenGLFunctions();

	int nAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttributes);
	qDebug() << "Maximum of vertex attributes supported:" << nAttributes;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    //���ñ���ɫΪ��ɫ


	//1.������ɫ������
	program = new QOpenGLShaderProgram;
	program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
	program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
	program->link();
	program->bind();//����Program����


	//2.��ʼ��VBO,���������ݴ洢��buffer��,�ȴ�VAO���������ͷ�
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,	1.0f,0.0f,0.0f,
	0.5f, -0.5f, 0.0f,	0.0f,1.0f,0.0f,
	0.0f, 0.5f, 0.0f,	0.0f,0.0f,1.0f,
	};

	vbo.create();
	vbo.bind();              //�󶨵���ǰ��OpenGL������,
	vbo.allocate(vertices, 18 * sizeof(GLfloat));
	vbo.setUsagePattern(QOpenGLBuffer::StreamDraw);  //����Ϊһ���޸ģ����ʹ��

	//3.��ʼ��VAO,���ö�������״̬(���㣬���ߣ����������)
	vao.create();
	vao.bind();

	program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
	program->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float), 3, 6 * sizeof(float));

	program->enableAttributeArray(0);
	program->enableAttributeArray(1);

	//6.������ж���
	vao.release();
	vbo.release();

}
void myGlWidget::resizeEvent(QResizeEvent *e)
{


}