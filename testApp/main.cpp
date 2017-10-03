#include <base/FiniteStateMachine/fsm.h>

#include <iostream>


int main()
{
	std::cout << "Example app start:\n";	

	// Create FSM:
	std::shared_ptr<Base::IFsm> spFsm = Base::CFsm::Factory().CreateFiniteStateMachine();
	
	std::cout << "Example app end:\n";
}
