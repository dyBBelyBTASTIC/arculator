/*Arculator 2.1 by Sarah Walker
  Main function*/
#include "wx-app.h"
#include <SDL.h>
#include <wx/filename.h>
#include "wx-config_sel.h"

extern "C"
{
#if !defined(_WIN32) && !defined(__APPLE__)
	#include <X11/Xlib.h>
#endif
	#include "arc.h"
	#include "config.h"
	#include "podules.h"
}

int main(int argc, char **argv)
{
#if !defined(_WIN32) && !defined(__APPLE__)
	XInitThreads();
#endif

	strncpy(exname, argv[0], 511);
	char *p = (char *)get_filename(exname);
	*p = 0;

	/* Scan for -fullscreen and strip it out of argv so it isn't mistaken
	   for a machine configuration name. */
	int new_argc = 1;
	char **new_argv = (char **)malloc(argc * sizeof(char *));
	new_argv[0] = argv[0];
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-fullscreen"))
			cmdline_fullscreen = 1;
		else
			new_argv[new_argc++] = argv[i];
	}
	argc = new_argc;
	argv = new_argv;

	if(argc > 1)
	{
		wxString config_path = GetConfigPath(argv[1]);

		if(wxFileName(config_path).Exists())
		{
			strcpy(machine_config_file, config_path.mb_str());
			strcpy(machine_config_name, argv[1]);
		}
		else
		{
			wxMessageBox("A configuration with the name '" + wxString(argv[1]) + "' does not exist", "Arculator", wxOK | wxCENTRE | wxSTAY_ON_TOP);
			exit(-1);
		}
	}

	podule_build_list();
	opendlls();
#ifdef _WIN32
	SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
#endif

	wxApp::SetInstance(new App());
	wxEntry(argc, argv);

	return 0;
}
