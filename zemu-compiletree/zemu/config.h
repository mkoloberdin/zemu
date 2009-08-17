/*
    Tanks - Tank arcade game
    Copyright (c) 2005 ProZroks

    <<< LICENSE >>>

    ProZroks
    support@prozroks.com

    Last modifed: Restorer (16 Jun, 2005)
*/

/*
    Whatsnew.

    Restorer (24 May, 2005)
        Только начало.

    Restorer (2 Jun, 2005)
        [add] Load
        [fix] Save

    Restorer (8 Jun, 2005)
    	[add] FindItem, CreateNode
        [add] GetInt, SetInt, GetString, SetString, GetBool, SetBool, Remove

    Restorer (16 Jun, 2005)
	    [fix] Теперь адекватно реагирует на TAB (код 9)
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_ 1

#include "file.h"

class C_Config
{
	public:

	C_Config();
	~C_Config();

	void Load(const char *filename);
	void Save(const char *filename);
	bool GetInt(const char *path, int *value);	// "root/server/max_connections"
	void SetInt(const char *path, int value);
	bool GetString(const char *path, char **value);
	void SetString(const char *path, const char *value);
	bool GetBool(const char *path, bool *value);
	void SetBool(const char *path, bool value);
	void Remove(const char *path);

	private:

	struct s_config_item
	{
		char *name;
		char *value;
		s_config_item *child;
		s_config_item *next;
	};

	void DeleteTree(s_config_item *f);
	void SaveItem(C_File &fl, s_config_item *f, int depth);
	s_config_item* LoadItem(C_File &fl);
	s_config_item* FindItem(const char *path, s_config_item*** prev);
	s_config_item* CreateNode(const char *path);

	s_config_item *root;
};

#endif
