/* IRLibCombo.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 */
/*
 * This file is for creating a customer decode or send class which contains only
 * the protocols that you will actually use. At the top of your sketch you should
 * include the Send and/or Decode base modules followed by at least one or more
 * protocol module. Then conclude with this module.
 * WARNING: The lowest numbered protocol which you are using MUST be included first.
 *		The remaining protocol modules technically could be in any order however we
 *		recommend that you maintain numerical order because you might at some point
 *		comment out the top one and then the lowest would not be first causing an error.
 *
 * Here is an example...
 *
 * #include <IRLibSendBase.h>	//Only include if you are sending
 * #include <IRLibDecodeBase.h>	//Only include if you are decoding
 * #include <IRLib_Pxx_protocol_name.h>	//Include at least one protocol
 * #include <IRLib_Pxx_another_name.h>	//Include as many as you want
 * #include <IRLibCombo.h>		//Include this file
 * IRdecode My_Decoder;	//declare an instance of the decoder if needed
 * IRsend My_Sender;	//declarative sense of the sending routine if needed
 * //The rest of your code goes here
 */

#ifndef IRLIB_COMBO_H
#define IRLIB_COMBO_H

#ifndef IRLIB_HASHRAW_H
	#define IR_SEND_RAW
	#define IR_DECODE_HASH
	#define PV_IR_DECODE_HASH
	#define PV_IR_SEND_RAW
#endif
//Add additional protocols 15, 16, etc. above.

/*
 * You may add additional protocols by copying and modifying the previous six lines.
 * You must also add appropriate macros in each segment below. Be sure to maintain
 * numerical order. Also the final entry in each list MUST BE the Hash_Raw version.
 */
#ifdef IRLIBDECODEBASE_H
class IRdecode:
	PV_IR_DECODE_HASH	//Must be last one.
{
public:
	bool decode(void) {
	  IR_DECODE_HASH	//Must be last one.
		return false;
	};
};
#endif //IRLIBDECODEBASE_H

#ifdef IRLIBSENDBASE_H
class IRsend:
	PV_IR_SEND_RAW	//Must be last one.
{
public:
	void send(uint8_t protocolNum, uint32_t data, uint16_t data2=0, uint8_t khz=38) {
    if(khz==0)khz=38;
		switch(protocolNum) {
			IR_SEND_RAW	//Must be last one.
		}
	}
};
#endif  //IRLIBSENDBASE_H

#endif //IRLIB_COMBO_H
