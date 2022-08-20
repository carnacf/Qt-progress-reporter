#pragma once
#include "Common/ProgressReporter.h"

#include <QObject>

class SimpleProgressReporter : public QObject, public IProgressReporter
{
	Q_OBJECT
public:
	std::unique_ptr<ProgressHandler> startReport(const std::string& name, int nbSteps) const override;
	void updateProgress(const ProgressHandler* bar, int nbIncrement = 1) const override;
	void stopReport(const ProgressHandler* bar) const override;
	bool isCanceled() const override;

signals:
	void closed(int barId) const;
	void incremented(int nbSteps, int barId) const;
	void created(const ProgressHandler*) const;

private:
	bool m_isCanceled = false;

	friend class ProgressDialog;
};

