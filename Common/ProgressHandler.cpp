#include "ProgressHandler.h"
#include "ProgressReporter.h"

ProgressHandler::ProgressHandler(const std::string& name, int nbSteps, int id, const IProgressReporter& reporter) noexcept
	:m_reporter(reporter), m_name(name), m_id(id), m_nbSteps(nbSteps)
{

}

ProgressHandler::ProgressHandler(const ProgressHandler& toCopy) noexcept
	:m_reporter(toCopy.m_reporter), m_name(toCopy.m_name), 
	m_id(toCopy.m_id), m_nbSteps(toCopy.m_nbSteps), m_autoClose(toCopy.m_autoClose)
{
}

ProgressHandler::ProgressHandler(ProgressHandler&& toMove) noexcept
	:m_reporter(toMove.m_reporter), m_name(toMove.m_name),
	m_id(toMove.m_id), m_nbSteps(toMove.m_nbSteps), m_autoClose(toMove.m_autoClose)
{
	toMove.m_name = "";
	toMove.m_id = -1;
	toMove.m_nbSteps = 0;
	toMove.m_autoClose = false;
}

ProgressHandler::~ProgressHandler()
{
	if (m_autoClose)
	{
		m_reporter.stopReport(this);
	}
}

int ProgressHandler::getId() const
{
	return m_id;
}

std::string ProgressHandler::getName() const
{
	return m_name;
}

int ProgressHandler::getNbSteps() const
{
	return m_nbSteps;
}

void ProgressHandler::increment() const
{
	m_reporter.updateProgress(this);
}

void ProgressHandler::setAutoClose(bool close)
{
	m_autoClose = close;
}

ProgressHandler& ProgressHandler::operator++(int)
{
	increment();
	return *this;
}

ProgressHandler& ProgressHandler::operator=(const ProgressHandler& other) noexcept
{
	if (this != &other)
	{
		//Bad design ?
		const_cast<IProgressReporter&>(m_reporter) = other.m_reporter;
		m_name = other.m_name;
		m_id = other.m_id;
		m_nbSteps = other.m_nbSteps;
		m_autoClose = other.m_autoClose;
	}

	return *this;
}

ProgressHandler& ProgressHandler::operator=(ProgressHandler&& other) noexcept
{
	if (this != &other)
	{
		const_cast<IProgressReporter&>(m_reporter) = other.m_reporter;
		m_name = other.m_name;
		m_id = other.m_id;
		m_nbSteps = other.m_nbSteps;
		m_autoClose = other.m_autoClose;

		other.m_name = "";
		other.m_id = -1;
		other.m_nbSteps = 0;
		other.m_autoClose = false;
	}

	return *this;
}
