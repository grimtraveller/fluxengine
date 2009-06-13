/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_logger.h"

Logger::Logger()
{
	char filename[] = "log.txt";
	m_bON = true;

	Write(LOG_DEFAULT, "No Error");

	//use default filename
	if(filename == NULL)
		sprintf(m_szFilename, "log.txt");
	else
	{
		if( (strlen(filename) > 4) && (filename[strlen(filename)-4] == '.'))
			sprintf(m_szFilename, "%s", filename);
		else
			sprintf(m_szFilename, "%s.txt", filename);
	}

	if(!m_bON)
		return;

	FILE* fp;
	fp = fopen("log.txt", "wt");

	if(fp != NULL)
		fclose(fp);
}

Logger::~Logger()
{
}

void Logger::Write(LOGENTRY_TYPE log_entry_type, const char *_szMessage, ...)
{
	va_list ap;
	va_start(ap, _szMessage);
	vsprintf(m_szLogMsg, _szMessage, ap);
	va_end(ap);

	m_log_entry_id = log_entry_type;

	if(!m_bON)
		return;

	FILE* fp;
	fp = fopen("log.txt", "at");
	if(fp != NULL)
	{
		char datebuf[128];
		char timebuf[128];
		_strdate(datebuf);
		_strtime(timebuf);

		if(log_entry_type == LOG_DEFAULT)
			fprintf(fp, "%s", m_szLogMsg);
			//fprintf(fp, "%s %s %s", datebuf, timebuf, m_szLogMsg);
		else
			fprintf(fp, "%s %s %s %s", datebuf, timebuf, LOGENTRY_TYPE_STR[log_entry_type].c_str(), m_szLogMsg);
		fclose(fp);
	}
}

LOGENTRY_TYPE Logger::GetErrorCode()
{
	return m_log_entry_id;
}

char* Logger::GetErrorMessage()
{
	return m_szLogMsg;
}

void Logger::ShowLogMessage()
{
	switch(m_log_entry_id)
	{
	case LOG_DEFAULT:
		{
			MessageBox(GetDesktopWindow(),
				m_szLogMsg,
				"",
				MB_ICONINFORMATION | MB_OK);
			break;
		}
	case LOG_ERROR:
		{
			MessageBox(GetDesktopWindow(),
				m_szLogMsg,
				"Memory Error",
				MB_ICONERROR | MB_OK);
			break;
		}
	default:
		{
			MessageBox(GetDesktopWindow(),
				"Unrecognized Error",
				"Unkown Message",
				MB_ICONQUESTION | MB_OK);
			break;
		}
	}
	return;
}