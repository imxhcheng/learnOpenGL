#include "myglwidget.h"
#include <QtDebug>

//GLSL3.0�汾��,������attribute�ؼ��֣��Լ�varying�ؼ��֣������Ա���ͳһ��in/out��Ϊǰ�ùؼ���
#define GL_VERSION  "#version 330 core\n"
#define GLCHA(x)  #@x           //�ӵ�����,��x��Ϊ�ַ�
#define GLSTR(x)  #x            //��˫���ţ���x��Ϊ�ַ���
#define GET_GLSTR(x) GL_VERSION#x


const char *vsrc = GET_GLSTR(
layout(location = 0) in vec3 aPos;
void main(void)
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
);

const char *fsrc = GET_GLSTR(

out vec4 FragColor;
void main(void)
{
	FragColor = vec4(1.0f,1.0f,0.0f,1.0f);
}
);

//fsrc �ȼ��� -->   const char *fsrc ="#version 330 core\n"
//                      "out vec4 FragColor;\n"
//                      "void main()\n"
//                      "{\n"
//                      "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
//                      "}\n\0";

myGlWidget::myGlWidget(QWidget *parent) :QOpenGLWidget(parent)
{

}


void myGlWidget::paintGL()
{
	// ����
   // glViewport(0, 0, width(), height());

	glClear(GL_COLOR_BUFFER_BIT);

	// ��ȾShader
	program->bind(); //�󶨼���Program����
	vao.bind();      //�󶨼���vao
	glDrawArrays(GL_TRIANGLES, 0, 3);    //����3������,��ʽΪ������
	//glDrawArrays(GL_LINE_LOOP, 0, 3);    //����3������,��ʽΪ������

	vao.release();       //���
	program->release();  //���

}
void myGlWidget::initializeGL()
{

	// Ϊ��ǰ������ʼ��OpenGL����
	initializeOpenGLFunctions();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    //���ñ���ɫΪ��ɫ

	//1.����������ɫ��
	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	vshader->compileSourceCode(vsrc);

	//2.����ƬԪ��ɫ�� rgba(1.0f, 1.0f, 0.0f, 1.0f)��ʾ��ɫ,��alphaֵΪ1.0,��ʾ��ȫ��͸��
	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	fshader->compileSourceCode(fsrc);

	//3.������ɫ������
	program = new QOpenGLShaderProgram;
	program->addShader(vshader);
	program->addShader(fshader);
	program->link();
	program->bind();//����Program����


	//4.��ʼ��VBO,���������ݴ洢��buffer��,�ȴ�VAO���������ͷ�
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
	};

	vbo.create();
	vbo.bind();              //�󶨵���ǰ��OpenGL������,
	vbo.allocate(vertices, 9 * sizeof(GLfloat));
	vbo.setUsagePattern(QOpenGLBuffer::StreamDraw);  //����Ϊһ���޸ģ����ʹ��


	//5.��ʼ��VAO,���ö�������״̬(���㣬���ߣ����������)
	vao.create();
	vao.bind();

	GLint aPos = program->attributeLocation("aPos");        //��ȡaPosλ��
	if (aPos == -1)  //δ�ҵ�
	{
		return;
	}
	program->setAttributeBuffer(aPos, GL_FLOAT, 0, 3, 0);   //���ö�������
	program->enableAttributeArray(aPos); //ʹ�ܶ�������


	//6.������ж���
	vao.release();
	vbo.release();
	program->release();


}
void myGlWidget::resizeEvent(QResizeEvent *e)
{


}