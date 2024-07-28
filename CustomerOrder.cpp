// Vladislav Zolotukhin
// Date of completion: 2024-04-06
//

#include <string>
#include <cstring>
#include <iostream>
#include <utility>
#include <algorithm>

#include "CustomerOrder.h"
#include "Utilities.h"
#include "Station.h"

namespace seneca {
	size_t CustomerOrder::m_widthField = 0;
	Utilities u2;

	CustomerOrder::CustomerOrder(const std::string& src) {
		size_t pos = 0;
		bool more = true;
		

		//same as Station, call utilities for extraction
		//Customer name, order name, list of items in order
		try {
			std::string token = u2.extractToken(src, pos, more);
			m_name = token;
			if (more) {
				token = u2.extractToken(src, pos, more);
				m_product = token;
			}
			auto temppos = pos; //save the pos as a new temp variable for the next iteration
			//count how many items we need to create for this customer based on how many fields to extract
			while (more)
			{
				u2.extractToken(src, temppos, more);
				m_cntItem++;
			}
			//after, rewind to the original pos and reset more back to true, and create the new array of pointers
			this->m_lstItem = new Item*[m_cntItem];
			more = true;

			int i = 0; //for the dynamic array of items
			while (more) { 
				token = u2.extractToken(src, pos, more);
				this->m_lstItem[i] = new Item(token);
				i++;
			}
			if (m_widthField < u2.getFieldWidth()) m_widthField = u2.getFieldWidth();

		}
		catch (...) {};
	}

	CustomerOrder::CustomerOrder(CustomerOrder&& src) noexcept {
		*this = std::move(src);
	}

	CustomerOrder& CustomerOrder::operator=(CustomerOrder&& src) noexcept {
		if (this != &src) {
			//clean up current resource
			if (m_lstItem != nullptr) {
				for (size_t i = 0u; i < m_cntItem; i++)
					delete m_lstItem[i];
			}
			delete[] m_lstItem;
			//copy src into this
			m_name = src.m_name;
			m_product = src.m_product;
			m_cntItem = src.m_cntItem;
			m_lstItem = src.m_lstItem;

			src.m_lstItem = nullptr;
			src.m_cntItem = 0;
		}
		return *this;
	}

	CustomerOrder::~CustomerOrder() {
		if (m_lstItem) {
			for (size_t i = 0u; i < m_cntItem; i++)
				delete m_lstItem[i];
			delete[] m_lstItem;
		}
		m_lstItem = nullptr;
	}

	bool CustomerOrder::isOrderFilled() const {
		//returns true if all the items in the order have been filled; false otherwise
		bool allGood = true;
		for (size_t i = 0u; i < m_cntItem; i++) {
			if (!m_lstItem[i]->m_isFilled) allGood = false;
		}
		return allGood;
	}

	bool CustomerOrder::isItemFilled(const std::string& itemName) const {
		//returns true if all items specified by itemName have been filled. 
		//If the item doesn't exist in the order, this query returns true.
		bool allGood = true;
		for (size_t i = 0u; i < m_cntItem; i++) { 
			if (!strcmp(m_lstItem[i]->m_itemName.c_str(), itemName.c_str()) && !m_lstItem[i]->m_isFilled) allGood = false;
		}
		return allGood;
	}

	void CustomerOrder::fillItem(Station& station, std::ostream& os) {
		//check if the customer order has the item 
		//check by item name
		bool containItem = false;
		int index = 0;
		for (size_t i = 0u; i < m_cntItem; ++i) {
			if (station.getItemName() == m_lstItem[i]->m_itemName && m_lstItem[i]->m_isFilled == false) {
				containItem = true;
				index = i;
				//if the order contains the item and the station has it in the inventory
				if (containItem && station.getQuantity() > 0) {
					//then we subtract from the station inventory and fill the order
					station.updateQuantity();
					m_lstItem[index]->m_serialNumber = station.getNextSerialNumber();
					m_lstItem[index]->m_isFilled = true;
					os << "    Filled " << m_name << ", " << m_product << " [" << m_lstItem[index]->m_itemName << "]" << std::endl;
					break; //once we fill an item, we need to exit the loop. this function only fills one item at a time!
				}
				else if (containItem && station.getQuantity() == 0) {
					os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_lstItem[index]->m_itemName << "]" << std::endl;
					m_lstItem[index]->m_serialNumber = 0;
					m_lstItem[index]->m_isFilled = false;
				}
			}	
		}
	}

	void CustomerOrder::display(std::ostream& os) const {
		os << m_name << " - " << m_product << std::endl;
		for (size_t i = 0u; i < m_cntItem; i++) {
			os << "[";
			os.width(6);
			os.fill('0');
			os.setf(std::ios::right);
			os << m_lstItem[i]->m_serialNumber;
			os.unsetf(std::ios::right);
			os << "] ";
			os.fill(' ');
			//os.width(u2.getFieldWidth());
			os.width(m_widthField);
			os << m_lstItem[i]->m_itemName;
			os << " - ";
			if (m_lstItem[i]->m_isFilled) os << "FILLED";
			else os << "TO BE FILLED";
			os << std::endl;
		}
	}
}