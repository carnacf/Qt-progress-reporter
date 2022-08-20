#pragma once

#include <string>

class IProgressReporter;

class ProgressHandler
{
public:
	ProgressHandler(const std::string& name, int nbSteps, int id, const IProgressReporter& reporter) noexcept;
	ProgressHandler(const ProgressHandler& toCopy) noexcept;
	ProgressHandler(ProgressHandler&& toMove) noexcept;
	~ProgressHandler() noexcept;

	int getId() const;
	std::string getName() const;
	int getNbSteps() const;

	void increment() const;

	void setAutoClose(bool close);

	ProgressHandler& operator++(int);
	ProgressHandler& operator=(const ProgressHandler& other) noexcept;
	ProgressHandler& operator=(ProgressHandler&& other) noexcept;

private:
	const IProgressReporter& m_reporter;

	std::string m_name;
	int m_id = -1;
	int m_nbSteps = 0;

	bool m_autoClose = true;
};