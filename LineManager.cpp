// Vladislav Zolotukhin
// Date of completion: 2024-04-04
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "LineManager.h"
#include "Utilities.h"


namespace seneca {
	LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
		/*this constructor receives the name of the file that identifies the active stations on the assembly line */

		//Create an input file stream object using the name of the file (from the string& file parameter)
		std::ifstream f(file);
		// Utilities functions for file data extraction 
		Utilities u;
		size_t pos = 0;
		bool more = true;
		std::string record = ""; //need to pass a string into extractToken, not a file object

		//check if file name is valid and it exists
		if (f.is_open()) {
			while (!f.eof()) { //keep reading until reaching the end of the file
				std::getline(f, record); //extract a single line (record) from the file
				if (!record.empty()) {
					try {
						std::string token = u.extractToken(record, pos, more);
						std::string token2 = ""; //if there is another field in the record after the delimiter (ex: Nighttable|Desk)
						if (more) token2 = u.extractToken(record, pos, more);

						//reset the position
						pos = 0;//THIS IS SUPER IMPORTANT!! Otherwise it won't read any of the following records after the 1st
						//finding the workstations
						auto workstation = std::find_if(stations.begin(), stations.end(),
							[&](Workstation* ws) { return ws->getItemName() == token; });
						auto nextWorkstation = std::find_if(stations.begin(), stations.end(),
							[&](Workstation* ws) { return ws->getItemName() == token2; });
						//set the next workstation and add the current workstation to the vector
						if (workstation != stations.end()) {
							// If token2 is empty, there's no next workstation
							if (nextWorkstation != stations.end()) {
								(*workstation)->setNextStation(*nextWorkstation);
							}
							m_activeLine.push_back(*workstation);
						}
						//DEBUG:
						//std::cout << "Token: " << token << std::endl;
						//std::cout << "Token 2: " << token2 << std::endl;

						//The first workstation should be the only one that no other workstation points to as the next one
						//because it might be reordered and shouldn't be dependant on being the first record in the file
						for (auto workstation : m_activeLine) {
							bool isNextStation = false;
							for (auto otherStation : m_activeLine) {
								if (otherStation->getNextStation() == workstation) {
									isNextStation = true;
									break;
								}
							}
							if (!isNextStation) {
								m_firstStation = workstation;
								break;
							}
						}
						m_cntCustomerOrder++;

					}
					catch (...) {};
				}
			}
		}

	}

	void LineManager::reorderStations() {
		/* Reorders the workstations present in the instance variable m_activeLine (loaded by the constructor) and
		stores the reordered collection in the same instance variable. The elements in the reordered collection start with
		the first station, proceeds to the next, and so forth until the end of the line. */

		//create a temporary container:
		std::vector<Workstation*> temp;
		//starting with the first station, point toward it
		Workstation* start = m_firstStation;
		//go through each of the stations (each current workstation points to the next one) and push them into the
		//temp container in that order
		while (start != nullptr) {
			temp.push_back(start);
			start = start->getNextStation();
		}
		//reordered temp container becomes the active line
		m_activeLine = temp;

	}

	bool LineManager::run(std::ostream& os) {
		/* performs one iteration of operations on all of the workstations in the current assembly line by doing the following:
		 - keeps track of the current iteration number (using a local variable)
		 - inserts into stream os the iteration number (how many times this function has been called by the client) in the format Line Manager Iteration: COUNT<endl>
		 - moves the order at the front of the g_pending queue to the m_firstStation and remove it from the queue. This function moves only one order to the line 
		   on a single iteration.
		 - for each station on the line, executes one fill operation
		 - for each station on the line, attempts to move an order down the line
		 - return true if all customer orders have been filled or cannot be filled, otherwise returns false.*/

		static int count = 0; //my iteration variable
		os << "Line Manager Iteration: " << ++count << std::endl;

		//check if g_pending actually has any workstations, if so then move one of them to m_firstStation and pop from the queue

		if (!g_pending.empty()) {
			m_firstStation->operator+=(std::move(g_pending.front()));
			g_pending.pop_front();
		}
		std::for_each(m_activeLine.begin(), m_activeLine.end(), [&os](auto& ws) {
				ws->fill(os);
			});
		std::for_each(m_activeLine.begin(), m_activeLine.end(), [](auto& ws) {
			ws->attemptToMoveOrder();
			});
		
		//check if every single object in the container has been filled
		for (const auto& workstation : m_activeLine) {
			// Iterate through each CustomerOrder in the current workstation
			for (const auto& order : workstation->getOrders()) {
				// Check if the order is not filled
				if (!order.isOrderFilled()) {
					// If any order is not filled, return false immediately
					return false;
				}
			}
		}
		// If all orders are filled, return true
		return true;
	}

	void LineManager::display(std::ostream& os) const {
		//this query displays all active stations on the assembly line in their current order
		for (auto it = m_activeLine.begin(); it < m_activeLine.end(); ++it) {
			Workstation* ws = *it;
			ws->display(os);
		}
	}
}