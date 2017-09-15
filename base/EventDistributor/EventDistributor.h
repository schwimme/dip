#pragma once


#include <functional>
#include <algorithm>
#include <vector>
#include <map>
#include <mutex>

struct IRegistrator
{
	virtual ~IRegistrator() {}
};

// KTTODO could be this private member of CEventDistributor?
namespace detail
{
	template<class ICallback, typename Context, class Distributor>
	struct CRegistrator:
		public IRegistrator
	{
		CRegistrator(ICallback* pCallback, Context ctx, Distributor& distributor):
			m_pCallback(pCallback),
			m_pDistributor(&distributor),
			m_ctx(ctx)
		{}

		~CRegistrator()
		{
			std::lock_guard<std::mutex> lock_c(m_pDistributor->m_lock_callback);
			std::vector<ICallback*>& callbackStorage = m_pDistributor->m_callbacks[m_ctx];

			// Find and erase registration:
			std::vector<ICallback*>::iterator i = std::find(callbackStorage.begin(), callbackStorage.end(), m_pCallback);
			callbackStorage.erase(i);

			// Check if is any callback invoked:
			std::lock_guard<std::mutex> lock_u(m_pDistributor->m_lock_unregister);
		}

	private:
		ICallback*		m_pCallback;
		Distributor*	m_pDistributor;
		Context			m_ctx;
	};
}


template<class ICallback, typename Context = int>
struct CEventDistributor
{
	IRegistrator* Register(ICallback& callback, Context ctx = Context());
	void NotifyAll(std::function<void(ICallback *)> func, Context ctx = Context());

private:
	using DistributorType = CEventDistributor<ICallback, Context>;

	// All registered callback
	std::map<Context, std::vector<ICallback*>> m_callbacks;

	std::mutex	m_lock_callback;
	std::mutex	m_lock_unregister;

	friend struct detail::CRegistrator<ICallback, Context, DistributorType>;
};


#include "eventdistributor.inl"
