// Vladislav Zolotukhin
// Date of completion: 2024-03-11
//

#include <iostream>
#include <string>
#include <sstream>
#include "Utilities.h"

namespace seneca {
	char Utilities::m_delimiter = '\0';

	std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
		//uses the delimiter to extract the next token from str starting at position next_pos
		//If successful, return a copy of the extracted token found (without spaces at the beginning/end)
		//update next_pos with the position of the next token, and set more to true (false otherwise).
		//reports an exception if a delimiter is found at next_pos
		//updates the current object's m_widthField data member if its current value is less than the size of the token extracted.
		std::string token, line;
		//first check that the string position is OK (not greater than the length or pointing to a delimiter)
		//try {
			if (next_pos >= str.length()) {
				more = false;
				throw std::runtime_error("   ERROR: No token.");
			}

			//Try to get the next delimiter
			size_t delimiter = str.find(m_delimiter, next_pos);

			//If we find a delimiter, extract the token string from next_pos until the delimiter and update next_pos
			if (delimiter != std::string::npos) {
				token = str.substr(next_pos, delimiter - next_pos);
				//update the bool to true, next_pos, and field width if necessary
				next_pos = delimiter + 1; 
				more = true;
			} else {
				//otherwise, read the string from next pos until the end of the line (this caused me lots of problems)
				line = str.substr(next_pos);
				std::stringstream newStr(line);
				std::getline(newStr, token, m_delimiter);
				//update the bool to true, next_pos, and field width if necessary
				more = false;
				next_pos = str.length();

			}
			//modify the token to trim any leading or trailing whitespace
			//std::cout << "Token : " << token << std::endl; //debug
			//If the token is empty, there's been an uncaught error. We don't want to trim or display it!
			if (token.length() > 0) token = trimWS(token);
			else {
				more = false;
				next_pos = str.length() + 1;
				throw std::runtime_error("   ERROR: No token.");

			}
			if (token.length() > m_widthField) m_widthField = token.length();
			//return the token string if successful, otherwise throw an exception
			if (token.length() > 0) return token;
			else return "";
		//}
		//catch (std::exception& e) {
			//more = false;
			//next_pos = str.length() + 1;
			//std::cerr << e.what() << std::endl;			
		//}
			//By removing the try...catch block in this function, the exceptions being thrown are instead
			//propagated up the call stack to be caught by the catch block in the ms1.cpp loop
			//This way, empty token strings aren't being printed after the "ERROR: No Token" messages like
			//they were when the error catching was handled locally in this function, because the iterator
			//in ms1.cpp would continue to execute the code block with the cout statements instead of the catch.
		
	}

	void Utilities::setDelimiter(char newDelimiter) {
		m_delimiter = newDelimiter;
	}


	//This function takes a reference to a string, will trim any leading or trailing whitespace from the string
	//Afterward, it will return the trimmed string
	//Borrowed from my Toy module in WS04
	std::string trimWS(std::string& tokenstr) {
		std::string trimmed;
		size_t first, last;
		if (tokenstr[0] != '\0') {
			//searching for the index of the first non-whitespace character
			first = tokenstr.find_first_not_of(" \t\n\r");
			last = tokenstr.find_last_not_of(" \t\n\r");
			//create a substring out of the token string using the first and last indexes
			trimmed = tokenstr.substr(first, last - first + 1);
			return trimmed; //return the trimmed string so it could be assigned to the name attribute
		}
		else return "Invalid string!"; //if the string has a length of 0 it hasn't been initialized

	}
}