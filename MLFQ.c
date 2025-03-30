#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define time 5

typedef struct process
{
    char name;
    int rem_time;
    int turn_time;
   
}process;

int n;
process **q0, **q1, **q2, **q3;
int cnt0 = 0, cnt1 = 0, cnt2 = 0, cnt3 = 0;

int done = 0;
int curr_time = 0;

pthread_mutex_t lock;
pthread_cond_t cv0,cv1,cv2,cv3;
int arrive0 = 1, arrive1 = 0, arrive2 = 0, arrive3 = 0;

int compare(const void *a, const void *b)
{
    process *p = *(process **)a;
    process *q = *(process **)b;
    return (p->rem_time - q->rem_time);
}

void execute(process *p, int q)
{
    int exec_time = (p->rem_time > time ) ? time: p->rem_time;
    p->rem_time -= exec_time;
    curr_time += exec_time;
    if(p->rem_time == 0)
        p->turn_time += curr_time;
    printf("Process %c ran in Q%d . Remaining time is %d \n",p->name,q,p->rem_time);
    sleep(1);
}

void *round_robin(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(!arrive0)
            pthread_cond_wait(&cv0,&lock);
        
        while(cnt0 > 0)
        {
            process *p = q0[0];

            for(int i=0; i<cnt0-1; i++)
                q0[i] = q0[i+1];
            cnt0--;

            execute(p,0);
            if(p->rem_time > 0)
                q1[cnt1++] = p;

            if(cnt0 == 0)
            {
                if(cnt1 > 0)
                {
                    arrive1=1;
                    pthread_cond_signal(&cv1);
                }
                else
                    done = 1;
            }
        }

        arrive0=0;
        pthread_mutex_unlock(&lock);
    }
}

void *fcfs(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        while(!arrive1)
            pthread_cond_wait(&cv1,&lock);

        while(cnt1 > 0)
        {
            process *p = q1[0];

            for(int i=0; i<cnt1-1; i++)
                q1[i]=q1[i+1];
            cnt1--;

            execute(p,1);
            if(p->rem_time > 0)
                q2[cnt2++] = p;
            
            if(cnt1 == 0)
            {
                if(cnt2>0)
                {
                    arrive2=1;
                    pthread_cond_signal(&cv2);
                }
                else
                    done = 1;
            }
        }
        arrive1 = 0;
        pthread_mutex_unlock(&lock);
    }
}

void *priority(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);

        while(!arrive2)
            pthread_cond_wait(&cv2,&lock);
        
        qsort(q2,cnt2,sizeof(process *),compare);

        while(cnt2>0)
        {
            process *p = q2[0];
            for(int i=0; i<cnt2-1;i++)
                q2[i] = q2[i+1];
            cnt2--;
            
            execute(p,2);
            if(p->rem_time > 0)
                q3[cnt3++] = p;
            
            if(cnt2 == 0)
            {
                if(cnt3>0)
                {
                    arrive3=1;
                    pthread_cond_signal(&cv3);
                }
                else
                    done = 1;
            }
        }

        arrive2 = 0;
        pthread_mutex_unlock(&lock);
    }
}

void *sjf(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);

        while(!arrive3)
            pthread_cond_wait(&cv3,&lock);
        
        qsort(q3,cnt3,sizeof(process*),compare);

        while(cnt3 > 0)
        {
            process *p = q3[0];
            for(int i = 0; i < cnt3-1; i++)
                q3[i] = q3[i+1];
            cnt3--;

            execute(p,3);
            if(p->rem_time > 0)
                q0[cnt0++] = p;
            
            if(cnt3 == 0 )
            {
                if(cnt0>0)
                {
                    arrive0=1;
                    pthread_cond_signal(&cv0);
                }
                else
                    done = 1;
            }
        }

        arrive3=0;
        pthread_mutex_unlock(&lock);
    }
}

int main()
{
   
   printf("Enter number of processes: ");
   if (scanf("%d", &n) != 1) 
   {
	    printf("Invalid input!\n");
	    return 1;
   }

   process p[n];
   q0 = (process **)malloc(n * sizeof(process *));
   q1 = (process **)malloc(n * sizeof(process *));
   q2 = (process **)malloc(n * sizeof(process *));
   q3 = (process **)malloc(n * sizeof(process *));

   if (!q0 || !q1 || !q2 || !q3) 
   {
    printf("Memory allocation failed!\n");
    return 1;
   }
   
   printf("Enter each Process name (A,B,..like that) CPU Time \n");
   for(int i=0; i<n;i++)
   {
   	if(scanf(" %c %d",&p[i].name, &p[i].rem_time)!=2)
   		return 1;
   	p[i].turn_time = 0;
        q0[cnt0++] = &p[i];
   }
   
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cv0,NULL);
    pthread_cond_init(&cv1,NULL);
    pthread_cond_init(&cv2,NULL);
    pthread_cond_init(&cv3,NULL);

    pthread_t t0,t1,t2,t3;
    pthread_create(&t0,NULL,round_robin,NULL);
    pthread_create(&t1,NULL,fcfs,NULL);
    pthread_create(&t2,NULL,priority,NULL);
    pthread_create(&t3,NULL,sjf,NULL);

    while(!done)
    {
        pthread_mutex_lock(&lock);
        if (cnt0 > 0 && !arrive0) {
            arrive0 = 1;
            pthread_cond_signal(&cv0);
        }
        pthread_mutex_unlock(&lock);
    }

    int total_turn_time = 0;
    for(int i=0; i<n;i++)
    {
        printf("Turnaround time of process %c is %d\n",p[i].name,p[i].turn_time);
        total_turn_time+=p[i].turn_time;
    }

    printf("Average turnaround time is %.2f ms\n",(float)total_turn_time/5);

    pthread_mutex_destroy(&lock);
    return 0;
}
