#ifndef _LOG4CXX_WRAPPER_H
#define _LOG4CXX_WRAPPER_H

#include <log4cxx/logger.h>
#include <log4cxx/logstring.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>  
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

using namespace log4cxx;


typedef enum
{
    LOGGER_TRACE = 0,

    LOGGER_WARN = 1,

    LOGGER_INFO    = 2,
    
    LOGGER_DEBUG = 3,

    LOGGER_ERROR = 4,

    LOGGER_FATAL  = 5

} LogGrade;
  

class LoggerWrapper
{
	LoggerPtr m_logger;
       LogGrade m_logGrade; 
//	char m_funcName[255];
	LoggerWrapper()
	{
            PropertyConfigurator::configure("log4cxx.cfg");
//            memset(m_funcName,0,255);
//            sprintf(m_funcName,"%s",__FUNCTION__);        
            m_logger = Logger::getLogger(""/*m_funcName*/);
	}
public:

	static LoggerWrapper& GetInstance()
	{
		static LoggerWrapper instance_;
		return instance_;
	}

	bool Log(bool bPrintScreen, LogGrade logGrade,const char * strFormat,...)
	{
            if((logGrade >LOGGER_FATAL) ||(logGrade  < LOGGER_TRACE))
            {
                return false;
            }            

            char *line,*pfree;
            int allocated=100/*2048*/,result=allocated;
                
            va_list ap;
            
            line = (char*)malloc(allocated);
            
            pfree = line;
        
            while(line)
            {
                va_start(ap,strFormat);
                    
                pfree = line;
        
                result = vsnprintf(line,allocated-1,strFormat,ap);
        
                if (result < allocated-1)
                {
                    line[result] = '\0';
                    va_end(ap);
                    break;
                }
                
                allocated += result;
        
                line = (char*)realloc(line,allocated);
                va_end(ap);
                
            }
            
            if( LOGGER_TRACE ==logGrade)
            {
                LOG4CXX_TRACE(m_logger, pfree);
                if(bPrintScreen)
                {
                    printf("TRACE -- %s\n",pfree); 
                }
            }
            else if( LOGGER_WARN ==logGrade)
            {
                LOG4CXX_WARN(m_logger, pfree);
                if(bPrintScreen)
                {
                    printf("WARN -- %s\n",pfree); 
                }
            }
            else if( LOGGER_INFO ==logGrade)
            {
                LOG4CXX_INFO(m_logger, pfree);
                if(bPrintScreen)
                {
                    printf("INFO -- %s\n",pfree); 
                }
            }            
            else if( LOGGER_DEBUG ==logGrade)
            {
                LOG4CXX_DEBUG(m_logger, pfree);
                if(bPrintScreen)
                {
                    printf("DEBUG -- %s\n",pfree); 
                }
            }
            else if( LOGGER_ERROR ==logGrade)
            {
                LOG4CXX_ERROR(m_logger, pfree);
                if(bPrintScreen)
                {
                    printf("ERROR -- %s\n",pfree); 
                }
            }
            else if( LOGGER_FATAL ==logGrade)
            {
                LOG4CXX_FATAL(m_logger, pfree);
                if(bPrintScreen)
                {
                    printf("FATAL -- %s\n",pfree); 
                }
            }            

            if (pfree)        
            {    
                free(pfree);    
            } 
        
            return true;
	}

};


#endif//_LOG4CXX_WRAPPER_H


