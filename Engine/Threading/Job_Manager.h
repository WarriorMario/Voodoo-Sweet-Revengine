#ifndef INCLUDED_JOB_MANAGER_H
#define INCLUDED_JOB_MANAGER_H
#define __cplusplus 1?
#include <stdint.h>

	typedef struct JobManager JobManager;
	typedef struct Job
	{
		void(*function)(void*);
		void* param;
	}Job;
	JobManager* JM_AllocJobManager();
	void JM_FreeJobManager(JobManager* jm);
	int32_t JM_InitJobManager(JobManager* jm, int threadcount);
	int32_t JM_SubmitJob(JobManager* jm, Job* job);
	void JM_Sync(JobManager* jm);
  JobManager*& JM_Get();
#endif//INCLUdED_JOB_MANAGER_H