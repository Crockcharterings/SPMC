/*
 *      Copyright (C) 2005-2010 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <stdlib.h>
#include <string.h>
#include <sstream>

#include "Variant.h"

using namespace std;

string trimRight(const string &str)
{
  string tmp = str;
  // find_last_not_of will return string::npos (which is defined as -1)
  // or a value between 0 and size() - 1 => find_last_not_of() + 1 will
  // always result in a valid index between 0 and size()
  tmp.erase(tmp.find_last_not_of(" \n\r\t") + 1);

  return tmp;
}

wstring trimRight(const wstring &str)
{
  wstring tmp = str;
  // find_last_not_of will return string::npos (which is defined as -1)
  // or a value between 0 and size() - 1 => find_last_not_of() + 1 will
  // always result in a valid index between 0 and size()
  tmp.erase(tmp.find_last_not_of(L" \n\r\t") + 1);

  return tmp;
}

int64_t str2int64(const string &str, int64_t fallback /* = 0 */)
{
  char *end = NULL;
  int64_t result = strtol(trimRight(str).c_str(), &end, 0);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

int64_t str2int64(const wstring &str, int64_t fallback /* = 0 */)
{
  wchar_t *end = NULL;
  int64_t result = wcstol(trimRight(str).c_str(), &end, 0);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

uint64_t str2uint64(const string &str, uint64_t fallback /* = 0 */)
{
  char *end = NULL;
  uint64_t result = strtoul(trimRight(str).c_str(), &end, 0);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

uint64_t str2uint64(const wstring &str, uint64_t fallback /* = 0 */)
{
  wchar_t *end = NULL;
  uint64_t result = wcstoul(trimRight(str).c_str(), &end, 0);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

double str2double(const string &str, double fallback /* = 0.0 */)
{
  char *end = NULL;
  double result = strtod(trimRight(str).c_str(), &end);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

double str2double(const wstring &str, double fallback /* = 0.0 */)
{
  wchar_t *end = NULL;
  double result = wcstod(trimRight(str).c_str(), &end);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

CVariant CVariant::ConstNullVariant = CVariant::VariantTypeConstNull;

CVariant::CVariant(VariantType type)
{
  m_type = type;

  switch (type)
  {
    case VariantTypeInteger:
      m_data.integer = 0;
      break;
    case VariantTypeUnsignedInteger:
      m_data.unsignedinteger = 0;
      break;
    case VariantTypeBoolean:
      m_data.boolean = false;
      break;
    case VariantTypeDouble:
      m_data.dvalue = 0.0;
      break;
    case VariantTypeString:
    case VariantTypeWideString:
    case VariantTypeArray:
    case VariantTypeObject:
    default:
      memset(&m_data, 0, sizeof(m_data));
      break;
  }
}

CVariant::CVariant(int integer)
{
  m_type = VariantTypeInteger;
  m_data.integer = integer;
}

CVariant::CVariant(int64_t integer)
{
  m_type = VariantTypeInteger;
  m_data.integer = integer;
}

CVariant::CVariant(unsigned int unsignedinteger)
{
  m_type = VariantTypeUnsignedInteger;
  m_data.unsignedinteger = unsignedinteger;
}

CVariant::CVariant(uint64_t unsignedinteger)
{
  m_type = VariantTypeUnsignedInteger;
  m_data.unsignedinteger = unsignedinteger;
}

CVariant::CVariant(double value)
{
  m_type = VariantTypeDouble;
  m_data.dvalue = value;
}

CVariant::CVariant(float value)
{
  m_type = VariantTypeDouble;
  m_data.dvalue = (double)value;
}

CVariant::CVariant(bool boolean)
{
  m_type = VariantTypeBoolean;
  m_data.boolean = boolean;
}

CVariant::CVariant(const char *str)
{
  m_type = VariantTypeString;
  m_string = str;
}

CVariant::CVariant(const char *str, unsigned int length)
{
  m_type = VariantTypeString;
  m_string = string(str, length);
}

CVariant::CVariant(const string &str)
{
  m_type = VariantTypeString;
  m_string = str;
}

CVariant::CVariant(const wchar_t *str)
{
  m_type = VariantTypeWideString;
  m_wstring = str;
}

CVariant::CVariant(const wchar_t *str, unsigned int length)
{
  m_type = VariantTypeWideString;
  m_wstring = wstring(str, length);
}

CVariant::CVariant(const wstring &str)
{
  m_type = VariantTypeWideString;
  m_wstring = str;
}

CVariant::CVariant(const std::vector<std::string> &strArray)
{
  m_type = VariantTypeArray;
  m_array.clear();
  for (unsigned int index = 0; index < strArray.size(); index++)
    m_array.push_back(strArray.at(index));
}

CVariant::CVariant(const CVariant &variant)
{
  m_type = variant.m_type;
  *this = variant;
}

bool CVariant::isInteger() const
{
  return m_type == VariantTypeInteger;
}

bool CVariant::isUnsignedInteger() const
{
  return m_type == VariantTypeUnsignedInteger;
}

bool CVariant::isBoolean() const
{
  return m_type == VariantTypeBoolean;
}

bool CVariant::isDouble() const
{
  return m_type == VariantTypeDouble;
}

bool CVariant::isString() const
{
  return m_type == VariantTypeString;
}

bool CVariant::isWideString() const
{
  return m_type == VariantTypeWideString;
}

bool CVariant::isArray() const
{
  return m_type == VariantTypeArray;
}

bool CVariant::isObject() const
{
  return m_type == VariantTypeObject;
}

bool CVariant::isNull() const
{
  return m_type == VariantTypeNull || m_type == VariantTypeConstNull;
}

CVariant::VariantType CVariant::type() const
{
  return m_type;
}

int64_t CVariant::asInteger(int64_t fallback) const
{
  switch (m_type)
  {
    case VariantTypeInteger:
      return m_data.integer;
    case VariantTypeUnsignedInteger:
      return (int64_t)m_data.unsignedinteger;
    case VariantTypeDouble:
      return (int64_t)m_data.dvalue;
    case VariantTypeString:
      return str2int64(m_string, fallback);
    case VariantTypeWideString:
      return str2int64(m_wstring, fallback);
    default:
      return fallback;
  }
  
  return fallback;
}

uint64_t CVariant::asUnsignedInteger(uint64_t fallback) const
{
  switch (m_type)
  {
    case VariantTypeUnsignedInteger:
      return m_data.unsignedinteger;
    case VariantTypeInteger:
      return (uint64_t)m_data.integer;
    case VariantTypeDouble:
      return (uint64_t)m_data.dvalue;
    case VariantTypeString:
      return str2uint64(m_string, fallback);
    case VariantTypeWideString:
      return str2uint64(m_wstring, fallback);
    default:
      return fallback;
  }
  
  return fallback;
}

double CVariant::asDouble(double fallback) const
{
  switch (m_type)
  {
    case VariantTypeDouble:
      return m_data.dvalue;
    case VariantTypeInteger:
      return (double)m_data.integer;
    case VariantTypeUnsignedInteger:
      return (double)m_data.unsignedinteger;
    case VariantTypeString:
      return str2double(m_string, fallback);
    case VariantTypeWideString:
      return str2double(m_wstring, fallback);
    default:
      return fallback;
  }
  
  return fallback;
}

float CVariant::asFloat(float fallback) const
{
  switch (m_type)
  {
    case VariantTypeDouble:
      return (float)m_data.dvalue;
    case VariantTypeInteger:
      return (float)m_data.integer;
    case VariantTypeUnsignedInteger:
      return (float)m_data.unsignedinteger;
    case VariantTypeString:
      return (float)str2double(m_string, fallback);
    case VariantTypeWideString:
      return (float)str2double(m_wstring, fallback);
    default:
      return fallback;
  }
  
  return fallback;
}

bool CVariant::asBoolean(bool fallback) const
{
  switch (m_type)
  {
    case VariantTypeBoolean:
      return m_data.boolean;
    case VariantTypeInteger:
      return (m_data.integer != 0);
    case VariantTypeUnsignedInteger:
      return (m_data.unsignedinteger != 0);
    case VariantTypeDouble:
      return (m_data.dvalue != 0);
    case VariantTypeString:
      if (m_string.empty() || m_string.compare("0") == 0 || m_string.compare("false") == 0)
        return false;
      return true;
    case VariantTypeWideString:
      if (m_wstring.empty() || m_wstring.compare(L"0") == 0 || m_wstring.compare(L"false") == 0)
        return false;
      return true;
    default:
      return fallback;
  }
  
  return fallback;
}

std::string CVariant::asString(const std::string &fallback /* = "" */) const
{
  switch (m_type)
  {
    case VariantTypeString:
      return m_string;
    case VariantTypeBoolean:
      return m_data.boolean ? "true" : "false";
    case VariantTypeInteger:
    case VariantTypeUnsignedInteger:
    case VariantTypeDouble:
    {
      std::ostringstream strStream;
      if (m_type == VariantTypeInteger)
        strStream << m_data.integer;
      else if (m_type == VariantTypeUnsignedInteger)
        strStream << m_data.unsignedinteger;
      else
        strStream << m_data.dvalue;
      return strStream.str();
    }
    default:
      return fallback;
  }
  
  return fallback;
}

std::wstring CVariant::asWideString(const std::wstring &fallback /* = L"" */) const
{
  switch (m_type)
  {
    case VariantTypeWideString:
      return m_wstring;
    case VariantTypeBoolean:
      return m_data.boolean ? L"true" : L"false";
    case VariantTypeInteger:
    case VariantTypeUnsignedInteger:
    case VariantTypeDouble:
    {
      std::wostringstream strStream;
      if (m_type == VariantTypeInteger)
        strStream << m_data.integer;
      else if (m_type == VariantTypeUnsignedInteger)
        strStream << m_data.unsignedinteger;
      else
        strStream << m_data.dvalue;
      return strStream.str();
      break;
    }
    default:
      return fallback;
  }
  
  return fallback;
}

CVariant &CVariant::operator[](const std::string &key)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeObject;
    m_map.clear();
  }

  if (m_type == VariantTypeObject)
    return m_map[key];
  else
    return ConstNullVariant;
}

const CVariant &CVariant::operator[](const std::string &key) const
{
  VariantMap::const_iterator it;
  if (m_type == VariantTypeObject && (it = m_map.find(key)) != m_map.end())
    return it->second;
  else
    return ConstNullVariant;
}

CVariant &CVariant::operator[](unsigned int position)
{
  if (m_type == VariantTypeArray && size() > position)
    return m_array[position];
  else
    return ConstNullVariant;
}

const CVariant &CVariant::operator[](unsigned int position) const
{
  if (m_type == VariantTypeArray && size() > position)
    return m_array.at(position);
  else
    return ConstNullVariant;
}

CVariant &CVariant::operator=(const CVariant &rhs)
{
  if (m_type == VariantTypeConstNull)
    return *this;

  m_type = rhs.m_type;

  switch (m_type)
  {
  case VariantTypeInteger:
    m_data.integer = rhs.m_data.integer;
    break;
  case VariantTypeUnsignedInteger:
    m_data.integer = rhs.m_data.unsignedinteger;
    break;
  case VariantTypeBoolean:
    m_data.boolean = rhs.m_data.boolean;
    break;
  case VariantTypeDouble:
    m_data.dvalue = rhs.m_data.dvalue;
    break;
  case VariantTypeString:
    m_string = rhs.m_string;
    break;
  case VariantTypeWideString:
    m_wstring = rhs.m_wstring;
    break;
  case VariantTypeArray:
    m_array.assign(rhs.m_array.begin(), rhs.m_array.end());
    break;
  case VariantTypeObject:
    m_map.clear();
    m_map.insert(rhs.m_map.begin(), rhs.m_map.end());
    break;
  default:
    break;
  }

  return *this;
}

bool CVariant::operator==(const CVariant &rhs) const
{
  if (m_type == rhs.m_type)
  {
    switch (m_type)
    {
    case VariantTypeInteger:
      return m_data.integer == rhs.m_data.integer;
    case VariantTypeUnsignedInteger:
      return m_data.unsignedinteger == rhs.m_data.unsignedinteger;
    case VariantTypeBoolean:
      return m_data.boolean == rhs.m_data.boolean;
    case VariantTypeDouble:
      return m_data.dvalue == rhs.m_data.dvalue;
    case VariantTypeString:
      return m_string == rhs.m_string;
    case VariantTypeWideString:
      return m_wstring == rhs.m_wstring;
    case VariantTypeArray:
      return m_array == rhs.m_array;
    case VariantTypeObject:
      return m_map == rhs.m_map;
    default:
      break;
    }
  }

  return false;
}

void CVariant::push_back(const CVariant &variant)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeArray;
    m_array.clear();
  }

  if (m_type == VariantTypeArray)
    m_array.push_back(variant);
}

void CVariant::append(const CVariant &variant)
{
  push_back(variant);
}

const char *CVariant::c_str() const
{
  if (m_type == VariantTypeString)
    return m_string.c_str();
  else
    return NULL;
}

void CVariant::swap(CVariant &rhs)
{
  VariantType  temp_type = m_type;
  VariantUnion temp_data = m_data;
  string       temp_string = m_string;
  wstring      temp_wstring = m_wstring;
  VariantArray temp_array = m_array;
  VariantMap   temp_map = m_map;

  m_type = rhs.m_type;
  m_data = rhs.m_data;
  m_string = rhs.m_string;
  m_wstring = rhs.m_wstring;
  m_array = rhs.m_array;
  m_map = rhs.m_map;

  rhs.m_type = temp_type;
  rhs.m_data = temp_data;
  rhs.m_string = temp_string;
  rhs.m_wstring = temp_wstring;
  rhs.m_array = temp_array;
  rhs.m_map = temp_map;
}

CVariant::iterator_array CVariant::begin_array()
{
  if (m_type == VariantTypeArray)
    return m_array.begin();
  else
    return iterator_array();
}

CVariant::const_iterator_array CVariant::begin_array() const
{
  if (m_type == VariantTypeArray)
    return m_array.begin();
  else
    return const_iterator_array();
}

CVariant::iterator_array CVariant::end_array()
{
  if (m_type == VariantTypeArray)
    return m_array.end();
  else
    return iterator_array();
}

CVariant::const_iterator_array CVariant::end_array() const
{
  if (m_type == VariantTypeArray)
    return m_array.end();
  else
    return const_iterator_array();
}

CVariant::iterator_map CVariant::begin_map()
{
  if (m_type == VariantTypeObject)
    return m_map.begin();
  else
    return iterator_map();
}

CVariant::const_iterator_map CVariant::begin_map() const
{
  if (m_type == VariantTypeObject)
    return m_map.begin();
  else
    return const_iterator_map();
}

CVariant::iterator_map CVariant::end_map()
{
  if (m_type == VariantTypeObject)
    return m_map.end();
  else
    return iterator_map();
}

CVariant::const_iterator_map CVariant::end_map() const
{
  if (m_type == VariantTypeObject)
    return m_map.end();
  else
    return const_iterator_map();
}

unsigned int CVariant::size() const
{
  if (m_type == VariantTypeObject)
    return m_map.size();
  else if (m_type == VariantTypeArray)
    return m_array.size();
  else if (m_type == VariantTypeString)
    return m_string.size();
  else if (m_type == VariantTypeWideString)
    return m_wstring.size();
  else
    return 0;
}

bool CVariant::empty() const
{
  if (m_type == VariantTypeObject)
    return m_map.empty();
  else if (m_type == VariantTypeArray)
    return m_array.empty();
  else if (m_type == VariantTypeString)
    return m_string.empty();
  else if (m_type == VariantTypeWideString)
    return m_wstring.empty();
  else
    return true;
}

void CVariant::clear()
{
  if (m_type == VariantTypeObject)
    m_map.clear();
  else if (m_type == VariantTypeArray)
    m_array.clear();
  else if (m_type == VariantTypeString)
    m_string.clear();
  else if (m_type == VariantTypeWideString)
    m_wstring.clear();
}

void CVariant::erase(const std::string &key)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeObject;
    m_map.clear();
  }
  else if (m_type == VariantTypeObject)
    m_map.erase(key);
}

void CVariant::erase(unsigned int position)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeArray;
    m_array.clear();
  }

  if (m_type == VariantTypeArray && position < size())
    m_array.erase(m_array.begin() + position);
}

bool CVariant::isMember(const std::string &key) const
{
  if (m_type == VariantTypeObject)
    return m_map.find(key) != m_map.end();

  return false;
}
