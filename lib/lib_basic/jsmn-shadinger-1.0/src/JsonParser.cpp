/*
  JsonParser.cpp - lightweight JSON parser

  Copyright (C) 2020  Stephan Hadinger

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "JsonParser.h"
#include <Arduino.h>

/*********************************************************************************************\
 * Utilities
\*********************************************************************************************/

const char * k_current_json_buffer = "";

/*********************************************************************************************\
 * Lightweight String to Float, because atof() or strtof() takes 10KB
 * 
 * To remove code, exponents are not parsed
 * (commented out below, just in case we need them after all)
\*********************************************************************************************/
// Inspired from https://searchcode.com/codesearch/view/22115068/
float json_strtof(const char* s) {
  const char*  p     = s;
  float        value = 0.;
  int32_t      sign  = +1;
  float        factor;
  // unsigned int expo;

  while (isspace(*p)){    // skip any leading white-spaces
    p++;
  }

  switch (*p) {
    case '-': sign = -1;
    case '+': p++;
    default : break;
  }

  while ((unsigned int)(*p - '0') < 10u) {
    value = value*10 + (*p++ - '0');  
  }

  if (*p == '.' ) {
    factor = 1.0f;

    p++;
    while ((unsigned int)(*p - '0') < 10u) {
      factor *= 0.1f;
      value  += (*p++ - '0') * factor;
    }
  }

//   if ( (*p | 32) == 'e' ) {
//       expo   = 0;
//       factor = 10.L;

//       switch (*++p) {                 // ja hier weiß ich nicht, was mindestens nach einem 'E' folgenden MUSS.
//       case '-': factor = 0.1;
//       case '+': p++;
//                 break;
//       case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
//                 break;
//       default : value = 0.L;
//                 p     = s;
//                 goto done;
//       }

//       while ( (unsigned int)(*p - '0') < 10u )
//           expo = 10 * expo + (*p++ - '0');

//       while ( 1 ) {
//           if ( expo & 1 )
//               value *= factor;
//           if ( (expo >>= 1) == 0 )
//               break;
//           factor *= factor;
//       }
//   }

// done:
  // if ( endptr != NULL )
  //     *endptr = (char*)p;

  return value * sign;
}

/*********************************************************************************************\
 * Read-only JSON token object, fits in 32 bits
\*********************************************************************************************/

void JsonParserToken::skipToken(void) {
  // Serial.printf("skipToken type = %d   %s\n", t->type, t->start);
  switch (t->type) {
    case JSMN_OBJECT:
      skipObject();
      break;
    case JSMN_ARRAY:
      skipArray();
      break;
    case JSMN_STRING:
    case JSMN_PRIMITIVE:
    case JSMN_KEY:
    case JSMN_NULL:
    case JSMN_BOOL_FALSE:
    case JSMN_BOOL_TRUE:
    case JSMN_FLOAT:
    case JSMN_INT:
    case JSMN_UINT:
      t++;        // skip 1 token
      break;
    case JSMN_INVALID:
    default:
      break;      // end of stream, stop advancing
  }
}

void JsonParserToken::skipArray(void) {
  if (t->type == JSMN_ARRAY) {
    size_t obj_size = t->size;
    t++;    // array root
    if (t->type == JSMN_INVALID) { return; }
    for (uint32_t i=0; i<obj_size; i++) {
      skipToken();
    }
  }
}

// skip to right after the current Object
void JsonParserToken::skipObject(void) {
  if (t->type == JSMN_OBJECT) {
    size_t obj_size = t->size;
    t++;    // object root
    if (t->type == JSMN_INVALID) { return; }
    for (uint32_t i=0; i<obj_size; i++) {
      t++;    // skip Key
      if (t->type == JSMN_INVALID) { return; }
      skipToken();
    }
  }
}

/*********************************************************************************************\
 * JsonParserArray
\*********************************************************************************************/

JsonParserArray::JsonParserArray(const jsmntok_t * token) : JsonParserToken(token) {
  if (t->type != JSMN_ARRAY) {
    t = &token_bad;
  }
}
JsonParserArray::JsonParserArray(const JsonParserToken token) : JsonParserToken(token.t) {
  if (t->type != JSMN_ARRAY) {
    t = &token_bad;
  }
}

JsonParserArray::const_iterator::const_iterator(const JsonParserArray t): tok(t), remaining(0) {
  if (tok.t == &token_bad) { tok.t = nullptr; }
  if (nullptr != tok.t) {
    // ASSERT type == JSMN_ARRAY by constructor
    remaining = tok.t->size;
    tok.nextOne();    // skip array root token
  }
}

JsonParserArray::const_iterator JsonParserArray::const_iterator::const_iterator::operator++() {
  if (remaining <= 1) { tok.t = nullptr; }
  else {
    remaining--;
    tok.skipToken();  // munch value
    if (tok.t->type == JSMN_INVALID) { tok.t = nullptr; }   // unexpected end of stream
  }
  return *this;
}

JsonParserToken JsonParserArray::operator[](int32_t i) const {
  if ((i >= 0) && (i < t->size)) {
    uint32_t index = 0;
    for (const auto elt : *this) {
      if (i == index) {
        return elt;
      }
      index++;
    }
  }
  // fallback
  return JsonParserToken(&token_bad);
}

#ifdef ENABLE_DEVFEATURE_JSONPAIR
/*********************************************************************************************\
 * JsonPair
\*********************************************************************************************/

JsonPair::JsonPair(const jsmntok_t * token) : JsonParserToken(token) {
  Serial.println("JsonPair token"); //delay(4000);
  if (t->type != JSMN_OBJECT) {
    t = &token_bad;
  }
}

JsonPair::JsonPair(const JsonParserToken token) : JsonParserToken(token.t) {
  
    Serial.printf("sizeB1=%d\n\r",t->size);


  Serial.println("JsonPair token.t");//delay(4000);
  if (t->type != JSMN_OBJECT) { 
    Serial.println("JsonPair != JSMN_OBJECT");//delay(4000);
  
    t = &token_bad;
  }
   Serial.println("JsonPair == JSMN_OBJECT");//delay(4000);
  
}

JsonPair::const_iterator::const_iterator(const JsonPair t): tok(t), remaining(0) {
  Serial.println("const_iterator 1");
  if (tok.t == &token_bad) { tok.t = nullptr; }
  if (nullptr != tok.t) {
    // ASSERT type == JSMN_ARRAY by constructor
    remaining = tok.t->size*2;
    tok.nextOne();    // skip array root token
  }
}

JsonPair::const_iterator JsonPair::const_iterator::const_iterator::operator++() {
  if (remaining <= 1) { tok.t = nullptr; }
  else {
    remaining--;
    tok.skipToken();  // munch value
    if (tok.t->type == JSMN_INVALID) { tok.t = nullptr; }   // unexpected end of stream
    // token_pair.key = tok;
  }
  // if (remaining <= 1) { tok.t = nullptr; }
  // else {
  //   remaining--;
  //   tok.skipToken();  // munch value
  //   if (tok.t->type == JSMN_INVALID) { tok.t = nullptr; }   // unexpected end of stream
  //   token_pair.value = tok;
  // }
  return *this;
}

// JsonParserKey JsonParserObject::getFirstElement(void) const {
//   if (t->size > 0) {
//     return JsonParserKey(t+1);    // return next element and cast to Key
//   } else {
//     return JsonParserKey(&token_bad);
//   }
// }
/**
 * @brief This function will need to iter across all possible pairs until "i" setting the key/value token
 * 
 * @param i 
 * @return JsonPair2 
 */
JsonPair2 JsonPair::getObjectPair(uint8_t i)
{
    Serial.printf("getObjectPair sizeB1=%d\n\r",t->size);
//  if (remaining <= 1) { tok.t = nullptr; }

// lets find each key/value place

    tok = t;

    Serial.printf("tok.getType() = %d\n\r", tok.getType());
    Serial.println(tok.getStr());
    tok.skipToken();  // munch value
    
    JsonParserArray array_group = tok;
  for(auto address_id : array_group) 
  {
    Serial.printf("\t\tValue%d \n\r", address_id.getInt());
  }


    Serial.printf("tok.getType() = %d\n\r", tok.getType());
    Serial.println(tok.getStr());
    tok.skipToken();


    JsonParserArray array_group2 = tok;
  for(auto address_id : array_group2) 
  {
    Serial.printf("\t\tValue%d \n\r", address_id.getInt());
  }


    Serial.printf("tok.getType() = %d\n\r", tok.getType());
    Serial.println(tok.getStr());
    tok.skipToken();
    Serial.printf("tok.getType() = %d\n\r", tok.getType());
    Serial.println(tok.getStr());
    tok.skipToken();
    Serial.printf("tok.getType() = %d\n\r", tok.getType());
    Serial.println(tok.getStr());
    tok.nextOne();
    Serial.printf("tok.getType() = %d\n\r", tok.getType());
    Serial.println(tok.getStr());
    tok.nextOne();
    Serial.printf("tok.getType() = %d\n\r", tok.getType());
    Serial.println(tok.getStr());
    tok.nextOne();
    Serial.printf("tok.getType() = %d\n\r", tok.getType());
    Serial.println(tok.getStr());
    tok.nextOne();
    Serial.printf("tok.getType() = %d\n\r", tok.getType());
    Serial.println(tok.getStr());
    tok.nextOne();
    Serial.printf("tok.getType() = %d\n\r", tok.getType());
    Serial.println(tok.getStr());
    tok.nextOne();
    // t = 

    // Serial.println(t->)
 
  // key = tok.
 if (t->size > 0) {
  Serial.println("JsonPair::getObjectPair Good token");
  // Return the head of the next key/value pair, then JsonPair2 will iter between key and pair
    return JsonPair2(t+1);    // return next element and cast to Key
  } else {
  Serial.println("Bad token");
    return JsonPair2(&token_bad);
  }
  

}




/*********************************************************************************************\
 * JsonParserKey
\*********************************************************************************************/


JsonPair2::JsonPair2(const jsmntok_t * token) : JsonParserToken(token) {
  if (t->type != JSMN_OBJECT) {
    Serial.println("bad tok1");
    t = &token_bad;
  }
    Serial.println("! bad tok1");
}
JsonPair2::JsonPair2(const JsonParserToken token) : JsonParserToken(token.t) {
  if (t->type != JSMN_OBJECT) {
    Serial.println("bad tok2");
    t = &token_bad;
  }
    Serial.println("! bad tok2");
}

JsonParserToken JsonPair2::getValue(void) const {
  return JsonParserToken(t+1);
}
JsonParserToken JsonPair2::getKey(void) const {
  return JsonParserToken(t+2);
}







JsonParserToken JsonPair::operator[](int32_t i) const {
  Serial.println("const_iterator OP");
  if ((i >= 0) && (i < t->size)) {
    uint32_t index = 0;
    for (const auto elt : *this) {
      if (i == index) {
        return elt;
      }
      index++;
    }
  }
  // fallback
  return JsonParserToken(&token_bad);
}





#endif// ENABLE_DEVFEATURE_JSONPAIR

/*********************************************************************************************\
 * JsonParserObject
\*********************************************************************************************/

JsonParserObject::JsonParserObject(const jsmntok_t * token) : JsonParserToken(token) {
  if (t->type != JSMN_OBJECT) {
    t = &token_bad;
  }
}
JsonParserObject::JsonParserObject(const JsonParserToken token) : JsonParserToken(token.t) {
  if (t->type != JSMN_OBJECT) {
    t = &token_bad;
  }
}

JsonParserKey JsonParserObject::getFirstElement(void) const {
  if (t->size > 0) {
    return JsonParserKey(t+1);    // return next element and cast to Key
  } else {
    return JsonParserKey(&token_bad);
  }
}

JsonParserObject::const_iterator::const_iterator(const JsonParserObject t): tok(t), remaining(0) {
  if (tok.t == &token_bad) { tok.t = nullptr; }
  if (nullptr != tok.t) {
    // ASSERT type == JSMN_OBJECT by constructor
    remaining = tok.t->size;
    tok.nextOne();
  }
}

JsonParserObject::const_iterator JsonParserObject::const_iterator::operator++() {
  if (remaining <= 1) { tok.t = nullptr; }
  else {
    remaining--;
    tok.nextOne();          // munch key
    if (tok.t->type == JSMN_INVALID) { tok.t = nullptr; }   // unexpected end of stream
    tok.skipToken();  // munch value
    if (tok.t->type == JSMN_INVALID) { tok.t = nullptr; }   // unexpected end of stream
  }
  return *this;
}

/*********************************************************************************************\
 * JsonParserKey
\*********************************************************************************************/


JsonParserKey::JsonParserKey(const jsmntok_t * token) : JsonParserToken(token) {
  if (t->type != JSMN_KEY) {
    t = &token_bad;
  }
}
JsonParserKey::JsonParserKey(const JsonParserToken token) : JsonParserToken(token.t) {
  if (t->type != JSMN_KEY) {
    t = &token_bad;
  }
}

JsonParserToken JsonParserKey::getValue(void) const {
  return JsonParserToken(t+1);
}

/*********************************************************************************************\
 * Implementation for JSON Parser
\*********************************************************************************************/

// fall-back token object when parsing failed
const jsmntok_t token_bad = { JSMN_INVALID, 0, 0, 0 };

JsonParser::JsonParser(char * json_in) :
  _size(0),
  _token_len(0),
  _tokens(nullptr),
  _json(nullptr)
{
  parse(json_in);
}

JsonParser::~JsonParser() {
  this->free();
}

const JsonParserObject JsonParser::getRootObject(void) const {
  return JsonParserObject(&_tokens[0]);
}

const JsonParserToken JsonParser::operator[](int32_t i) const {
if ((_token_len > 0) && (i < _token_len)) {
    return JsonParserToken(&_tokens[i]);
  } else {
    return JsonParserToken(&token_bad);
  }
}

// pointer arithmetic
// ptrdiff_t JsonParser::index(JsonParserToken token) const {
//   return token.t - _tokens;
// }

bool JsonParserToken::getBool(bool val) const {
  if (t->type == JSMN_INVALID) { return val; }
  if (t->type == JSMN_BOOL_TRUE) return true;
  if (t->type == JSMN_BOOL_FALSE) return false;
  if (isSingleToken()) return strtol(&k_current_json_buffer[t->start], nullptr, 0) != 0;
  return false;
}
int32_t JsonParserToken::getInt(int32_t val) const {
  if (t->type == JSMN_INVALID) { return val; }
  if (t->type == JSMN_BOOL_TRUE) return 1;
  if (isSingleToken()) return strtol(&k_current_json_buffer[t->start], nullptr, 0);
  return 0;
}
uint32_t JsonParserToken::getUInt(uint32_t val) const {
  if (t->type == JSMN_INVALID) { return val; }
  if (t->type == JSMN_BOOL_TRUE) return 1;
  if (isSingleToken()) return strtoul(&k_current_json_buffer[t->start], nullptr, 0);
  return 0;
}
uint64_t JsonParserToken::getULong(uint64_t val) const {
  if (t->type == JSMN_INVALID) { return val; }
  if (t->type == JSMN_BOOL_TRUE) return 1;
  if (isSingleToken()) return strtoull(&k_current_json_buffer[t->start], nullptr, 0);
  return 0;
}
float JsonParserToken::getFloat(float val) const {
  if (t->type == JSMN_INVALID) { return val; }
  if (t->type == JSMN_BOOL_TRUE) return 1;
  if (isSingleToken()) return json_strtof(&k_current_json_buffer[t->start]);
  return 0;
}
const char * JsonParserToken::getStr(const char * val) const {
  if (t->type == JSMN_INVALID) { return val; }
  if (t->type == JSMN_NULL) return "";
  return (t->type >= JSMN_STRING) ? &k_current_json_buffer[t->start] : val;
}


JsonParserObject JsonParserToken::getObject(void) const { return JsonParserObject(*this); }
JsonParserArray JsonParserToken::getArray(void) const { return JsonParserArray(*this); }


bool JsonParserToken::getBool(void) const { return getBool(false); }
int32_t JsonParserToken::getInt(void) const { return getInt(0); }
uint32_t JsonParserToken::getUInt(void) const { return getUInt(0); }
uint64_t JsonParserToken::getULong(void) const { return getULong(0); }
float JsonParserToken::getFloat(void) const { return getFloat(0); }
const char * JsonParserToken::getStr(void) const { return getStr(""); }

int32_t JsonParserObject::getInt(const char * needle, int32_t val) const {
  return (*this)[needle].getInt(val);
}
uint32_t JsonParserObject::getUInt(const char * needle, uint32_t val) const {
  return (*this)[needle].getUInt(val);
}
uint64_t JsonParserObject::getULong(const char * needle, uint64_t val) const {
  return (*this)[needle].getULong(val);
}
float JsonParserObject::getFloat(const char * needle, float val) const {
  return (*this)[needle].getFloat(val);
}
const char * JsonParserObject::getStr(const char * needle, const char * val) const {
  return (*this)[needle].getStr(val);
}
const char * JsonParserObject::getStr(const char * needle) const {
  return getStr(needle, "");
}

void JsonParser::parse(char* json_in) {
  k_current_json_buffer = "";
  if (nullptr == json_in) { return; }
  _json = json_in;
  k_current_json_buffer = _json;
  size_t json_len = strlen(json_in);
  
  #ifdef ENABLE_DEBUG_JSON_LARGE_PARSING
  Serial.printf("json_len=%d\n\r", json_len);
  #endif 

  if (_size == 0) {
    // first run is used to count tokens before allocation
    jsmn_init(&this->_parser);
    int32_t _token_len = jsmn_parse(&this->_parser, json_in, json_len, nullptr, 0);
    if (_token_len <= 0) { return; }
    _size = _token_len + 1;
  }
  DEBUG_JSON_LARGE_PARSING;
  allocate();
  DEBUG_JSON_LARGE_PARSING;
  jsmn_init(&this->_parser);
  DEBUG_JSON_LARGE_PARSING;
  _token_len = jsmn_parse(&this->_parser, json_in, json_len, _tokens, _size);
  
  #ifdef ENABLE_DEBUG_JSON_LARGE_PARSING
  Serial.printf("_token_len=%d\n\r", _token_len);
  #endif 


  // TODO error checking
  if (_token_len >= 0) {
    postProcess(json_len);
  }
}

// post process the parsing by pre-munching extended types
void JsonParser::postProcess(size_t json_len) {
  // add an end marker
  if (_size > _token_len) {
    _tokens[_token_len].type = JSMN_INVALID;
    _tokens[_token_len].start = json_len;
    _tokens[_token_len].len = 0;
    _tokens[_token_len].size = 0;
  }
  for (uint32_t i=0; i<_token_len; i++) {
    jsmntok_t & tok = _tokens[i];

    if (tok.type >= JSMN_STRING) {
      // we modify to null-terminate the primitive
      _json[tok.start + tok.len] = 0;
    }

    if (tok.type == JSMN_STRING) {
      // Serial.printf("tok.size=%d\n\r", tok.size);
      if (tok.size == 1) { tok.type = JSMN_KEY; }
      else { json_unescape(&_json[tok.start]); }
    } else if (tok.type == JSMN_PRIMITIVE) {
      if (tok.len >= 0) {
        // non-null string
        char c0 = _json[tok.start];
        switch (c0) {
          case 'n':
          case 'N':
            tok.type = JSMN_NULL;
            break;
          case 't':
          case 'T':
            tok.type = JSMN_BOOL_TRUE;
            break;
          case 'f':
          case 'F':
            tok.type = JSMN_BOOL_FALSE;
            break;
          case '-':
          case '0'...'9':
            // look if there is a '.' in the string
            if (nullptr != memchr(&_json[tok.start], '.', tok.len)) {
              tok.type = JSMN_FLOAT;
            } else if (c0 == '-') {
              tok.type = JSMN_INT;
            } else {
              tok.type = JSMN_UINT;
            }
            break;
          default:
            tok.type = JSMN_PRIMITIVE;
            break;
        }
      } else {
        tok.type = JSMN_PRIMITIVE;
      }
    }
  }
}

JsonParserToken JsonParserObject::operator[](const char * needle) const {
  // key can be in PROGMEM
  if ((!this->isValid()) || (nullptr == needle) || (0 == pgm_read_byte(needle))) {
    return JsonParserToken(&token_bad);
  }
  // if needle == "?" then we return the first valid key
  bool wildcard = (strcmp_P("?", needle) == 0);

  for (const auto key : *this) {
    if (wildcard) { return key.getValue(); }
    if (0 == strcasecmp_P(key.getStr(), needle)) { return key.getValue(); }
  }
  // if not found
  return JsonParserToken(&token_bad);
}

JsonParserToken JsonParserObject::operator[](const String & needle) const {
  return (*this)[needle.c_str()];
}

JsonParserToken JsonParserObject::findStartsWith(const char * needle) const {
  // key can be in PROGMEM
  if ((!this->isValid()) || (nullptr == needle) || (0 == pgm_read_byte(needle))) {
    return JsonParserToken(&token_bad);
  }
  
  String needle_s((const __FlashStringHelper *)needle);
  needle_s.toLowerCase();

  for (const auto key : *this) {
    String key_s(key.getStr());
    key_s.toLowerCase();

    if (key_s.startsWith(needle_s)) {
      return key.getValue();
    }
  }
  // if not found
  return JsonParserToken(&token_bad);
}

const char * JsonParserObject::findConstCharNull(const char * needle) const {
  const char * r = (*this)[needle].getStr();
  if (*r == 0) { r = nullptr; }   // if empty string
  return r;
}

// JsonParserToken JsonParser::find(JsonParserObject obj, const char *needle, bool case_sensitive) const {
//   // key can be in PROGMEM
//   if ((!obj.isValid()) || (nullptr == needle) || (0 == pgm_read_byte(needle))) {
//     return JsonParserToken(&token_bad);
//   }
//   // if needle == "?" then we return the first valid key
//   bool wildcard = (strcmp_P("?", needle) == 0);

//   for (const auto key : obj) {
//     if (wildcard) { return key.getValue(); }
//     if (case_sensitive) {
//       if (0 == strcmp_P(this->getStr(key), needle)) { return key.getValue(); }
//     } else {
//       if (0 == strcasecmp_P(this->getStr(key), needle)) { return key.getValue(); }
//     }
//   }
//   // if not found
//   return JsonParserToken(&token_bad);
// }

void JsonParser::free(void) {
  if (nullptr != _tokens) {
    delete[] _tokens;     // TODO
    _tokens = nullptr;
  }
}

void JsonParser::allocate(void) {
  this->free();
  if (_size != 0) {
    _tokens = new jsmntok_t[_size];
  }
}


// // Function to count total tokens and specific types
// void JsonParser::countJsonElements(const char* json, int &objectCount, int &arrayCount, int &pairCount) {
//   jsmn_parser parser;
//   jsmntok_t tokens[128];  // Adjust size according to your needs
//   jsmn_init(&parser);
  
//   int tokenCount = jsmn_parse(&parser, json, strlen(json), tokens, 128);
//   if (tokenCount < 0) {
//       Serial.printf("Failed to parse JSON: %d\n", tokenCount);
//       return;
//   }
  
//   for (int i = 0; i < tokenCount; i++) {
//     switch (tokens[i].type) {
//       case JSMN_OBJECT:
//         objectCount++;
//         pairCount += tokens[i].size;  // Assuming each key-value pair in the object is counted
//         break;
//       case JSMN_ARRAY:
//         arrayCount++;
//         break;
//       case JSMN_STRING:
//         // Strings are typically part of key-value pairs, so they can be counted differently if needed
//         break;
//       case JSMN_PRIMITIVE:
//         // Primitives include numbers, booleans, and null
//         break;
//       default:
//         break;
//     }
//   }
// }


