#include "myglwidget03.h"
#include <QtDebug>

//GLSL3.0�汾��,������attribute�ؼ��֣��Լ�varying�ؼ��֣������Ա���ͳһ��in/out��Ϊǰ�ùؼ���
#define GL_VERSION			"#version 330 core\n"
#define GLCHA(x)			#@x           //�ӵ�����,��x��Ϊ�ַ�
#define GLSTR(x)			#x            //��˫���ţ���x��Ϊ�ַ���
#define GET_GLSTR(x)		GL_VERSION#x


const char *vsrc = GET_GLSTR(

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
}
);

const char *fsrc = GET_GLSTR(

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
void main()
{
	FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
	//FragColor = texture(ourTexture, TexCoord);
	//FragColor = vec4(ourColor, 1.0);
}
);


myGlWidget::myGlWidget(QWidget *parent) 
	:QOpenGLWidget(parent)
	, m_texture(nullptr)
{

}


void myGlWidget::paintGL()
{
	// ����
   // glViewport(0, 0, width(), height());

	glClear(GL_COLOR_BUFFER_BIT);

	// ��ȾShader
	vao.bind();      //�󶨼���vao
	m_texture->bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);    //����3������,��ʽΪ������

	//���
	m_texture->release();
	vao.release();       
}
void myGlWidget::initializeGL()
{
	// Ϊ��ǰ������ʼ��OpenGL����
	initializeOpenGLFunctions();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    //���ñ���ɫΪ��ɫ

	//��ʼ���������
	m_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);

	QImage imgTexture = QImage(QString::fromLocal8Bit("D:\\�ز�\\�ز�\\5a091f1c06a97ea0fb390c013905cad5.jpg"));
	m_texture->setData(imgTexture);
	m_texture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Nearest);
	//������С�ͷŴ�ķ�ʽ,��СͼƬ����LinearMipMapLinear���Թ���,��ʹ�ö༶��Զ�����ڽ�����,�Ŵ�ͼƬ����:Nearest�ڽ�����

	m_texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
	m_texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);

	//3.������ɫ������
	program = new QOpenGLShaderProgram;
	program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
	program->addShaderFromSourceCode(QOpenGLShader::Fragment,fsrc);

	program->link();
	program->bind();//����Program����


	//4.��ʼ��VBO,���������ݴ洢��buffer��,�ȴ�VAO���������ͷ�
	float vertices[] = {
	0.5f, -0.5f, 0.0f,		 1.0f,0.0f,0.0f,  2.0f,0.0f,
	-0.5f,-0.5f, 0.0f,		 0.0f,1.0f,0.0f,  0.0f,0.0f,
	0.0f,  0.5f,  0.0f,		 0.0f,0.0f,1.0f,  1.0f,2.0f
	};


	vbo.create();
	vbo.bind();              //�󶨵���ǰ��OpenGL������,
	vbo.allocate(vertices, sizeof(vertices));
	vbo.setUsagePattern(QOpenGLBuffer::StreamDraw);  //����Ϊһ���޸ģ����ʹ��


	//5.��ʼ��VAO,���ö�������״̬(���㣬���ߣ����������)
	vao.create();
	vao.bind();

	//offset:��һ�����ݵ�ƫ����
	//tupleSize:һ�������ж��ٸ�Ԫ��,����λ��Ϊxyz,��ɫΪrgb,������3
	//stride:����,�¸����ݾ��뵱ǰ���ݵ�֮�����,��������λ�ú�����λ��֮������:3��xyzֵ+3��rgbֵ+2������,�������� 8 * sizeof(float)
	program->setAttributeBuffer(0, GL_FLOAT, 0,					3,	8 * sizeof(float));	  //���ö�������
	program->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float),	3,	8 * sizeof(float));   //���ö�������
	program->setAttributeBuffer(2, GL_FLOAT, 6*sizeof(float),	2,	8 * sizeof(float));   //���ö�������


	program->enableAttributeArray(0); //ʹ�ܶ�������
	program->enableAttributeArray(1); //ʹ�ܶ�������
	program->enableAttributeArray(2); //ʹ�ܶ�������


	//6.������ж���
	vao.release();
	vbo.release();

}
void myGlWidget::resizeEvent(QResizeEvent *e)
{


}