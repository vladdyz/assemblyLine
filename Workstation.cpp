// Vladislav Zolotukhin
// Date of completion: 2024-04-06
//



#include <deque>
#include <iostream>
#include <string>
#include <utility>
#include "Workstation.h"

namespace seneca {
	std::deque<CustomerOrder> g_pending;
	std::deque<CustomerOrder> g_completed;
	std::deque<CustomerOrder> g_incomplete;


	void Workstation::fill(std::ostream& os) {
		// this modifier fills the order at the front of the queue if there are CustomerOrders in the queue; 
		//otherwise, does nothing.
		if (!m_orders.empty()) {
			m_orders.front().fillItem(*this, os);
		}

	}

	bool Workstation::attemptToMoveOrder() {
		/* attempts to move the order order at the front of the queue to the next station in the assembly line :
		   if the order requires no more service at this station or cannot be filled (not enough inventory), 
		   move it to the next station; otherwise do nothing
		   if there is no next station in the assembly line, then the order is moved into g_completed or g_incomplete queue
		   if an order has been moved, return true; false otherwise. */
		if (!m_orders.empty()) {
			if (m_orders.front().isItemFilled(this->getItemName()) || this->getQuantity() == 0) {
				if (m_pNextStation != nullptr) {
					m_pNextStation->operator+=(std::move(m_orders.front()));
				}
				else {
					//move to g_completed or g_incomplete
					if (m_orders.front().isOrderFilled()) {
						g_completed.push_back(std::move(m_orders.front()));
					}
					else {
						g_incomplete.push_back(std::move(m_orders.front()));
					}
				}
			
				m_orders.pop_front();
				return true;
			}
			//std::cout << "Why are you seeing this?" << std::endl; //Debug

		}
		return false;
	}

	void Workstation::setNextStation(Workstation* station = nullptr) {
		//this modifier stores the address of the referenced Workstation object in the pointer to the m_pNextStation. 
		//Parameter defaults to nullptr.
		m_pNextStation = station;
	}

	void Workstation::display(std::ostream& os) const {
		/* this query inserts the name of the Item for which the current object is responsible
		   into stream os following the format: ITEM_NAME --> NEXT_ITEM_NAME
		   if the current object is the last Workstation in the assembly line this query inserts: ITEM_NAME --> End of Line.
		   in either case, the message is terminated with \n */
		if (m_pNextStation != nullptr) {
			os << getItemName() << " --> " << m_pNextStation->getItemName() << "\n";
		}
		else {
			os << getItemName() << " --> End of Line" << "\n";
		}
	}

	Workstation& Workstation::operator+=(CustomerOrder&& newOrder) {
		//moves the CustomerOrder referenced in parameter newOrder to the back of the queue.
		m_orders.push_back(std::move(newOrder));
		return *this;
	}

}