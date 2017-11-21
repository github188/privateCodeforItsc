/******************************************************************************

                  版权所有 (C), 2003-2014, 老虎工作室

 ******************************************************************************
  文 件 名   : Util.c
  版 本 号   : 初稿
  作    者   : 老虎
  生成日期   : 2014年6月25日
  最近修改   :
  功能描述   : 这里描述了一些通用接口如Log_debug等
  函数列表   :
              GetLocalTime
              GetTickCount
              log_debug
  修改历史   :
  1.日    期   : 2014年6月25日
    作    者   : 老虎
    修改内容   : 创建文件

******************************************************************************/

#include "Util.h"

unsigned long GetTickCount()
{
	struct timeval tv;
	ulong time_val;
	gettimeofday(&tv, NULL);
	// return ulong(tv.tv_sec*1000L+tv.tv_usec/1000L);
	time_val = tv.tv_sec*1000L + tv.tv_usec/1000L;
	return time_val;
}

/*****************************************************************************
 函 数 名  : GetLocalTime
 功能描述  : 获得当前时间，可重入线程安全
 输入参数  : char *localTime  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年6月27日
    作    者   : 老虎
    修改内容   : 新生成函数

*****************************************************************************/
void GetLocalTime(char *localTime)
{
    if(!localTime)
    {
        return;
    }

    time_t timep;
    time(&timep);

    struct tm now;
    localtime_r(&timep,&now);

    strftime(localTime,24,"%Y-%m-%d %H:%M:%S",&now);
}

void GetLocalDate(char *date)
{
    if(!date)
    {
        return;
    }

    time_t timep;
    time(&timep);

    struct tm now;
    localtime_r(&timep,&now);

    strftime(date,24,"%Y-%m-%d-",&now);
}

/*****************************************************************************
 函 数 名  : log_debug
 功能描述  : 格式化输出接口函数，可重入，线程安全
 输入参数  : const char* format  
             ...                 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年6月27日
    作    者   : 老虎
    修改内容   : 新生成函数
 
  2.日    期   : 2014年6月27日
    作    者   : 老虎
    修改内容   : 为支持本地日志系统，修改buf起始
*****************************************************************************/
void log_debug(const char* format, ...)
{
	//printf("==========\n");
	//char localTime[24];
	char buff[2048] = {0};
	memset(buff,0,sizeof(buff));
	GetLocalTime(buff);//最前面是当前日期函数
	//printf("strlen buff  %d\n",strlen(buff));
	strcat(buff,"  ");//用2个字节的空格使日期与具体日志内容分开
	strcat(buff,"<DEBUG>");
	strcat(buff,"  ");	
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buff+strlen(buff),sizeof(buff)-strlen(buff)-1, format, argptr);//将buff的位置向右移动strlen(buff)字节，最前面这24字节用来存放当前时间，紧接着是2字节的空格用来分割内容和时间
	va_end(argptr);
	strcat(buff,"\n");
	printf("%s",buff);

#ifdef LOG4HIK
    AddLogItem(buff);
#endif
	
	//memset(localTime,0,sizeof(localTime));
	//GetLocalTime(localTime);
	//printf("%s  %s\n",localTime,buff);
}

void log_error(const char* format, ...)
{
	//printf("==========\n");
	//char localTime[24];
	char buff[2048] = {0};
	memset(buff,0,sizeof(buff));
	GetLocalTime(buff);//最前面是当前日期函数
	//printf("strlen buff  %d\n",strlen(buff));
	strcat(buff,"  ");//用2个字节的空格使日期与具体日志内容分开
	strcat(buff,"<ERROR>");
	strcat(buff,"  ");
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buff+strlen(buff),sizeof(buff)-strlen(buff)-1, format, argptr);//将buff的位置向右移动strlen(buff)字节，最前面这24字节用来存放当前时间，紧接着是2字节的空格用来分割内容和时间
	va_end(argptr);
	strcat(buff,"\n");
	printf("%s",buff);

#ifdef LOG4HIK
    AddLogItem(buff);
#endif
	
	//memset(localTime,0,sizeof(localTime));
	//GetLocalTime(localTime);
	//printf("%s  %s\n",localTime,buff);
}


/*****************************************************************************
 函 数 名  : semPost
 功能描述  : 用于线程通过sem同步
 输入参数  : sem_t * pSem   
             int nMaxCount  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年6月30日
    作    者   : 老虎
    修改内容   : 新生成函数

*****************************************************************************/
int semPost(sem_t * pSem, int nMaxCount)
{
	int nRet, nSemCount;

	sem_getvalue(pSem, &nSemCount);
	if (nSemCount>=nMaxCount)
	{
	    printf("semPost  lost\n");
		return 0;
	}
	else
	{
		nRet=sem_post(pSem);
		return nRet;
	}
}
/*****************************************************************************
 函 数 名  : semWait
 功能描述  : 用于线程通过sem同步
 输入参数  : sem_t * pSem  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年6月30日
    作    者   : 老虎
    修改内容   : 新生成函数

*****************************************************************************/
int semWait(sem_t * pSem)
{
	int nRet;
	nRet=sem_wait(pSem);

	while (sem_trywait(pSem)==0) {}
	return nRet;
}
/*****************************************************************************
 函 数 名  : gettid
 功能描述  : 获得线程ID，利于调试top -Hp
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年6月30日
    作    者   : 老虎
    修改内容   : 新生成函数

*****************************************************************************/
pid_t gettid()
{
     return syscall(SYS_gettid); 
}
/*****************************************************************************
 函 数 名  : ThreadClearMem
 功能描述  : 文件读写时系统总是会缓存很大空间，这个线程可以定时清空缓冲区
 输入参数  : 无
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年6月30日
    作    者   : 老虎
    修改内容   : 新生成函数

*****************************************************************************/
void * ThreadClearMem()
{

    while(1)
    {
        sleep(1);
        system("sync");
        system("echo 1 > /proc/sys/vm/drop_caches");
        system("echo \"0\" >> /proc/sys/vm/overcommit_memory");

    }

    return null;
}


/*****************************************************************************
 函 数 名  : IsItemInIntArray
 功能描述  : 判断整型变量val是否在array数组中
 输入参数  : int *array  
             int length  
             int val     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年7月3日
    作    者   : xiaowh
    修改内容   : 新生成函数

*****************************************************************************/
int IsItemInIntArray(int *array,int length,int val)
{
    if(!array)
    {
        return false;
    }
    
    int i = 0;

    for(i = 0 ; i < length ; i++)
    {
        if(array[i] == val)
        {
            return true;
        }

    }

    return false;


}

pthread_t   ThreadCreate(thrfunc *routine, void *args)
{
    pthread_t id = 0;
    pthread_attr_t attributes;
    int ret;

    pthread_attr_init(&attributes);
    pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);

    ret =  pthread_create(&id, &attributes, routine, args);
    if (ret != 0)
    {
        log_debug("%s, pthread_create failed with %d\n", __FUNCTION__, ret);

    }

    pthread_attr_destroy(&attributes);

    AddThreadControllerItem(id);
    
    return id;
    
}


#if 0

int main(int argc,char **argv)
{
    log_debug("Hello World ");

    return 0;
}


#endif


