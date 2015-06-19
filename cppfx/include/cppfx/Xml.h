#pragma once
#ifndef __CPPFX_XML_H
#define __CPPFX_XML_H

#include <cppfx/rapidxml.h>
#include <cppfx/string.h>

namespace cppfx
{
	namespace xml
	{
		typedef rapidxml::xml_node<> node;
		typedef rapidxml::xml_attribute<> attribute;
		typedef rapidxml::xml_document<> document;

		inline string getAttribValue(rapidxml::xml_attribute<>* const & attrib)
		{
			if (attrib != nullptr) {
				auto ptr = attrib->value();
				auto len = attrib->value_size();

				return string(ptr, ptr + len);
			}
			return "";
		}
		template<typename Node>
		//, rapidxml::xml_attribute<>* const & attrib
		inline int getAttribInt(Node& node, const string& name)
		{
			auto attrib = node->first_attribute(name.data(), name.size());
			auto ptr = attrib->value();
			auto len = attrib->value_size();
			string s(ptr, ptr + len);
			int i = atoi(get_string_buffer(s));
			return i;
		}
	}
}

#endif