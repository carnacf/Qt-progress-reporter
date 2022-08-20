#include "ProgressDialog.h"

#include <QtWidgets>

ProgressWidget::ProgressWidget(const std::string& name, int nbSteps, QWidget* parent) : QWidget(parent)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(new QLabel(QString::fromStdString(name), this));

	m_bar = new QProgressBar(this);
	m_bar->setMaximum(nbSteps - 1);
	layout->addWidget(m_bar);
}

ProgressWidget::~ProgressWidget()
{
	delete m_bar;
	delete m_label;
	delete m_layout;
}

void ProgressWidget::incrementBar(int nbStep)
{
	m_bar->setValue(m_bar->value() + nbStep);
}

ProgressDialog::ProgressDialog(QWidget* parent) : QDialog(parent)
{
	m_mainLayout = new QVBoxLayout();
	setLayout(m_mainLayout);

	setModal(true);

	QObject::connect(&m_handler, &SimpleProgressReporter::created, this, &ProgressDialog::insertProgressBar);
	QObject::connect(&m_handler, &SimpleProgressReporter::incremented, this, &ProgressDialog::updateProgress);
	QObject::connect(&m_handler, &SimpleProgressReporter::closed, this, &ProgressDialog::removeProgressBar);
	/*QObject::connect(this, &ProgressDialog::canceled, [this]() {
		m_handler.m_isCanceled = true;
	});*/
}

IProgressReporter& ProgressDialog::getReporter()
{
	return m_handler;
}

void ProgressDialog::removeProgressBar(int id)
{
	if (m_bars.find(id) != m_bars.end())
	{
		delete m_bars.at(id);
		m_bars.erase(id);
	}

	if (id == m_firstBarId)
	{
		close();
	}
}

void ProgressDialog::insertProgressBar(const ProgressHandler* handler)
{
	if (m_bars.empty())
	{
		m_firstBarId = handler->getId();
	}

	ProgressWidget* prgWidget = new ProgressWidget(handler->getName(), handler->getNbSteps(), this);
	m_bars.insert({ handler->getId(), prgWidget });

	m_mainLayout->addWidget(prgWidget);
}

void ProgressDialog::updateProgress(int nbSteps, int id)
{
	if (m_bars.find(id) != m_bars.end())
	{
		m_bars[id]->incrementBar(nbSteps);
	}
}
