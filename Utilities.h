// Vladislav Zolotukhin
// Date of completion: 2024-03-11
//

#ifndef SENECA_UTILITIES_H
#define SENECA_UTILITIES_H

namespace seneca {

	class Utilities {
		size_t m_widthField{ 1 };
		static char m_delimiter;
	public:
		void setFieldWidth(size_t newWidth) { if (newWidth > 0) m_widthField = newWidth; };
		size_t getFieldWidth() const { return m_widthField; };
		std::string extractToken(const std::string& str, size_t& next_pos, bool& more);
		static void setDelimiter(char newDelimiter);
		static char getDelimiter() { return m_delimiter; };
		Utilities() {};
	};

	std::string trimWS(std::string& tokenstr);
}

#endif