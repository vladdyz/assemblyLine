// Vladislav Zolotukhin
// Date of completion: 2024-03-11
//

#ifndef SENECA_STATION_H
#define SENECA_STATION_H

namespace seneca {

	class Station {
		int m_id{ 0 };
		std::string m_name{ "" };
		std::string m_desc{ "" };
		size_t m_serial{ 0 };
		size_t m_numItems{ 0 };
		static size_t m_widthField;
		static size_t id_generator;
	public:
		Station(const std::string&);
		const std::string& getItemName() const { return m_name; };
		size_t getNextSerialNumber();
		size_t getQuantity() const { return m_numItems; };
		void updateQuantity() { if (m_numItems > 0) --m_numItems; };
		void display(std::ostream& os, bool full) const;
	};

}

#endif

