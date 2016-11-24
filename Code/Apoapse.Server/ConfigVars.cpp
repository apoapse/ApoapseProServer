#include "stdafx.h"
#include "ConfigVars.h"

void ConfigVars::RegisterConfigVars()
{
	global->settings->RegisterConfigVar_uint("system.glboal_threadpool_nb_threads", 8);

	global->settings->RegisterConfigVar_double("messages.attachments.max_attachement_size_mb", 50.0);
}