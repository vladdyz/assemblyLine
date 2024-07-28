// Vladislav Zolotukhin
// Date of completion: 2024-04-04
//

#ifndef SENECA_WORKSTATION_H
#define SENECA_WORKSTATION_H
#include <deque>
#include <string>
#include "CustomerOrder.h"
#include "Station.h"

namespace seneca {
	/*
	g_pending holds the orders to be placed onto the assembly line at the first station.
	g_completed holds the orders that have been removed from the last station and have been completely filled.
	g_incomplete holds the orders that have been removed from the last station and could not be filled completely.
	*/
	extern std::deque<CustomerOrder> g_pending;
	extern std::deque<CustomerOrder> g_completed;
	extern std::deque<CustomerOrder> g_incomplete;
 
	class Workstation : public Station {
		//cannot be copied or moved!
		//double ended queue
		std::deque<CustomerOrder> m_orders;
		Workstation* m_pNextStation{ nullptr };

	public:
		Workstation(const std::string& src) : Station(src) {};
		void fill(std::ostream& os);
		bool attemptToMoveOrder();
		void setNextStation(Workstation* station);
		Workstation* getNextStation() const { return m_pNextStation; };
		Workstation(const Workstation&) = delete;
		Workstation& operator=(const Workstation&) = delete;
		Workstation(Workstation&&) = delete;
		Workstation& operator=(Workstation&&) = delete;
		void display(std::ostream& os) const;
		Workstation& operator+=(CustomerOrder&& newOrder);
		std::deque<CustomerOrder>& getOrders() { return m_orders; }
	};
}

#endif
