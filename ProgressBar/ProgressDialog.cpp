#include "ProgressDialog.h"

#include <QtWidgets>
#include <qdialogbuttonbox.h>

ProgressWidget::ProgressWidget(const std::string& name, int nbSteps, QWidget* parent) : QWidget(parent)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(new QLabel(QString::fromStdString(name), this));

	m_bar = new QProgressBar(this);
	m_bar->setRange(0, nbSteps);
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
	m_currentValue += nbStep;
	m_bar->setValue(m_currentValue);
}

ProgressDialog::ProgressDialog(QWidget* parent) : QDialog(parent)
{
	m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel, this);
	setWindowFlag(Qt::WindowCloseButtonHint);
	QVBoxLayout* mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	//Bar displayed when cancel.
	m_waitEndBar = new ProgressWidget("Waiting for process to end: ", 0, this);
	mainLayout->addWidget(m_waitEndBar);
	m_waitEndBar->hide();

	m_barsWidget = new QWidget();
	mainLayout->addWidget(m_barsWidget);
	mainLayout->addWidget(m_buttonBox);

	m_barLayout = new QVBoxLayout(m_barsWidget);
	m_barsWidget->setLayout(m_barLayout);

	setModal(true);

	QObject::connect(&m_handler, &SimpleProgressReporter::created, this, &ProgressDialog::insertProgressBar);
	QObject::connect(&m_handler, &SimpleProgressReporter::incremented, this, &ProgressDialog::updateProgress);
	QObject::connect(&m_handler, &SimpleProgressReporter::closed, this, &ProgressDialog::removeProgressBar);
	QObject::connect(m_buttonBox, &QDialogButtonBox::rejected, [this]() {
		m_handler.m_isCanceled = true;
		m_waitEndBar->show();
		m_barsWidget->hide();
		m_buttonBox->hide();
	});
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

	m_barLayout->addWidget(prgWidget);
}

void ProgressDialog::updateProgress(int nbSteps, int id)
{
	if (m_bars.find(id) != m_bars.end())
	{
		m_bars[id]->incrementBar(nbSteps);
	}
}
