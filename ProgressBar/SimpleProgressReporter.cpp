#include "SimpleProgressReporter.h"

#include <functional>

std::unique_ptr<ProgressHandler> SimpleProgressReporter::startReport(const std::string& name, int nbSteps) const
{
	static size_t id = 0;
	auto handler = std::make_unique<ProgressHandler>(name, nbSteps, id++, *this);
	emit created(handler.get());

	return handler;
}

void SimpleProgressReporter::updateProgress(const ProgressHandler* bar, int nbIncrement) const
{
	emit incremented(nbIncrement, bar->getId());
}

void SimpleProgressReporter::stopReport(const ProgressHandler* bar) const
{
	emit closed(bar->getId());
}

bool SimpleProgressReporter::isCanceled() const
{
	return m_isCanceled;
}