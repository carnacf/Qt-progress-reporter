#include "ProgressReporter.h"

std::unique_ptr<ProgressHandler> EmptyProgressReporter::startReport(const std::string& /*name*/, int /*nbSteps*/) const
{
	return std::make_unique<ProgressHandler>("", 0, 0, *this);
}

void EmptyProgressReporter::updateProgress(const ProgressHandler* /*bar*/, int /*nbIncrement = 1*/) const
{

}

void EmptyProgressReporter::stopReport(const ProgressHandler* /*bar*/) const
{

}

bool EmptyProgressReporter::isCanceled() const
{
	return false;
}