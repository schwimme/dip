namespace Base
{


template<class ICallback, typename Context>
IRegistrator* CEventDistributor<ICallback, Context>::Register(ICallback& callback, Context ctx)
{
	std::lock_guard<std::mutex> h(m_lock_callback);
	
	std::vector<ICallback*>& callbackStorage = m_callbacks[ctx];
	
	auto iterator = callbackStorage.insert(callbackStorage.begin(), &callback);

	// Create registration:
	IRegistrator* pRegistrator = new (std::nothrow) detail::CRegistrator<ICallback, Context, DistributorType>(&callback, ctx, *this);

	// Error during register -> must be unregistered:
	if (!pRegistrator)
	{
		callbackStorage.erase(iterator);
	}

	return pRegistrator;
}


template<class ICallback, typename Context>
void CEventDistributor<ICallback, Context>::NotifyAll(std::function<void(ICallback *)> func, Context ctx)
{
	std::lock_guard<std::mutex> lock_c(m_lock_callback);
	std::lock_guard<std::mutex> lock_u(m_lock_unregister);

	// Create local copy:
	std::vector<ICallback*> callbackStorage = m_callbacks[ctx];

	// Allow to register:
	lock_c.Unlock();

	// Process callbacks:
	for (ICallback* i : callbackStorage)
	{
		func(i);
	}
}


}
