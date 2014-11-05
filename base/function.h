#ifndef __base_function__
#define __base_function__

#ifndef ASSERT
#define ASSERT assert
#endif

#define KF_PROCESS_ERROR(Condition) \
	do \
	{ \
		if (!(Condition)) \
			goto Exit0;	\
	} while (false)

#define KF_PROCESS_SUCCESS(Condition) \
	do \
	{ \
		if (Condition) \
			goto Exit1;	\
	} while (false)

#endif
