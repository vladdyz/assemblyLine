// Vladislav Zolotukhin
// Date of completion: 2024-03-21
//

#ifndef SENECA_CUSTOMERORDER_H
#define SENECA_CUSTOMERORDER_H
#include <string>
#include "Station.h"


namespace seneca {

	struct Item
	{
		std::string m_itemName;
		size_t m_serialNumber{ 0 };
		bool m_isFilled{ false };

		Item(const std::string& src) : m_itemName(src) {};
	};

	class CustomerOrder {
		Item** m_lstItem{ nullptr };
		std::string m_name{ "" };
		std::string m_product{ "" };
		size_t m_cntItem{ 0 };
		static size_t m_widthField;
	public:
		CustomerOrder() {};
		CustomerOrder(const std::string&);
		CustomerOrder(const CustomerOrder& src) { throw std::runtime_error("Not allowed!"); };
		CustomerOrder operator=(const CustomerOrder& src) = delete;
		CustomerOrder(CustomerOrder&& src) noexcept;
		CustomerOrder& operator=(CustomerOrder&& src) noexcept;
		~CustomerOrder();
		bool isOrderFilled() const;
		bool isItemFilled(const std::string& itemName) const;
		void fillItem(Station& station, std::ostream& os);
		void display(std::ostream& os) const;






	};
	

}


#endif

