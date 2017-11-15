#include "job_manager.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <memory>
#include <thread>

#define CRASH() (*(volatile int*)(0) = 0x1337);


typedef struct JobQueue
{
	Job* jobs;
	size_t capacity;
	size_t front;
	size_t end;
	size_t stored;
}JobQueue;

void InitQueue(JobQueue* queue)
{
	queue->capacity = 65536;
	queue->jobs = (Job*)malloc(queue->capacity * sizeof(Job));
	queue->front = 0;
	queue->end = 0;
	queue->stored = 0;
}
void CleanUpQueue(JobQueue* queue)
{
	free(queue->jobs);
}
void PushQueue(JobQueue* queue, const Job* job)
{
	if (queue->stored >= queue->capacity)
		CRASH();
	
		queue->jobs[queue->end++] = *job;
	
		queue->stored++;
	
		if (queue->end == queue->capacity)
			queue->end = 0;
}

void PushQueue(JobQueue* queue, Job** jobs, int num)
{
	if (queue->stored >= queue->capacity)
		CRASH();

	for (int i = 0; i < num; ++i)
	{
		queue->jobs[queue->end++] = (*jobs)[i];

		queue->stored++;

		if (queue->end == queue->capacity)
			queue->end = 0;
	}
}

Job PopQueue(JobQueue* queue)
{
	if (queue->stored == 0)
		CRASH();
	
		Job result = queue->jobs[queue->front++];
	queue->stored--;
	
		
		if (queue->front == queue->capacity)
			queue->front = 0;
	
		return result;
}

typedef struct ThreadData
{
	JobManager* jobmanager;
	int threadid;
}ThreadData;

typedef struct JobManager
{
	HANDLE* threads;
	HANDLE* waitevents;
	HANDLE* stopevents;
	Job* jobs;
	ThreadData* threaddata;
	CRITICAL_SECTION critsection;
	int threadcount;
	JobQueue queue;
}JobManager;

static JobManager* job_manager;

static BOOL AskForNewJob(JobManager* jm, Job *job)
{
	EnterCriticalSection(&jm->critsection);
	BOOL result = FALSE;
	if (jm->queue.stored > 0u)
	{
		result = TRUE;
		*job = PopQueue(&jm->queue);
	}
	LeaveCriticalSection(&jm->critsection);
	return result;
}

static DWORD WINAPI ThreadFunc(void* data)
{
	ThreadData* td = (ThreadData*)data;
	HANDLE* waitevent = &td->jobmanager->waitevents[td->threadid];
	HANDLE* stopevent = &td->jobmanager->stopevents[td->threadid];
	Job* job = &td->jobmanager->jobs[td->threadid];
	for (;;)
	{
		//wait until this thread is enabled
		WaitForSingleObject(*waitevent, INFINITE);
		
			//check if thread is supposed to stop running
			if (WaitForSingleObject(*stopevent, 0) == WAIT_OBJECT_0)
				return 0;
		
			//if the thread is not supposed to quit, it means there is a job waiting 
			
			for (;;)
			{
				//execute the job we got
				job->function(job->param);
				
					//ask the job manager for a new job
					BOOL gotnewjob = AskForNewJob(td->jobmanager, job);
				
					//if the thread did not get a new job, the thread stops executing jobs
					if (!gotnewjob)
						break;
			}
		
			//indicate that the thread is done executing jobs
			ResetEvent(*waitevent);
	}
	
		return ~0u; //should never be reached
}

JobManager* JM_AllocJobManager()
{
	return (JobManager*)malloc(sizeof(JobManager));
}
void JM_FreeJobManager(JobManager* jm)
{
	free((void*)jm);
}


int32_t JM_InitJobManager(JobManager* jm, int threadcount)
{
	if (jm == NULL) return 1;
	
		jm->threads = (HANDLE*)malloc(sizeof(HANDLE) * threadcount);
	jm->threadcount = threadcount;
	
		jm->waitevents = (HANDLE*)malloc(sizeof(HANDLE) * threadcount);
	jm->stopevents = (HANDLE*)malloc(sizeof(HANDLE) * threadcount);
	
		jm->threaddata = (ThreadData*)malloc(sizeof(ThreadData) * threadcount);
	jm->jobs = (Job*)malloc(sizeof(Job) * threadcount);
	
		InitQueue(&jm->queue);
	
		InitializeCriticalSection(&jm->critsection);
	
		for (int i = 0; i < threadcount; ++i)
		{
			jm->waitevents[i] = CreateEventA(0, TRUE, FALSE, NULL);
			jm->stopevents[i] = CreateEventA(0, TRUE, FALSE, NULL);
			
				jm->threaddata[i].jobmanager = jm;
			jm->threaddata[i].threadid = i;
			
				//create a new thread
				jm->threads[i] = CreateThread(
					NULL,               //thread security attributes (NULL for none)
					0,                  //stack size in bytes (0 -> default stack size)
					&ThreadFunc,        //function to be executed on new thread
					jm->threaddata + i, //parameter to be passed to the function
					0,                  //creation flags (0 for none)
					NULL);              //thread ID as output (pass NULL to ignore output)
		}
	
		return 0;
}

int32_t JM_SubmitJob(JobManager* jm, Job* job)
{
	EnterCriticalSection(&jm->critsection);
	
		for (int i = 0; i < jm->threadcount; ++i)
		{
			BOOL isbusy = (WaitForSingleObject(jm->waitevents[i], 0) == WAIT_OBJECT_0);
			if (!isbusy)
			{
				jm->jobs[i] = *job;
				SetEvent(jm->waitevents[i]);
				LeaveCriticalSection(&jm->critsection);
				return 0;
			}
		}
	
		PushQueue(&jm->queue, job);
	LeaveCriticalSection(&jm->critsection);
	return 0;
}
void JM_Sync(JobManager* jm)
{
	
		for (int i = 0; i < jm->threadcount; ++i)
		{
			for (;;)
			{
				if (WaitForSingleObject(jm->waitevents[i], 0) == WAIT_TIMEOUT)
				{
					break;
				}
				if (SwitchToThread() == 0)
					Sleep(0);				
			}
		}
}

JobManager *& JM_Get()
{
  return job_manager;
}
