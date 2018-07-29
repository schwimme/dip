#pragma once


#include <functional>
#include <thread>
#include <list>
#include <condition_variable>


namespace sys
{
	

template<size_t N>
class thread_pool
{
public:
	using job_t = std::function<void()>;

public:
	thread_pool()
	{
		for (size_t i = 0; i < N; ++i)
		{
			m_workers[i] = std::thread([&] { thread_procedure(); });
		}
	}

	~thread_pool()
	{
		{
			std::lock_guard<std::mutex> l(m_jobs_lock);
			m_stopping = true;
		}

		m_jobs_cv.notify_all();

		for (size_t i = 0; i < N; ++i)
		{
			if (m_workers[i].joinable())
			{
				m_workers[i].join();
			}
		}
	}

	void push(job_t&& job)
	{
		{
			std::lock_guard<std::mutex> l(m_jobs_lock);
			if (m_stopping)
			{
				return;
			}

			m_jobs.push_back(std::move(job));
		}

		m_jobs_cv.notify_one();
	}

protected:
	void thread_procedure()
	{
		std::unique_lock<std::mutex> l(m_jobs_lock);
		m_jobs_cv.wait(l, [&] { return ((m_jobs.empty() == false) || m_stopping); });

		if (m_stopping)
		{
			// Flush jobs:
			return;
		}

		job_t j = m_jobs.front();
		m_jobs.pop_front();

		l.unlock();

		j();
	}

private:
	bool                    m_stopping = false;
	std::condition_variable m_jobs_cv;
	std::mutex              m_jobs_lock;
	std::list<job_t>        m_jobs;
	std::thread             m_workers[N];
};
	

}
