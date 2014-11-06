#ifndef __base_function__
#define __base_function__

#ifndef ASSERT
#define ASSERT assert
#endif

#define KF_PROCESS_ERROR(Condition) \
	do \
	{ \
		if (!(Condition)) \
			goto ExitFailed;	\
	} while (false)

#define KF_PROCESS_SUCCESS(Condition) \
	do \
	{ \
		if (Condition) \
			goto ExitSucc;	\
	} while (false)

#endif
