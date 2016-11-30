#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class QOpenGLShaderProgram;

class MainWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
	Q_OBJECT

		
public:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

protected slots:
	void cleanGL();

private:
	// OpenGL State Information
	QOpenGLBuffer m_vertex;
	QOpenGLVertexArrayObject m_object;
	QOpenGLShaderProgram *m_program;

	// Private Helpers
	void printVersionInformation();
};


#endif // MAINWINDOW_H
