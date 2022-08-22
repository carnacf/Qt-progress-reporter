#pragma once
#include "SimpleProgressReporter.h"

#include <QDialog>
#include <QWidget>
#include <QProgressbar>
#include <QDialogButtonBox>
#include <QThread>

class QHBoxLayout;
class QVBoxLayout;
class QLabel;

class ProgressWidget : public QWidget
{
	Q_OBJECT;

public:
	ProgressWidget(const std::string& name, int nbSteps, QWidget* parent = nullptr);
	~ProgressWidget();

	void incrementBar(int nbStep);

private:
	int m_currentValue = 0;
	QProgressBar* m_bar = nullptr;
	QLabel* m_label = nullptr;
	QHBoxLayout* m_layout = nullptr;
};

class ProgressDialog : public QDialog
{
	Q_OBJECT;

public:
	ProgressDialog(QWidget* parent = nullptr);

	IProgressReporter& getReporter();

	template<class Function, class... Args>
	void exec(Function&& f, Args&&... args);

private:
	void removeProgressBar(int id);
	
	void insertProgressBar(const ProgressHandler* handler);
	
	void updateProgress(int id, int nbSteps = 1);


	SimpleProgressReporter m_handler;

	int m_firstBarId = 0;
	QVBoxLayout* m_barLayout = nullptr;
	QWidget* m_barsWidget = nullptr;
	QDialogButtonBox* m_buttonBox = nullptr;
	ProgressWidget* m_waitEndBar = nullptr;

	std::unordered_map<int, ProgressWidget*> m_bars;

	QThread* m_thread = nullptr;
};

template<class Function, class... Args>
inline void ProgressDialog::exec(Function&& f, Args &&... args)
{
	show();
	if (m_thread != nullptr)
	{
		//stop thread
		m_thread->exit();
		delete m_thread;
	}
	m_thread = QThread::create(f, args...);
	m_thread->start();
	QObject::connect(m_thread, &QThread::finished, this, &QDialog::close);
	QDialog::exec();
	m_thread->wait();
}
