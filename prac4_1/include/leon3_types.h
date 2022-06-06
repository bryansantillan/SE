#ifndef LEON3_TYPES_H_
#define LEON3_TYPES_H_

#ifndef NULL
	#define NULL 0
#endif

#ifndef __cplusplus
	typedef unsigned char 	bool_t;
	#define 		true	1
	#define 		false	0
#else
	typedef bool			bool_t;
#endif

	//TODO-declaracion de tipos usados
typedef unsigned char		byte_t;

//TODO- unsigneds words
typedef unsigned short int	word16_t;
typedef unsigned int		word32_t;
typedef unsigned long int	word64_t;
//TODO - signeds ints
typedef signed char			int8_t;
typedef signed short int	int16_t;
typedef signed int			int32_t;
typedef signed long int		int64_t;
//TODO - unsigneds uints
typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long int	uint64_t;

#endif
