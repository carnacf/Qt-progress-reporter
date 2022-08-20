#pragma once

#include <string>
#include <memory>

#include "ProgressHandler.h"

class IProgressReporter
{
public:
	virtual std::unique_ptr<ProgressHandler> startReport(const std::string& name, int nbSteps) const = 0;
	virtual void updateProgress(const ProgressHandler* bar, int nbIncrement = 1) const = 0;
	virtual void stopReport(const ProgressHandler* bar) const = 0;
	virtual bool isCanceled() const = 0;
};

class EmptyProgressReporter final : public IProgressReporter
{
public:
	std::unique_ptr<ProgressHandler> startReport(const std::string& /*name*/, int /*nbSteps*/) const override;
	void updateProgress(const ProgressHandler* /*bar*/, int /*nbIncrement = 1*/) const override;
	void stopReport(const ProgressHandler* /*bar*/) const override;
	bool isCanceled() const override;
};