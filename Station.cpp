// Vladislav Zolotukhin
// Date of completion: 2024-03-11
//

#include <iostream>
#include <string>
#include "Station.h"
#include "Utilities.h"


namespace seneca {
	size_t Station::m_widthField = 0;
	size_t Station::id_generator = 0;
	Utilities u;

	Station::Station(const std::string& str) {
		//using the extractToken function from the Utilities class to read file data & assign Station variables
		size_t pos = 0;
		bool more = true;
		m_widthField = 0;

		//extract one field at a time
		//name of the item, starting serial number, quantity in stock, description
		//before extracting description, it updates Station::m_widthField
		//to the maximum value of Station::m_widthField and Utilities::m_widthField
		try {
			std::string token = u.extractToken(str, pos, more);
			if (more) m_name = token;
			token = u.extractToken(str, pos, more);
			if (more) m_serial = std::stoi(token);
			token = u.extractToken(str, pos, more);
			if (more) m_numItems = std::stoi(token);
			if (u.getFieldWidth() > m_widthField) m_widthField = u.getFieldWidth();
			token = str.substr(pos);
			token = trimWS(token);
			m_desc = token;
			m_id = ++id_generator;
			//increment the m_id for every instance of a Station object
		}
		catch (std::exception& e) {};
	}

	size_t Station::getNextSerialNumber() {
		return m_serial++;
	}

	void Station::display(std::ostream& os, bool full) const {
		//inserts information about the current object into stream os.
		//the ID field uses 3 characters, the NAME field uses m_widthField characters, 
		//the QUANTITY field uses 4 characters, the SERIAL field uses 6 characters; 
		//the DESCRIPTION has no formatting options (see the sample output for other formatting options)
		os.setf(std::ios::right);
		if (full) {
			//if the second parameter if true, this function inserts the information
			//in the following format: ID | NAME | SERIAL | QUANTITY | DESCRIPTION			
			os.width(3);
			os.fill('0');
			os << m_id << " | ";
			os.fill(' ');
			os.width(m_widthField);
			os.unsetf(std::ios::right);
			os.setf(std::ios::left);
			os << m_name << " | ";
			os.width(6);
			os.unsetf(std::ios::left);
			os.fill('0');
			os.setf(std::ios::right);
			os << m_serial << " | ";
			os.fill(' ');
			os.unsetf(std::ios::right);
			os.width(4);
			os << m_numItems << " | ";
			os.setf(std::ios::left);
			os << m_desc;

		}
		else {
			//if the second parameter is false, this function inserts only
			//the ID, name, and serial number in the format: ID | NAME | SERIAL | 
			os.width(3);
			os.fill('0');
			os << m_id << " | ";
			os.fill(' ');
			os.width(m_widthField);
			os.unsetf(std::ios::right);
			os.setf(std::ios::left);
			os << m_name << " | ";
			os.width(6);
			os.unsetf(std::ios::left);
			os.fill('0');
			os.setf(std::ios::right);
			os << m_serial << " | ";
			os.fill(' ');
		}
		os << std::endl;
	}
}